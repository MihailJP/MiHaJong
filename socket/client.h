#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include "server.h"

namespace mihajong_socket {
namespace client {
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
		//static DWORD WINAPI initiate (LPVOID param); // CreateThread()�ɓn�������p
		starter (const std::string& InputPlayerName, unsigned short port, const std::string& server); // �R���X�g���N�^
		//void terminate (); // �����ɊJ�n
		//bool isFinished (); // �ҋ@�p�X���b�h���I��������ǂ���
		//unsigned int chkCurrentConnection (); // ���݂̐ڑ���
		//std::string getPlayerName (unsigned id); // �v���C���[��
	};

	void send (unsigned char SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂�
	DLL void send (int SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂� [Transitional API]
	DLL void receive (int* const ClientReceived, int* const ReceivedMsg); // �T�[�o�[�̃��b�Z�[�W����M����
}
}

#endif
