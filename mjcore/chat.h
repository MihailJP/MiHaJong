#ifndef CHAT_H
#define CHAT_H

#include "../socket/socket.h"
#include "envtbl.h"
#include <string>
#include <array>

#define SOCK_CHAT 10
#define PORT_CHAT 50020

namespace chat {

	class ChatThread {
	private:
		HANDLE myHandle;
		volatile bool terminate;
		std::string myServerAddr;
		static DWORD WINAPI thread_loop (LPVOID param);
		void init();
		void receive();
		void send();
		void cleanup();
	public:
		ChatThread (std::string& server_addr);
		~ChatThread ();
	};
	
}

#endif
