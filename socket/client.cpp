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

	// ---------------------------------------------------------------------

	DLL void receive (int* const ClientReceived, int* const ReceivedMsg) { // サーバーのメッセージを受信する
		*ClientReceived = 0; *ReceivedMsg = 1023;
		try {
			*ClientReceived = 1;
			*ReceivedMsg = sockets[0]->getc();
		}
		catch (queue_empty) { // Falling back if empty...
			*ClientReceived = 0;
			*ReceivedMsg = 1023;
		}
		// かつてはここでログ出力していた
	}

}
}
