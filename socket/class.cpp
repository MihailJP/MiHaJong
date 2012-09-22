#include "class.h"

uint32_t mihajong_socket::Sock::addr2var(const std::string& address) { // アドレスを取得
	uint32_t addr = inet_addr(address.c_str()); // まずは xxx.xxx.xxx.xxx 形式であると仮定する
	if ((addr == INADDR_NONE) || (addr == INADDR_ANY /* XP以前で空の場合 */)) { // 失敗した場合
		hostent* host = gethostbyname(address.c_str()); // ホスト名を解釈
		if (host == NULL) throw invalid_address(WSAGetLastError()); // 失敗したら例外を投げる
		addr = *(uint32_t *)host->h_addr_list[0]; // 成功したらそのアドレス
	}
	return addr;
}

mihajong_socket::Sock::Sock (const std::string& destination, uint16_t port) { // クライアント接続
	sock = socket(AF_INET, SOCK_STREAM, 0); // ソケットを初期化
	if (sock == INVALID_SOCKET) throw socket_creation_error(WSAGetLastError()); // ソケットの作成に失敗したら例外

	this->connect(destination, port); // 接続
}

mihajong_socket::Sock::~Sock() { // 接続を切る
	this->disconnect();
}

mihajong_socket::Sock::Sock (uint16_t port) { // クライアント接続
	this->listen(port); // 接続
}

void mihajong_socket::Sock::listen (uint16_t port) { // サーバー開始
	lsock = socket(AF_INET, SOCK_STREAM, 0); // ソケットを初期化
	if (lsock == INVALID_SOCKET) throw socket_creation_error(WSAGetLastError()); // ソケットの作成に失敗したら例外
	isServer = true; // サーバーである
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // ポート番号
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // アドレス
	if (bind(lsock, (sockaddr *)&addr, sizeof(addr)))
		throw socket_bind_error(WSAGetLastError()); // 失敗したら例外を投げる
	this->listen();
}

void mihajong_socket::Sock::listen () { // サーバー開始
	threadPtr.server = new server_thread();
	threadPtr.server->setaddr(addr);
	threadPtr.server->setsock(&sock, &lsock);
	CreateThread(NULL, 0, server_thread::thread, (LPVOID)threadPtr.server, 0, NULL);
}

void mihajong_socket::Sock::connect (const std::string& destination, uint16_t port) { // クライアント接続
	isServer = false; // クライアントである
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // ポート番号
	addr.sin_addr.S_un.S_addr = addr2var(destination.c_str()); // アドレス
	this->connect();
}

void mihajong_socket::Sock::connect () { // クライアント再接続
	threadPtr.client = new client_thread();
	threadPtr.client->setaddr(addr);
	threadPtr.client->setsock(&sock);
	CreateThread(NULL, 0, client_thread::thread, (LPVOID)threadPtr.client, 0, NULL);
}

bool mihajong_socket::Sock::connected () { // 接続されているかを確認
	if (isServer) {
		threadPtr.server->chkError();
		return threadPtr.server->isConnected();
	} else {
		threadPtr.client->chkError();
		return threadPtr.client->isConnected();
	}
};

unsigned char mihajong_socket::Sock::getc () { // 読み込み
	if (isServer) {
		threadPtr.server->chkError();
		return threadPtr.server->read();
	} else {
		threadPtr.client->chkError();
		return threadPtr.client->read();
	}
}

void mihajong_socket::Sock::putc (unsigned char byte) { // 書き込み
	if (isServer) {
		threadPtr.server->write(byte);
		threadPtr.server->chkError();
	} else {
		threadPtr.client->write(byte);
		threadPtr.client->chkError();
	}
}

void mihajong_socket::Sock::disconnect () { // 接続を切る
	if (isServer) {
		threadPtr.server->terminate();
	} else {
		threadPtr.client->terminate();
	}
	closesocket(sock);
	if (isServer) {
		delete threadPtr.server;
		threadPtr.server = NULL;
	} else {
		delete threadPtr.client;
		threadPtr.client = NULL;
	}
}

// -------------------------------------------------------------------------

mihajong_socket::Sock::network_thread::network_thread() {
	errtype = errNone; errcode = 0;
	connected = terminated = finished = false;
	myRecvQueueMutex = CreateMutex(NULL, TRUE, NULL);
	mySendQueueMutex = CreateMutex(NULL, TRUE, NULL);
}

mihajong_socket::Sock::network_thread::~network_thread() {
	CloseHandle(myRecvQueueMutex);
	CloseHandle(mySendQueueMutex);
}

DWORD WINAPI mihajong_socket::Sock::network_thread::thread(LPVOID lp) { // スレッドを起動するための処理
	return ((client_thread*)lp)->myThreadFunc();
}

void mihajong_socket::Sock::network_thread::chkError () { // エラーをチェックし、もしエラーだったら例外を投げる
	switch (errtype) {
	case errNone: // エラーなし
		break;
	case errListen: // listen失敗
		throw listen_failure(errcode);
	case errAccept: // accept失敗
		throw accept_failure(errcode);
	case errConnection: // 接続失敗
		throw connection_failure(errcode);
	case errRecv: // 受信失敗
		throw recv_error(errcode);
	case errSend: // 送信失敗
		throw send_error(errcode);
	default: // ほかのエラー
		throw socket_error(errcode);
	}
}

