#include "client.h"

namespace mihajong_socket {
namespace client {
	unsigned int NumberOfPlayers = 4;

	starter::starter (const std::string& InputPlayerName, unsigned short port, const std::string& server) { // �R���X�g���N�^
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
				MessageBox(nullptr, "�T�[�o�[�ɐڑ��ł��܂���\r\n�I�t���C�����[�h�ŊJ�n���܂�", "�ڑ����s", MB_ICONERROR | MB_TOPMOST | MB_OK);
				playerName[0] = std::string("[A]") + myName;
				playerName[1] = std::string("[b]COM");
				playerName[2] = std::string("[c]COM");
				playerName[3] = std::string("[d]COM");
				finished = true;
				return -1;
			}
			ClientNumber = sockets[0]->syncgetc() - 1;
			sockets[0]->disconnect();
			sockets[0]->connect(serveraddr, portnum + ClientNumber);
			while (!sockets[0]->connected()) Sleep(0);
			connected = true;
			putString(0, myName);
			while (sockets[0]->syncgetc() != protocol::Server_StartGame_Signature) Sleep(0); // �J�n��҂�
			for (unsigned int i = 0; i < NumberOfPlayers; ++i)
				playerName[i] = getString(0); // ���O����M
			for (unsigned i = 0; i < (RULESIZE/RULE_IN_LINE); ++i)
				strcpy_s(ruleConf[i], RULE_IN_LINE + 1, getString(0).c_str()); // ���[������M
			break;
		}
		finished = true;
		return S_OK;
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

	DLL void receive (int* const ClientReceived, int* const ReceivedMsg) { // �T�[�o�[�̃��b�Z�[�W����M����
		*ClientReceived = 0; *ReceivedMsg = 1023;
		try {
			*ClientReceived = 1;
			*ReceivedMsg = sockets[0]->getc();
		}
		catch (queue_empty) { // Falling back if empty...
			*ClientReceived = 0;
			*ReceivedMsg = 1023;
		}
		// ���Ă͂����Ń��O�o�͂��Ă���
	}

}
}
