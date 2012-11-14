#include "client.h"

namespace mihajong_socket {
namespace client {
	starter* starterThread = nullptr;
	unsigned int NumberOfPlayers = 4;

	starter::starter (const CodeConv::tstring& InputPlayerName, const std::string& server, unsigned short port) { // �R���X�g���N�^
		connected = finished = failed = false;
		myName = InputPlayerName;
		portnum = port;
		serveraddr = std::string(server);
		std::memset(ruleConf, 0, sizeof(ruleConf));
		ClientNumber = -1;
	}

	// ---------------------------------------------------------------------

	DWORD WINAPI starter::preparationThread () { // �ڑ���҂��A�ڑ�����������
		sockets[0] = new Sock(serveraddr, portnum);
		while (true) {
			try {
				if (!sockets[0]->connected()) Sleep(0);
			}
			catch (socket_error& e) { // Connection failed...
				failed = true;
				errordlg(e);
				MessageBox(nullptr, _T("�T�[�o�[�ɐڑ��ł��܂���\r\n�I�t���C�����[�h�ŊJ�n���܂�"), _T("�ڑ����s"), MB_ICONERROR | MB_TOPMOST | MB_OK);
				playerName[0] = CodeConv::tstring(_T("[A]")) + myName;
				playerName[1] = CodeConv::tstring(_T("[b]COM"));
				playerName[2] = CodeConv::tstring(_T("[c]COM"));
				playerName[3] = CodeConv::tstring(_T("[d]COM"));
				finished = true;
				return S_OK;
			}
			ClientNumber = sockets[0]->syncgetc() - 1;
			sockets[0]->disconnect();
			sockets[0]->connect(serveraddr, portnum + ClientNumber);
			while (!sockets[0]->connected()) Sleep(0);
			connected = true;
			putString(0, myName);
			while (sockets[0]->syncgetc() != protocol::Server_StartGame_Signature) Sleep(10); // �J�n��҂�
			for (unsigned int i = 0; i < NumberOfPlayers; ++i)
				playerName[i] = getString(0); // ���O����M
			for (unsigned i = 0; i < RULE_LINES; ++i)
				strcpy_s(ruleConf[i], RULE_IN_LINE + 1, CodeConv::toANSI(getString(0)).c_str()); // ���[������M
			break;
		}
		finished = true;
		return S_OK;
	}

	// ---------------------------------------------------------------------

	DWORD WINAPI starter::initiate (LPVOID param) { // CreateThread()�ɓn�������p
		return ((starter*)param)->preparationThread();
	}
	bool starter::isConnected () { // �ڑ������������ǂ���
		return connected;
	}
	bool starter::isFailed () { // �ڑ����s�������ǂ���
		return failed;
	}
	bool starter::isFinished () { // �ҋ@�p�X���b�h���I��������ǂ���
		return finished;
	}
	CodeConv::tstring starter::getPlayerName (unsigned id) { // �v���C���[��
		return playerName[id];
	}
	const char* starter::getRules (unsigned line) {
		return ruleConf[line];
	}
	int starter::getClientNumber () { // �N���C�A���g�ԍ�
		return ClientNumber;
	}

	// ---------------------------------------------------------------------

	DLL void start (LPCTSTR const name, LPCSTR const server, int port, int players) { // �T�[�o�[���J�n������(DLL)
		NumberOfPlayers = (unsigned int)players;
		starterThread = new starter(name, server, (unsigned short)port);
		CreateThread(nullptr, 0, starter::initiate, (LPVOID)starterThread, 0, nullptr);
	}
	DLL int isStartingFinished () { // �ҋ@�p�X���b�h���I��������ǂ���
		return starterThread->isFinished() ? 1 : 0;
	}
	DLL int isConnectionSucceded () { // �ڑ�������
		return starterThread->isConnected() ? 1 : 0;
	}
	DLL int isConnectionFailed () { // �ڑ����s��
		return starterThread->isFailed() ? 1 : 0;
	}
	DLL int getClientNumber () { // �N���C�A���g�ԍ�
		return starterThread->getClientNumber();
	}
	DLL void getPlayerNames (LPTSTR playerName1, LPTSTR playerName2, LPTSTR playerName3, LPTSTR playerName4, unsigned bufsz) {
		_tcscpy_s(playerName1, bufsz, starterThread->getPlayerName(0).c_str());
		_tcscpy_s(playerName2, bufsz, starterThread->getPlayerName(1).c_str());
		_tcscpy_s(playerName3, bufsz, starterThread->getPlayerName(2).c_str());
		_tcscpy_s(playerName4, bufsz, starterThread->getPlayerName(3).c_str());
	}
	DLL void checkout_rules (char** rules) { // ���[�����`�F�b�N�A�E�g
		for (unsigned i = 0; i < RULE_LINES; ++i)
			std::memcpy(rules[i], starterThread->getRules(i), RULE_IN_LINE);
	}
	DLL void releaseobj () { // �f�X�g���N�^���ĂԂ���
		delete starterThread; starterThread = nullptr;
	}

	// ---------------------------------------------------------------------

	void send (unsigned char SendingMsg) { // �T�[�o�[�Ƀ��b�Z�[�W�𑗂�
		sockets[0]->putc(SendingMsg);
		// ���Ă͂����Ń��O�o�͂��Ă���
	}
	DLL void send (int SendingMsg) { // �T�[�o�[�Ƀ��b�Z�[�W�𑗂� [Transitional API]
		send((unsigned char)SendingMsg);
	}

	// ---------------------------------------------------------------------

	DLL void receive (volatile int* const ClientReceived, int* const ReceivedMsg) { // �T�[�o�[�̃��b�Z�[�W����M����
		*ClientReceived = 0; *ReceivedMsg = 1023;
		try {
			*ReceivedMsg = sockets[0]->getc();
			*ClientReceived = 1;
		}
		catch (queue_empty) { // Falling back if empty...
			*ClientReceived = 0;
			*ReceivedMsg = 1023;
		}
		// ���Ă͂����Ń��O�o�͂��Ă���
	}

}
}
