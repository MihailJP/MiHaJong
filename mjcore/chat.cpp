#include "chat.h"

#include "../socket/socket.h"
#include "envtbl.h"
#include "func.h"
#include "../graphic/graphic.h"

namespace chat {

DWORD WINAPI ChatThread::thread_loop (LPVOID param) {
	reinterpret_cast<ChatThread*>(param)->init();
	while (!(reinterpret_cast<ChatThread*>(param)->terminate)) {
		reinterpret_cast<ChatThread*>(param)->receive();
		reinterpret_cast<ChatThread*>(param)->send();
		Sleep(0);
	}
	reinterpret_cast<ChatThread*>(param)->cleanup();
	return S_OK;
}
StreamLog::StreamLog () {}
ChatThread::ChatThread (std::string& server_addr, int clientNum) {
	terminate = false;
	myServerAddr = server_addr; myClientNum = clientNum;
	myHandle = CreateThread(nullptr, 0, thread_loop, this, 0, nullptr);
}
StreamLog::~StreamLog () {
}
ChatThread::~ChatThread () {
	terminate = true;
	while (true) {
		DWORD exitcode;
		GetExitCodeThread(myHandle, &exitcode);
		if (exitcode != STILL_ACTIVE) break;
		else Sleep(0);
	}
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
			Sleep(0);
		}
	}
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
		mihajong_socket::connect(SOCK_CHAT+0, myServerAddr.c_str(), PORT_CHAT-1+myClientNum);
		while (!mihajong_socket::connected(SOCK_CHAT+0)) Sleep(0); // Wait until connection established
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
							((!chkGameType(&GameStat, SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == k))) {
								if (buf[_tcslen(buf) - 1] != _T('\n'))
									_tcscat_s(buf, bufsize,
#ifdef _WIN32
									_T("\r\n")
#else
									_T("\n")
#endif
									);
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
			_T("(") << windName(playerwind(&GameStat, tmpPlayer, GameStat.GameRound)) << _T(") : ") <<
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
	streamLock.acquire();
	super::chatappend(buf); // ←ラムダを使うとこれが呼び出せない
	streamLock.release();
}

void ChatThread::send() {
	sendQueueLock.syncDo<void>([this]() -> void {
		if (!sendQueue.empty()) {
			TCHAR buf[bufsize] = {0}; //buf[0] = GameStat.PlayerID + _T('0');
			if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
				for (int k = 1; k <= 3; k++) {
					if ((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == k) ||
						(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag == k) ||
						(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag == k) ||
						((!chkGameType(&GameStat, SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == k))) {
							_tcscat_s(buf, bufsize, sendQueue.front().c_str());
							if ((_tcslen(buf)) && (buf[_tcslen(buf) - 1] != _T('\n')))
								_tcscat_s(buf, bufsize,
#ifdef _WIN32
								_T("\r\n")
#else
								_T("\n")
#endif
								);
							mihajong_socket::puts(SOCK_CHAT + k - 1, buf);
							chatappend(buf);
					}
				}
			} else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
				_tcscat_s(buf, bufsize, sendQueue.front().c_str());
				if ((_tcslen(buf)) && (buf[_tcslen(buf) - 1] != _T('\n')))
					_tcscat_s(buf, bufsize,
#ifdef _WIN32
					_T("\r\n")
#else
					_T("\n")
#endif
					);
				mihajong_socket::puts(SOCK_CHAT, buf);
			}
			sendQueue.pop();
		}
	});
}

void ChatThread::cleanup() {
	for (int i = 0; i < Players; i++) // 開けてないソケットを閉じようとしても安全になるようにしてる
		mihajong_socket::hangup(SOCK_CHAT + i);
}

CodeConv::tstring StreamLog::getlog () {
	return CodeConv::tstring(mihajong_graphic::logwnd::getlogptr());
}
CodeConv::tstring ChatThread::getlog () {
	return streamLock.syncDo<CodeConv::tstring>([this]() {
		return CodeConv::tstring(mihajong_graphic::logwnd::getlogptr());
	});
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
	streamLock.acquire();
	super::sysmsg(str); // ←ラムダを使うとこれが呼び出せない
	streamLock.release();
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
	sendQueueLock.syncDo<void>([this, &tmpnum, msg]() -> void {
		sendQueue.push(CodeConv::tstring(tmpnum) + msg);
	});
}

// -------------------------------------------------------------------------

StreamLog* chatobj = nullptr;

__declspec(dllexport) void initchat (const char* const server_addr, int clientNum) {
	std::string serverAddr(server_addr);
	if ((EnvTable::Instantiate()->GameMode == EnvTable::Server) ||
		(EnvTable::Instantiate()->GameMode == EnvTable::Client))
		chatobj = new ChatThread(serverAddr, clientNum);
	else chatobj = new StreamLog();
}
__declspec(dllexport) void appendchat (LPCTSTR const chatstr) {
	chatobj->sysmsg(chatstr);
}
__declspec(dllexport) void sendchat (LPCTSTR const chatstr) {
	chatobj->sendstr(chatstr);
}
void sendchatx (int player, LPCTSTR const chatstr) {
	chatobj->sendstrx(player, chatstr);
}
__declspec(dllexport) void closechat () {
	delete chatobj; chatobj = nullptr;
}

} /* namespace */
