#pragma once

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
		std::array<CodeConv::tstring, 4> playerName;
		char ruleConf[RULE_LINES][RULE_IN_LINE + 1];
		std::string serveraddr;
		CodeConv::tstring myName;
#ifdef _WIN32
		DWORD WINAPI preparationThread (); // �ڑ���҂��A�ڑ�����������
#else /* _WIN32 */
		pthread_t myThread;
		int preparationThread (); // �ڑ���҂��A�ڑ�����������
#endif /* _WIN32 */
		int ClientNumber;
	public:
#ifdef _WIN32
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()�ɓn�������p
#else /* _WIN32 */
		static void* initiate (void* param); // CreateThread()�ɓn�������p
#endif /* _WIN32 */
		void startThread (); // �X���b�h���J�n����
		starter (const CodeConv::tstring& InputPlayerName, const std::string& server, unsigned short port); // �R���X�g���N�^
		starter (const Sock&) = delete; // Delete unexpected copy constructor
		starter& operator= (const Sock&) = delete; // Delete unexpected assign operator
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
