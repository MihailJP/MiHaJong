#pragma once

#include "socket.h"
#include "server.h"

namespace mihajong_socket {
namespace client {
#ifdef SOCKET_EXPORTS
	using server::getString; using server::putString;

	extern unsigned int NumberOfPlayers;

	class starter {
	private:
		bool connected, finished, failed;
		unsigned short portnum;
		std::array<CodeConv::tstring, 4> playerName;
		char ruleConf[RULE_LINES][RULE_IN_LINE + 1];
		std::string serveraddr;
		CodeConv::tstring myName;
#ifdef _WIN32
		DWORD WINAPI preparationThread (); // 接続を待ち、接続処理をする
#else /* _WIN32 */
		pthread_t myThread;
		int preparationThread (); // 接続を待ち、接続処理をする
#endif /* _WIN32 */
		int ClientNumber;
	public:
#ifdef _WIN32
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()に渡す引数用
#else /* _WIN32 */
		static void* initiate (void* param); // CreateThread()に渡す引数用
#endif /* _WIN32 */
		void startThread (); // スレッドを開始する
		starter (const CodeConv::tstring& InputPlayerName, const std::string& server, unsigned short port); // コンストラクタ
		starter (const Sock&) = delete; // Delete unexpected copy constructor
		starter& operator= (const Sock&) = delete; // Delete unexpected assign operator
		bool isConnected(); // 接続成功したかどうか
		bool isFailed (); // 接続失敗したかどうか
		bool isFinished (); // 待機用スレッドが終わったかどうか
		CodeConv::tstring getPlayerName (unsigned id); // プレイヤー名
		const char* getRules (unsigned line);
		int getClientNumber (); // クライアント番号
	};
	
	extern starter* starterThread;
	void send (unsigned char SendingMsg); // サーバーにメッセージを送る
#endif
	DLL void start (LPCTSTR const name, const char* const server, int port, int players); // クライアントを開始させる(DLL)
	DLL int isStartingFinished (); // 待機用スレッドが終わったかどうか
	DLL int isConnectionSucceded (); // 接続成功か
	DLL int isConnectionFailed (); // 接続失敗か
	DLL int getClientNumber (); // クライアント番号
	DLL void getPlayerNames (LPTSTR playerName1, LPTSTR playerName2, LPTSTR playerName3, LPTSTR playerName4, unsigned bufsz);
	DLL void checkout_rules (char** rules); // ルールをチェックアウト
	DLL void releaseobj (); // デストラクタを呼ぶだけ
	DLL void send (int SendingMsg); // サーバーにメッセージを送る [Transitional API]
	DLL void receive (volatile int* const ClientReceived, int* const ReceivedMsg); // サーバーのメッセージを受信する
}
}
