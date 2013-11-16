#include "chat.h"

#include "../socket/socket.h"
#include "envtbl.h"
#include "func.h"
#include "../graphic/graphic.h"
#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/

namespace chat {

void ChatThread::thread_loop (ChatThread* inst) {
	inst->init();
	while (!(inst->terminate)) {
		inst->receive();
		inst->send();
		std::this_thread::yield();
	}
	inst->cleanup();
}
StreamLog::StreamLog () {
	mihajong_graphic::logwnd::reset();
}
ChatThread::ChatThread (std::string& server_addr, int clientNum) : StreamLog() {
	terminate = false;
	myServerAddr = server_addr; myClientNum = clientNum;
	myThread = std::thread(thread_loop, this);
}
StreamLog::~StreamLog () {
}
ChatThread::~ChatThread () {
	terminate = true;
	myThread.join();
}

void ChatThread::init() {
	if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
		bool tmpClientWaiting[Players];
		for (int i = 0; i < Players; i++) {
			if (tmpClientWaiting[i] = (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag > 0))
				mihajong_socket::listen(
					SOCK_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag,
					PORT_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag);
		}
		while (tmpClientWaiting[0] || tmpClientWaiting[1] || tmpClientWaiting[2] || tmpClientWaiting[3]) {
			for (int i = 0; i < Players; i++)
				if ((tmpClientWaiting[i] = (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag > 0)) &&
					(mihajong_socket::connected(
					SOCK_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag)))
					tmpClientWaiting[i] = false;
			std::this_thread::yield();
		}
	}
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
		mihajong_socket::connect(SOCK_CHAT+0, myServerAddr.c_str(), PORT_CHAT-1+myClientNum);
		while (!mihajong_socket::connected(SOCK_CHAT+0)) // Wait until connection established
			std::this_thread::yield();
	}
}

void ChatThread::receive() {
	if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
		for (int i = 0; i < Players; i++) {
			if (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag >= 1) {
				TCHAR buf[bufsize] = {0};
				int stat = mihajong_socket::gets(
					SOCK_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag,
					buf, bufsize);
				if (stat == 0) {
					for (int k = 1; k <= 3; k++) {
						if ((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == k) ||
							(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag == k) ||
							(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag == k) ||
							((!GameStat.chkGameType(SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == k))) {
								if (buf[_tcslen(buf) - 1] != _T('\n'))
#if defined(_MSC_VER)
									_tcscat_s(buf, bufsize, _T("\r\n"));
#elif defined(_WIN32)
									_tcsncat(buf, _T("\r\n"), bufsize - _tcslen(buf));
#else
									_tcsncat(buf, _T("\n"), bufsize - _tcslen(buf));
#endif
								mihajong_socket::puts(SOCK_CHAT + k - 1, buf);
								chatappend(buf);
						}
					}
				}
			}
		}
	}
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
		TCHAR buf[bufsize] = {0};
		int stat = mihajong_socket::gets(SOCK_CHAT, buf, bufsize);
		if (stat == 0) chatappend(buf);
	}
}

CodeConv::tstring StreamLog::chatstr(const CodeConv::tstring& buf) {
	CodeConv::tostringstream o;
	int tmpPlayer = static_cast<int>(buf[0] - _T('0'));
	if ((tmpPlayer >= 0) && (tmpPlayer <= ACTUAL_PLAYERS)) {
		o << EnvTable::Instantiate()->PlayerDat[tmpPlayer].PlayerName <<
			_T("(") << windName(GameStat.playerwind(tmpPlayer)) << _T(") : ") <<
			CodeConv::tstring(buf.begin() + 1, buf.end());
		if (buf[buf.length() - 1] != _T('\n'))
			o <<
#ifdef _WIN32
			_T("\r") <<
#endif
			std::endl;
		else o.flush();
	}
	return o.str();
}
void StreamLog::chatappend(const CodeConv::tstring& buf) {
	int tmpPlayer = static_cast<int>(buf[0] - _T('0'));
	if ((tmpPlayer >= 0) && (tmpPlayer <= ACTUAL_PLAYERS)) {
		mihajong_graphic::logwnd::append(chatstr(buf).c_str());
	}
}
void ChatThread::chatappend(const CodeConv::tstring& buf) {
	std::unique_lock<std::recursive_mutex> lock(streamLock);
	super::chatappend(buf);
}

