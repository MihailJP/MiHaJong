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

	class StreamLog {
	protected:
		static const int bufsize = 65536;
		std::ostringstream myChatStream;
		HWND logWindow;
		std::string chatstr(const std::string& buf);
		virtual void chatappend(const std::string& buf);
	public:
		StreamLog ();
		virtual ~StreamLog ();
		virtual std::string getlog ();
		virtual void sysmsg (const std::string& msg);
		virtual void sendstr (const std::string& msg);
		virtual void sendstrx (PLAYER_ID player, const std::string& msg);
		virtual void updateWindow ();
		void setLogWindow (HWND wndHandle);
	};
	class ChatThread : public StreamLog {
		typedef StreamLog super;
	private:
		std::queue<std::string> sendQueue;
		CRITICAL_SECTION streamLock;
		CRITICAL_SECTION sendQueueLock;
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
		void sysmsg (const std::string& msg);
		void sendstr (const std::string& msg);
		void sendstrx (PLAYER_ID player, const std::string& msg);
		void updateWindow ();
	};
	
	extern StreamLog* chatobj;

	__declspec(dllexport) void initchat (const char* const server_addr, int clientNum);
	__declspec(dllexport) void appendchat (const char* const chatstr);
	__declspec(dllexport) void sendchat (const char* const chatstr);
#ifdef MJCORE_EXPORTS
	void sendchatx (int player, const char* const chatstr);
#endif
	__declspec(dllexport) void closechat ();

	__declspec(dllexport) void setlogwnd (HWND window);
}

#endif
