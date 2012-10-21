#include "remote.h"

namespace RemoteAction {

/* 接続先の打牌 */
RemoteActionPtr rDahaiProc = {nullptr};
RemoteDahai::RemoteDahai (GameTable* const gStat) {
	gameStat = gStat; finished = false;
	remoteDahai.type = DiscardTileNum::Normal; remoteDahai.id = 0;
	CreateThread(nullptr, 0, startthread, this, 0, nullptr);
}
DWORD WINAPI RemoteDahai::startthread(LPVOID param) {
	return reinterpret_cast<RemoteDahai*>(param)->thread();
}
DWORD WINAPI RemoteDahai::thread () {
	int ReceivedMsg;
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
		int ClientReceived;
		while (true) {
			// TODO: チャットの受信
			//chatrecv GameStat, GameEnv
			mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
			if (ClientReceived) break;
			Sleep(0); // ポーリング
		}
		if ((ReceivedMsg == mihajong_socket::protocol::Dahai_Remote_Disconnect) &&
			(!gameStat->Player[gameStat->CurrentPlayer.Active].ConnectionLost)) {
				gameStat->Player[gameStat->CurrentPlayer.Active].ConnectionLost = true;
				std::ostringstream o; o << "プレイヤー [" <<
					static_cast<int>(gameStat->CurrentPlayer.Active) <<
					"] の回線切断を検知しました。";
				warn(o.str().c_str());
		}
	}
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
		int ServerReceived;
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::server::receive(&ServerReceived, &ReceivedMsg);
			if (ServerReceived == (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag - 1)) {
				break;
			}
			Sleep(0);
		}
		// 受信失敗の時
		if (ReceivedMsg == 1023) {
			for (int i = 0; i < PLAYERS; i++) {
				if ((EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag == (ServerReceived + 1)) &&
					(!gameStat->Player[i].ConnectionLost)) {
						gameStat->Player[i].ConnectionLost = true;
						std::ostringstream o; o << "プレイヤー [" <<
							static_cast<int>(gameStat->CurrentPlayer.Active) <<
							"] の回線切断を検知しました。";
				}
			}
			ReceivedMsg = mihajong_socket::protocol::Dahai_Remote_Disconnect;
		}
		// クライアントに送信
		for (int i = 0; i < ACTUAL_PLAYERS - 1; i++) {
			if ((ServerReceived != i + 1) &&
				((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == i + 2) ||
				(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag == i + 2) ||
				(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag == i + 2) ||
				((!chkGameType(gameStat, SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == i + 2)))) {
					int stat = mihajong_socket::putc(i + 1, ReceivedMsg);
					std::ostringstream o;
					if (stat) {
						o << "データの送信に失敗しました。クライアント [" << (i + 1) << "] ステータスコード [" << stat << "]";
						error(o.str().c_str());
					} else {
						o << "クライアント [" << (i + 1) << "] にデータを送信 [0x" <<
							std::hex << std::setw(2) << std::setfill('0') << ReceivedMsg << "]";
						trace(o.str().c_str());
					}
			}
		}
	}
	{
		using namespace mihajong_socket::protocol;
		if ((ReceivedMsg >= Dahai_Type_Normal_Offset) && (ReceivedMsg < (Dahai_Type_Normal_Offset + NUM_OF_TILES_IN_HAND))) {
			remoteDahai.type = DiscardTileNum::Normal; remoteDahai.id = ReceivedMsg - Dahai_Type_Normal_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Ankan_Offset) && (ReceivedMsg < (Dahai_Type_Ankan_Offset + NUM_OF_TILES_IN_HAND))) {
			remoteDahai.type = DiscardTileNum::Ankan; remoteDahai.id = ReceivedMsg - Dahai_Type_Ankan_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Kakan_Offset) && (ReceivedMsg < (Dahai_Type_Kakan_Offset + NUM_OF_TILES_IN_HAND))) {
			remoteDahai.type = DiscardTileNum::Kakan; remoteDahai.id = ReceivedMsg - Dahai_Type_Kakan_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Riichi_Offset) && (ReceivedMsg < (Dahai_Type_Riichi_Offset + NUM_OF_TILES_IN_HAND))) {
			remoteDahai.type = DiscardTileNum::Riichi; remoteDahai.id = ReceivedMsg - Dahai_Type_Riichi_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_ORiichi_Offset) && (ReceivedMsg < (Dahai_Type_ORiichi_Offset + NUM_OF_TILES_IN_HAND))) {
			remoteDahai.type = DiscardTileNum::OpenRiichi; remoteDahai.id = ReceivedMsg - Dahai_Type_ORiichi_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Flower_Offset) && (ReceivedMsg < (Dahai_Type_Flower_Offset + NUM_OF_TILES_IN_HAND))) {
			remoteDahai.type = DiscardTileNum::Flower; remoteDahai.id = ReceivedMsg - Dahai_Type_Flower_Offset;
		} else if (ReceivedMsg == Dahai_Kyuushu) {
			remoteDahai.type = DiscardTileNum::Kyuushu; remoteDahai.id = 0;
		} else if (ReceivedMsg == Dahai_Tsumo) {
			remoteDahai.type = DiscardTileNum::Agari; remoteDahai.id = 0;
		} else if (ReceivedMsg == Dahai_Remote_Disconnect) {
			remoteDahai.type = DiscardTileNum::Normal; remoteDahai.id = NUM_OF_TILES_IN_HAND - 1;
		}
	}
	finished = true;
	return S_OK;
}
/* 今は諸事情により呼び出し元で管理しているが、完全移植達成後は是正する予定 */
__declspec(dllexport) void remotedahai_begin (GameTable* const gameStat) {
	rDahaiProc.dahai = new RemoteDahai(gameStat);
}
__declspec(dllexport) int remotedahai_isfinished () {
	return rDahaiProc.dahai->isFinished() ? 1 : 0;
}
__declspec(dllexport) int remotedahai_getdiscard () {
	return rDahaiProc.dahai->get().toSingleInt();
}
__declspec(dllexport) void remotedahai_end () {
	delete rDahaiProc.dahai; rDahaiProc.dahai = nullptr;
}

