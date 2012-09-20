#include "socket.h"

WSADATA mihajong_socket::SocketInfo;
HINSTANCE mihajong_socket::dllInst;

DLL int mihajong_socket::socket_init () { // �\�P�b�g������������
	try {
		if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw socket_initialization_error(err);
		return 0;
	}
	catch (socket_error& err) {
		// TODO: �_�C�A���O�Ƃ��o��
		return err.error_code();
	}
}

DLL int mihajong_socket::socket_bye () { // �\�P�b�g�̃N�����i�b�v
	return WSACleanup();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { // �������Ȃǂ��s��
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
