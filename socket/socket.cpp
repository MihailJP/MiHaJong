#include "socket.h"
#include <mutex>

namespace mihajong_socket {

#ifdef _WIN32
WSADATA SocketInfo;
HINSTANCE dllInst;
#endif /* _WIN32 */
std::array<Sock*, numOfSockets> sockets = {nullptr,};
std::array<std::recursive_mutex, numOfSockets> socketExistenceMutex;

constexpr int Error_NoSuchSocket = -32768;

void errordlg (socket_error& err) { // エラーダイアログ【廃止】
	//MessageBox(nullptr, CodeConv::EnsureTStr(err.what()).c_str(), _T("Socket Error"), MB_ICONERROR | MB_TOPMOST | MB_OK);
}

DLL int init () try { // ソケットを初期化する
#ifdef _WIN32 // Linuxでは初期化不要
	if (int err = WSAStartup(MAKEWORD(2, 0), &SocketInfo)) throw socket_initialization_error(err);
#endif /* _WIN32 */
	return 0;
}
catch (socket_error& err) {
	errordlg(err); // ダイアログを表示する
	return err.error_code();
}

DLL int listen (int sock_id, int port) try { // サーバー待ち受け開始
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	sockets[sock_id] = new Sock(port); // 接続する
	return 0;
}
catch (socket_error& err) {
	errordlg(err); // ダイアログを表示する
	return err.error_code();
}

DLL int connect (int sock_id, const char* const addr, int port) try { // クライアント接続開始
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	sockets[sock_id] = new Sock(addr, port); // 接続する
	return 0;
}
catch (socket_error& err) {
	errordlg(err); // ダイアログを表示する
	return 1;
}

DLL int connected (int sock_id) try { // 接続されているか確認
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	if (!sockets[sock_id]) return Error_NoSuchSocket;
	return sockets[sock_id]->connected() ? 1 : 0; // 接続されているか確認
}
catch (socket_error& err) { // 送信失敗時
	errordlg(err); // ダイアログを表示する
	return -(err.error_code());
}

DLL int putc (int sock_id, int byte) try { // 1バイト送信
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	if (!sockets[sock_id]) return Error_NoSuchSocket;
	sockets[sock_id]->putc(static_cast<unsigned char>(byte)); // 1バイト送信
	return 0;
}
catch (socket_error& err) { // 送信失敗時
	errordlg(err); // ダイアログを表示する
	return err.error_code();
}

DLL int puts (int sock_id, LPCTSTR const str) try { // 文字列送信
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	if (!sockets[sock_id]) return Error_NoSuchSocket;
	sockets[sock_id]->puts(CodeConv::tstring(str)); // null-terminated (C-style) string 送信
	return 0;
}
catch (socket_error& err) { // 送信失敗時
	errordlg(err); // ダイアログを表示する
	return err.error_code();
}

DLL int getc (int sock_id) try { // 1バイト受信
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	if (!sockets[sock_id]) return Error_NoSuchSocket;
	return static_cast<int>(sockets[sock_id]->getc()); // 1バイト受信
}
catch (queue_empty&) { // まだ受信するデータがない場合
	return -1;
}
catch (socket_error& err) { // 受信失敗時
	errordlg(err); // ダイアログを表示する
	return -(err.error_code());
}

DLL int gets (int sock_id, LPTSTR const stringline, int bufsize) try { // 1行受信
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	if (!sockets[sock_id]) return Error_NoSuchSocket;
#if defined(_MSC_VER)
	_tcscpy_s(stringline, bufsize, sockets[sock_id]->gets().c_str());
#else
	_tcsncpy(stringline, sockets[sock_id]->gets().c_str(), bufsize);
#endif
	return 0;
}
catch (queue_empty&) { // まだ受信するデータがない場合
	return -1;
}
catch (socket_error& err) { // 受信失敗時
	errordlg(err); // ダイアログを表示する
	return -(err.error_code());
}

DLL int hangup (int sock_id) { // 接続を切る
	std::unique_lock<std::recursive_mutex> lock(socketExistenceMutex[sock_id]);
	if (sockets[sock_id]) delete sockets[sock_id];
	sockets[sock_id] = nullptr;
	return 0;
}

DLL int bye () { // ソケットのクリンナップ
	for (unsigned i = 0; i < numOfSockets; ++i)
		hangup(i);
#ifdef _WIN32 // Linuxでは不要
	return WSACleanup();
#else /* _WIN32 */
	return 0;
#endif /* _WIN32 */
}

}
// -------------------------------------------------------------------------

#ifdef _WIN32 // Linuxでは不要
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { // 初期化などを行う
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		mihajong_socket::dllInst = hinstDLL;
		mihajong_socket::init();
		break;
	case DLL_PROCESS_DETACH:
		mihajong_socket::dllInst = nullptr;
		mihajong_socket::bye();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
#endif /* _WIN32 */
