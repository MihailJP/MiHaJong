#pragma once

#include "socket.h"
#include "protocol.h"
#ifdef SOCKET_EXPORTS
#include "except.h"
#include "../common/nmrules.h"
#include <windows.h>
#include <array>
#include <string>
#include <cstdint>
#include "../common/strcode.h"
#endif

namespace mihajong_socket {
namespace server {
#ifdef SOCKET_EXPORTS
	extern unsigned int NumberOfPlayers;
	CodeConv::tstring getString(unsigned int socketID); // 文字列を返す(同期)
	void putString(unsigned int socketID, const CodeConv::tstring& sendingStr); // 文字列を送信
	class starter {
	private:
		volatile bool terminated, finished;
		unsigned short portnum;
		unsigned int CurrentConnection;
		std::array<CodeConv::tstring, 4> playerName;
		char ruleConf[RULE_LINES][RULE_IN_LINE + 1];
		DWORD WINAPI preparationThread (); // 接続を待ち、接続処理をする
	public:
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()に渡す引数用
		starter (const CodeConv::tstring& InputPlayerName, unsigned short port, const char* const * const rule); // コンストラクタ
		void terminate (); // すぐに開始
		bool isFinished (); // 待機用スレッドが終わったかどうか
		unsigned int chkCurrentConnection (); // 現在の接続数
		CodeConv::tstring getPlayerName (unsigned id); // プレイヤー名
	};
	extern starter* starterThread;
	extern std::array<unsigned, 3> ServerCheckRotation; // クライアントの読み出し優先順位
	void sendstr (const CodeConv::tstring& sendingStr); // サーバーからの文字列送信
#endif
	DLL void start (LPCTSTR const name, int port, int players, const char* const * const rule); // サーバーを開始させる(DLL)
	DLL void doStart(); // 接続待機をやめ、直ちに開始する
	DLL int isStartingFinished (); // 待機用スレッドが終わったかどうか
	DLL int chkCurrentConnection (); // 現在の接続数
	DLL void getPlayerNames (LPTSTR playerName1, LPTSTR playerName2, LPTSTR playerName3, LPTSTR playerName4, unsigned bufsz);
	DLL void releaseobj (); // デストラクタを呼ぶだけ
	DLL void send (unsigned char SendingMsg); // サーバーからの送信
	DLL void send (int SendingMsg, void*); // サーバーからの送信(DLL)
	DLL void rotation_reset (); // 取得優先順位のリセット
	DLL void receive (volatile int* const ServerReceived, int* const ReceivedMsg); // 取得する
}
}
