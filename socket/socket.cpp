#include "socket.h"

namespace mihajong_socket {

const unsigned int numOfSockets = 32u;
WSADATA SocketInfo;
HINSTANCE dllInst;
Sock* sockets[numOfSockets] = {NULL,};

void errordlg (socket_error& err) { // エラーダイアログ
	MessageBox(NULL, err.what(), "Socket Error", MB_ICONERROR | MB_TOPMOST | MB_OK);
}

DLL int init () { // ソケットを初期化する
	try {
		if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw socket_initialization_error(err);
		return 0;
	}
	catch (socket_error& err) {
		errordlg(err); // ダイアログを表示する
		return err.error_code();
	}
}

DLL int listen (int sock_id, int port) { // サーバー待ち受け開始
	try {
		sockets[sock_id] = new Sock(port); // 接続する
		return 0;
	}
	catch (socket_error& err) {
		errordlg(err); // ダイアログを表示する
		return 1;
	}
}

DLL int connect (int sock_id, const char* const addr, int port) { // クライアント接続開始
	try {
		sockets[sock_id] = new Sock(addr, port); // 接続する
		return 0;
	}
	catch (socket_error& err) {
		errordlg(err); // ダイアログを表示する
		return 1;
	}
}

DLL int putc (int sock_id, int byte) try { // 1バイト送信
	sockets[sock_id]->putc((unsigned char)byte); // 1バイト送信
	return 0;
}
catch (socket_error& err) { // 送信失敗時
	errordlg(err); // ダイアログを表示する
	return err.error_code();
}

DLL int bye () { // ソケットのクリンナップ
	return WSACleanup();
}

}
// -------------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { // 初期化などを行う
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		mihajong_socket::dllInst = hinstDLL;
		mihajong_socket::init();
		break;
	case DLL_PROCESS_DETACH:
		mihajong_socket::dllInst = NULL;
		mihajong_socket::bye();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
