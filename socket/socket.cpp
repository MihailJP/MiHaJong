#include "socket.h"

WSADATA mihajong_socket::SocketInfo;
HINSTANCE mihajong_socket::dllInst;

DLL int mihajong_socket::socket_init () { // ソケットを初期化する
	try {
		if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw socket_initialization_error(err);
		return 0;
	}
	catch (socket_error& err) {
		// TODO: ダイアログとか出す
		return err.error_code();
	}
}

DLL int mihajong_socket::socket_bye () { // ソケットのクリンナップ
	return WSACleanup();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { // 初期化などを行う
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		mihajong_socket::dllInst = hinstDLL;
		mihajong_socket::socket_init();
		break;
	case DLL_PROCESS_DETACH:
		mihajong_socket::dllInst = NULL;
		mihajong_socket::socket_bye();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
