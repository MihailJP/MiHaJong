#ifndef EXCEPT_H
#define EXCEPT_H

#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>

namespace mihajong_socket {

class socket_error : public std::runtime_error { // �\�P�b�g�G���[�̊��N���X
private:
	int myErrorCode;
public:
	socket_error(const std::string& wh) : std::runtime_error(wh) {myErrorCode = 0;}
	socket_error(const int err) : std::runtime_error(
		[](int i) -> std::string {std::ostringstream o; o << "Error " << i; return o.str();} (err))
	{
		myErrorCode = err;
	}
	socket_error(const std::string& wh, const int err) : std::runtime_error(
		[](const std::string& wh,int i) -> std::string
		{std::ostringstream o; o << wh << " (Error " << i << ")"; return o.str();} (wh, err))
	{
		myErrorCode = err;
	}
	int error_code() {return myErrorCode;}
};

class socket_initialization_error : public socket_error { // WinSock2���������̃G���[
public:
	socket_initialization_error(const int err) : socket_error("Initialization failure", err) {};
};

class socket_creation_error : public socket_error { // �\�P�b�g�쐬���̃G���[
public:
	socket_creation_error(const int err) : socket_error("Socket creation failure", err) {};
};

class invalid_address : public socket_error { // �A�h���X���������Ȃ�
public:
	invalid_address(const int err) : socket_error("Invalid or nonexistent address", err) {};
};

class connection_failure : public socket_error { // �ڑ��Ɏ��s����
public:
	connection_failure(const int err) : socket_error("Connection failed", err) {};
};

class queue_empty : public socket_error { // �L���[����
public:
	queue_empty() : socket_error("Queue is empty") {};
};

class recv_error : public socket_error { // ��M�Ɏ��s����
public:
	recv_error(const int err) : socket_error("Receive error", err) {};
};

class send_error : public socket_error { // ���M�Ɏ��s����
public:
	send_error(const int err) : socket_error("Send error", err) {};
};
}
#endif
