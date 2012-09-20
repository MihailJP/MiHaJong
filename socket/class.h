#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <cstdint>
#include <WinSock2.h>
#include "except.h"

namespace mihajong_socket {

class Sock {
private:
	static uint32_t addr2var(const std::string& address); // �A�h���X���擾
	bool isServer;
	sockaddr_in addr;
	SOCKET sock;
public:
	Sock (const std::string& destination, uint16_t port); // �N���C�A���g�ڑ�
};

}
#endif
