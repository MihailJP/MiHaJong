#pragma once

#ifdef _WIN32
#ifndef _WINSOCKAPI_
#include <winsock2.h>
#endif
#else /* _WIN32 */
#include <sys/socket.h>
#endif /* _WIN32 */
#include <cstdarg>
#include "../common/strcode.h"

#define NUM_OF_SOCKETS 32

#if !defined(_WIN32)
#define DLL /**/
#elif defined(SOCKET_EXPORTS)
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#ifdef SOCKET_EXPORTS
#include "class.h"
#include "except.h"
#else
#include "protocol.h"
#include "server.h"
#include "client.h"
#include "logger.h"
#endif
#include <array>

namespace mihajong_socket {
#ifdef SOCKET_EXPORTS
class Sock;
constexpr unsigned int numOfSockets = 32u;
extern std::array<Sock*, numOfSockets> sockets;
#ifdef _WIN32
extern WSADATA SocketInfo;
extern HINSTANCE dllInst;
#endif /* _WIN32 */

void errordlg (socket_error& err); // エラーダイアログ
#endif

DLL int init (); // ソケットを初期化する
DLL int listen (int sock_id, int port); // サーバー待ち受け開始
DLL int connect (int sock_id, const char* const addr, int port); // クライアント接続開始
DLL int connected (int sock_id); // 接続されているか確認
DLL int putc (int sock_id, int byte); // 1バイト送信
DLL int puts (int sock_id, LPCTSTR const str); // 文字列送信
DLL int getc (int sock_id); // 1バイト受信
DLL int gets (int sock_id, LPTSTR stringline, int bufsize); // 1行受信
DLL int hangup (int sock_id); // 接続を切る
DLL int bye (); // ソケットのクリンナップ

}
