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
ChatThread::ChatThread (std::string& server_addr) {
	myServerAddr = server_addr; terminate = false;
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



} /* namespace */
