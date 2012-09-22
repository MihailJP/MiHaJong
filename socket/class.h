#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <cstdint>
#include <queue>
#include <WinSock2.h>
#include "except.h"

namespace mihajong_socket {

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
	SOCKET sock, lsock;
	Thread threadPtr;
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
	unsigned char getc (); // 読み込み
	void putc (unsigned char byte); // 読み込み
	void disconnect (); // 接続を切る
};

class Sock::network_thread { // スレッド(スーパークラス)
public:
	network_thread();
	virtual ~network_thread();
	static DWORD WINAPI thread(LPVOID lp); // スレッドを起動するための処理
	bool isConnected (); // 接続済かを返す関数
	void setaddr (const sockaddr_in destination); // 接続先を設定する
	void setsock (SOCKET* const socket); // ソケットを設定する
	void terminate (); // 切断する
	void chkError (); // エラーをチェックし、もしエラーだったら例外を投げる
	unsigned char read (); // 1バイト読み込み
	void write (unsigned char byte); // 1バイト書き込み
protected:
	enum errorType {errNone, errListen, errAccept, errConnection, errRecv, errSend};
	static const unsigned int bufsize = 65536;
	SOCKET* mySock; // ソケット(ポインタ)
	SOCKET* listenerSock; // ソケット(ポインタ)
	errorType errtype; // エラーの種類
	int errcode; // エラーコード
	bool connected; // 接続済みかのフラグ[ワーカースレッドから書き込み]
	bool terminated; // 接続済みかのフラグ[親スレッドから書き込み]
	bool finished; // 終了済みかのフラグ[ワーカースレッドから書き込み]
	sockaddr_in myAddr; // アドレス情報[親スレッドから書き込み]
	std::queue<unsigned char> myMailBox; // 受け取ったバイト列
	HANDLE myRecvQueueMutex; // 受信バッファ用ミューテックス
	std::queue<unsigned char> mySendBox; // 送る予定のバイト列
	HANDLE mySendQueueMutex; // 送信バッファ用ミューテックス
	virtual int establishConnection () = 0; // 接続を確立する
	int reader (); // 読み込み
	int writer (); // 書き込み
	DWORD WINAPI myThreadFunc(); // スレッドの処理
};

class Sock::client_thread : public network_thread { // クライアントのスレッド
protected:
	int establishConnection (); // 接続を確立する
};

class Sock::server_thread : public network_thread { // サーバーのスレッド
public:
	void setsock (SOCKET* const socket, SOCKET* const lsocket); // ソケットを設定する
protected:
	int establishConnection (); // 接続を確立する
private:
	void setsock (SOCKET* const socket); // ソケットを設定する
};

}
#endif
