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
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) throw socket_creation_error(WSAGetLastError()); // �\�P�b�g�̍쐬�Ɏ��s�������O

	isServer = false; // �N���C�A���g�ł���
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // �|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = addr2var(destination.c_str()); // �A�h���X

	if (::connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) // �ڑ�
		throw connection_failure(WSAGetLastError());
}

mihajong_socket::Sock::~Sock() { // �ڑ���؂�
	shutdown(sock, SD_BOTH);
	closesocket(sock);
}