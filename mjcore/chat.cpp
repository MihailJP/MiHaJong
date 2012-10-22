#include "chat.h"

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
StreamLog::StreamLog () {
	logWindow = nullptr;
	myChatStream.str("");
}
ChatThread::ChatThread (std::string& server_addr, int clientNum) {
	InitializeCriticalSection(&streamLock);
	InitializeCriticalSection(&sendQueueLock);
	myChatStream.str(""); terminate = false;
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
	DeleteCriticalSection(&streamLock);
	DeleteCriticalSection(&sendQueueLock);
}

void ChatThread::init() {
	if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
		bool tmpClientWaiting[PLAYERS];
		for (int i = 0; i < PLAYERS; i++) {
			tmpClientWaiting[i] = (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag > 0);
			mihajong_socket::listen(
				SOCK_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag,
				PORT_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag);
		}
		while (tmpClientWaiting[0] || tmpClientWaiting[1] || tmpClientWaiting[2] || tmpClientWaiting[3]) {
			for (int i = 0; i < PLAYERS; i++)
				if (mihajong_socket::connected(
					SOCK_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag))
					tmpClientWaiting[i] = false;
			Sleep(0);
		}
	}
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
		mihajong_socket::connect(SOCK_CHAT+0, myServerAddr.c_str(), PORT_CHAT+myClientNum);
		while (!mihajong_socket::connected(SOCK_CHAT+0)) Sleep(0); // Wait until connection established
	}
}

void ChatThread::receive() {
	if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
		for (int i = 0; i < PLAYERS; i++) {
			if (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag >= 2) {
				char buf[bufsize] = {0};
				int stat = mihajong_socket::gets(
					SOCK_CHAT-1+EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag,
					buf, bufsize);
				if (stat == 0) {
					for (int k = 2; k <= 4; k++) {
						if ((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == k) ||
							(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag == k) ||
							(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag == k) ||
							((!chkGameType(&GameStat, SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == k))) {
								strcat_s(buf, bufsize,
#ifdef _WIN32
									"\r\n"
#else
									"\n"
#endif
									);
								mihajong_socket::puts(SOCK_CHAT + k - 1, buf);
						}
						chatappend(buf);
					}
				}
			}
		}
	}
	else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
		char buf[bufsize] = {0};
		int stat = mihajong_socket::gets(SOCK_CHAT, buf, bufsize);
		if (stat == 0) chatappend(buf);
	}
}

void ChatThread::chatappend(const std::string& buf) {
	int tmpPlayer = static_cast<int>(buf[0] - '0');
	if ((tmpPlayer >= 0) && (tmpPlayer <= ACTUAL_PLAYERS)) {
		EnterCriticalSection(&streamLock);
		myChatStream << EnvTable::Instantiate()->PlayerDat[tmpPlayer].PlayerName <<
			"(" << windName(playerwind(&GameStat, tmpPlayer, GameStat.GameRound)) << ") : " <<
			std::string(buf.begin() + 1, buf.end()) <<
#ifdef _WIN32
			"\r" <<
#endif
			std::endl;
		LeaveCriticalSection(&streamLock);
		updateWindow();
	}
}

void ChatThread::send() {
	EnterCriticalSection(&sendQueueLock);
	if (!sendQueue.empty()) {
		char buf[bufsize] = {0}; buf[0] = GameStat.PlayerID + '0';
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			for (int k = 2; k <= 4; k++) {
				if ((EnvTable::Instantiate()->PlayerDat[0].RemotePlayerFlag == k) ||
					(EnvTable::Instantiate()->PlayerDat[1].RemotePlayerFlag == k) ||
					(EnvTable::Instantiate()->PlayerDat[2].RemotePlayerFlag == k) ||
					((!chkGameType(&GameStat, SanmaT)) && (EnvTable::Instantiate()->PlayerDat[3].RemotePlayerFlag == k))) {
						strcat_s(buf, bufsize, sendQueue.front().c_str());
						strcat_s(buf, bufsize,
#ifdef _WIN32
							"\r\n"
#else
							"\n"
#endif
							);
						mihajong_socket::puts(SOCK_CHAT + k - 1, buf);
				}
			}
		} else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
			strcat_s(buf, bufsize, sendQueue.front().c_str());
			strcat_s(buf, bufsize,
#ifdef _WIN32
				"\r\n"
#else
				"\n"
#endif
				);
			mihajong_socket::puts(SOCK_CHAT, buf);
		}
		sendQueue.pop();
	}
	LeaveCriticalSection(&sendQueueLock);
}

void ChatThread::cleanup() {
	for (int i = 0; i < PLAYERS; i++) // 開けてないソケットを閉じようとしても安全になるようにしてる
		mihajong_socket::hangup(SOCK_CHAT + i);
}

std::string StreamLog::getlog () {
	return std::string(myChatStream.str());
}
std::string ChatThread::getlog () {
	EnterCriticalSection(&streamLock);
	std::string& logbuf = myChatStream.str();
	LeaveCriticalSection(&streamLock);
	return std::string(logbuf);
}

void StreamLog::sysmsg(const std::string& str) {
	myChatStream << str <<
#ifdef _WIN32
		"\r" <<
#endif
		std::endl;
	updateWindow();
}
void ChatThread::sysmsg(const std::string& str) {
	EnterCriticalSection(&streamLock);
	myChatStream << str <<
#ifdef _WIN32
		"\r" <<
#endif
		std::endl;
	LeaveCriticalSection(&streamLock);
	updateWindow();
}

void StreamLog::sendstr (const std::string& msg) {
}
void ChatThread::sendstr (const std::string& msg) {
	EnterCriticalSection(&sendQueueLock);
	sendQueue.push(msg);
	LeaveCriticalSection(&sendQueueLock);
}

void StreamLog::setLogWindow (HWND wndHandle) {
	logWindow = wndHandle;
}

void StreamLog::updateWindow () {
	if (logWindow) {
		SendMessage(logWindow, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(myChatStream.str().c_str()));
		auto lines = SendMessage(logWindow, EM_GETLINECOUNT, 0, 0);
		SendMessage(logWindow, EM_SETSEL, myChatStream.str().length(), myChatStream.str().length());
		SendMessage(logWindow, EM_LINESCROLL, 0, lines);
	}
}
void ChatThread::updateWindow () {
	if (logWindow) {
		EnterCriticalSection(&streamLock);
		StreamLog::updateWindow();
		LeaveCriticalSection(&streamLock);
	}
}

// -------------------------------------------------------------------------

StreamLog* chatobj = nullptr;

__declspec(dllexport) void initchat (const char* const server_addr, int clientNum) {
	if ((EnvTable::Instantiate()->GameMode == EnvTable::Server) ||
		(EnvTable::Instantiate()->GameMode == EnvTable::Client))
		chatobj = new ChatThread(std::string(server_addr), clientNum);
	else chatobj = new StreamLog();
}
__declspec(dllexport) void appendchat (const char* const chatstr) {
	chatobj->sysmsg(chatstr);
}
__declspec(dllexport) void sendchat (const char* const chatstr) {
	chatobj->sendstr(chatstr);
}
__declspec(dllexport) void closechat () {
	delete chatobj; chatobj = nullptr;
}

__declspec(dllexport) void setlogwnd (HWND window) {
	chatobj->setLogWindow(window);
}

} /* namespace */
