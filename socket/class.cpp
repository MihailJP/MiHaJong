#include "class.h"

uint32_t mihajong_socket::Sock::addr2var(const std::string& address) { // �A�h���X���擾
	uint32_t addr = inet_addr(address.c_str()); // �܂��� xxx.xxx.xxx.xxx �`���ł���Ɖ��肷��
	if ((addr == INADDR_NONE) || (addr == INADDR_ANY /* XP�ȑO�ŋ�̏ꍇ */)) { // ���s�����ꍇ
		hostent* host = gethostbyname(address.c_str()); // �z�X�g��������
		if (host == nullptr) throw invalid_address(WSAGetLastError()); // ���s�������O�𓊂���
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

mihajong_socket::Sock::Sock (uint16_t port) { // �N���C�A���g�ڑ�
	this->listen(port); // �ڑ�
}

void mihajong_socket::Sock::listen (uint16_t port) { // �T�[�o�[�J�n
	lsock = socket(AF_INET, SOCK_STREAM, 0); // �\�P�b�g��������
	if (lsock == INVALID_SOCKET) throw socket_creation_error(WSAGetLastError()); // �\�P�b�g�̍쐬�Ɏ��s�������O
	isServer = true; // �T�[�o�[�ł���
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // �|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // �A�h���X
	if (bind(lsock, (sockaddr *)&addr, sizeof(addr)))
		throw socket_bind_error(WSAGetLastError()); // ���s�������O�𓊂���
	this->listen();
}

void mihajong_socket::Sock::listen () { // �T�[�o�[�J�n
	threadPtr.server = new server_thread();
	threadPtr.server->setaddr(addr);
	threadPtr.server->setsock(&sock, &lsock);
	CreateThread(nullptr, 0, server_thread::thread, (LPVOID)threadPtr.server, 0, nullptr);
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
	threadPtr.client->setsock(&sock);
	CreateThread(nullptr, 0, client_thread::thread, (LPVOID)threadPtr.client, 0, nullptr);
}

bool mihajong_socket::Sock::connected () { // �ڑ�����Ă��邩���m�F
	if (isServer) {
		threadPtr.server->chkError();
		return threadPtr.server->isConnected();
	} else {
		threadPtr.client->chkError();
		return threadPtr.client->isConnected();
	}
};

void mihajong_socket::Sock::wait_until_connected () { // �����ʂ�̂��Ƃ����
	while (!connected()) Sleep(0);
};
unsigned char mihajong_socket::Sock::getc () { // �ǂݍ���(�񓯊�)
	if (isServer) {
		threadPtr.server->chkError();
		return threadPtr.server->read();
	} else {
		threadPtr.client->chkError();
		return threadPtr.client->read();
	}
}

unsigned char mihajong_socket::Sock::syncgetc () { // �ǂݍ���(����)
	while (true) {
		try {
			return getc();
		}
		catch (queue_empty) {
			Sleep(0); // Yield and try again
		}
	}
}

std::string mihajong_socket::Sock::gets () { // NewLine�܂œǂݍ���
	if (isServer) {
		threadPtr.server->chkError();
		return threadPtr.server->readline();
	} else {
		threadPtr.client->chkError();
		return threadPtr.client->readline();
	}
}

void mihajong_socket::Sock::putc (unsigned char byte) { // ��������
	if (isServer) {
		threadPtr.server->write(byte);
		threadPtr.server->chkError();
	} else {
		threadPtr.client->write(byte);
		threadPtr.client->chkError();
	}
}

void mihajong_socket::Sock::puts (const std::string& str) { // �����񏑂�����
	if (isServer) {
		for (auto k = str.begin(); k != str.end(); ++k)
			threadPtr.server->write((unsigned char)*k);
		threadPtr.server->chkError();
	} else {
		for (auto k = str.begin(); k != str.end(); ++k)
			threadPtr.client->write((unsigned char)*k);
		threadPtr.client->chkError();
	}
}

void mihajong_socket::Sock::disconnect () { // �ڑ���؂�
	if (isServer) {
		threadPtr.server->terminate();
	} else {
		threadPtr.client->terminate();
	}
	closesocket(sock);
	if (isServer) {
		delete threadPtr.server;
		threadPtr.server = nullptr;
	} else {
		delete threadPtr.client;
		threadPtr.client = nullptr;
	}
}

// -------------------------------------------------------------------------

mihajong_socket::Sock::network_thread::network_thread() {
	errtype = errNone; errcode = 0;
	connected = terminated = finished = false;
	InitializeCriticalSection(&myRecvQueueCS);
	InitializeCriticalSection(&mySendQueueCS);
}

mihajong_socket::Sock::network_thread::~network_thread() {
	DeleteCriticalSection(&myRecvQueueCS);
	DeleteCriticalSection(&mySendQueueCS);
}

DWORD WINAPI mihajong_socket::Sock::network_thread::thread(LPVOID lp) { // �X���b�h���N�����邽�߂̏���
	return ((client_thread*)lp)->myThreadFunc();
}

void mihajong_socket::Sock::network_thread::chkError () { // �G���[���`�F�b�N���A�����G���[���������O�𓊂���
	switch (errtype) {
	case errNone: // �G���[�Ȃ�
		break;
	case errListen: // listen���s
		throw listen_failure(errcode);
	case errAccept: // accept���s
		throw accept_failure(errcode);
	case errConnection: // �ڑ����s
		throw connection_failure(errcode);
	case errRecv: // ��M���s
		throw recv_error(errcode);
	case errSend: // ���M���s
		throw send_error(errcode);
	default: // �ق��̃G���[
		throw socket_error(errcode);
	}
}

int mihajong_socket::Sock::network_thread::reader() { // ��M����
	unsigned char buf[bufsize] = {0,};
	WSABUF buffer; buffer.buf = reinterpret_cast<CHAR*>(buf); buffer.len = bufsize;
	DWORD recvsz; DWORD flag = 0;
	if (WSARecv(*mySock, &buffer, 1, &recvsz, &flag, nullptr, nullptr) == 0) { // ��M����
		EnterCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X���擾
		for (unsigned int i = 0; i < recvsz; ++i) myMailBox.push(buf[i]); // �L���[�ɒǉ�
		LeaveCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X�����
	} else { // ��M�ł��Ȃ���
		switch (int err = WSAGetLastError()) {
		case WSAEWOULDBLOCK:
			break; // �f�[�^���Ȃ��ꍇ
		default: // �G���[����
			errtype = errRecv; errcode = err; terminated = finished = true; connected = false;
			return -((int)errtype);
		}
	}
	return 0;
}

int mihajong_socket::Sock::network_thread::writer() { // ���M����
	unsigned char buf[bufsize] = {0,};
	WSABUF buffer; buffer.buf = reinterpret_cast<CHAR*>(buf); buffer.len = bufsize;
	DWORD sendsz = 0;

	EnterCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X���擾
	while (!mySendBox.empty()) {
		buf[sendsz++] = mySendBox.front(); mySendBox.pop(); // �L���[������o��
	}
	LeaveCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X�����
	if (sendsz && (WSASend(*mySock, &buffer, 1, &sendsz, 0, nullptr, nullptr))) { // ���M
		switch (int err = WSAGetLastError()) {
		case WSAEWOULDBLOCK:
			break; // ���̃G���[�͖�������
		default:
			errtype = errSend; errcode = err; terminated = finished = true; connected = false;
			return -((int)errtype);
		}
	}
	return 0;
}

DWORD WINAPI mihajong_socket::Sock::network_thread::myThreadFunc() { // �X���b�h�̏���
	u_long arg = 1; ioctlsocket(*mySock, FIONBIO, &arg); // non-blocking ���[�h�ɐݒ�
	if (int err = establishConnection()) return err; // �ڑ�
	while (!terminated) { // ��~�����܂�
		if (int err = reader()) return err; // �ǂݍ���
		if (int err = writer()) return err; // ��������
	}
	finished = true;
	return S_OK;
}

unsigned char mihajong_socket::Sock::network_thread::read () { // 1�o�C�g�ǂݍ���
	unsigned char byte; bool empty = false;
	EnterCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X���擾
	if (myMailBox.empty()) empty = true; // �L���[����̏ꍇ
	else {byte = myMailBox.front(); myMailBox.pop();} // ��łȂ���Ύ��o��
	LeaveCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X�����
	if (empty) throw queue_empty(); // �󂾂������O
	else return byte; // �����łȂ���Ύ��o�����l��Ԃ�
}

std::string mihajong_socket::Sock::network_thread::readline () { // 1�s�ǂݍ���
	std::string line = ""; bool nwl_not_found = true;
	EnterCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X���擾
	auto tmpMailBox = myMailBox; // �L���[����Ɨp�R�s�[
	while (!tmpMailBox.empty()) {
		unsigned char tmpchr[sizeof(int)] = {0,};
		tmpchr[0] = tmpMailBox.front(); tmpMailBox.pop();
		if (*tmpchr == '\n') {
			nwl_not_found = false; break;
		} else if (*tmpchr != '\r') { // CR is just ignored
			line += std::string(reinterpret_cast<char*>(tmpchr));
		}
	}
	if (!nwl_not_found) myMailBox = tmpMailBox; // �L���[���R�~�b�g
	LeaveCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X�����
	if (nwl_not_found) throw queue_empty(); // �󂾂������O
	else return line; // �����łȂ���Ό��ʂ�Ԃ�
}

void mihajong_socket::Sock::network_thread::write (unsigned char byte) { // 1�o�C�g��������
	EnterCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X���擾
	mySendBox.push(byte); // �L���[�ɒǉ�
	LeaveCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X�����
}

bool mihajong_socket::Sock::network_thread::isConnected() { // �ڑ��ς���Ԃ��֐�
	return connected;
}

void mihajong_socket::Sock::network_thread::setaddr (const sockaddr_in destination) { // �ڑ����ݒ肷��
	myAddr = destination;
}
void mihajong_socket::Sock::network_thread::setsock (SOCKET* const socket) { // �\�P�b�g��ݒ肷��
	mySock = socket;
}

void mihajong_socket::Sock::network_thread::terminate () { // �ؒf����
	terminated = true; // �t���O�𗧂Ă�
	while (!finished) Sleep(0); // �X���b�h���I������܂ő҂�
	finished = terminated = connected = false; // �t���O�̌�n��
	errtype = errNone; errcode = 0;
}

// -------------------------------------------------------------------------

int mihajong_socket::Sock::client_thread::establishConnection() { // �ڑ����m������
	while (true) {
		if (::connect(*mySock, (sockaddr*)&myAddr, sizeof(myAddr)) == SOCKET_ERROR) { // �ڑ�
			errcode = WSAGetLastError();
			if (errcode != WSAEWOULDBLOCK) {
				errtype = errConnection; return -((int)errtype);
			}
		} else break;
		Sleep(0);
		if (terminated) return 0; // ���~�̏ꍇ
	}
	connected = true; // �ڑ��ς݃t���O�𗧂Ă�
	return 0;
}

int mihajong_socket::Sock::server_thread::establishConnection() { // �ڑ����m������
	u_long arg = 1; ioctlsocket(*listenerSock, FIONBIO, &arg); // non-blocking ���[�h�ɐݒ�
	if (::listen(*listenerSock, SOMAXCONN) == SOCKET_ERROR) { // �ҋ@
		errtype = errListen; errcode = WSAGetLastError(); return -((int)errtype);
	}
	while (true) {
		*mySock = accept(*listenerSock, nullptr, 0);
		if (*mySock == INVALID_SOCKET) { // �\�P�b�g�̍쐬�Ɏ��s�ꍇ
			errcode = WSAGetLastError();
			if (errcode != WSAEWOULDBLOCK) {
				errtype = errAccept;
				closesocket(*listenerSock);
				return -((int)errtype);
			}
		} else break;
		if (terminated) return 0; // ���~�̏ꍇ
		Sleep(0);
	}
	closesocket(*listenerSock);
	connected = true; // �ڑ��ς݃t���O�𗧂Ă�
	return 0;
}

void mihajong_socket::Sock::server_thread::setsock (SOCKET* const socket, SOCKET* const lsocket) { // �\�P�b�g��ݒ肷��
	mySock = socket; listenerSock = lsocket;
}
