#include "remote.h"

#include "../socket/socket.h"
#include "envtbl.h"
#include "tileutil.h"
#include "func.h"
#include "chat.h"
#include <sstream>
#include <iomanip>
#include "../common/nmrules.h"
#include "../graphic/graphic.h"

namespace RemoteAction {

/* 接続先の打牌 */
void proc_abrupt_disconnect(GameTable* const gameStat, PlayerID player) {
	{
		gameStat->Player[player].ConnectionLost = true;
		CodeConv::tostringstream o; o << _T("プレイヤー [") << static_cast<int>(player) << _T("] の回線切断を検知しました。");
		warn(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; o << _T("*** ") << EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
			_T("(") << windName(playerwind(gameStat, player, gameStat->GameRound)) << _T(") の接続が切れました。");
		chat::appendchat(o.str().c_str());
		chat::appendchat(_T("*** この局はツモ切り、次局からCPUが代走します。"));
	}
}
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
		volatile int ClientReceived = 0;
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
			if (ClientReceived) break;
			Sleep(0); // ポーリング
		}
		if ((ReceivedMsg == mihajong_socket::protocol::Dahai_Remote_Disconnect) &&
			(!gameStat->statOfActive().ConnectionLost))
			proc_abrupt_disconnect(gameStat, gameStat->CurrentPlayer.Active);
	}
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
		volatile int ServerReceived = 0;
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::server::receive(&ServerReceived, &ReceivedMsg);
			if (ServerReceived == EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag) {
				break;
			}
			Sleep(0);
		}
		// 受信失敗の時
		if (ReceivedMsg == 1023) {
			for (int i = 0; i < Players; i++) {
				if ((EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag == ServerReceived) &&
					(!gameStat->Player[i].ConnectionLost))
					proc_abrupt_disconnect(gameStat, i);
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
					CodeConv::tostringstream o;
					if (stat) {
						o << _T("データの送信に失敗しました。クライアント [") << (i + 1) << _T("] ステータスコード [") << stat << _T("]");
						error(o.str().c_str());
					} else {
						o << _T("クライアント [") << (i + 1) << _T("] にデータを送信 [0x") <<
							std::hex << std::setw(2) << std::setfill(_T('0')) << ReceivedMsg << _T("]");
						trace(o.str().c_str());
					}
			}
		}
	}
	{
		using namespace mihajong_socket::protocol;
		if ((ReceivedMsg >= Dahai_Type_Normal_Offset) && (ReceivedMsg < (Dahai_Type_Normal_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardTileNum::Normal; remoteDahai.id = ReceivedMsg - Dahai_Type_Normal_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Ankan_Offset) && (ReceivedMsg < (Dahai_Type_Ankan_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardTileNum::Ankan; remoteDahai.id = ReceivedMsg - Dahai_Type_Ankan_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Kakan_Offset) && (ReceivedMsg < (Dahai_Type_Kakan_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardTileNum::Kakan; remoteDahai.id = ReceivedMsg - Dahai_Type_Kakan_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Riichi_Offset) && (ReceivedMsg < (Dahai_Type_Riichi_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardTileNum::Riichi; remoteDahai.id = ReceivedMsg - Dahai_Type_Riichi_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_ORiichi_Offset) && (ReceivedMsg < (Dahai_Type_ORiichi_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardTileNum::OpenRiichi; remoteDahai.id = ReceivedMsg - Dahai_Type_ORiichi_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Flower_Offset) && (ReceivedMsg < (Dahai_Type_Flower_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardTileNum::Flower; remoteDahai.id = ReceivedMsg - Dahai_Type_Flower_Offset;
		} else if (ReceivedMsg == Dahai_Kyuushu) {
			remoteDahai.type = DiscardTileNum::Kyuushu; remoteDahai.id = 0;
		} else if (ReceivedMsg == Dahai_Tsumo) {
			remoteDahai.type = DiscardTileNum::Agari; remoteDahai.id = 0;
		} else if (ReceivedMsg == Dahai_Remote_Disconnect) {
			remoteDahai.type = DiscardTileNum::Normal; remoteDahai.id = NumOfTilesInHand - 1;
		}
	}
	finished = true;
	return S_OK;
}
DiscardTileNum remotedahai (GameTable* const gameStat) {
	RemoteDahai* rDahai = new RemoteDahai(gameStat);
	while (!rDahai->isFinished()) Sleep(1);
	DiscardTileNum d = rDahai->get();
	delete rDahai; rDahai = nullptr;
	return d;
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
	int ReceivedMsg; volatile int ClientReceived = 0;
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
				gameStat->Player[tmp].DeclarationFlag.Chi = chiiLower;
				break;
			case Naki_Chii_Middle:
				gameStat->Player[tmp].DeclarationFlag.Chi = chiiMiddle;
				break;
			case Naki_Chii_Upper:
				gameStat->Player[tmp].DeclarationFlag.Chi = chiiUpper;
				break;
			case Naki_Remote_Disconnect:
				if (!gameStat->Player[tmp].ConnectionLost)
					proc_abrupt_disconnect(gameStat, tmp);
				break;
			}
		}
	}
}
void RemoteNaki::thread_server() {
	bool Received[3] = {false, false, false,};
	int ReceivedMsg; volatile int ServerReceived = 0;
	for (int i = 0; i < 3; i++)
		if (((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag != i + 1) &&
			(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag != i + 1) &&
			(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag != i + 1) &&
			((!chkGameType(gameStat, SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag != i + 1))))
			Received[i] = true;
	while (true) {
		//chatrecv GameStat, GameEnv
		mihajong_socket::server::receive(&ServerReceived, &ReceivedMsg);
		if (ServerReceived) {
			for (int i = 0; i < Players; i++) {
				for (int j = 0; j < 3; j++) {
					if ((ServerReceived == (j + 1)) && (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag == (j + 1))) {
						checkremotenaki(i, ReceivedMsg);
						Received[j] = true;
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
	for (int i = 0; i < Players; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron) // ロンしたら自摸牌位置にロン牌を設定(実装上の都合)
			gameStat->Player[i].Tsumohai() = gameStat->CurrentDiscard;
	finished = true;
	return S_OK;
}
void RemoteNaki::checkremotenaki(PlayerID player, int& ReceivedMsg) {
	using namespace mihajong_socket::protocol;
	switch (ReceivedMsg) {
	case 1023:
		if (!gameStat->Player[player].ConnectionLost)
			proc_abrupt_disconnect(gameStat, player);
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
		gameStat->Player[player].DeclarationFlag.Chi = chiiLower;
		break;
	case Naki_Chii_Middle:
		gameStat->Player[player].DeclarationFlag.Chi = chiiMiddle;
		break;
	case Naki_Chii_Upper:
		gameStat->Player[player].DeclarationFlag.Chi = chiiUpper;
		break;
	}
	return;
}

void remotenaki (GameTable* const gameStat) {
	RemoteNaki* rNaki = new RemoteNaki(gameStat);
	while (!rNaki->isFinished())
		Sleep(1);
	delete rNaki; rNaki = nullptr;
}

} /* namespace */

namespace RemoteConnection {

void startServer(std::string& serverAddr) {
	mihajong_graphic::Transit(mihajong_graphic::sceneServerWaiting);

	char RuleConf[RULE_LINES][RULE_IN_LINE + 4];
	char* RuleConfPtr[RULE_LINES];
	for (int i = 0; i < RULE_LINES; i++) RuleConfPtr[i] = RuleConf[i];

	EnvTable::Instantiate()->GameMode = EnvTable::Server;
	serverAddr = "";
	RuleData::exportRule(RuleConfPtr);

	const std::string nomen(RuleData::chkPreference((std::string)"name"));
	const CodeConv::tstring Nomen(CodeConv::EnsureTStr(nomen));
	mihajong_socket::server::start(Nomen.c_str(), /*PORT_GAME*/50000, ACTUAL_PLAYERS, RuleConfPtr);

	int numOfClientsPrev = 0; int numOfClients = 0;
	while (true) {
		numOfClients = mihajong_socket::server::chkCurrentConnection();
		if (numOfClients != numOfClientsPrev) {
			if (ACTUAL_PLAYERS == 3) {
				if      (numOfClients == 1) mihajong_graphic::Subscene(mihajong_graphic::srvwSubscene1of3);
				else if (numOfClients == 2) mihajong_graphic::Subscene(mihajong_graphic::srvwSubscene2of3);
				else if (numOfClients == 3) mihajong_graphic::Subscene(mihajong_graphic::srvwSubscene3of3);
				else                        mihajong_graphic::Subscene(mihajong_graphic::srvwSubsceneNone);
			} else {
				if      (numOfClients == 1) mihajong_graphic::Subscene(mihajong_graphic::srvwSubscene1of4);
				else if (numOfClients == 2) mihajong_graphic::Subscene(mihajong_graphic::srvwSubscene2of4);
				else if (numOfClients == 3) mihajong_graphic::Subscene(mihajong_graphic::srvwSubscene3of4);
				else if (numOfClients == 4) mihajong_graphic::Subscene(mihajong_graphic::srvwSubscene4of4);
				else                        mihajong_graphic::Subscene(mihajong_graphic::srvwSubsceneNone);
			}
			numOfClientsPrev = numOfClients;
		}
		if (mihajong_graphic::ui::WaitUIWithTimeout(50) != 0xffffffff)
			mihajong_socket::server::doStart();
		if (mihajong_socket::server::isStartingFinished()) break;
	}
	TCHAR playerName[4][256];
	mihajong_socket::server::getPlayerNames(playerName[0], playerName[1], playerName[2], playerName[3], 256);
	for (int i = 0; i < 4; ++i)
		EnvTable::Instantiate()->PlayerDat[i].PlayerName = playerName[i];
	if (numOfClients >= 2) EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag = 1;
	if (numOfClients >= 3) EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag = 1;
	if (numOfClients >= 4) EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag = 1;
	mihajong_socket::server::releaseobj();
}

void startClient(std::string& serverAddr, unsigned& ClientNumber) {
	mihajong_graphic::Transit(mihajong_graphic::sceneClientWaiting);
	serverAddr = RuleData::chkPreference((std::string)"server");
	EnvTable::Instantiate()->GameMode = EnvTable::Client;

	const std::string nomen(RuleData::chkPreference((std::string)"name"));
	const CodeConv::tstring Nomen(CodeConv::EnsureTStr(nomen));

	mihajong_graphic::Subscene(mihajong_graphic::cliwSubsceneConnecting);
	mihajong_socket::client::start(Nomen.c_str(), serverAddr.c_str(), /*PORT_GAME*/50000, ACTUAL_PLAYERS);

	while (true) {
		if (mihajong_socket::client::isConnectionSucceded()) {
			mihajong_graphic::Subscene(mihajong_graphic::cliwSubsceneWaiting);
			break;
		} else if (mihajong_socket::client::isConnectionFailed()) {
			mihajong_graphic::Transit(mihajong_graphic::sceneWaitingError);
			EnvTable::Instantiate()->GameMode = EnvTable::Standalone;
			Sleep(1500);
			return;
		}
		Sleep(50);
	}
	while (!mihajong_socket::client::isStartingFinished())
		Sleep(50);
	ClientNumber = mihajong_socket::client::getClientNumber();

	TCHAR playerName[4][256];
	mihajong_socket::client::getPlayerNames(playerName[0], playerName[1], playerName[2], playerName[3], 256);
	for (int i = 0; i < 4; ++i)
		EnvTable::Instantiate()->PlayerDat[i].PlayerName = playerName[i];
	for (int i = 0; i < ACTUAL_PLAYERS; ++i)
		if (ClientNumber != i) EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag = 1;

	char RuleConf[RULE_LINES][RULE_IN_LINE + 4];
	char* RuleConfPtr[RULE_LINES];
	for (int i = 0; i < RULE_LINES; i++) RuleConfPtr[i] = RuleConf[i];
	mihajong_socket::client::checkout_rules(RuleConfPtr);
	RuleData::storeRule(const_cast<const char**>(RuleConfPtr) /* constを付けてないと何故かエラーに…… */);

	mihajong_socket::client::releaseobj();
	return;
}

} /* namespace */
