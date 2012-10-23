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
	{
		std::ostringstream o;
		o << "�\�P�b�g�I�u�W�F�N�g�R���X�g���N�^(�N���C�A���g) ���� [" << destination << "] �|�[�g [" << port << "]";
		info(o.str().c_str());
	}
	this->connect(destination, port); // �ڑ�
	info("�R���X�g���N�^�̏������������܂���");
}

mihajong_socket::Sock::~Sock() { // �ڑ���؂�
	info("�\�P�b�g�I�u�W�F�N�g�f�X�g���N�^");
	this->disconnect();
	info("�f�X�g���N�^�̏������������܂���");
}

mihajong_socket::Sock::Sock (uint16_t port) { // �T�[�o�ڑ�
	{
		std::ostringstream o;
		o << "�\�P�b�g�I�u�W�F�N�g�R���X�g���N�^(�T�[�o) �|�[�g [" << port << "]";
		info(o.str().c_str());
	}
	this->listen(port); // �ڑ�
	info("�R���X�g���N�^�̏������������܂���");
}

void mihajong_socket::Sock::listen (uint16_t port) { // �T�[�o�[�J�n
	{
		std::ostringstream o;
		o << "�ڑ��҂����J�n���܂� �|�[�g [" << port << "]";
		info(o.str().c_str());
	}
	portnum = port;
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
	threadPtr.server = new server_thread(this);
	threadPtr.server->setaddr(addr);
	threadPtr.server->setsock(&sock, &lsock);
	CreateThread(nullptr, 0, server_thread::thread, (LPVOID)threadPtr.server, 0, nullptr);
}

void mihajong_socket::Sock::connect (const std::string& destination, uint16_t port) { // �N���C�A���g�ڑ�
	{
		std::ostringstream o;
		o << "�ڑ����J�n���܂� ���� [" << destination << "] �|�[�g [" << port << "]";
		info(o.str().c_str());
	}
	portnum = port;
	sock = socket(AF_INET, SOCK_STREAM, 0); // �\�P�b�g��������
	if (sock == INVALID_SOCKET) throw socket_creation_error(WSAGetLastError()); // �\�P�b�g�̍쐬�Ɏ��s�������O

	isServer = false; // �N���C�A���g�ł���
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // �|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = addr2var(destination.c_str()); // �A�h���X
	this->connect();
}

void mihajong_socket::Sock::connect () { // �N���C�A���g�Đڑ�
	threadPtr.client = new client_thread(this);
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
	{
		std::ostringstream o;
		o << "�ڑ��ҋ@ �|�[�g [" << portnum << "]";
		info(o.str().c_str());
	}
	while (!connected()) Sleep(50);
	{
		std::ostringstream o;
		o << "�ڑ��ҋ@���� �|�[�g [" << portnum << "]";
		info(o.str().c_str());
	}
};
unsigned char mihajong_socket::Sock::getc () { // �ǂݍ���(�񓯊�)
	unsigned char byte;
	if (isServer) {
		threadPtr.server->chkError();
		byte = threadPtr.server->read();
	} else {
		threadPtr.client->chkError();
		byte = threadPtr.client->read();
	}
	{
		std::ostringstream o;
		o << "�o�C�g��M���� �|�[�g [" << portnum << "] �o�C�g [0x" <<
			std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte << "]";
		trace(o.str().c_str());
	}
	return byte;
}

unsigned char mihajong_socket::Sock::syncgetc () { // �ǂݍ���(����)
	unsigned char byte; bool fini = false;
	{
		std::ostringstream o;
		o << "�o�C�g��M����(����)�J�n �|�[�g [" << portnum << "]";
		trace(o.str().c_str());
	}
	while (!fini) {
		try {
			byte = getc(); fini = true;
		}
		catch (queue_empty&) {
			Sleep(50); // Yield and try again
		}
	}
	{
		std::ostringstream o;
		o << "�o�C�g��M����(����) �|�[�g [" << portnum << "] �o�C�g [0x" <<
			std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte << "]";
		trace(o.str().c_str());
	}
	return byte;
}

