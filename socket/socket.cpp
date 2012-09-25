#include "socket.h"

namespace mihajong_socket {

const unsigned int numOfSockets = 32u;
WSADATA SocketInfo;
HINSTANCE dllInst;
Sock* sockets[numOfSockets] = {nullptr,};

void errordlg (socket_error& err) { // �G���[�_�C�A���O
	MessageBox(nullptr, err.what(), "Socket Error", MB_ICONERROR | MB_TOPMOST | MB_OK);
}

DLL int init () try { // �\�P�b�g������������
	if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw socket_initialization_error(err);
	return 0;
}
catch (socket_error& err) {
	errordlg(err); // �_�C�A���O��\������
	return err.error_code();
}

DLL int listen (int sock_id, int port) try { // �T�[�o�[�҂��󂯊J�n
	sockets[sock_id] = new Sock(port); // �ڑ�����
	return 0;
}
catch (socket_error& err) {
	errordlg(err); // �_�C�A���O��\������
	return 1;
}

DLL int connect (int sock_id, const char* const addr, int port) try { // �N���C�A���g�ڑ��J�n
	sockets[sock_id] = new Sock(addr, port); // �ڑ�����
	return 0;
}
catch (socket_error& err) {
	errordlg(err); // �_�C�A���O��\������
	return 1;
}

DLL int connected (int sock_id) try { // �ڑ�����Ă��邩�m�F
	return sockets[sock_id]->connected() ? 1 : 0; // �ڑ�����Ă��邩�m�F
}
catch (socket_error& err) { // ���M���s��
	errordlg(err); // �_�C�A���O��\������
	return -(err.error_code());
}

DLL int putc (int sock_id, int byte) try { // 1�o�C�g���M
	sockets[sock_id]->putc((unsigned char)byte); // 1�o�C�g���M
	return 0;
}
catch (socket_error& err) { // ���M���s��
	errordlg(err); // �_�C�A���O��\������
	return err.error_code();
}

DLL int puts (int sock_id, const char* const str) try { // �����񑗐M
	sockets[sock_id]->puts(std::string(str)); // null-terminated (C-style) string ���M
	return 0;
}
catch (socket_error& err) { // ���M���s��
	errordlg(err); // �_�C�A���O��\������
	return err.error_code();
}

DLL int getc (int sock_id) try { // 1�o�C�g��M
	return (int)sockets[sock_id]->getc(); // 1�o�C�g��M
}
catch (queue_empty&) { // �܂���M����f�[�^���Ȃ��ꍇ
	return -1;
}
catch (socket_error& err) { // ���M���s��
	errordlg(err); // �_�C�A���O��\������
	return -(err.error_code());
}

DLL int hangup (int sock_id) { // �ڑ���؂�
	sockets[sock_id]->disconnect();
	return 0;
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
		mihajong_socket::dllInst = nullptr;
		mihajong_socket::bye();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
