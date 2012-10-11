#include "chat.h"

namespace chat {

DWORD WINAPI ChatThread::thread_loop (LPVOID param) {
	reinterpret_cast<ChatThread*>(param)->init();
	while (!(reinterpret_cast<ChatThread*>(param)->terminate)) {
		reinterpret_cast<ChatThread*>(param)->receive();
		reinterpret_cast<ChatThread*>(param)->send();
	}
	reinterpret_cast<ChatThread*>(param)->cleanup();
	return S_OK;
}
ChatThread::ChatThread (std::string& server_addr, int clientNum) {
	myChatStream.str(""); terminate = false;
	myServerAddr = server_addr; myClientNum = clientNum;
	myHandle = CreateThread(nullptr, 0, thread_loop, this, 0, nullptr);
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
		bool tmpClientWaiting[PLAYERS];
		for (int i = 0; i < PLAYERS; i++) {
			tmpClientWaiting[i] = (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag > 1);
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

} /* namespace */