void ChatThread::send() {
	std::unique_lock<std::recursive_mutex> lock(sendQueueLock);
	if (!sendQueue.empty()) {
		TCHAR buf[bufsize] = {0}; //buf[0] = GameStat.PlayerID + _T('0');
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			for (int k = 1; k <= 3; k++) {
				if ((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == k) ||
					(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag == k) ||
					(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag == k) ||
					((!GameStat.chkGameType(SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == k))) {
#if defined(_MSC_VER)
						_tcscat_s(buf, bufsize, sendQueue.front().c_str());
#else
						_tcsncat(buf, sendQueue.front().c_str(), bufsize - _tcslen(buf));
#endif
						if ((_tcslen(buf)) && (buf[_tcslen(buf) - 1] != _T('\n')))
#if defined(_MSC_VER)
							_tcscat_s(buf, bufsize, _T("\r\n"));
#elif defined(_WIN32)
							_tcsncat(buf, _T("\r\n"), bufsize - _tcslen(buf));
#else
							_tcsncat(buf, _T("\n"), bufsize - _tcslen(buf));
#endif
						mihajong_socket::puts(SOCK_CHAT + k - 1, buf);
						chatappend(buf);
				}
			}
		} else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
#if defined(_MSC_VER)
			_tcscat_s(buf, bufsize, sendQueue.front().c_str());
#else
			_tcsncat(buf, sendQueue.front().c_str(), bufsize - _tcslen(buf));
#endif
			if ((_tcslen(buf)) && (buf[_tcslen(buf) - 1] != _T('\n')))
#if defined(_MSC_VER)
				_tcscat_s(buf, bufsize, _T("\r\n"));
#elif defined(_WIN32)
				_tcsncat(buf, _T("\r\n"), bufsize - _tcslen(buf));
#else
				_tcsncat(buf, _T("\n"), bufsize - _tcslen(buf));
#endif
			mihajong_socket::puts(SOCK_CHAT, buf);
		}
		sendQueue.pop();
	}
}

void ChatThread::cleanup() {
	for (int i = 0; i < Players; i++) // 開けてないソケットを閉じようとしても安全になるようにしてる
		mihajong_socket::hangup(SOCK_CHAT + i);
}

CodeConv::tstring StreamLog::getlog () {
	return CodeConv::tstring(mihajong_graphic::logwnd::getlogptr());
}
CodeConv::tstring ChatThread::getlog () {
	std::unique_lock<std::recursive_mutex> lock(streamLock);
	return CodeConv::tstring(mihajong_graphic::logwnd::getlogptr());
}

void StreamLog::sysmsg(const CodeConv::tstring& str) {
	CodeConv::tstring tmpstr = str;
	if (str[str.length() - 1] != _T('\n'))
		tmpstr +=
#ifdef _WIN32
		_T("\r\n");
#else
		_T("\n");
#endif
	mihajong_graphic::logwnd::append(tmpstr.c_str());
}
void ChatThread::sysmsg(const CodeConv::tstring& str) {
	std::unique_lock<std::recursive_mutex> lock(streamLock);
	super::sysmsg(str);
}

void StreamLog::sendstr (const CodeConv::tstring& msg) {
	sendstrx(GameStat.PlayerID, msg);
}
void StreamLog::sendstrx (PlayerID player, const CodeConv::tstring& msg) {
	CodeConv::tostringstream s;
	s << static_cast<int>(player) << msg;
	if ((player >= 0) && (player <= ACTUAL_PLAYERS)) {
		mihajong_graphic::logwnd::append(s.str().c_str());
	}
}
void ChatThread::sendstr (const CodeConv::tstring& msg) {
	sendstrx(GameStat.PlayerID, msg);
}
void ChatThread::sendstrx (PlayerID player, const CodeConv::tstring& msg) {
	TCHAR tmpnum[2] = {0}; tmpnum[0] = player + _T('0');
	std::unique_lock<std::recursive_mutex> lock(sendQueueLock);
	sendQueue.push(CodeConv::tstring(tmpnum) + msg);
}

// -------------------------------------------------------------------------

StreamLog* chatobj = nullptr;

void initchat (const char* const server_addr, int clientNum) {
	std::string serverAddr(server_addr);
	if ((EnvTable::Instantiate()->GameMode == EnvTable::Server) ||
		(EnvTable::Instantiate()->GameMode == EnvTable::Client))
		chatobj = new ChatThread(serverAddr, clientNum);
	else chatobj = new StreamLog();
}
void appendchat (LPCTSTR const chatstr) {
	chatobj->sysmsg(chatstr);
}
void sendchat (LPCTSTR const chatstr) {
	static CodeConv::tstring ChatStr;
	ChatStr = chatstr;
	chatobj->sendstr(ChatStr.c_str());
}
void sendchatx (int player, LPCTSTR const chatstr) {
	chatobj->sendstrx(player, chatstr);
}
void closechat () {
	delete chatobj; chatobj = nullptr;
}

} /* namespace */
