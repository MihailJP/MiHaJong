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
	class network_thread; // スレッド(スーパークラス)
	class client_thread; // クライアントのスレッド
	class server_thread; // サーバーのスレッド
	union Thread { // スレッドオブジェクトのポインタ(共用体)
	public:
		client_thread* client;
		server_thread* server;
	};
	static uint32_t addr2var(const std::string& address); // アドレスを取得
	bool isServer;
	sockaddr_in addr;
	SocketDescriptor sock, lsock;
	Thread threadPtr;
	uint16_t portnum;
public:
	Sock () {}; // ソケット初期化
	Sock (uint16_t port); // サーバー開始
	Sock (const std::string& destination, uint16_t port); // クライアント接続
	~Sock (); // 接続を切る
	void listen (uint16_t port); // サーバー開始
	void listen (); // サーバー開始
	void connect (const std::string& destination, uint16_t port); // クライアント接続
	void connect (); // クライアント再接続
	bool connected (); // 接続されているかを確認
	void wait_until_connected (); // 文字通りのことをやる
	unsigned char getc (); // 読み込み(非同期)
	unsigned char syncgetc (); // 読み込み(同期)
	CodeConv::tstring gets (); // 一行読み込み
	void putc (unsigned char byte); // 書き込み
	void puts (const CodeConv::tstring& str); // 文字列書き込み
	void disconnect (); // 接続を切る
};

class Sock::network_thread { // スレッド(スーパークラス)
public:
	network_thread(Sock* caller);
	virtual ~network_thread();
#ifdef _WIN32
	static DWORD WINAPI thread(LPVOID lp); // スレッドを起動するための処理
#else /* _WIN32 */
#endif /* _WIN32 */
	bool isConnected (); // 接続済かを返す関数
	void setaddr (const sockaddr_in destination); // 接続先を設定する
	void setsock (SocketDescriptor* const socket); // ソケットを設定する
	void terminate (); // 切断する
	void chkError (); // エラーをチェックし、もしエラーだったら例外を投げる
	unsigned char read (); // 1バイト読み込み
	void write (unsigned char byte); // 1バイト書き込み
	CodeConv::tstring readline (); // 1行読み込み
protected:
	Sock* myCaller;
	enum errorType {errNone, errListen, errAccept, errConnection, errRecv, errSend};
	static const unsigned int bufsize = 65536;
	SocketDescriptor* mySock; // ソケット(ポインタ)
	SocketDescriptor* listenerSock; // ソケット(ポインタ)
	errorType errtype; // エラーの種類
	int errcode; // エラーコード
	volatile bool connecting; // 接続中かのフラグ[ワーカースレッドから書き込み]
	volatile bool connected; // 接続済みかのフラグ[ワーカースレッドから書き込み]
	volatile bool terminated; // 接続済みかのフラグ[親スレッドから書き込み]
	volatile bool send_ended; // 送信が全て終わったかのフラグ
	volatile bool sender_closed; // 送信が全て終わったかのフラグ
	volatile bool receive_ended; // 受信が全て終わったかのフラグ
	volatile bool receiver_closed; // 受信が全て終わったかのフラグ
	volatile bool finished; // 終了済みかのフラグ[ワーカースレッドから書き込み]
	sockaddr_in myAddr; // アドレス情報[親スレッドから書き込み]
	std::queue<unsigned char> myMailBox; // 受け取ったバイト列
	MHJMutex myRecvQueueCS; // 受信バッファ用ミューテックス(クリティカルセクションに変更)
	std::queue<unsigned char> mySendBox; // 送る予定のバイト列
	MHJMutex mySendQueueCS; // 送信バッファ用ミューテックス(クリティカルセクションに変更)
	virtual int establishConnection () = 0; // 接続を確立する
	int reader (); // 読み込み
	int writer (); // 書き込み
#ifdef _WIN32
	DWORD WINAPI myThreadFunc(); // スレッドの処理
#else /* _WIN32 */
#endif /* _WIN32 */
	void wait_until_sent(); // 送信キューが空になるまで待つ
};

class Sock::client_thread : public network_thread { // クライアントのスレッド
public:
	client_thread(Sock* callee) : network_thread(callee) {}
protected:
	int establishConnection (); // 接続を確立する
};

class Sock::server_thread : public network_thread { // サーバーのスレッド
public:
	server_thread(Sock* callee) : network_thread(callee) {}
	void setsock (SocketDescriptor* const socket, SocketDescriptor* const lsocket); // ソケットを設定する
protected:
	int establishConnection (); // 接続を確立する
private:
	void setsock (SocketDescriptor* const socket); // ソケットを設定する
};

}
