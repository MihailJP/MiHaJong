#pragma once

#include <queue>
#include "gametbl.h"
#include "../common/strcode.h"
#include <mutex>
#include "../common/thread.h"

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
		StreamLog(const StreamLog&) = delete; // Delete unexpected copy constructor
		StreamLog& operator= (const StreamLog&) = delete; // Delete unexpected assign operator
		virtual ~StreamLog();
		virtual CodeConv::tstring getlog ();
		virtual void sysmsg (const CodeConv::tstring& msg);
		virtual void sendstr (const CodeConv::tstring& msg);
		virtual void sendstrx (PlayerID player, const CodeConv::tstring& msg);
	};
	class ChatThread : public StreamLog {
		typedef StreamLog super;
	private:
		std::queue<CodeConv::tstring> sendQueue;
		std::recursive_mutex streamLock;
		std::recursive_mutex sendQueueLock;
		THREADLIB::thread myThread;
		volatile bool terminate;
		std::string myServerAddr;
		int myClientNum;
		static void thread_loop (ChatThread* inst);
		void chatappend(const CodeConv::tstring& buf);
		void init();
		void receive();
		void send();
		void cleanup();
	public:
		ChatThread (std::string& server_addr, int clientNum);
		ChatThread(const ChatThread&) = delete; // Delete unexpected copy constructor
		ChatThread& operator= (const ChatThread&) = delete; // Delete unexpected assign operator
		~ChatThread();
		CodeConv::tstring getlog ();
		void sysmsg (const CodeConv::tstring& msg);
		void sendstr (const CodeConv::tstring& msg);
		void sendstrx (PlayerID player, const CodeConv::tstring& msg);
		void updateWindow ();
	};
	
	extern StreamLog* chatobj;

	void initchat (const char* const server_addr, int clientNum);
	void appendchat (LPCTSTR const chatstr);
	void sendchat (LPCTSTR const chatstr);
#ifdef MJCORE_EXPORTS
	void sendchatx (int player, LPCTSTR const chatstr);
#endif
	void closechat ();
}

inline void writeChat(const CodeConv::tstring& ResultDesc) {
	chat::appendchat((CodeConv::tstring(_T("*** ")) + ResultDesc + CodeConv::tstring(_T("\n"))).c_str());
}