/* 接続先の鳴き */
RemoteNaki::RemoteNaki (GameTable* const gStat) {
	gameStat = gStat; finished = false;
	CreateThread(nullptr, 0, startthread, this, 0, nullptr);
}
DWORD WINAPI RemoteNaki::startthread(LPVOID param) {
	return reinterpret_cast<RemoteNaki*>(param)->thread();
}
void RemoteNaki::thread_client() {
	int ReceivedMsg, ClientReceived;
	for (int tmp = 0; tmp < ACTUAL_PLAYERS; tmp++) {
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
			if (ClientReceived) break;
			Sleep(0);
		}
		if (tmp != gameStat->PlayerID) {
			using namespace mihajong_socket::protocol;
			switch (ReceivedMsg) {
			case Naki_Ron:
				gameStat->Player[tmp].DeclarationFlag.Ron = true;
				break;
			case Naki_Pon:
				gameStat->Player[tmp].DeclarationFlag.Pon = true;
				break;
			case Naki_Kan:
				gameStat->Player[tmp].DeclarationFlag.Kan = true;
				break;
			case Naki_Chii_Lower:
				gameStat->Player[tmp].DeclarationFlag.Chi = 1;
				break;
			case Naki_Chii_Middle:
				gameStat->Player[tmp].DeclarationFlag.Chi = 2;
				break;
			case Naki_Chii_Upper:
				gameStat->Player[tmp].DeclarationFlag.Chi = 3;
				break;
			case Naki_Remote_Disconnect:
				gameStat->Player[tmp].ConnectionLost = true;
				break;
			}
		}
	}
}
void RemoteNaki::thread_server() {
	bool Received[3] = {false,}; int ReceivedMsg, ServerReceived;
	for (int i = 0; i < 3; i++)
		if (((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag != i + 2) ||
			(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag != i + 2) ||
			(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag != i + 2) ||
			((!chkGameType(gameStat, SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag != i + 2))))
			Received[i] = true;
	while (true) {
		//chatrecv GameStat, GameEnv
		mihajong_socket::server::receive(&ServerReceived, &ReceivedMsg);
		if (ServerReceived) {
			for (int i = 0; i < PLAYERS; i++) {
				for (int j = 0; j < 3; j++) {
					if ((ServerReceived == j + 1) && (EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == j + 2)) {
						checkremotenaki(i, ReceivedMsg);
						Received[i] = true;
					}
				}
			}
		}
		if ((Received[0]) && (Received[1]) && (chkGameType(gameStat, SanmaT) || Received[2]))
			break;
		Sleep(0);
	}
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		using namespace mihajong_socket::protocol;
		if (gameStat->Player[i].DeclarationFlag.Ron) mihajong_socket::server::send(Naki_Ron);
		else if (gameStat->Player[i].DeclarationFlag.Pon) mihajong_socket::server::send(Naki_Pon);
		else if (gameStat->Player[i].DeclarationFlag.Kan) mihajong_socket::server::send(Naki_Kan);
		else if (gameStat->Player[i].DeclarationFlag.Chi == 1) mihajong_socket::server::send(Naki_Chii_Lower);
		else if (gameStat->Player[i].DeclarationFlag.Chi == 2) mihajong_socket::server::send(Naki_Chii_Middle);
		else if (gameStat->Player[i].DeclarationFlag.Chi == 3) mihajong_socket::server::send(Naki_Chii_Upper);
		else if (gameStat->Player[i].ConnectionLost) mihajong_socket::server::send(Naki_Remote_Disconnect);
		else mihajong_socket::server::send(Naki_Ignore);
	}
}
DWORD WINAPI RemoteNaki::thread() {
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
		thread_client();
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Server)
		thread_server();
	for (int i = 0; i < PLAYERS; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron) // ロンしたら自摸牌位置にロン牌を設定(実装上の都合)
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1] = gameStat->CurrentDiscard;
	finished = true;
	return S_OK;
}
void RemoteNaki::checkremotenaki(PLAYER_ID player, int& ReceivedMsg) {
	using namespace mihajong_socket::protocol;
	switch (ReceivedMsg) {
	case 1023:
		if (!gameStat->Player[player].ConnectionLost) {
			gameStat->Player[player].ConnectionLost = true;
			std::ostringstream o; o << "プレイヤー [" << static_cast<int>(player) << "] の回線切断を検知しました。";
			warn(o.str().c_str());
		}
		ReceivedMsg = Naki_Remote_Disconnect;
		break;
	case Naki_Ron:
		gameStat->Player[player].DeclarationFlag.Ron = true;
		break;
	case Naki_Pon:
		gameStat->Player[player].DeclarationFlag.Pon = true;
		break;
	case Naki_Kan:
		gameStat->Player[player].DeclarationFlag.Kan = true;
		break;
	case Naki_Chii_Lower:
		gameStat->Player[player].DeclarationFlag.Chi = 1;
		break;
	case Naki_Chii_Middle:
		gameStat->Player[player].DeclarationFlag.Chi = 2;
		break;
	case Naki_Chii_Upper:
		gameStat->Player[player].DeclarationFlag.Chi = 3;
		break;
	}
	return;
}

__declspec(dllexport) void remotenaki_begin (GameTable* const gameStat) {
	rDahaiProc.naki = new RemoteNaki(gameStat);
}
__declspec(dllexport) int remotenaki_isfinished () {
	return rDahaiProc.naki->isFinished() ? 1 : 0;
}
__declspec(dllexport) void remotenaki_end () {
	delete rDahaiProc.naki; rDahaiProc.naki = nullptr;
}

} /* namespace */
