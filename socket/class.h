#pragma once

#include <string>
#include <cstdint>
#include <queue>
#include <sstream>
#include <iomanip>
#ifdef _WIN32
#ifndef _WINSOCKAPI_
#include <winsock2.h>
#endif
#else /* _WIN32 */
#include <sys/socket.h>
#include <netinet/in.h>
#endif /* _WIN32 */
#include "except.h"
#include "logger.h"
#include "../common/mutex.h"

namespace mihajong_socket {


#ifdef _WIN32
typedef SOCKET SocketDescriptor;
#else /* _WIN32 */
typedef int SocketDescriptor;
#endif /* _WIN32 */

class Sock {
private:
	class network_thread; // �X���b�h(�X�[�p�[�N���X)
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
	SocketDescriptor sock, lsock;
	Thread threadPtr;
	uint16_t portnum;
public:
	Sock () {}; // �\�P�b�g������
	Sock (uint16_t port); // �T�[�o�[�J�n
	Sock (const std::string& destination, uint16_t port); // �N���C�A���g�ڑ�
	~Sock (); // �ڑ���؂�
	void listen (uint16_t port); // �T�[�o�[�J�n
	void listen (); // �T�[�o�[�J�n
	void connect (const std::string& destination, uint16_t port); // �N���C�A���g�ڑ�
	void connect (); // �N���C�A���g�Đڑ�
	bool connected (); // �ڑ�����Ă��邩���m�F
	void wait_until_connected (); // �����ʂ�̂��Ƃ����
	unsigned char getc (); // �ǂݍ���(�񓯊�)
	unsigned char syncgetc (); // �ǂݍ���(����)
	CodeConv::tstring gets (); // ��s�ǂݍ���
	void putc (unsigned char byte); // ��������
	void puts (const CodeConv::tstring& str); // �����񏑂�����
	void disconnect (); // �ڑ���؂�
};

class Sock::network_thread { // �X���b�h(�X�[�p�[�N���X)
public:
	network_thread(Sock* caller);
	virtual ~network_thread();
#ifdef _WIN32
	static DWORD WINAPI thread(LPVOID lp); // �X���b�h���N�����邽�߂̏���
#else /* _WIN32 */
#endif /* _WIN32 */
	bool isConnected (); // �ڑ��ς���Ԃ��֐�
	void setaddr (const sockaddr_in destination); // �ڑ����ݒ肷��
	void setsock (SocketDescriptor* const socket); // �\�P�b�g��ݒ肷��
	void terminate (); // �ؒf����
	void chkError (); // �G���[���`�F�b�N���A�����G���[���������O�𓊂���
	unsigned char read (); // 1�o�C�g�ǂݍ���
	void write (unsigned char byte); // 1�o�C�g��������
	CodeConv::tstring readline (); // 1�s�ǂݍ���
protected:
	Sock* myCaller;
	enum errorType {errNone, errListen, errAccept, errConnection, errRecv, errSend};
	static const unsigned int bufsize = 65536;
	SocketDescriptor* mySock; // �\�P�b�g(�|�C���^)
	SocketDescriptor* listenerSock; // �\�P�b�g(�|�C���^)
	errorType errtype; // �G���[�̎��
	int errcode; // �G���[�R�[�h
	volatile bool connecting; // �ڑ������̃t���O[���[�J�[�X���b�h���珑������]
	volatile bool connected; // �ڑ��ς݂��̃t���O[���[�J�[�X���b�h���珑������]
	volatile bool terminated; // �ڑ��ς݂��̃t���O[�e�X���b�h���珑������]
	volatile bool send_ended; // ���M���S�ďI��������̃t���O
	volatile bool sender_closed; // ���M���S�ďI��������̃t���O
	volatile bool receive_ended; // ��M���S�ďI��������̃t���O
	volatile bool receiver_closed; // ��M���S�ďI��������̃t���O
	volatile bool finished; // �I���ς݂��̃t���O[���[�J�[�X���b�h���珑������]
	sockaddr_in myAddr; // �A�h���X���[�e�X���b�h���珑������]
	std::queue<unsigned char> myMailBox; // �󂯎�����o�C�g��
	MHJMutex myRecvQueueCS; // ��M�o�b�t�@�p�~���[�e�b�N�X(�N���e�B�J���Z�N�V�����ɕύX)
	std::queue<unsigned char> mySendBox; // ����\��̃o�C�g��
	MHJMutex mySendQueueCS; // ���M�o�b�t�@�p�~���[�e�b�N�X(�N���e�B�J���Z�N�V�����ɕύX)
	virtual int establishConnection () = 0; // �ڑ����m������
	int reader (); // �ǂݍ���
	int writer (); // ��������
#ifdef _WIN32
	DWORD WINAPI myThreadFunc(); // �X���b�h�̏���
#else /* _WIN32 */
#endif /* _WIN32 */
	void wait_until_sent(); // ���M�L���[����ɂȂ�܂ő҂�
};

class Sock::client_thread : public network_thread { // �N���C�A���g�̃X���b�h
public:
	client_thread(Sock* callee) : network_thread(callee) {}
protected:
	int establishConnection (); // �ڑ����m������
};

class Sock::server_thread : public network_thread { // �T�[�o�[�̃X���b�h
public:
	server_thread(Sock* callee) : network_thread(callee) {}
	void setsock (SocketDescriptor* const socket, SocketDescriptor* const lsocket); // �\�P�b�g��ݒ肷��
protected:
	int establishConnection (); // �ڑ����m������
private:
	void setsock (SocketDescriptor* const socket); // �\�P�b�g��ݒ肷��
};

}
