#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include "protocol.h"
#include "except.h"
#include "../mjcore/nmrules.h"
#include <Windows.h>
#include <array>
#include <string>
#include <cstdint>

namespace mihajong_socket {
namespace server {
	extern unsigned int NumberOfPlayers;
	std::string getString(unsigned int socketID); // �������Ԃ�(����)
	void putString(unsigned int socketID, const std::string& sendingStr); // ������𑗐M
	class starter {
	private:
		bool terminated, finished;
		unsigned short portnum;
		unsigned int CurrentConnection;
		std::array<std::string, 4> playerName;
		char ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
		DWORD WINAPI preparationThread (); // �ڑ���҂��A�ڑ�����������
	public:
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()�ɓn�������p
		starter (const std::string& InputPlayerName, unsigned short port, const char* const * const rule); // �R���X�g���N�^
		void terminate (); // �����ɊJ�n
		bool isFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
		unsigned int chkCurrentConnection (); // ���݂̐ڑ���
	};
	extern starter* starterThread;
	DLL void start (const char* const name, int port, int players, const char* const * const rule); // �T�[�o�[���J�n������(DLL)
	DLL void doStart(); // �ڑ��ҋ@����߁A�����ɊJ�n����
	DLL int isStartingFinished (); // �ҋ@�p�X���b�h���I��������ǂ����A�I�������I�u�W�F�N�g���������
	DLL int chkCurrentConnection (); // ���݂̐ڑ���
	void send (unsigned char SendingMsg); // �T�[�o�[����̑��M
	void sendstr (const std::string& sendingStr); // �T�[�o�[����̕����񑗐M
	DLL void send (int SendingMsg, void*); // �T�[�o�[����̑��M(DLL)
	extern std::array<unsigned, 3> ServerCheckRotation; // �N���C�A���g�̓ǂݏo���D�揇��
	DLL void rotation_reset (); // �擾�D�揇�ʂ̃��Z�b�g
	DLL void receive (int* const ServerReceived, int* const ReceivedMsg); // �擾����
}
}

#endif
