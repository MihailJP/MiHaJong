#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include "server.h"

namespace mihajong_socket {
namespace client {
	using server::getString; using server::putString;

	void send (unsigned char SendingMsg); // サーバーにメッセージを送る
	DLL void send (int SendingMsg); // サーバーにメッセージを送る [Transitional API]
}
}

#endif
