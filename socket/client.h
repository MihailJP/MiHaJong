#ifndef CLIENT_H
#define CLIENT_H

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
		std::array<std::string, 4> playerName;
		char ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
		std::string serveraddr;
		std::string myName;
		DWORD WINAPI preparationThread (); // 接続を待ち、接続処理をする
		int ClientNumber;
	public:
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()に渡す引数用
		starter (const std::string& InputPlayerName, const std::string& server, unsigned short port); // コンストラクタ
		bool isConnected (); // 接続成功したかどうか
		bool isFailed (); // 接続失敗したかどうか
		bool isFinished (); // 待機用スレッドが終わったかどうか
		std::string getPlayerName (unsigned id); // プレイヤー名
		const char* getRules (unsigned line);
		int getClientNumber (); // クライアント番号
	};
	
	extern starter* starterThread;
	void send (unsigned char SendingMsg); // サーバーにメッセージを送る
#endif
	DLL void start (const char* const name, const char* const server, int port, int players); // クライアントを開始させる(DLL)
	DLL int isStartingFinished (); // 待機用スレッドが終わったかどうか
	DLL int isConnectionSucceded (); // 接続成功か
	DLL int isConnectionFailed (); // 接続失敗か
	DLL int getClientNumber (); // クライアント番号
	DLL void getPlayerNames (char* playerName1, char* playerName2, char* playerName3, char* playerName4, unsigned bufsz);
	DLL void checkout_rules (char** rules); // ルールをチェックアウト
	DLL void releaseobj (); // デストラクタを呼ぶだけ
	DLL void send (int SendingMsg); // サーバーにメッセージを送る [Transitional API]
	DLL void receive (int* const ClientReceived, int* const ReceivedMsg); // サーバーのメッセージを受信する
}
}

#endif
