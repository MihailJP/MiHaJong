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
	CreateThread(NULL, 0, client_thread::thread, (LPVOID)threadPtr.client, 0, NULL);
}

bool mihajong_socket::Sock::connected () { // 接続されているかを確認
	if (isServer) {
		return false; // 仮置き
	} else {
		threadPtr.client->chkError();
		return threadPtr.client->isConnected();
	}
};

void mihajong_socket::Sock::disconnect () { // 接続を切る
	if (isServer) {
		// 仮置き
	} else {
		threadPtr.client->terminate();
	}
	closesocket(sock);
	if (isServer) {
		// 仮置き
	} else {
		delete threadPtr.client;
		threadPtr.client = NULL;
	}
}

// -------------------------------------------------------------------------

mihajong_socket::Sock::client_thread::client_thread() {
	errtype = errNone; errcode = 0;
	connected = terminated = finished = false;
}

DWORD WINAPI mihajong_socket::Sock::client_thread::thread(LPVOID lp) { // スレッドを起動するための処理
	return ((client_thread*)lp)->myThreadFunc();
}

void mihajong_socket::Sock::client_thread::chkError () { // エラーをチェックし、もしエラーだったら例外を投げる
	switch (errtype) {
	case errNone: // エラーなし
		break;
	case errConnection: // 接続失敗
		throw connection_failure(errcode);
	default: // ほかのエラー
		throw socket_error(errcode);
	}
}

DWORD WINAPI mihajong_socket::Sock::client_thread::myThreadFunc() { // スレッドの処理
	if (::connect(*mySock, (sockaddr*)&myAddr, sizeof(myAddr)) == SOCKET_ERROR) { // 接続
		errtype = errConnection; errcode = WSAGetLastError(); ExitThread(-((int)errtype));
	}
	// TODO: ここにループを書く
	return S_OK;
}

bool mihajong_socket::Sock::client_thread::isConnected() { // 接続済かを返す関数
	return connected;
}

void mihajong_socket::Sock::client_thread::setaddr (const sockaddr_in destination) { // 接続先を設定する
	myAddr = destination;
}
void mihajong_socket::Sock::client_thread::setsock (SOCKET* const socket) { // ソケットを設定する
	mySock = socket;
}

void mihajong_socket::Sock::client_thread::terminate () { // 切断する
	terminated = true; // フラグを立てる
	while (!finished) Sleep(0); // スレッドが終了するまで待つ
	finished = terminated = connected = false; // フラグの後始末
	errtype = errNone; errcode = 0;
}
