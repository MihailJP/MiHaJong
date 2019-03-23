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
#include <WS2tcpip.h>
#endif
#else /* _WIN32 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif /* _WIN32 */
#include "except.h"
#include "logger.h"
#include "../common/clock.h"

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
	bool isServer;
	addrinfo* addrInfo;
	SocketDescriptor sock, lsock;
	network_thread* threadPtr;
	MUTEXLIB::recursive_mutex threadExistenceMutex;
	uint16_t portnum();
public:
	Sock () : isServer(false), addrInfo(nullptr), sock(0), lsock(0), threadPtr(nullptr) {} // ソケット初期化
	explicit Sock (uint16_t port); // サーバー開始
	Sock (const std::string& destination, uint16_t port); // クライアント接続
	Sock (const Sock&) = delete; // Delete unexpected copy constructor
	Sock& operator= (const Sock&) = delete; // Delete unexpected assign operator
	~Sock(); // 接続を切る
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
	explicit network_thread(Sock* caller);
	network_thread(const network_thread&) = delete; // Delete unexpected copy constructor
	network_thread& operator= (const network_thread&) = delete; // Delete unexpected assign operator
	virtual ~network_thread();
	static void thread(network_thread* Instance); // スレッドを起動するための処理
	virtual void startThread () = 0; // スレッドを開始する
	bool isConnected (); // 接続済かを返す関数
	void setaddr (const addrinfo* destination); // 接続先を設定する
	void setsock (SocketDescriptor* const socket, SocketDescriptor* const lsocket = nullptr); // ソケットを設定する
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
	volatile TimerMicrosec terminate_time; // 打ち切り命令の時間
	static const TimerMicrosec disconnection_timeout = 3000000ULL; // 切断処理タイムアウト
	const addrinfo* myAddr; // アドレス情報[親スレッドから書き込み]
	std::queue<unsigned char> myMailBox; // 受け取ったバイト列
	MUTEXLIB::recursive_mutex myRecvQueueCS; // 受信バッファ用ミューテックス(クリティカルセクションに変更)
	std::queue<unsigned char> mySendBox; // 送る予定のバイト列
	MUTEXLIB::recursive_mutex mySendQueueCS; // 送信バッファ用ミューテックス(クリティカルセクションに変更)
	virtual int establishConnection () = 0; // 接続を確立する
	int reader (); // 読み込み
	int writer (); // 書き込み
	THREADLIB::thread myThread;
	int myThreadFunc(); // スレッドの処理
	void wait_until_sent(); // 送信キューが空になるまで待つ
};

class Sock::client_thread : public network_thread { // クライアントのスレッド
public:
	client_thread(Sock* callee) : network_thread(callee) {}
	client_thread(const client_thread&) = delete; // Delete unexpected copy constructor
	client_thread& operator= (const client_thread&) = delete; // Delete unexpected assign operator
	void startThread(); // スレッドを開始する
protected:
	int establishConnection () override; // 接続を確立する
};

class Sock::server_thread : public network_thread { // サーバーのスレッド
public:
	server_thread(Sock* callee) : network_thread(callee) {}
	server_thread(const server_thread&) = delete; // Delete unexpected copy constructor
	server_thread& operator= (const server_thread&) = delete; // Delete unexpected assign operator
	void startThread(); // スレッドを開始する
protected:
	int establishConnection () override; // 接続を確立する
};

}
