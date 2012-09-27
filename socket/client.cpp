#include "client.h"

namespace mihajong_socket {
namespace client {

	void send (unsigned char SendingMsg) { // サーバーにメッセージを送る
		sockets[0]->putc(SendingMsg);
		// かつてはここでログ出力していた
	}
	DLL void send (int SendingMsg) { // サーバーにメッセージを送る [Transitional API]
		send((unsigned char)SendingMsg);
	}

}
}
