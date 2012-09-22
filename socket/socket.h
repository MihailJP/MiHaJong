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

void errordlg (socket_error& err); // エラーダイアログ
DLL int socket_init (); // ソケットを初期化する
DLL int listen (int sock_id, int port); // サーバー待ち受け開始
DLL int connect (int sock_id, const char* const addr, int port); // クライアント接続開始
DLL int socket_bye (); // ソケットのクリンナップ

}
#endif
