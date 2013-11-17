#pragma once

#include "socket.h"
#include "server.h"
#include "../common/thread.h"

namespace mihajong_socket {
namespace client {
#ifdef SOCKET_EXPORTS
	using server::getString; using server::putString;

	extern unsigned int NumberOfPlayers;

	class starter {
	private:
		bool connected, finished, failed;
		unsigned short portnum;
		std::array<CodeConv::tstring, 4> playerName;
		char ruleConf[RULE_LINES][RULE_IN_LINE + 1];
		std::string serveraddr;
		CodeConv::tstring myName;
		THREADLIB::thread myThread;
		int preparationThread (); // �ڑ���҂��A�ڑ�����������
		int ClientNumber;
	public:
		static void initiate (starter* inst); // CreateThread()�ɓn�������p
		void startThread (); // �X���b�h���J�n����
		starter (const CodeConv::tstring& InputPlayerName, const std::string& server, unsigned short port); // �R���X�g���N�^
		starter (const Sock&) = delete; // Delete unexpected copy constructor
		starter& operator= (const Sock&) = delete; // Delete unexpected assign operator
		~starter();
		bool isConnected(); // �ڑ������������ǂ���
		bool isFailed (); // �ڑ����s�������ǂ���
		bool isFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
		CodeConv::tstring getPlayerName (unsigned id); // �v���C���[��
		const char* getRules (unsigned line);
		int getClientNumber (); // �N���C�A���g�ԍ�
	};
	
	extern starter* starterThread;
	void send (unsigned char SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂�
#endif
	DLL void start (LPCTSTR const name, const char* const server, int port, int players); // �N���C�A���g���J�n������(DLL)
	DLL int isStartingFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
	DLL int isConnectionSucceded (); // �ڑ�������
	DLL int isConnectionFailed (); // �ڑ����s��
	DLL int getClientNumber (); // �N���C�A���g�ԍ�
	DLL void getPlayerNames (LPTSTR playerName1, LPTSTR playerName2, LPTSTR playerName3, LPTSTR playerName4, unsigned bufsz);
	DLL void checkout_rules (char** rules); // ���[�����`�F�b�N�A�E�g
	DLL void releaseobj (); // �f�X�g���N�^���ĂԂ���
	DLL void send (int SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂� [Transitional API]
	DLL void receive (volatile int* const ClientReceived, int* const ReceivedMsg); // �T�[�o�[�̃��b�Z�[�W����M����
}
}