std::string mihajong_socket::Sock::gets () { // NewLine�܂œǂݍ���
	trace("�������NWL(0x0a)�܂Ŏ擾���܂��B");
	std::string str;
	if (isServer) {
		threadPtr.server->chkError();
		str = threadPtr.server->readline();
	} else {
		threadPtr.client->chkError();
		str = threadPtr.client->readline();
	}
	{
		std::ostringstream o;
		o << "�������M���� �|�[�g [" << portnum << "] ���� [" << str.length() << "] ������ [" << str << "]";
		trace(o.str().c_str());
	}
	return str;
}

void mihajong_socket::Sock::putc (unsigned char byte) { // ��������
	{
		std::ostringstream o;
		o << "�o�C�g���M���� �|�[�g [" << portnum << "] �o�C�g [0x" <<
			std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte << "]";
		trace(o.str().c_str());
	}
	if (isServer) {
		threadPtr.server->write(byte);
		threadPtr.server->chkError();
	} else {
		threadPtr.client->write(byte);
		threadPtr.client->chkError();
	}
}

void mihajong_socket::Sock::puts (const std::string& str) { // �����񏑂�����
	{
		std::ostringstream o;
		o << "�����񑗐M���� �|�[�g [" << portnum << "] ���� [" << str.length() << "] ������ [" << str << "]";
		trace(o.str().c_str());
	}
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
	shutdown(sock, SD_BOTH);
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

mihajong_socket::Sock::network_thread::network_thread(Sock* caller) {
	myCaller = caller;
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
	std::ostringstream o;
	switch (errtype) {
	case errNone: // �G���[�Ȃ�
		break;
	case errListen: // listen���s
		o << "listen���̃G���[�ł��B�G���[�R�[�h [" << errcode << "]"; error(o.str().c_str());
		throw listen_failure(errcode);
	case errAccept: // accept���s
		o << "�ڑ��󂯓��ꎞ�̃G���[�ł��B�G���[�R�[�h [" << errcode << "]"; error(o.str().c_str());
		throw accept_failure(errcode);
	case errConnection: // �ڑ����s
		o << "�ڑ����̃G���[�ł��B�G���[�R�[�h [" << errcode << "]"; error(o.str().c_str());
		throw connection_failure(errcode);
	case errRecv: // ��M���s
		o << "��M���̃G���[�ł��B�G���[�R�[�h [" << errcode << "]"; error(o.str().c_str());
		throw recv_error(errcode);
	case errSend: // ���M���s
		o << "���M���̃G���[�ł��B�G���[�R�[�h [" << errcode << "]"; error(o.str().c_str());
		throw send_error(errcode);
	default: // �ق��̃G���[
		o << "�\�P�b�g�G���[�ł��B�G���[�R�[�h [" << errcode << "]"; error(o.str().c_str());
		throw socket_error(errcode);
	}
}

int mihajong_socket::Sock::network_thread::reader() { // ��M����
	unsigned char buf[bufsize] = {0,};
	WSABUF buffer; buffer.buf = reinterpret_cast<CHAR*>(buf); buffer.len = bufsize;
	DWORD recvsz; DWORD flag = 0;
	if (WSARecv(*mySock, &buffer, 1, &recvsz, &flag, nullptr, nullptr) == 0) { // ��M����
		std::ostringstream o;
		o << "�f�[�^��M �|�[�g [" << myCaller->portnum << "] �X�g���[�� [";
		EnterCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X���擾
		unsigned count = 0;
		for (unsigned int i = 0; i < recvsz; ++i) {
			myMailBox.push(buf[i]); // �L���[�ɒǉ�
			if (i > 0) o << " ";
			o << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned int>(buf[i]);
		}
		o << "] �T�C�Y [" << std::dec << recvsz << "]";
		if (recvsz) trace(o.str().c_str());
		LeaveCriticalSection(&myRecvQueueCS); // ��M�p�~���[�e�b�N�X�����
		if (recvsz == 0) receive_ended = true; // ��M�I���H
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

	{
		std::ostringstream o;
		o << "�f�[�^���M �|�[�g [" << myCaller->portnum << "] �X�g���[�� [";
		EnterCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X���擾
		while (!mySendBox.empty()) {
			buf[sendsz++] = mySendBox.front(); mySendBox.pop(); // �L���[������o��
			if (sendsz > 1) o << " ";
			o << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned int>(buf[sendsz - 1]);
		}
		o << "] �T�C�Y [" << std::dec << sendsz << "]";
		if (sendsz) trace(o.str().c_str());
		if (receiver_closed) send_ended = true; // ���M�|�[�g�������Ă�����I��������
		LeaveCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X�����
	}
	buffer.len = sendsz; // ���M�T�C�Y
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
	while (sender_closed || receiver_closed) { // �I������܂�
		int err = 0;
		if (err = reader()) return err; // �ǂݍ���
		if (receive_ended) {shutdown(*mySock, SD_RECEIVE); receiver_closed = true;} // �I���Ȃ�\�P�b�g���V���b�g�_�E��
		if ((!sender_closed) && (err = writer())) return err; // ��������
		if (send_ended) {shutdown(*mySock, SD_SEND); sender_closed = true; send_ended = false;} // �I���Ȃ�\�P�b�g���V���b�g�_�E��
		Sleep(0);
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

void mihajong_socket::Sock::network_thread::wait_until_sent() { // ���M�L���[����ɂȂ�܂ő҂�
	while (true) { // ���M����������܂ő҂�
		EnterCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X���擾
		bool flag = mySendBox.empty(); // �I���������ǂ����̃t���O
		LeaveCriticalSection(&mySendQueueCS); // ���M�p�~���[�e�b�N�X�����
		if (flag) { // ����ׂ��f�[�^�����ׂđ���I������
			send_ended = true; break; // �t���O�𗧂ĂāA���[�v�𔲂���
		}
		Sleep(0);
	}
}

void mihajong_socket::Sock::network_thread::terminate () { // �ؒf����
	terminated = true; // �t���O�𗧂Ă�
	wait_until_sent(); // ���M����������܂ő҂�
	while (!finished) Sleep(0); // �X���b�h���I������܂ő҂�
	finished = terminated = send_ended = sender_closed = receive_ended = receiver_closed = connected = false; // �t���O�̌�n��
	errtype = errNone; errcode = 0;
}

// -------------------------------------------------------------------------

int mihajong_socket::Sock::client_thread::establishConnection() { // �ڑ����m������
	info("�N���C�A���g�ڑ��������J�n���܂�");
	while (true) {
		if (::connect(*mySock, (sockaddr*)&myAddr, sizeof(myAddr)) == SOCKET_ERROR) { // �ڑ�
			errcode = WSAGetLastError();
			if (errcode == WSAEISCONN) {
				break; // ����ɐڑ����������Ƃ݂Ȃ�
			} else if ((errcode != WSAEWOULDBLOCK) && (errcode != WSAEALREADY)) {
				errtype = errConnection; return -((int)errtype);
			}
		} else break;
		Sleep(50);
		if (terminated) { // ���~�̏ꍇ
			info("�N���C�A���g�ڑ������𒆎~���܂���");
			return 0;
		}
	}
	info("�N���C�A���g�ڑ��������������܂���");
	connected = true; // �ڑ��ς݃t���O�𗧂Ă�
	return 0;
}

int mihajong_socket::Sock::server_thread::establishConnection() { // �ڑ����m������
	info("�T�[�o�Ҏ󏈗����J�n���܂�");
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
		if (terminated) { // ���~�̏ꍇ
			info("�T�[�o�Ҏ󏈗��𒆎~���܂���");
			return 0;
		}
		Sleep(50);
	}
	shutdown(*listenerSock, SD_BOTH);
	closesocket(*listenerSock);
	info("�T�[�o�Ҏ󏈗����������܂���");
	connected = true; // �ڑ��ς݃t���O�𗧂Ă�
	return 0;
}

void mihajong_socket::Sock::server_thread::setsock (SOCKET* const socket, SOCKET* const lsocket) { // �\�P�b�g��ݒ肷��
	mySock = socket; listenerSock = lsocket;
}
