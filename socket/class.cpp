#include "class.h"

uint32_t mihajong_socket::Sock::addr2var(const std::string& address) { // アドレスを取得
	uint32_t addr = inet_addr(address.c_str()); // まずは xxx.xxx.xxx.xxx 形式であると仮定する
	if ((addr == INADDR_NONE) || (addr == INADDR_ANY /* XP以前で空の場合 */)) { // 失敗した場合
		hostent* host = gethostbyname(address.c_str()); // ホスト名を解釈
		if (host == nullptr) throw invalid_address(WSAGetLastError()); // 失敗したら例外を投げる
		addr = *(uint32_t *)host->h_addr_list[0]; // 成功したらそのアドレス
	}
	return addr;
}

mihajong_socket::Sock::Sock (const std::string& destination, uint16_t port) { // クライアント接続
	{
		CodeConv::tostringstream o;
		o << _T("ソケットオブジェクトコンストラクタ(クライアント) 宛先 [") << CodeConv::EnsureTStr(destination) << _T("] ポート [") << port << _T("]");
		info(o.str().c_str());
	}
	this->connect(destination, port); // 接続
	info(_T("コンストラクタの処理が完了しました"));
}

mihajong_socket::Sock::~Sock() { // 接続を切る
	info(_T("ソケットオブジェクトデストラクタ"));
	this->disconnect();
	info(_T("デストラクタの処理が完了しました"));
}

mihajong_socket::Sock::Sock (uint16_t port) { // サーバ接続
	{
		CodeConv::tostringstream o;
		o << _T("ソケットオブジェクトコンストラクタ(サーバ) ポート [") << port << _T("]");
		info(o.str().c_str());
	}
	this->listen(port); // 接続
	info(_T("コンストラクタの処理が完了しました"));
}

void mihajong_socket::Sock::listen (uint16_t port) { // サーバー開始
	{
		CodeConv::tostringstream o;
		o << _T("接続待ちを開始します ポート [") << port << _T("]");
		info(o.str().c_str());
	}
	portnum = port;
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
	threadPtr.server = new server_thread(this);
	threadPtr.server->setaddr(addr);
	threadPtr.server->setsock(&sock, &lsock);
	CreateThread(nullptr, 0, server_thread::thread, (LPVOID)threadPtr.server, 0, nullptr);
}

void mihajong_socket::Sock::connect (const std::string& destination, uint16_t port) { // クライアント接続
	{
		CodeConv::tostringstream o;
		o << _T("接続を開始します 宛先 [") << CodeConv::EnsureTStr(destination) << _T("] ポート [") << port << _T("]");
		info(o.str().c_str());
	}
	portnum = port;
	sock = socket(AF_INET, SOCK_STREAM, 0); // ソケットを初期化
	if (sock == INVALID_SOCKET) throw socket_creation_error(WSAGetLastError()); // ソケットの作成に失敗したら例外

	isServer = false; // クライアントである
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // ポート番号
	addr.sin_addr.S_un.S_addr = addr2var(destination.c_str()); // アドレス
	this->connect();
}

