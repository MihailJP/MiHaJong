#ifndef SOCKET_H
#define SOCKET_H

#include <WinSock2.h>

#define NUM_OF_SOCKETS 32

#ifdef SOCKET_EXPORTS
#include "except.h"
#define DLL __declspec(dllexport)
class Sock;
extern WSADATA SocketInfo;
extern HINSTANCE dllInst;
#else
#define DLL __declspec(dllimport)
#endif

DLL int socket_init (); // ソケットを初期化する
DLL int socket_bye (); // ソケットのクリンナップ

#endif
