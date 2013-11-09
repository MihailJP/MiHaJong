﻿#include "client.h"
#ifndef _WIN32
#include <unistd.h>
#endif /* _WIN32 */

namespace mihajong_socket {
namespace client {
	starter* starterThread = nullptr;
	unsigned int NumberOfPlayers = 4;

	starter::starter (const CodeConv::tstring& InputPlayerName, const std::string& server, unsigned short port) { // コンストラクタ
		connected = finished = failed = false;
		myName = InputPlayerName;
		portnum = port;
		serveraddr = std::string(server);
		memset(ruleConf, 0, sizeof(ruleConf));
		ClientNumber = -1;
	}

	// ---------------------------------------------------------------------

#ifdef _WIN32
	DWORD WINAPI starter::preparationThread () { // 接続を待ち、接続処理をする
#else /* _WIN32 */
	int starter::preparationThread () { // 接続を待ち、接続処理をする
#endif /* _WIN32 */
		try {
			sockets[0] = new Sock(serveraddr, portnum);
		}
		catch (socket_error& e) { // Connection failed...
			failed = true;
			errordlg(e);
			//MessageBox(nullptr, _T("サーバーに接続できません\r\nオフラインモードで開始します"), _T("接続失敗"), MB_ICONERROR | MB_TOPMOST | MB_OK);
			playerName[0] = CodeConv::tstring(_T("[A]")) + myName;
			playerName[1] = CodeConv::tstring(_T("[b]COM"));
			playerName[2] = CodeConv::tstring(_T("[c]COM"));
			playerName[3] = CodeConv::tstring(_T("[d]COM"));
			finished = true;
#ifdef _WIN32
			return S_OK;
#else /* _WIN32 */
			return 0;
#endif /* _WIN32 */
		}
		while (true) {
			try {
				if (!sockets[0]->connected()) {
#ifdef _WIN32
					Sleep(50); continue;
#else /* _WIN32 */
					usleep(50000); continue;
#endif /* _WIN32 */
				}
			}
			catch (socket_error& e) { // Connection failed...
				failed = true;
				errordlg(e);
				//MessageBox(nullptr, _T("サーバーに接続できません\r\nオフラインモードで開始します"), _T("接続失敗"), MB_ICONERROR | MB_TOPMOST | MB_OK);
				playerName[0] = CodeConv::tstring(_T("[A]")) + myName;
				playerName[1] = CodeConv::tstring(_T("[b]COM"));
				playerName[2] = CodeConv::tstring(_T("[c]COM"));
				playerName[3] = CodeConv::tstring(_T("[d]COM"));
				finished = true;
#ifdef _WIN32
				return S_OK;
#else /* _WIN32 */
				return 0;
#endif /* _WIN32 */
			}
			ClientNumber = sockets[0]->syncgetc() - 1;
			sockets[0]->disconnect();
			sockets[0]->connect(serveraddr, portnum + ClientNumber);
			while (!sockets[0]->connected())
#ifdef _WIN32
				Sleep(50);
#else /* _WIN32 */
				usleep(50000);
#endif /* _WIN32 */
			connected = true;
			putString(0, myName);
			while (sockets[0]->syncgetc() != protocol::Server_StartGame_Signature)
#ifdef _WIN32
				Sleep(10); // 開始を待つ
#else /* _WIN32 */
				usleep(10000); // 開始を待つ
#endif /* _WIN32 */
			for (unsigned int i = 0; i < NumberOfPlayers; ++i)
				playerName[i] = getString(0); // 名前を受信
			for (unsigned i = 0; i < RULE_LINES; ++i)
#if defined(_MSC_VER) /* For newer VC++ use strcpy_s */
				strcpy_s(ruleConf[i], RULE_IN_LINE + 1, CodeConv::toANSI(getString(0)).c_str()); // ルールを受信
#else /* For MinGW use strncpy */
				strncpy(ruleConf[i], CodeConv::toANSI(getString(0)).c_str(), RULE_IN_LINE); // ルールを受信
#endif
			break;
		}
		finished = true;
#ifdef _WIN32
		return S_OK;
#else /* _WIN32 */
		return 0;
#endif /* _WIN32 */
	}

	// ---------------------------------------------------------------------

#ifdef _WIN32
	DWORD WINAPI starter::initiate (LPVOID param) { // CreateThread()に渡す引数用
		return ((starter*)param)->preparationThread();
	}
#else /* _WIN32 */
	void* starter::initiate (void* param) { // CreateThread()に渡す引数用
		((starter*)param)->preparationThread();
		return nullptr;
	}
#endif /* _WIN32 */
	void starter::startThread () { // スレッドを開始する
#ifdef _WIN32
		CreateThread(nullptr, 0, initiate, (LPVOID)this, 0, nullptr);
#else /* _WIN32 */
		pthread_create(&myThread, nullptr, initiate, this);
		pthread_detach(myThread);
#endif /* _WIN32 */
	}
	bool starter::isConnected () { // 接続成功したかどうか
		return connected;
	}
	bool starter::isFailed () { // 接続失敗したかどうか
		return failed;
	}
	bool starter::isFinished () { // 待機用スレッドが終わったかどうか
		return finished;
	}
	CodeConv::tstring starter::getPlayerName (unsigned id) { // プレイヤー名
		return playerName[id];
	}
	const char* starter::getRules (unsigned line) {
		return ruleConf[line];
	}
	int starter::getClientNumber () { // クライアント番号
		return ClientNumber;
	}

	// ---------------------------------------------------------------------

	DLL void start (LPCTSTR const name, const char* const server, int port, int players) { // サーバーを開始させる(DLL)
		NumberOfPlayers = (unsigned int)players;
		starterThread = new starter(name, server, (unsigned short)port);
		starterThread->startThread();
	}
	DLL int isStartingFinished () { // 待機用スレッドが終わったかどうか
		return starterThread->isFinished() ? 1 : 0;
	}
	DLL int isConnectionSucceded () { // 接続成功か
		return starterThread->isConnected() ? 1 : 0;
	}
	DLL int isConnectionFailed () { // 接続失敗か
		return starterThread->isFailed() ? 1 : 0;
	}
	DLL int getClientNumber () { // クライアント番号
		return starterThread->getClientNumber();
	}
	DLL void getPlayerNames (LPTSTR playerName1, LPTSTR playerName2, LPTSTR playerName3, LPTSTR playerName4, unsigned bufsz) {
#if defined(_MSC_VER)
		_tcscpy_s(playerName1, bufsz, starterThread->getPlayerName(0).c_str());
		_tcscpy_s(playerName2, bufsz, starterThread->getPlayerName(1).c_str());
		_tcscpy_s(playerName3, bufsz, starterThread->getPlayerName(2).c_str());
		_tcscpy_s(playerName4, bufsz, starterThread->getPlayerName(3).c_str());
#else
		_tcsncpy(playerName1, starterThread->getPlayerName(0).c_str(), bufsz);
		_tcsncpy(playerName2, starterThread->getPlayerName(1).c_str(), bufsz);
		_tcsncpy(playerName3, starterThread->getPlayerName(2).c_str(), bufsz);
		_tcsncpy(playerName4, starterThread->getPlayerName(3).c_str(), bufsz);
#endif
	}
	DLL void checkout_rules (char** rules) { // ルールをチェックアウト
		for (unsigned i = 0; i < RULE_LINES; ++i)
			memcpy(rules[i], starterThread->getRules(i), RULE_IN_LINE);
	}
	DLL void releaseobj () { // デストラクタを呼ぶだけ
		delete starterThread; starterThread = nullptr;
	}

	// ---------------------------------------------------------------------

	void send (unsigned char SendingMsg) { // サーバーにメッセージを送る
		sockets[0]->putc(SendingMsg);
		// かつてはここでログ出力していた
	}
	DLL void send (int SendingMsg) try { // サーバーにメッセージを送る [Transitional API]
		send((unsigned char)SendingMsg);
	} catch (socket_error& err) {
		CodeConv::tostringstream o;
		o << _T("サーバーへの送信に失敗 エラーコード [") << err.error_code() << _T(']');
		error(o.str().c_str());
	}

	// ---------------------------------------------------------------------

	DLL void receive (volatile int* const ClientReceived, int* const ReceivedMsg) { // サーバーのメッセージを受信する
		*ClientReceived = 0; *ReceivedMsg = 1023;
		try {
			*ReceivedMsg = sockets[0]->getc();
			*ClientReceived = 1;
		}
		catch (queue_empty&) { // Falling back if empty...
			*ClientReceived = 0;
			*ReceivedMsg = 1023;
		}
		catch (socket_error&) { // Sorry, you are disconnected.
			*ClientReceived = 1;
			*ReceivedMsg = 1023;
		}
		// かつてはここでログ出力していた
	}

}
}
