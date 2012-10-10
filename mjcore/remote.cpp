#include "remote.h"

namespace RemoteAction {

/* 接続先の打牌 */
RemoteDahai* rDahaiProc = nullptr;
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
	rDahaiProc = new RemoteDahai(gameStat);
}
__declspec(dllexport) int remotedahai_isfinished () {
	return rDahaiProc->isFinished() ? 1 : 0;
}
__declspec(dllexport) int remotedahai_getdiscard () {
	return rDahaiProc->get().toSingleInt();
}
__declspec(dllexport) void remotedahai_end () {
	delete rDahaiProc; rDahaiProc = nullptr;
}

} /* namespace */
