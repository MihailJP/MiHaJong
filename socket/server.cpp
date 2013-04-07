#include "server.h"
#include <climits>

namespace mihajong_socket {
namespace server {
	starter* starterThread = nullptr;
	unsigned int NumberOfPlayers = 4;
	CodeConv::tstring getString(unsigned int socketID) { // ��������擾(����)
		std::string tmpString; unsigned int l = 0u;
		while (true) { // ������J�n�V�O�l�`�����󂯎��
			if (sockets[socketID]->syncgetc() == protocol::StartString_Signature)
				break;
		}
		l = sockets[socketID]->syncgetc(); // ������̒������󂯎��
		unsigned char tmpchr[sizeof(int)] = {0};
		for (unsigned int sp = 0; sp < l; ++sp) { // ���������M����
			tmpchr[0] = sockets[socketID]->syncgetc();
			tmpString += std::string(reinterpret_cast<char*>(tmpchr));
		}
		return CodeConv::DecodeStr(tmpString);
	}
	void putString(unsigned int socketID, const CodeConv::tstring& sendingStr) { // ������𑗐M
		sockets[socketID]->putc(protocol::StartString_Signature);
		unsigned strsz = sendingStr.length();
		if (strsz > UCHAR_MAX) {
			sockets[socketID]->putc(UCHAR_MAX);
			sockets[socketID]->puts(sendingStr.substr(0, UCHAR_MAX));
		} else {
			sockets[socketID]->putc(strsz);
			sockets[socketID]->puts(sendingStr);
		}
	}

	// ---------------------------------------------------------------------

	starter::starter (const CodeConv::tstring& InputPlayerName, unsigned short port, const char* const * const rule) { // �R���X�g���N�^
		terminated = finished = false;
		playerName[0] = CodeConv::tstring(_T("[A]")) + InputPlayerName;
		playerName[1] = CodeConv::tstring(_T("[b]COM"));
		playerName[2] = CodeConv::tstring(_T("[c]COM"));
		playerName[3] = CodeConv::tstring(_T("[d]COM"));
		portnum = port;
		memset(ruleConf, 0, sizeof(ruleConf));
		for (unsigned i = 0; i < RULE_LINES; ++i)
			memcpy(ruleConf[i], rule[i], RULE_IN_LINE);
	}

	// ---------------------------------------------------------------------

	DWORD WINAPI starter::preparationThread () { // �ڑ���҂��A�ڑ�����������
		sockets[0] = new Sock(portnum);
		CurrentConnection = 1;
		while (!terminated) {
			if (sockets[0]->connected()) {
				sockets[0]->putc(CurrentConnection + 1);
				delete sockets[0]; sockets[0] = nullptr;
				sockets[CurrentConnection] = new Sock(portnum + CurrentConnection);
				sockets[CurrentConnection]->wait_until_connected(); // �ڑ���҂�
				TCHAR tmpsignature[] = _T("[A]"); tmpsignature[1] += CurrentConnection; // �p��
				playerName[CurrentConnection] = CodeConv::tstring(tmpsignature) + getString(CurrentConnection);
				sockets[0] = new Sock(portnum); // �ēxlisten�J�n
				++CurrentConnection;
			}
			if (CurrentConnection >= NumberOfPlayers) break;
			Sleep(50); // Yield
		}
		delete sockets[0]; sockets[0] = nullptr; // �ҋ@�p�̃\�P�b�g�����
		send(protocol::Server_StartGame_Signature);
		for (unsigned int i = 0; i < NumberOfPlayers; ++i)
			sendstr(playerName[i]); // ���O�𑗐M
		for (unsigned i = 0; i < RULE_LINES; ++i)
			sendstr(CodeConv::EnsureTStr(ruleConf[i])); // ���[���𑗐M
		finished = true;
		return S_OK;
	}

	// ---------------------------------------------------------------------

	DWORD WINAPI starter::initiate (LPVOID param) { // CreateThread()�ɓn�������p
		return ((starter*)param)->preparationThread();
	}

	void starter::terminate () { // �����ɊJ�n
		terminated = true;
	}
	bool starter::isFinished () { // �ҋ@�p�X���b�h���I��������ǂ���
		return finished;
	}
	unsigned int starter::chkCurrentConnection () { // ���݂̐ڑ���
		return CurrentConnection;
	}
	CodeConv::tstring starter::getPlayerName (unsigned id) { // �v���C���[��
		return playerName[id];
	}

