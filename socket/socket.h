#ifndef SOCKET_H
#define SOCKET_H

#include <WinSock2.h>
#include "class.h"

#define NUM_OF_SOCKETS 32

#ifdef SOCKET_EXPORTS
#include "except.h"
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

namespace mihajong_socket {
#ifdef SOCKET_EXPORTS
class Sock;
extern WSADATA SocketInfo;
extern HINSTANCE dllInst;
#else
#define DLL __declspec(dllimport)
#endif

void errordlg (socket_error& err); // �G���[�_�C�A���O
DLL int init (); // �\�P�b�g������������
DLL int listen (int sock_id, int port); // �T�[�o�[�҂��󂯊J�n
DLL int connect (int sock_id, const char* const addr, int port); // �N���C�A���g�ڑ��J�n
DLL int connected (int sock_id); // �ڑ�����Ă��邩�m�F
DLL int putc (int sock_id, int byte); // 1�o�C�g���M
DLL int puts (int sock_id, const char* const str); // �����񑗐M
DLL int getc (int sock_id); // 1�o�C�g��M
DLL int bye (); // �\�P�b�g�̃N�����i�b�v

}
#endif