int mihajong_socket::Sock::network_thread::reader() { // 受信処理
	unsigned char buf[bufsize] = {0,};
	WSABUF buffer; buffer.buf = reinterpret_cast<CHAR*>(buf); buffer.len = bufsize;
	DWORD recvsz; DWORD flag = 0;
	if (WSARecv(*mySock, &buffer, 1, &recvsz, &flag, NULL, NULL) == 0) { // 受信する
		WaitForSingleObject(myRecvQueueMutex, 0); // 受信用ミューテックスを取得
		for (unsigned int i = 0; i < recvsz; ++i) myMailBox.push(buf[i]); // キューに追加
		ReleaseMutex(myRecvQueueMutex); // 受信用ミューテックスを解放
	} else { // 受信できない時
		switch (int err = WSAGetLastError()) {
		case WSAEWOULDBLOCK:
			break; // データがない場合
		default: // エラー処理
			errtype = errRecv; errcode = err; terminated = finished = true; connected = false;
			return -((int)errtype);
		}
	}
	return 0;
}

int mihajong_socket::Sock::network_thread::writer() { // 送信処理
	unsigned char buf[bufsize] = {0,};
	WSABUF buffer; buffer.buf = reinterpret_cast<CHAR*>(buf); buffer.len = bufsize;
	DWORD sendsz = 0;

	WaitForSingleObject(mySendQueueMutex, 0); // 送信用ミューテックスを取得
	while (!mySendBox.empty()) {
		buf[sendsz++] = mySendBox.front(); mySendBox.pop(); // キューから取り出し
	}
	ReleaseMutex(mySendQueueMutex); // 送信用ミューテックスを解放
	if (sendsz && (WSASend(*mySock, &buffer, 1, &sendsz, 0, NULL, NULL))) { // 送信
		switch (int err = WSAGetLastError()) {
		case WSAEWOULDBLOCK:
			break; // このエラーは無視する
		default:
			errtype = errSend; errcode = err; terminated = finished = true; connected = false;
			return -((int)errtype);
		}
	}
	return 0;
}

DWORD WINAPI mihajong_socket::Sock::network_thread::myThreadFunc() { // スレッドの処理
	if (int err = establishConnection()) return err; // 接続
	u_long arg = 1;
	ioctlsocket(*mySock, FIONBIO, &arg); // non-blocking モードに設定
	while (!terminated) { // 停止されるまで
		if (int err = reader()) return err; // 読み込み
		if (int err = writer()) return err; // 書き込み
	}
	finished = true;
	return S_OK;
}

unsigned char mihajong_socket::Sock::network_thread::read () { // 1バイト読み込み
	unsigned char byte; bool empty = false;
	WaitForSingleObject(myRecvQueueMutex, 0); // 受信用ミューテックスを取得
	if (myMailBox.empty()) empty = true; // キューが空の場合
	else {byte = myMailBox.front(); myMailBox.pop();} // 空でなければ取り出す
	ReleaseMutex(myRecvQueueMutex); // 受信用ミューテックスを解放
	if (empty) throw queue_empty(); // 空だったら例外
	else return byte; // そうでなければ取り出した値を返す
}

void mihajong_socket::Sock::network_thread::write (unsigned char byte) { // 1バイト書き込み
	WaitForSingleObject(mySendQueueMutex, 0); // 送信用ミューテックスを取得
	mySendBox.push(byte); // キューに追加
	ReleaseMutex(mySendQueueMutex); // 送信用ミューテックスを解放
}

bool mihajong_socket::Sock::network_thread::isConnected() { // 接続済かを返す関数
	return connected;
}

void mihajong_socket::Sock::network_thread::setaddr (const sockaddr_in destination) { // 接続先を設定する
	myAddr = destination;
}
void mihajong_socket::Sock::network_thread::setsock (SOCKET* const socket) { // ソケットを設定する
	mySock = socket;
}

void mihajong_socket::Sock::network_thread::terminate () { // 切断する
	terminated = true; // フラグを立てる
	while (!finished) Sleep(0); // スレッドが終了するまで待つ
	finished = terminated = connected = false; // フラグの後始末
	errtype = errNone; errcode = 0;
}

// -------------------------------------------------------------------------

int mihajong_socket::Sock::client_thread::establishConnection() { // 接続を確立する
	if (::connect(*mySock, (sockaddr*)&myAddr, sizeof(myAddr)) == SOCKET_ERROR) { // 接続
		errtype = errConnection; errcode = WSAGetLastError(); return -((int)errtype);
	}
	connected = true; // 接続済みフラグを立てる
	return 0;
}

int mihajong_socket::Sock::server_thread::establishConnection() { // 接続を確立する
	if (::listen(*listenerSock, SOMAXCONN) == SOCKET_ERROR) { // 待機
		errtype = errListen; errcode = WSAGetLastError(); return -((int)errtype);
	}
	*mySock = accept(*listenerSock, NULL, 0);
	if (*mySock == INVALID_SOCKET) { // ソケットの作成に失敗場合
		errtype = errAccept; errcode = WSAGetLastError();
		closesocket(*listenerSock);
		return -((int)errtype);
	}
	closesocket(*listenerSock);
	connected = true; // 接続済みフラグを立てる
	return 0;
}

void mihajong_socket::Sock::server_thread::setsock (SOCKET* const socket, SOCKET* const lsocket) { // ソケットを設定する
	mySock = socket; listenerSock = lsocket;
}