void mihajong_socket::Sock::connect () { // クライアント再接続
	threadPtr.client = new client_thread(this);
	threadPtr.client->setaddr(addr);
	threadPtr.client->setsock(&sock);
	CreateThread(nullptr, 0, client_thread::thread, (LPVOID)threadPtr.client, 0, nullptr);
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

void mihajong_socket::Sock::wait_until_connected () { // 文字通りのことをやる
	{
		CodeConv::tostringstream o;
		o << _T("接続待機 ポート [") << portnum << _T("]");
		info(o.str().c_str());
	}
	while (!connected()) Sleep(50);
	{
		CodeConv::tostringstream o;
		o << _T("接続待機完了 ポート [") << portnum << _T("]");
		info(o.str().c_str());
	}
};
unsigned char mihajong_socket::Sock::getc () { // 読み込み(非同期)
	unsigned char byte;
	if (isServer) {
		threadPtr.server->chkError();
		byte = threadPtr.server->read();
	} else {
		threadPtr.client->chkError();
		byte = threadPtr.client->read();
	}
	{
		CodeConv::tostringstream o;
		o << _T("バイト受信 dequeue ポート [") << portnum << _T("] バイト [0x") <<
			std::hex << std::setw(2) << std::setfill(_T('0')) << (unsigned int)byte << _T("]");
		trace(o.str().c_str());
	}
	return byte;
}

unsigned char mihajong_socket::Sock::syncgetc () { // 読み込み(同期)
	unsigned char byte; bool fini = false;
	{
		CodeConv::tostringstream o;
		o << _T("バイト受信処理(同期)開始 ポート [") << portnum << _T("]");
		trace(o.str().c_str());
	}
	while (!fini) {
		try {
			byte = getc(); fini = true;
		}
		catch (queue_empty&) {
			Sleep(50); // Yield and try again
		}
	}
	{
		CodeConv::tostringstream o;
		o << _T("バイト受信処理(同期) ポート [") << portnum << _T("] バイト [0x") <<
			std::hex << std::setw(2) << std::setfill(_T('0')) << (unsigned int)byte << _T("]");
		trace(o.str().c_str());
	}
	return byte;
}

CodeConv::tstring mihajong_socket::Sock::gets () { // NewLineまで読み込み
	//trace("文字列をNWL(0x0a)まで取得します。");
	CodeConv::tstring str;
	if (isServer) {
		threadPtr.server->chkError();
		str = threadPtr.server->readline();
	} else {
		threadPtr.client->chkError();
		str = threadPtr.client->readline();
	}
	{
		CodeConv::tostringstream o;
		o << _T("文字列受信処理 ポート [") << portnum << _T("] 長さ [") << str.length() << _T("] 文字列 [") << str << _T("]");
		trace(o.str().c_str());
	}
	return str;
}

void mihajong_socket::Sock::putc (unsigned char byte) { // 書き込み
	{
		CodeConv::tostringstream o;
		o << _T("バイト送信 enqueue ポート [") << portnum << _T("] バイト [0x") <<
			std::hex << std::setw(2) << std::setfill(_T('0')) << (unsigned int)byte << _T("]");
		trace(o.str().c_str());
	}
	if (isServer) {
		threadPtr.server->write(byte);
		threadPtr.server->chkError();
	} else {
		threadPtr.client->write(byte);
		threadPtr.client->chkError();
	}
}

void mihajong_socket::Sock::puts (const CodeConv::tstring& str) { // 文字列書き込み
	{
		CodeConv::tostringstream o;
		o << _T("文字列送信処理 ポート [") << portnum << _T("] 長さ [") << str.length() << _T("] 文字列 [") << str << _T("]");
		trace(o.str().c_str());
	}
	std::string encoded_str(CodeConv::EncodeStr(str));
	if (isServer) {
		for (auto k = encoded_str.begin(); k != encoded_str.end(); ++k)
			threadPtr.server->write((unsigned char)*k);
		threadPtr.server->chkError();
	} else {
		for (auto k = encoded_str.begin(); k != encoded_str.end(); ++k)
			threadPtr.client->write((unsigned char)*k);
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
		threadPtr.server = nullptr;
	} else {
		delete threadPtr.client;
		threadPtr.client = nullptr;
	}
}

// -------------------------------------------------------------------------

mihajong_socket::Sock::network_thread::network_thread(Sock* caller) {
	myCaller = caller;
	errtype = errNone; errcode = 0;
	finished = terminated = send_ended = sender_closed = receive_ended = receiver_closed = connected = false;
}

mihajong_socket::Sock::network_thread::~network_thread() {
}

DWORD WINAPI mihajong_socket::Sock::network_thread::thread(LPVOID lp) { // スレッドを起動するための処理
	return ((client_thread*)lp)->myThreadFunc();
}

void mihajong_socket::Sock::network_thread::chkError () { // エラーをチェックし、もしエラーだったら例外を投げる
	CodeConv::tostringstream o;
	switch (errtype) {
	case errNone: // エラーなし
		break;
	case errListen: // listen失敗
		o << _T("listen時のエラーです。エラーコード [") << errcode << _T(']'); error(o.str().c_str());
		throw listen_failure(errcode);
	case errAccept: // accept失敗
		o << _T("接続受け入れ時のエラーです。エラーコード [") << errcode << _T(']'); error(o.str().c_str());
		throw accept_failure(errcode);
	case errConnection: // 接続失敗
		o << _T("接続時のエラーです。エラーコード [") << errcode << _T(']'); error(o.str().c_str());
		throw connection_failure(errcode);
	case errRecv: // 受信失敗
		o << _T("受信時のエラーです。エラーコード [") << errcode << _T(']'); error(o.str().c_str());
		throw recv_error(errcode);
	case errSend: // 送信失敗
		o << _T("送信時のエラーです。エラーコード [") << errcode << _T(']'); error(o.str().c_str());
		throw send_error(errcode);
	default: // ほかのエラー
		o << _T("ソケットエラーです。エラーコード [") << errcode << _T(']'); error(o.str().c_str());
		throw socket_error(errcode);
	}
}

int mihajong_socket::Sock::network_thread::reader() { // 受信処理
	unsigned char buf[bufsize] = {0,};
	WSABUF buffer; buffer.buf = reinterpret_cast<CHAR*>(buf); buffer.len = bufsize;
	DWORD recvsz; DWORD flag = 0;
	if (WSARecv(*mySock, &buffer, 1, &recvsz, &flag, nullptr, nullptr) == 0) { // 受信する
		CodeConv::tostringstream o;
		o << _T("データ受信 ポート [") << myCaller->portnum << _T("] ストリーム [");
		myRecvQueueCS.syncDo<void>([this, recvsz, &buf, &o]() -> void { // 受信用ミューテックスを取得
			unsigned count = 0;
			for (unsigned int i = 0; i < recvsz; ++i) {
				myMailBox.push(buf[i]); // キューに追加
				if (i > 0) o << _T(" ");
				o << std::setw(2) << std::hex << std::setfill(_T('0')) << static_cast<unsigned int>(buf[i]);
			}
			o << _T("] サイズ [") << std::dec << recvsz << _T("]");
			if (recvsz) trace(o.str().c_str());
		}); // 受信用ミューテックスを解放
		if (recvsz == 0) {receive_ended = true;} // 受信終了？
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

	{
		CodeConv::tostringstream o;
		o << _T("データ送信 ポート [") << myCaller->portnum << _T("] ストリーム [");
		mySendQueueCS.syncDo<void>([this, &sendsz, &buf, &o]() -> void { // 送信用ミューテックスを取得
			while (!mySendBox.empty()) {
				buf[sendsz++] = mySendBox.front(); mySendBox.pop(); // キューから取り出し
				if (sendsz > 1) o << _T(" ");
				o << std::setw(2) << std::hex << std::setfill(_T('0')) << static_cast<unsigned int>(buf[sendsz - 1]);
			}
			o << _T("] サイズ [") << std::dec << sendsz << _T("]");
			if (sendsz) trace(o.str().c_str());
			//if (receiver_closed) send_ended = true; // 受信ポートが閉じられていたら終了処理へ
		}); // 送信用ミューテックスを解放
	}
	buffer.len = sendsz; // 送信サイズ
	if (sendsz && (WSASend(*mySock, &buffer, 1, &sendsz, 0, nullptr, nullptr))) { // 送信
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
	u_long arg = 1; ioctlsocket(*mySock, FIONBIO, &arg); // non-blocking モードに設定
	if (int err = establishConnection()) return err; // 接続
	while ((!sender_closed) || (!receiver_closed)) { // 終了するまで
		int err = 0;
		if ((!receiver_closed) && (err = reader())) return err; // 読み込み
		if (receive_ended) { // 終了ならソケットをシャットダウン
			CodeConv::tostringstream o; o << _T("受信ポートをシャットダウン ポート[") << myCaller->portnum << _T("]"); debug(o.str().c_str());
			shutdown(*mySock, SD_RECEIVE); receiver_closed = true; receive_ended = false;
		}
		if ((!sender_closed) && (err = writer())) return err; // 書き込み
		if (send_ended) { // 終了ならソケットをシャットダウン
			CodeConv::tostringstream o; o << _T("送信ポートをシャットダウン ポート[") << myCaller->portnum << _T("]"); debug(o.str().c_str());
			shutdown(*mySock, SD_SEND); sender_closed = true; send_ended = false;
		}
		Sleep(0);
	}
	{CodeConv::tostringstream o; o << _T("送受信スレッドループの終了 ポート[") << myCaller->portnum << _T("]"); debug(o.str().c_str());}
	finished = true;
	return S_OK;
}

unsigned char mihajong_socket::Sock::network_thread::read () { // 1バイト読み込み
	unsigned char byte; bool empty = false;
	myRecvQueueCS.syncDo<void>([this, &byte, &empty]() -> void { // 受信用ミューテックスを取得
		if (myMailBox.empty()) empty = true; // キューが空の場合
		else {byte = myMailBox.front(); myMailBox.pop();} // 空でなければ取り出す
	}); // 受信用ミューテックスを解放
	if (empty) throw queue_empty(); // 空だったら例外
	else return byte; // そうでなければ取り出した値を返す
}

CodeConv::tstring mihajong_socket::Sock::network_thread::readline () { // 1行読み込み
	std::string line = ""; bool nwl_not_found = true;
	myRecvQueueCS.syncDo<void>([this, &line, &nwl_not_found]() -> void { // 受信用ミューテックスを取得
		auto tmpMailBox = myMailBox; // キューを作業用コピー
		while (!tmpMailBox.empty()) {
			unsigned char tmpchr[sizeof(int)] = {0,};
			tmpchr[0] = tmpMailBox.front(); tmpMailBox.pop();
			if (*tmpchr == '\n') {
				nwl_not_found = false; break;
			} else if (*tmpchr != '\r') { // CR is just ignored
				line += std::string(reinterpret_cast<char*>(tmpchr));
			}
		}
		if (!nwl_not_found) myMailBox = tmpMailBox; // キューをコミット
	}); // 受信用ミューテックスを解放
	if (nwl_not_found) throw queue_empty(); // 空だったら例外
	else return CodeConv::DecodeStr(line); // そうでなければ結果を返す
}

void mihajong_socket::Sock::network_thread::write (unsigned char byte) { // 1バイト書き込み
	mySendQueueCS.syncDo<void>([this, byte]() -> void { // 送信用ミューテックスを取得
		mySendBox.push(byte); // キューに追加
	}); // 送信用ミューテックスを解放
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

void mihajong_socket::Sock::network_thread::wait_until_sent() { // 送信キューが空になるまで待つ
	{
		CodeConv::tostringstream o;
		o << _T("ポート [") << myCaller->portnum << _T("] の送信データはここまでらしい、全部送り終わるまで待ちます");
		debug(o.str().c_str());
	}
	while (true) { // 送信が完了するまで待つ
		bool flag = mySendQueueCS.syncDo<bool>([this]() { // 送信用ミューテックスを取得
			return mySendBox.empty(); // 終了したかどうかのフラグ
		}); // 送信用ミューテックスを解放
		if (flag) { // 送るべきデータをすべて送り終えたら
			send_ended = true; break; // フラグを立てて、ループを抜ける
		}
		Sleep(500);
	}
	{
		CodeConv::tostringstream o; o << _T("ポート [") << myCaller->portnum << _T("] の送信は終わったんじゃないかな？");
		debug(o.str().c_str());
	}
}

void mihajong_socket::Sock::network_thread::terminate () { // 切断する
	terminated = true; // フラグを立てる
	wait_until_sent(); // 送信が完了するまで待つ
	while (!finished) Sleep(10); // スレッドが終了するまで待つ
	finished = terminated = send_ended = sender_closed = receive_ended = receiver_closed = connected = false; // フラグの後始末
	errtype = errNone; errcode = 0;
}

// -------------------------------------------------------------------------

int mihajong_socket::Sock::client_thread::establishConnection() { // 接続を確立する
	info(_T("クライアント接続処理を開始します"));
	while (true) {
		if (::connect(*mySock, (sockaddr*)&myAddr, sizeof(myAddr)) == SOCKET_ERROR) { // 接続
			errcode = WSAGetLastError();
			if (errcode == WSAEISCONN) {
				break; // 正常に接続完了したとみなす
			} else if ((errcode != WSAEWOULDBLOCK) && (errcode != WSAEALREADY)) {
				errtype = errConnection; return -((int)errtype);
			}
		} else break;
		Sleep(50);
		if (terminated) { // 中止の場合
			info(_T("クライアント接続処理を中止しました"));
			return 0;
		}
	}
	info(_T("クライアント接続処理が完了しました"));
	connected = true; // 接続済みフラグを立てる
	return 0;
}

int mihajong_socket::Sock::server_thread::establishConnection() { // 接続を確立する
	info(_T("サーバ待受処理を開始します"));
	u_long arg = 1; ioctlsocket(*listenerSock, FIONBIO, &arg); // non-blocking モードに設定
	if (::listen(*listenerSock, SOMAXCONN) == SOCKET_ERROR) { // 待機
		errtype = errListen; errcode = WSAGetLastError(); return -((int)errtype);
	}
	while (true) {
		*mySock = accept(*listenerSock, nullptr, 0);
		if (*mySock == INVALID_SOCKET) { // ソケットの作成に失敗場合
			errcode = WSAGetLastError();
			if (errcode != WSAEWOULDBLOCK) {
				errtype = errAccept;
				closesocket(*listenerSock);
				return -((int)errtype);
			}
		} else break;
		if (terminated) { // 中止の場合
			info(_T("サーバ待受処理を中止しました"));
			return 0;
		}
		Sleep(50);
	}
	shutdown(*listenerSock, SD_BOTH);
	closesocket(*listenerSock);
	info(_T("サーバ待受処理が完了しました"));
	connected = true; // 接続済みフラグを立てる
	return 0;
}

void mihajong_socket::Sock::server_thread::setsock (SOCKET* const socket, SOCKET* const lsocket) { // ソケットを設定する
	mySock = socket; listenerSock = lsocket;
}
