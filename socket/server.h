#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include "protocol.h"
#include "except.h"
#include "../mjcore/nmrules.h"
#include <Windows.h>
#include <array>
#include <string>
#include <cstdint>

namespace mihajong_socket {
namespace server {
	extern unsigned int NumberOfPlayers;
	std::string getString(unsigned int socketID); // 文字列を返す(同期)
	void putString(unsigned int socketID, const std::string& sendingStr); // 文字列を送信
	class starter {
	private:
		bool terminated, finished;
		unsigned short portnum;
		unsigned int CurrentConnection;
		std::array<std::string, 4> playerName;
		char ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
		DWORD WINAPI preparationThread (); // 接続を待ち、接続処理をする
	public:
		static DWORD WINAPI initiate (LPVOID param); // CreateThread()に渡す引数用
		starter (const std::string& InputPlayerName, unsigned short port, const char* const * const rule); // コンストラクタ
		void terminate (); // すぐに開始
		bool isFinished (); // 待機用スレッドが終わったかどうか
		unsigned int chkCurrentConnection (); // 現在の接続数
	};
	extern starter* starterThread;
	DLL void start (const char* const name, int port, int players, const char* const * const rule); // サーバーを開始させる(DLL)
	DLL void doStart(); // 接続待機をやめ、直ちに開始する
	DLL int isStartingFinished (); // 待機用スレッドが終わったかどうか、終わったらオブジェクトを解放する
	DLL int chkCurrentConnection (); // 現在の接続数
	void send (unsigned char SendingMsg); // サーバーからの送信
	void sendstr (const std::string& sendingStr); // サーバーからの文字列送信
	DLL void send (int SendingMsg, void*); // サーバーからの送信(DLL)
	extern std::array<unsigned, 3> ServerCheckRotation; // クライアントの読み出し優先順位
	DLL void rotation_reset (); // 取得優先順位のリセット
	DLL void receive (int* const ServerReceived, int* const ReceivedMsg); // 取得する
}
}

#endif
