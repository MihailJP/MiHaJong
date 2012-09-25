#include "server.h"

namespace mihajong_socket {
namespace server {
	starter* starterThread = nullptr;
	DWORD WINAPI starter::preparationThread () {
		return S_OK;
	}
	DWORD WINAPI starter::initiate (LPVOID param) {
		return ((starter*)param)->preparationThread();
	}
	DLL void start () {
		starterThread = new starter();
		CreateThread(nullptr, 0, starter::initiate, (LPVOID)starterThread, 0, nullptr);
	}
}
}
