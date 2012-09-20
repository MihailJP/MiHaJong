#include "socket.h"

WSADATA SocketInfo;
HINSTANCE dllInst;

DLL int socket_init () { // ソケットを初期化する
	try {
		if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw err;
		return 0;
	}
	catch (int err) {
		// TODO: ダイアログとか出す
		return err;
	}
}

DLL int socket_bye () { // ソケットのクリンナップ
	return WSACleanup();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { // 初期化などを行う
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		dllInst = hinstDLL;
		socket_init();
		break;
	case DLL_PROCESS_DETACH:
		dllInst = NULL;
		socket_bye();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
