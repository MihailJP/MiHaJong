#include "socket.h"

WSADATA SocketInfo;
HINSTANCE dllInst;

DLL int socket_init () { // �\�P�b�g������������
	try {
		if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw socket_error(err);
		return 0;
	}
	catch (socket_error& err) {
		// TODO: �_�C�A���O�Ƃ��o��
		return err.error_code();
	}
}

DLL int socket_bye () { // �\�P�b�g�̃N�����i�b�v
	return WSACleanup();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { // �������Ȃǂ��s��
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
