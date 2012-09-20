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

int mihajong_socket::Sock::getc () { // 読み込み
	try {
		if (isServer) { // 仮置き
			throw socket_error("Not yet implemented");
		} else {
			return (int)threadPtr.client->read();
			threadPtr.client->chkError();
		}
	}
	catch (queue_empty) { // キューが空だったら
		return -1;
	}
}

void mihajong_socket::Sock::putc (unsigned char byte) { // 書き込み
	if (isServer) {
		// 仮置き
	} else {
		threadPtr.client->write(byte);
		threadPtr.client->chkError();
	}
}

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
	myRecvQueueMutex = CreateMutex(NULL, TRUE, NULL);
	mySendQueueMutex = CreateMutex(NULL, TRUE, NULL);
}

mihajong_socket::Sock::client_thread::~client_thread() {
	CloseHandle(myRecvQueueMutex);
	CloseHandle(mySendQueueMutex);
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
	case errRecv: // 受信失敗
		throw recv_error(errcode);
	case errSend: // 送信失敗
		throw send_error(errcode);
	default: // ほかのエラー
		throw socket_error(errcode);
	}
}

DWORD WINAPI mihajong_socket::Sock::client_thread::myThreadFunc() { // スレッドの処理
	if (::connect(*mySock, (sockaddr*)&myAddr, sizeof(myAddr)) == SOCKET_ERROR) { // 接続
		errtype = errConnection; errcode = WSAGetLastError(); return -((int)errtype);
	}
	connected = true; // 接続済みフラグを立てる
	u_long arg = 1;
	ioctlsocket(*mySock, FIONBIO, &arg); // non-blocking モードに設定
	while (!terminated) { // 停止されるまで
		{ // 受信処理
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
		}
		{ // 送信処理
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
		}
	}
	finished = true;
	return S_OK;
}

unsigned char mihajong_socket::Sock::client_thread::read () { // 1バイト読み込み
	unsigned char byte; bool empty = false;
	WaitForSingleObject(myRecvQueueMutex, 0); // 受信用ミューテックスを取得
	if (myMailBox.empty()) empty = true; // キューが空の場合
	else {byte = myMailBox.front(); myMailBox.pop();} // 空でなければ取り出す
	ReleaseMutex(myRecvQueueMutex); // 受信用ミューテックスを解放
	if (empty) throw queue_empty(); // 空だったら例外
	else return byte; // そうでなければ取り出した値を返す
}

void mihajong_socket::Sock::client_thread::write (unsigned char byte) { // 1バイト書き込み
	WaitForSingleObject(mySendQueueMutex, 0); // 送信用ミューテックスを取得
	mySendBox.push(byte); // キューに追加
	ReleaseMutex(mySendQueueMutex); // 送信用ミューテックスを解放
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