	DLL void start (LPCTSTR const name, int port, int players, const char* const * const rule) { // �T�[�o�[���J�n������(DLL)
		NumberOfPlayers = (unsigned int)players;
		starterThread = new starter(name, (unsigned short)port, rule);
		CreateThread(nullptr, 0, starter::initiate, (LPVOID)starterThread, 0, nullptr);
	}
	DLL void doStart() { // �ڑ��ҋ@����߁A�����ɊJ�n����
		starterThread->terminate();
	}
	DLL int isStartingFinished () { // �ҋ@�p�X���b�h���I��������ǂ���
		return starterThread->isFinished() ? 1 : 0;
	}
	DLL int chkCurrentConnection () { // ���݂̐ڑ���
		return (int)starterThread->chkCurrentConnection();
	}
	DLL void getPlayerNames (LPTSTR playerName1, LPTSTR playerName2, LPTSTR playerName3, LPTSTR playerName4, unsigned bufsz) {
#if defined(_MSC_VER)
		_tcscpy_s(playerName1, bufsz, starterThread->getPlayerName(0).c_str());
		_tcscpy_s(playerName2, bufsz, starterThread->getPlayerName(1).c_str());
		_tcscpy_s(playerName3, bufsz, starterThread->getPlayerName(2).c_str());
		_tcscpy_s(playerName4, bufsz, starterThread->getPlayerName(3).c_str());
#else
		_tcsncpy(playerName1, starterThread->getPlayerName(0).c_str(), bufsz);
		_tcsncpy(playerName2, starterThread->getPlayerName(1).c_str(), bufsz);
		_tcsncpy(playerName3, starterThread->getPlayerName(2).c_str(), bufsz);
		_tcsncpy(playerName4, starterThread->getPlayerName(3).c_str(), bufsz);
#endif
	}
	DLL void releaseobj () { // �f�X�g���N�^���ĂԂ���
		delete starterThread; starterThread = nullptr;
	}

	// ---------------------------------------------------------------------

	DLL void send (unsigned char SendingMsg) { // �T�[�o�[����̑��M
		for (unsigned int i = 1; i < NumberOfPlayers; ++i)
			if (sockets[i]&&(sockets[i]->connected())) sockets[i]->putc(SendingMsg);
	}
	void sendstr (const CodeConv::tstring& sendingStr) { // �T�[�o�[����̕����񑗐M
		for (unsigned int i = 1; i < NumberOfPlayers; ++i)
			if (sockets[i]&&(sockets[i]->connected())) putString(i, sendingStr);
	}
	DLL void send (int SendingMsg, void*) { // �T�[�o�[����̑��M
		send((unsigned char)SendingMsg);
	}

	// ---------------------------------------------------------------------

	std::array<unsigned, 3> ServerCheckRotation; // �N���C�A���g�̓ǂݏo���D�揇��

	DLL void rotation_reset () { // �擾�D�揇�ʂ̃��Z�b�g
		for (unsigned i = 0; i < 3; ++i) ServerCheckRotation[i] = i + 1;
	}
	DLL void receive (volatile int* const ServerReceived, int* const ReceivedMsg) { // �擾����
		*ServerReceived = 0; *ReceivedMsg = 1023; bool finished = false;
		for (unsigned i = 0; i < (NumberOfPlayers - 1); ++i) {
			if (sockets[ServerCheckRotation[i]] != nullptr) {
				try {
					*ReceivedMsg = sockets[ServerCheckRotation[i]]->getc();
					*ServerReceived = ServerCheckRotation[i];
					finished = true;
				}
				catch (queue_empty) { // Falling back if empty...
					*ServerReceived = 0;
					*ReceivedMsg = 1023;
					finished = false;
				}
				// ���Ă͂����Ń��O�𑗂��Ă���
			}
			if (finished) break;
		}
		{ // �擾�D�揇�ʂ��V�t�g
			unsigned tmp = ServerCheckRotation[0];
			ServerCheckRotation[0] = ServerCheckRotation[1];
			ServerCheckRotation[1] = ServerCheckRotation[2];
			ServerCheckRotation[2] = tmp;
		}
	}

}
}
