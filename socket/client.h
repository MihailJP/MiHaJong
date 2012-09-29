#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include "server.h"

namespace mihajong_socket {
namespace client {
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
		//static DWORD WINAPI initiate (LPVOID param); // CreateThread()に渡す引数用
		starter (const std::string& InputPlayerName, unsigned short port, const std::string& server); // コンストラクタ
		//void terminate (); // すぐに開始
		//bool isFinished (); // 待機用スレッドが終わったかどうか
		//unsigned int chkCurrentConnection (); // 現在の接続数
		//std::string getPlayerName (unsigned id); // プレイヤー名
	};

	void send (unsigned char SendingMsg); // サーバーにメッセージを送る
	DLL void send (int SendingMsg); // サーバーにメッセージを送る [Transitional API]
	DLL void receive (int* const ClientReceived, int* const ReceivedMsg); // サーバーのメッセージを受信する
}
}

#endif
