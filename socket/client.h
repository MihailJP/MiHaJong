#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include "server.h"

namespace mihajong_socket {
namespace client {
#ifdef SOCKET_EXPORTS
	using server::getString; using server::putString;

	extern unsigned int NumberOfPlayers;

	class starter {
	private:
		bool connected, finished, failed;
		unsigned short portnum;
		std::array<std::string, 4> playerName;
		char ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
		std::string serveraddr;
		std::string myName;
		DWORD WINAPI preparationThread (); // �ڑ���҂��A�ڑ�����������
		int ClientNumber;
	public:
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()�ɓn�������p
		starter (const std::string& InputPlayerName, const std::string& server, unsigned short port); // �R���X�g���N�^
		bool isConnected (); // �ڑ������������ǂ���
		bool isFailed (); // �ڑ����s�������ǂ���
		bool isFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
		std::string getPlayerName (unsigned id); // �v���C���[��
		const char* getRules (unsigned line);
		int getClientNumber (); // �N���C�A���g�ԍ�
	};
	
	extern starter* starterThread;
	void send (unsigned char SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂�
#endif
	DLL void start (const char* const name, const char* const server, int port, int players); // �N���C�A���g���J�n������(DLL)
	DLL int isStartingFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
	DLL int isConnectionSucceded (); // �ڑ�������
	DLL int isConnectionFailed (); // �ڑ����s��
	DLL int getClientNumber (); // �N���C�A���g�ԍ�
	DLL void getPlayerNames (char* playerName1, char* playerName2, char* playerName3, char* playerName4, unsigned bufsz);
	DLL void checkout_rules (char** rules); // ���[�����`�F�b�N�A�E�g
	DLL void releaseobj (); // �f�X�g���N�^���ĂԂ���
	DLL void send (int SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂� [Transitional API]
	DLL void receive (int* const ClientReceived, int* const ReceivedMsg); // �T�[�o�[�̃��b�Z�[�W����M����
}
}

#endif
