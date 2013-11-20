#pragma once

#include <string>
#include <cstdint>
#include <queue>
#include <sstream>
#include <iomanip>
#include "../common/mutex.h"
#include "../common/thread.h"
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
	static uint32_t addr2var(const std::string& address); // �A�h���X���擾
	bool isServer;
	sockaddr_in addr;
	SocketDescriptor sock, lsock;
	network_thread* threadPtr;
	uint16_t portnum;
public:
	Sock () {} // �\�P�b�g������
	explicit Sock (uint16_t port); // �T�[�o�[�J�n
	Sock (const std::string& destination, uint16_t port); // �N���C�A���g�ڑ�
	Sock (const Sock&) = delete; // Delete unexpected copy constructor
	Sock& operator= (const Sock&) = delete; // Delete unexpected assign operator
	~Sock(); // �ڑ���؂�
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
	explicit network_thread(Sock* caller);
	network_thread(const network_thread&) = delete; // Delete unexpected copy constructor
	network_thread& operator= (const network_thread&) = delete; // Delete unexpected assign operator
	virtual ~network_thread();
	static void thread(network_thread* Instance); // �X���b�h���N�����邽�߂̏���
	virtual void startThread () = 0; // �X���b�h���J�n����
	bool isConnected (); // �ڑ��ς���Ԃ��֐�
	void setaddr (const sockaddr_in destination); // �ڑ����ݒ肷��
	void setsock (SocketDescriptor* const socket, SocketDescriptor* const lsocket = nullptr); // �\�P�b�g��ݒ肷��
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
	MUTEXLIB::recursive_mutex myRecvQueueCS; // ��M�o�b�t�@�p�~���[�e�b�N�X(�N���e�B�J���Z�N�V�����ɕύX)
	std::queue<unsigned char> mySendBox; // ����\��̃o�C�g��
	MUTEXLIB::recursive_mutex mySendQueueCS; // ���M�o�b�t�@�p�~���[�e�b�N�X(�N���e�B�J���Z�N�V�����ɕύX)
	virtual int establishConnection () = 0; // �ڑ����m������
	int reader (); // �ǂݍ���
	int writer (); // ��������
	THREADLIB::thread myThread;
	int myThreadFunc(); // �X���b�h�̏���
	void wait_until_sent(); // ���M�L���[����ɂȂ�܂ő҂�
};

class Sock::client_thread : public network_thread { // �N���C�A���g�̃X���b�h
public:
	client_thread(Sock* callee) : network_thread(callee) {}
	client_thread(const client_thread&) = delete; // Delete unexpected copy constructor
	client_thread& operator= (const client_thread&) = delete; // Delete unexpected assign operator
	void startThread(); // �X���b�h���J�n����
protected:
	int establishConnection () override; // �ڑ����m������
};

class Sock::server_thread : public network_thread { // �T�[�o�[�̃X���b�h
public:
	server_thread(Sock* callee) : network_thread(callee) {}
	server_thread(const server_thread&) = delete; // Delete unexpected copy constructor
	server_thread& operator= (const server_thread&) = delete; // Delete unexpected assign operator
	void startThread(); // �X���b�h���J�n����
protected:
	int establishConnection () override; // �ڑ����m������
};

}
