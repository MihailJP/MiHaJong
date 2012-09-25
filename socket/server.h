#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include <Windows.h>

namespace mihajong_socket {
namespace server {

	class starter {
	private:
		DWORD WINAPI preparationThread ();
	public:
		static DWORD WINAPI initiate (LPVOID param);
	};
	extern starter* starterThread;
	DLL void start ();

}
}

#endif
