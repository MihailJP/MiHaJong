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

}
}
