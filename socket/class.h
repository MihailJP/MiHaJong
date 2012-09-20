#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <cstdint>
#include <WinSock2.h>

class Sock {
private:
	bool isServer;
	sockaddr_in addr;
	SOCKET sock;
public:
	Sock (std::string& destination, uint16_t port); // クライアント接続
};

#endif
