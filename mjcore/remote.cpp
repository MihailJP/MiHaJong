#include "remote.h"

#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
#include "../socket/socket.h"
#include "envtbl.h"
#include "tileutil.h"
#include "func.h"
#include "chat.h"
#include <sstream>
#include <iomanip>
#include "../common/nmrules.h"
#include "../graphic/graphic.h"
#include "../common/chrono.h"
#include "../common/sleep.h"

namespace RemoteAction {

/* �ڑ���̑Ŕv */
void proc_abrupt_disconnect(GameTable* const gameStat, PlayerID player) {
	if (player == -1) {
		CodeConv::tostringstream o; o << _T("�T�[�o�[����̐ؒf�����m���܂����B");
		warn(o.str().c_str());
	} else {
		gameStat->Player[player].ConnectionLost = true;
		CodeConv::tostringstream o; o << _T("�v���C���[ [") << static_cast<int>(player) << _T("] �̉���ؒf�����m���܂����B");
		warn(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; 
		if (player == -1)
			o << _T("*** �T�[�o�[�Ƃ̐ڑ����؂�܂����B");
		else
			o << _T("*** ") << EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
			_T("(") << windName(gameStat->playerwind(player)) << _T(") �̐ڑ����؂�܂����B");
		chat::appendchat(o.str().c_str());
		chat::appendchat(_T("*** ���̋ǂ̓c���؂�A���ǂ���CPU���㑖���܂��B"));
	}
}
RemoteDahai::RemoteDahai (GameTable* const gStat) {
	gameStat = gStat; finished = false;
	remoteDahai.type = DiscardTileNum::Normal; remoteDahai.id = 0;
	myThread = THREADLIB::thread(startthread, this);
}
RemoteDahai::~RemoteDahai() {
	myThread.join();
}
void RemoteDahai::startthread(RemoteDahai* inst) {
	inst->thread();
}
void RemoteDahai::thread () {
	int ReceivedMsg;
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
		volatile int ClientReceived = 0;
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
			if (ClientReceived) break;
			threadYield(); // �|�[�����O
		}
		// ��M���s�̎�
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
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
		volatile int ServerReceived = 0;
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::server::receive(&ServerReceived, &ReceivedMsg);
			if (ServerReceived == EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag) {
				break;
			}
			threadYield();
		}
		// ��M���s�̎�
		if (ReceivedMsg == 1023) {
			for (int i = 0; i < Players; i++) {
				if ((EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag == ServerReceived) &&
					(!gameStat->Player[i].ConnectionLost))
					proc_abrupt_disconnect(gameStat, i);
			}
			ReceivedMsg = mihajong_socket::protocol::Dahai_Remote_Disconnect;
		}
		// �N���C�A���g�ɑ��M
		for (int i = 0; i < ACTUAL_PLAYERS - 1; i++) {
			if ((ServerReceived != i + 1) &&
				((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == i + 2) ||
				(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag == i + 2) ||
				(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag == i + 2) ||
				((!gameStat->chkGameType(SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == i + 2)))) {
					int stat = mihajong_socket::putc(i + 1, ReceivedMsg);
					CodeConv::tostringstream o;
					if (stat) {
						o << _T("�f�[�^�̑��M�Ɏ��s���܂����B�N���C�A���g [") << (i + 1) << _T("] �X�e�[�^�X�R�[�h [") << stat << _T("]");
						error(o.str().c_str());
					} else {
						o << _T("�N���C�A���g [") << (i + 1) << _T("] �Ƀf�[�^�𑗐M [0x") <<
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
			remoteDahai.type = DiscardTileNum::Normal; remoteDahai.id = TsumohaiIndex;
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

/* �ڑ���̖� */
RemoteNaki::RemoteNaki (GameTable* const gStat) {
	gameStat = gStat; finished = false;
	myThread = THREADLIB::thread(startthread, this);
}
RemoteNaki::~RemoteNaki() {
	myThread.join();
}
void RemoteNaki::startthread(RemoteNaki* inst) {
	inst->thread();
}
void RemoteNaki::thread_client() {
	int ReceivedMsg; volatile int ClientReceived = 0;
	for (int tmp = 0; tmp < ACTUAL_PLAYERS; tmp++) {
		while (true) {
			//chatrecv GameStat, GameEnv
			mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
			if (ClientReceived) break;
			threadYield();
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
			((!gameStat->chkGameType(SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag != i + 1))))
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
		if ((Received[0]) && (Received[1]) && (gameStat->chkGameType(SanmaT) || Received[2]))
			break;
		threadYield();
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
void RemoteNaki::thread() {
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
		thread_client();
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Server)
		thread_server();
	for (int i = 0; i < Players; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron) // ���������玩�̔v�ʒu�Ƀ����v��ݒ�(������̓s��)
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
	mihajong_graphic::Transit(mihajong_graphic::sceneServerWaiting);

	char RuleConf[RULE_LINES][RULE_IN_LINE + 4];
	char* RuleConfPtr[RULE_LINES];
	for (int i = 0; i < RULE_LINES; i++) RuleConfPtr[i] = RuleConf[i];

	EnvTable::Instantiate()->GameMode = EnvTable::Server;
	serverAddr = "";
	RuleData::exportRule(RuleConfPtr);

	const std::string nomen(RuleData::chkPreference((std::string)"name"));
	const CodeConv::tstring Nomen(CodeConv::EnsureTStr(nomen));
	mihajong_socket::server::start(Nomen.c_str(), gamePort, ACTUAL_PLAYERS, RuleConfPtr);

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

void startClient(std::string& serverAddr, unsigned& ClientNumber, unsigned short gamePort) {
	mihajong_graphic::Transit(mihajong_graphic::sceneClientWaiting);
	serverAddr = RuleData::chkPreference((std::string)"server");
	EnvTable::Instantiate()->GameMode = EnvTable::Client;

	const std::string nomen(RuleData::chkPreference((std::string)"name"));
	const CodeConv::tstring Nomen(CodeConv::EnsureTStr(nomen));

	mihajong_graphic::Subscene(mihajong_graphic::cliwSubsceneConnecting);
	mihajong_socket::client::start(Nomen.c_str(), serverAddr.c_str(), gamePort, ACTUAL_PLAYERS);

	while (true) {
		if (mihajong_socket::client::isConnectionSucceded()) {
			mihajong_graphic::Subscene(mihajong_graphic::cliwSubsceneWaiting);
			break;
		} else if (mihajong_socket::client::isConnectionFailed()) {
			mihajong_graphic::Transit(mihajong_graphic::sceneWaitingError);
			EnvTable::Instantiate()->GameMode = EnvTable::Standalone;
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
	RuleData::storeRule(const_cast<const char**>(RuleConfPtr) /* const��t���ĂȂ��Ɖ��̂��G���[�Ɂc�c */);

	mihajong_socket::client::releaseobj();
	return;
}

} /* namespace */
