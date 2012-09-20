#include "class.h"

uint32_t mihajong_socket::Sock::addr2var(const std::string& address) { // �A�h���X���擾
	uint32_t addr = inet_addr(address.c_str()); // �܂��� xxx.xxx.xxx.xxx �`���ł���Ɖ��肷��
	if ((addr == INADDR_NONE) || (addr == INADDR_ANY /* XP�ȑO�ŋ�̏ꍇ */)) { // ���s�����ꍇ
		hostent* host = gethostbyname(address.c_str()); // �z�X�g��������
		if (host == NULL) throw invalid_address(WSAGetLastError()); // ���s�������O�𓊂���
		addr = *(uint32_t *)host->h_addr_list[0]; // ���������炻�̃A�h���X
	}
	return addr;
}

mihajong_socket::Sock::Sock (const std::string& destination, uint16_t port) { // �N���C�A���g�ڑ�
	sock = socket(AF_INET, SOCK_STREAM, 0); // �\�P�b�g��������
	if (sock == INVALID_SOCKET) throw socket_creation_error(WSAGetLastError()); // �\�P�b�g�̍쐬�Ɏ��s�������O

	this->connect(destination, port); // �ڑ�
}

mihajong_socket::Sock::~Sock() { // �ڑ���؂�
	this->disconnect();
}

void mihajong_socket::Sock::connect (const std::string& destination, uint16_t port) { // �N���C�A���g�ڑ�
	isServer = false; // �N���C�A���g�ł���
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // �|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = addr2var(destination.c_str()); // �A�h���X
	this->connect();
}

void mihajong_socket::Sock::connect () { // �N���C�A���g�Đڑ�
	threadPtr.client = new client_thread();
	threadPtr.client->setaddr(addr);
	CreateThread(NULL, 0, client_thread::thread, (LPVOID)threadPtr.client, 0, NULL);
}

bool mihajong_socket::Sock::connected () { // �ڑ�����Ă��邩���m�F
	if (isServer) {
		return false; // ���u��
	} else {
		threadPtr.client->chkError();
		return threadPtr.client->isConnected();
	}
};

void mihajong_socket::Sock::disconnect () { // �ڑ���؂�
	if (isServer) {
		// ���u��
	} else {
		threadPtr.client->terminate();
	}
	closesocket(sock);
	if (isServer) {
		// ���u��
	} else {
		delete threadPtr.client;
		threadPtr.client = NULL;
	}
}

// -------------------------------------------------------------------------

mihajong_socket::Sock::client_thread::client_thread() {
	errtype = errNone; errcode = 0;
	connected = terminated = finished = false;
}

DWORD WINAPI mihajong_socket::Sock::client_thread::thread(LPVOID lp) { // �X���b�h���N�����邽�߂̏���
	return ((client_thread*)lp)->myThreadFunc();
}

void mihajong_socket::Sock::client_thread::chkError () { // �G���[���`�F�b�N���A�����G���[���������O�𓊂���
	switch (errtype) {
	case errNone: // �G���[�Ȃ�
		break;
	case errConnection: // �ڑ����s
		throw connection_failure(errcode);
	default: // �ق��̃G���[
		throw socket_error(errcode);
	}
}

DWORD WINAPI mihajong_socket::Sock::client_thread::myThreadFunc() { // �X���b�h�̏���
	if (::connect(*mySock, (sockaddr*)&myAddr, sizeof(myAddr)) == SOCKET_ERROR) { // �ڑ�
		errtype = errConnection; errcode = WSAGetLastError(); ExitThread(-((int)errtype));
	}
	// TODO: �����Ƀ��[�v������
	return S_OK;
}

bool mihajong_socket::Sock::client_thread::isConnected() { // �ڑ��ς���Ԃ��֐�
	return connected;
}

void mihajong_socket::Sock::client_thread::setaddr (const sockaddr_in destination) { // �ڑ����ݒ肷��
	myAddr = destination;
}
void mihajong_socket::Sock::client_thread::setsock (SOCKET* const socket) { // �\�P�b�g��ݒ肷��
	mySock = socket;
}

void mihajong_socket::Sock::client_thread::terminate () { // �ؒf����
	terminated = true; // �t���O�𗧂Ă�
	while (!finished) Sleep(0); // �X���b�h���I������܂ő҂�
	finished = terminated = connected = false; // �t���O�̌�n��
	errtype = errNone; errcode = 0;
}
