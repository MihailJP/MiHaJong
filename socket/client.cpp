#include "client.h"

namespace mihajong_socket {
namespace client {

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
