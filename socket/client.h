#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include "server.h"

namespace mihajong_socket {
namespace client {
	using server::getString; using server::putString;

	void send (unsigned char SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂�
	DLL void send (int SendingMsg); // �T�[�o�[�Ƀ��b�Z�[�W�𑗂� [Transitional API]
}
}

#endif
