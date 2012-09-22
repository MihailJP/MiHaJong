#include "socket.h"

namespace mihajong_socket {

const unsigned int numOfSockets = 32u;
WSADATA SocketInfo;
HINSTANCE dllInst;
Sock* sockets[numOfSockets] = {NULL,};

void errordlg (socket_error& err) { // �G���[�_�C�A���O
	MessageBox(NULL, err.what(), "Socket Error", MB_ICONERROR | MB_TOPMOST | MB_OK);
}

DLL int init () { // �\�P�b�g������������
	try {
		if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw socket_initialization_error(err);
		return 0;
	}
	catch (socket_error& err) {
		errordlg(err); // �_�C�A���O��\������
		return err.error_code();
	}
}

DLL int listen (int sock_id, int port) { // �T�[�o�[�҂��󂯊J�n
	try {
		sockets[sock_id] = new Sock(port); // �ڑ�����
		return 0;
	}
	catch (socket_error& err) {
		errordlg(err); // �_�C�A���O��\������
		return 1;
	}
}

DLL int connect (int sock_id, const char* const addr, int port) { // �N���C�A���g�ڑ��J�n
	try {
		sockets[sock_id] = new Sock(addr, port); // �ڑ�����
		return 0;
	}
	catch (socket_error& err) {
		errordlg(err); // �_�C�A���O��\������
		return 1;
	}
}

DLL int putc (int sock_id, int byte) try { // 1�o�C�g���M
	sockets[sock_id]->putc((unsigned char)byte); // 1�o�C�g���M
	return 0;
}
catch (socket_error& err) { // ���M���s��
	errordlg(err); // �_�C�A���O��\������
	return err.error_code();
}

DLL int bye () { // �\�P�b�g�̃N�����i�b�v
	return WSACleanup();
}

}
// -------------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { // �������Ȃǂ��s��
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
