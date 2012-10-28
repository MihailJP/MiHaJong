#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include "protocol.h"
#ifdef SOCKET_EXPORTS
#include "except.h"
#include "../mjcore/nmrules.h"
#include <Windows.h>
#include <array>
#include <string>
#include <cstdint>
#endif

namespace mihajong_socket {
namespace server {
#ifdef SOCKET_EXPORTS
	extern unsigned int NumberOfPlayers;
	std::string getString(unsigned int socketID); // �������Ԃ�(����)
	void putString(unsigned int socketID, const std::string& sendingStr); // ������𑗐M
	class starter {
	private:
		volatile bool terminated, finished;
		unsigned short portnum;
		unsigned int CurrentConnection;
		std::array<std::string, 4> playerName;
		char ruleConf[RULE_LINES][RULE_IN_LINE + 1];
		DWORD WINAPI preparationThread (); // �ڑ���҂��A�ڑ�����������
	public:
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()�ɓn�������p
		starter (const std::string& InputPlayerName, unsigned short port, const char* const * const rule); // �R���X�g���N�^
		void terminate (); // �����ɊJ�n
		bool isFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
		unsigned int chkCurrentConnection (); // ���݂̐ڑ���
		std::string getPlayerName (unsigned id); // �v���C���[��
	};
	extern starter* starterThread;
	extern std::array<unsigned, 3> ServerCheckRotation; // �N���C�A���g�̓ǂݏo���D�揇��
	void sendstr (const std::string& sendingStr); // �T�[�o�[����̕����񑗐M
#endif
	DLL void start (const char* const name, int port, int players, const char* const * const rule); // �T�[�o�[���J�n������(DLL)
	DLL void doStart(); // �ڑ��ҋ@����߁A�����ɊJ�n����
	DLL int isStartingFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
	DLL int chkCurrentConnection (); // ���݂̐ڑ���
	DLL void getPlayerNames (char* playerName1, char* playerName2, char* playerName3, char* playerName4, unsigned bufsz);
	DLL void releaseobj (); // �f�X�g���N�^���ĂԂ���
	DLL void send (unsigned char SendingMsg); // �T�[�o�[����̑��M
	DLL void send (int SendingMsg, void*); // �T�[�o�[����̑��M(DLL)
	DLL void rotation_reset (); // �擾�D�揇�ʂ̃��Z�b�g
	DLL void receive (volatile int* const ServerReceived, int* const ReceivedMsg); // �擾����
}
}

#endif
