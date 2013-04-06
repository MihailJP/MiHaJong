#pragma once

#ifndef _WINSOCKAPI_
#include <winsock2.h>
#endif
#include <cstdarg>
#include "../common/strcode.h"

#define NUM_OF_SOCKETS 32

#ifdef SOCKET_EXPORTS
#include "class.h"
#include "except.h"
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#include "protocol.h"
#include "server.h"
#include "client.h"
#include "logger.h"
#endif

namespace mihajong_socket {
#ifdef SOCKET_EXPORTS
class Sock;
const unsigned int numOfSockets = 32u;
extern Sock* sockets[numOfSockets];
extern WSADATA SocketInfo;
extern HINSTANCE dllInst;

void errordlg (socket_error& err); // �G���[�_�C�A���O
#endif

DLL int init (); // �\�P�b�g������������
DLL int listen (int sock_id, int port); // �T�[�o�[�҂��󂯊J�n
DLL int connect (int sock_id, LPCSTR const addr, int port); // �N���C�A���g�ڑ��J�n
DLL int connected (int sock_id); // �ڑ�����Ă��邩�m�F
DLL int putc (int sock_id, int byte); // 1�o�C�g���M
DLL int puts (int sock_id, LPCTSTR const str); // �����񑗐M
DLL int getc (int sock_id); // 1�o�C�g��M
DLL int gets (int sock_id, LPTSTR stringline, int bufsize); // 1�s��M
DLL int hangup (int sock_id); // �ڑ���؂�
DLL int bye (); // �\�P�b�g�̃N�����i�b�v

}
