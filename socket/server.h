#pragma once

#include "socket.h"
#include "protocol.h"
#ifdef SOCKET_EXPORTS
#include "except.h"
#include "../common/nmrules.h"
#include <windows.h>
#include <array>
#include <string>
#include <cstdint>
#include "../common/strcode.h"
#endif

namespace mihajong_socket {
namespace server {
#ifdef SOCKET_EXPORTS
	extern unsigned int NumberOfPlayers;
	CodeConv::tstring getString(unsigned int socketID); // �������Ԃ�(����)
	void putString(unsigned int socketID, const CodeConv::tstring& sendingStr); // ������𑗐M
	class starter {
	private:
		volatile bool terminated, finished;
		unsigned short portnum;
		unsigned int CurrentConnection;
		std::array<CodeConv::tstring, 4> playerName;
		char ruleConf[RULE_LINES][RULE_IN_LINE + 1];
		DWORD WINAPI preparationThread (); // �ڑ���҂��A�ڑ�����������
	public:
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()�ɓn�������p
		starter (const CodeConv::tstring& InputPlayerName, unsigned short port, const char* const * const rule); // �R���X�g���N�^
		void terminate (); // �����ɊJ�n
		bool isFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
		unsigned int chkCurrentConnection (); // ���݂̐ڑ���
		CodeConv::tstring getPlayerName (unsigned id); // �v���C���[��
	};
	extern starter* starterThread;
	extern std::array<unsigned, 3> ServerCheckRotation; // �N���C�A���g�̓ǂݏo���D�揇��
	void sendstr (const CodeConv::tstring& sendingStr); // �T�[�o�[����̕����񑗐M
#endif
	DLL void start (LPCTSTR const name, int port, int players, const char* const * const rule); // �T�[�o�[���J�n������(DLL)
	DLL void doStart(); // �ڑ��ҋ@����߁A�����ɊJ�n����
	DLL int isStartingFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
	DLL int chkCurrentConnection (); // ���݂̐ڑ���
	DLL void getPlayerNames (LPTSTR playerName1, LPTSTR playerName2, LPTSTR playerName3, LPTSTR playerName4, unsigned bufsz);
	DLL void releaseobj (); // �f�X�g���N�^���ĂԂ���
	DLL void send (unsigned char SendingMsg); // �T�[�o�[����̑��M
	DLL void send (int SendingMsg, void*); // �T�[�o�[����̑��M(DLL)
	DLL void rotation_reset (); // �擾�D�揇�ʂ̃��Z�b�g
	DLL void receive (volatile int* const ServerReceived, int* const ReceivedMsg); // �擾����
}
}
