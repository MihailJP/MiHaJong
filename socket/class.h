#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <cstdint>
#include <queue>
#include <WinSock2.h>
#include "except.h"

namespace mihajong_socket {

class Sock {
private:
	class client_thread; // �N���C�A���g�̃X���b�h
	class server_thread; // �T�[�o�[�̃X���b�h
	union Thread { // �X���b�h�I�u�W�F�N�g�̃|�C���^(���p��)
	public:
		client_thread* client;
		server_thread* server;
	};
	static uint32_t addr2var(const std::string& address); // �A�h���X���擾
	bool isServer;
	sockaddr_in addr;
	SOCKET sock;
	Thread threadPtr;
public:
	Sock () {}; // �\�P�b�g������
	Sock (const std::string& destination, uint16_t port); // �N���C�A���g�ڑ�
	~Sock (); // �ڑ���؂�
	void connect (const std::string& destination, uint16_t port); // �N���C�A���g�ڑ�
	void connect (); // �N���C�A���g�Đڑ�
	bool connected (); // �ڑ�����Ă��邩���m�F
	int putc (unsigned char byte); // �ǂݍ���
	void disconnect (); // �ڑ���؂�
};

class Sock::client_thread { // �N���C�A���g�̃X���b�h
public:
	enum errorType {errNone, errConnection,};
	client_thread();
	~client_thread();
	static DWORD WINAPI thread(LPVOID lp); // �X���b�h���N�����邽�߂̏���
	bool isConnected (); // �ڑ��ς���Ԃ��֐�
	void setaddr (const sockaddr_in destination); // �ڑ����ݒ肷��
	void setsock (SOCKET* const socket); // �\�P�b�g��ݒ肷��
	void terminate (); // �ؒf����
	void chkError (); // �G���[���`�F�b�N���A�����G���[���������O�𓊂���
	unsigned char read (); // 1�o�C�g�ǂݍ���
private:
	static const unsigned int bufsize = 65536;
	SOCKET* mySock; // �\�P�b�g(�|�C���^)
	errorType errtype; // �G���[�̎��
	int errcode; // �G���[�R�[�h
	bool connected; // �ڑ��ς݂��̃t���O[���[�J�[�X���b�h���珑������]
	bool terminated; // �ڑ��ς݂��̃t���O[�e�X���b�h���珑������]
	bool finished; // �I���ς݂��̃t���O[���[�J�[�X���b�h���珑������]
	sockaddr_in myAddr; // �A�h���X���[�e�X���b�h���珑������]
	std::queue<unsigned char> myMailBox; // �󂯎�����o�C�g��
	HANDLE myRecvQueueMutex; // ��M�o�b�t�@�p�~���[�e�b�N�X
	DWORD WINAPI myThreadFunc(); // �X���b�h�̏���
};

class Sock::server_thread { // �T�[�o�[�̃X���b�h
	// TODO: �������������
};

}
#endif
