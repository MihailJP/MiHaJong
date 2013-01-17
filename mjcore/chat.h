#pragma once

#include <queue>
#include "gametbl.h"
#include "strcode.h"

#define SOCK_CHAT 10
#define PORT_CHAT 50020

namespace chat {

	class StreamLog {
	protected:
		static const int bufsize = 65536;
		CodeConv::tstring chatstr(const CodeConv::tstring& buf);
		virtual void chatappend(const CodeConv::tstring& buf);
	public:
		StreamLog ();
		virtual ~StreamLog ();
		virtual CodeConv::tstring getlog ();
		virtual void sysmsg (const CodeConv::tstring& msg);
		virtual void sendstr (const CodeConv::tstring& msg);
		virtual void sendstrx (PLAYER_ID player, const CodeConv::tstring& msg);
	};
	class ChatThread : public StreamLog {
		typedef StreamLog super;
	private:
		std::queue<CodeConv::tstring> sendQueue;
		CRITICAL_SECTION streamLock;
		CRITICAL_SECTION sendQueueLock;
		HANDLE myHandle;
		volatile bool terminate;
		std::string myServerAddr;
		int myClientNum;
		static DWORD WINAPI thread_loop (LPVOID param);
		void chatappend(const CodeConv::tstring& buf);
		void init();
		void receive();
		void send();
		void cleanup();
	public:
		ChatThread (std::string& server_addr, int clientNum);
		~ChatThread ();
		CodeConv::tstring getlog ();
		void sysmsg (const CodeConv::tstring& msg);
		void sendstr (const CodeConv::tstring& msg);
		void sendstrx (PLAYER_ID player, const CodeConv::tstring& msg);
		void updateWindow ();
	};
	
	extern StreamLog* chatobj;

	__declspec(dllexport) void initchat (const char* const server_addr, int clientNum);
	__declspec(dllexport) void appendchat (LPCTSTR const chatstr);
	__declspec(dllexport) void sendchat (LPCTSTR const chatstr);
#ifdef MJCORE_EXPORTS
	void sendchatx (int player, LPCTSTR const chatstr);
#endif
	__declspec(dllexport) void closechat ();
}
