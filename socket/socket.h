#ifndef SOCKET_H
#define SOCKET_H

#ifndef _WINSOCKAPI_
#include <WinSock2.h>
#endif
#include <cstdarg>

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
#endif

namespace mihajong_socket {
#ifdef SOCKET_EXPORTS
class Sock;
const unsigned int numOfSockets = 32u;
extern Sock* sockets[numOfSockets];
extern WSADATA SocketInfo;
extern HINSTANCE dllInst;

void errordlg (socket_error& err); // エラーダイアログ
#endif

DLL int init (); // ソケットを初期化する
DLL int listen (int sock_id, int port); // サーバー待ち受け開始
DLL int connect (int sock_id, const char* const addr, int port); // クライアント接続開始
DLL int connected (int sock_id); // 接続されているか確認
DLL int putc (int sock_id, int byte); // 1バイト送信
DLL int puts (int sock_id, const char* const str); // 文字列送信
DLL int getc (int sock_id); // 1バイト受信
DLL int gets (int sock_id, char* const stringline, int bufsize); // 1行受信
DLL int hangup (int sock_id); // 接続を切る
DLL int bye (); // ソケットのクリンナップ

}
#endif
