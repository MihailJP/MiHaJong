#ifndef CHAT_H
#define CHAT_H

#include "../socket/socket.h"
#include "envtbl.h"
#include "func.h"
#include <string>
#include <sstream>

#define SOCK_CHAT 10
#define PORT_CHAT 50020

namespace chat {

	class ChatThread {
	private:
		static const int bufsize = 65536;
		std::ostringstream myChatStream;
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
	};
	
}

#endif
