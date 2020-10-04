#include "remote.h"

#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
#include "../socket/socket.h"
#include "envtbl.h"
#include "tileutil.h"
#include "func.h"
#include <sstream>
#include <iomanip>
#include "../common/nmrules.h"
#include "../graphic/graphic.h"
#include <chrono>
#include "../common/sleep.h"

namespace RemoteAction {

/* 接続先の打牌 */
void proc_abrupt_disconnect(GameTable* const gameStat, PlayerID player) {
	if (player == -1) {
		CodeConv::tostringstream o; o << _T("サーバーからの切断を検知しました。");
		warn(o.str().c_str());
	} else {
		gameStat->Player[player].ConnectionLost = true;
		CodeConv::tostringstream o; o << _T("プレイヤー [") << static_cast<int>(player) << _T("] の回線切断を検知しました。");
		warn(o.str().c_str());
	}
}
RemoteDahai::RemoteDahai (GameTable* const gStat) {
	gameStat = gStat; finished = false;
	remoteDahai.type = DiscardType::normal; remoteDahai.id = 0;
	myThread = std::thread(startthread, this);
}
RemoteDahai::~RemoteDahai() {
	myThread.join();
}
void RemoteDahai::startthread(RemoteDahai* inst) {
	inst->thread();
}
void RemoteDahai::thread () {
	int ReceivedMsg;
	int lipaiFrom = 0;
begin:
	if (EnvTable::Instantiate()->GameMode == ClientType::client) {
		volatile int ClientReceived = 0;
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
			if (ClientReceived) break;
			threadYield(); // ポーリング
		}
		// 受信失敗の時
		if (ReceivedMsg == 1023) {
			bool flag = false;
			for (int i = 0; i < Players; i++) {
				if ((EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag > 0) &&
					(!gameStat->Player[i].ConnectionLost)) {
						gameStat->Player[i].ConnectionLost = true;
						flag = true;
				}
			}
			if (flag) proc_abrupt_disconnect(gameStat, -1);
			ReceivedMsg = mihajong_socket::protocol::Dahai_Remote_Disconnect;
		}
		if ((ReceivedMsg == mihajong_socket::protocol::Dahai_Remote_Disconnect) &&
			(!gameStat->statOfActive().ConnectionLost))
			proc_abrupt_disconnect(gameStat, gameStat->CurrentPlayer.Active);
	}
	else if (EnvTable::Instantiate()->GameMode == ClientType::server) {
		volatile int ServerReceived = 0;
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::server::receive(&ServerReceived, &ReceivedMsg);
			if (ServerReceived == (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag - 1)) {
				break;
			}
			threadYield();
		}
		// 受信失敗の時
		if (ReceivedMsg == 1023) {
			for (int i = 0; i < Players; i++) {
				if ((EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag == (ServerReceived + 1)) &&
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
				((!gameStat->chkGameType(SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == i + 2)))) {
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
	else {
		return; // 意味がないので何もしないで帰る
	}
	{
		using namespace mihajong_socket::protocol;
		if ((ReceivedMsg >= Lipai_From) && (ReceivedMsg < (Lipai_From + NumOfTilesInHand))) {
			lipaiFrom = ReceivedMsg - Lipai_From;
			goto begin;
		} else if ((ReceivedMsg >= Lipai_To) && (ReceivedMsg < (Lipai_To + NumOfTilesInHand))) {
			MoveTile::enqueue(gameStat->CurrentPlayer.Active, lipaiFrom, ReceivedMsg - Lipai_From);
			goto begin;
		} else if (ReceivedMsg == Lipai_Reset) {
			lipai(gameStat, gameStat->CurrentPlayer.Active);
			goto begin;
		} else if ((ReceivedMsg >= Dahai_Type_Normal_Offset) && (ReceivedMsg < (Dahai_Type_Normal_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardType::normal; remoteDahai.id = ReceivedMsg - Dahai_Type_Normal_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Ankan_Offset) && (ReceivedMsg < (Dahai_Type_Ankan_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardType::ankan; remoteDahai.id = ReceivedMsg - Dahai_Type_Ankan_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Kakan_Offset) && (ReceivedMsg < (Dahai_Type_Kakan_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardType::kakan; remoteDahai.id = ReceivedMsg - Dahai_Type_Kakan_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Riichi_Offset) && (ReceivedMsg < (Dahai_Type_Riichi_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardType::riichi; remoteDahai.id = ReceivedMsg - Dahai_Type_Riichi_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_ORiichi_Offset) && (ReceivedMsg < (Dahai_Type_ORiichi_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardType::openRiichi; remoteDahai.id = ReceivedMsg - Dahai_Type_ORiichi_Offset;
		} else if ((ReceivedMsg >= Dahai_Type_Flower_Offset) && (ReceivedMsg < (Dahai_Type_Flower_Offset + NumOfTilesInHand))) {
			remoteDahai.type = DiscardType::flower; remoteDahai.id = ReceivedMsg - Dahai_Type_Flower_Offset;
		} else if (ReceivedMsg == Dahai_Kyuushu) {
			remoteDahai.type = DiscardType::kyuushu; remoteDahai.id = 0;
		} else if (ReceivedMsg == Dahai_Tsumo) {
			remoteDahai.type = DiscardType::agari; remoteDahai.id = 0;
		} else if (ReceivedMsg == Dahai_Remote_Disconnect) {
			remoteDahai.type = DiscardType::normal; remoteDahai.id = TsumohaiIndex;
		}
	}
	finished = true;
	return;
}
DiscardTileNum remotedahai (GameTable* const gameStat) {
	RemoteDahai* rDahai = new RemoteDahai(gameStat);
	while (!rDahai->isFinished())
		threadYield();
	DiscardTileNum d = rDahai->get();
	delete rDahai; rDahai = nullptr;
	return d;
}

/* 接続先の鳴き */
RemoteNaki::RemoteNaki (GameTable* const gStat) {
	gameStat = gStat; finished = false;
	myThread = std::thread(startthread, this);
}
RemoteNaki::~RemoteNaki() {
	myThread.join();
}
void RemoteNaki::startthread(RemoteNaki* inst) {
	inst->thread();
}
void RemoteNaki::thread_client() {
	int ReceivedMsg; volatile int ClientReceived = 0;
	int moveFrom = 0;
	for (int tmp = 0; tmp < ACTUAL_PLAYERS; tmp++) {
begin:
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
			if (ClientReceived) break;
			threadYield();
		}
		if (tmp != gameStat->PlayerID) {
			using namespace mihajong_socket::protocol;
			if ((ReceivedMsg >= Lipai_From) && (ReceivedMsg < (Lipai_From + NumOfTilesInHand))) {
				moveFrom = ReceivedMsg - Lipai_From;
				goto begin;
			} else if ((ReceivedMsg >= Lipai_To) && (ReceivedMsg < (Lipai_To + NumOfTilesInHand))) {
				MoveTile::enqueue(tmp, moveFrom, ReceivedMsg - Lipai_From);
				goto begin;
			} else if (ReceivedMsg == Lipai_Reset) {
				lipai(gameStat, tmp);
				goto begin;
			}
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
		if (((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag != i + 2) &&
			(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag != i + 2) &&
			(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag != i + 2) &&
			(gameStat->chkGameType(SanmaT) || (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag != i + 2))))
			Received[i] = true;
	if (gameStat->chkGameType(SanmaT))
		Received[2] = true;
	while (true) {
		//chatrecv GameStat, GameEnv
		mihajong_socket::server::receive(&ServerReceived, &ReceivedMsg);
		if (ServerReceived) {
			for (int i = 0; i < Players; i++) {
				for (int j = 0; j < 3; j++) {
					if ((ServerReceived == (j + 1)) && (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag == (j + 2))) {
						checkremotenaki(i, ReceivedMsg);
						Received[j] = true;
					}
				}
			}
		}
		if ((Received[0]) && (Received[1]) && (gameStat->chkGameType(SanmaT) || Received[2]))
			break;
		threadYield();
	}
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		using namespace mihajong_socket::protocol;
		MoveTile::for_each([](std::pair<int, int>& indices) {
			mihajong_socket::server::send(Lipai_From + indices.first);
			mihajong_socket::server::send(Lipai_To + indices.second);
		}, i, true);
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
void RemoteNaki::thread() {
	if (EnvTable::Instantiate()->GameMode == ClientType::client)
		thread_client();
	else if (EnvTable::Instantiate()->GameMode == ClientType::server)
		thread_server();
	for (int i = 0; i < Players; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron) // ロンしたら自摸牌位置にロン牌を設定(実装上の都合)
			gameStat->Player[i].Tsumohai() = gameStat->CurrentDiscard;
	finished = true;
	return;
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
		threadYield();
	delete rNaki; rNaki = nullptr;
}

} /* namespace */

namespace RemoteConnection {

void startServer(std::string& serverAddr, unsigned short gamePort) {
	mihajong_graphic::Transit(mihajong_graphic::SceneID::serverWaiting);

	char RuleConf[RULE_LINES][RULE_IN_LINE + 4];
	char* RuleConfPtr[RULE_LINES];
	for (int i = 0; i < RULE_LINES; i++) RuleConfPtr[i] = RuleConf[i];

	EnvTable::Instantiate()->GameMode = ClientType::server;
	serverAddr = "";
	RuleData::exportRule(RuleConfPtr);

	const CodeConv::tstring Nomen(CodeConv::EnsureTStr(RuleData::confFile.playerName()));
	mihajong_socket::server::start(Nomen.c_str(), gamePort, ACTUAL_PLAYERS, RuleConfPtr);

	int numOfClientsPrev = 0; int numOfClients = 0;
	while (true) {
		numOfClients = mihajong_socket::server::chkCurrentConnection();
		if (numOfClients != numOfClientsPrev) {
			if (ACTUAL_PLAYERS == 3) {
				if      (numOfClients == 1) mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::oneOfThree);
				else if (numOfClients == 2) mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::twoOfThree);
				else if (numOfClients == 3) mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::threeOfThree);
				else                        mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::none);
			} else {
				if      (numOfClients == 1) mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::oneOfFour);
				else if (numOfClients == 2) mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::twoOfFour);
				else if (numOfClients == 3) mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::threeOfFour);
				else if (numOfClients == 4) mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::fourOfFour);
				else                        mihajong_graphic::Subscene(mihajong_graphic::ServerWaitingSubsceneID::none);
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
	if (numOfClients >= 2) EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag = 2;
	if (numOfClients >= 3) EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag = 3;
	if (numOfClients >= 4) EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag = 4;
	mihajong_socket::server::releaseobj();
}

void startClient(std::string& serverAddr, unsigned& ClientNumber, unsigned short gamePort) {
	auto tmpAddr(RuleData::confFile.serverAddress());
	mihajong_graphic::Transit(mihajong_graphic::SceneID::clientWaiting);
	serverAddr = CodeConv::toANSI(tmpAddr);
	EnvTable::Instantiate()->GameMode = ClientType::client;

	const CodeConv::tstring Nomen(RuleData::confFile.playerName());

	mihajong_graphic::Subscene(mihajong_graphic::ClientWaitingSubsceneID::connecting);
	mihajong_socket::client::start(Nomen.c_str(), serverAddr.c_str(), gamePort, ACTUAL_PLAYERS);

	while (true) {
		if (mihajong_socket::client::isConnectionSucceded()) {
			mihajong_graphic::Subscene(mihajong_graphic::ClientWaitingSubsceneID::waiting);
			break;
		} else if (mihajong_socket::client::isConnectionFailed()) {
			mihajong_graphic::Transit(mihajong_graphic::SceneID::waitingError);
			EnvTable::Instantiate()->GameMode = ClientType::standalone;
			threadSleep(1500);
			return;
		}
		threadYield();
	}
	while (!mihajong_socket::client::isStartingFinished())
		threadYield();
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
