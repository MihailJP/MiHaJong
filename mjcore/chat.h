#ifndef CHAT_H
#define CHAT_H

#include "../socket/socket.h"
#include "envtbl.h"
#include "func.h"
#include <string>
#include <sstream>
#include <queue>

#define SOCK_CHAT 10
#define PORT_CHAT 50020

namespace chat {

	class ChatThread {
	private:
		static const int bufsize = 65536;
		CRITICAL_SECTION streamLock;
		std::ostringstream myChatStream;
		CRITICAL_SECTION sendQueueLock;
		std::queue<std::string> sendQueue;
		HANDLE myHandle;
		volatile bool terminate;
		std::string myServerAddr;
		int myClientNum;
		static DWORD WINAPI thread_loop (LPVOID param);
		void chatappend(const std::string& buf);
		void init();
		void receive();
		void send();
		void cleanup();
	public:
		ChatThread (std::string& server_addr, int clientNum);
		~ChatThread ();
		std::string getlog ();
		void sendstr (const std::string& msg);
	};
	
	extern ChatThread* chatobj;

	__declspec(dllexport) void initchat (const char* const server_addr, int clientNum);
	__declspec(dllexport) void appendchat (const char* const chatstr);
	__declspec(dllexport) void getchatlog (const char* chatstr, int* const length);
	__declspec(dllexport) void closechat ();

}

#endif
