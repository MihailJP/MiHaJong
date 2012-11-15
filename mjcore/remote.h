#ifndef REMOTE_H
#define REMOTE_H

#include "../socket/socket.h"
#include "gametbl.h"
#include "envtbl.h"
#include "discard.h"
#include "tileutil.h"
#include <sstream>
#include <iomanip>

namespace RemoteAction {
	class RemoteDahai {
	private:
		volatile bool finished;
		volatile DiscardTileNum remoteDahai;
		GameTable* gameStat;
		DWORD WINAPI thread();
		static DWORD WINAPI startthread(LPVOID param);
	public:
		RemoteDahai(GameTable* const gStat);
		bool isFinished () {return finished;}
		DiscardTileNum get () {return *const_cast<DiscardTileNum*>(&remoteDahai);}
	};

	class RemoteNaki {
	private:
		volatile bool finished;
		GameTable* gameStat;
		void thread_client();
		void thread_server();
		DWORD WINAPI thread();
		void checkremotenaki(PLAYER_ID player, int& ReceivedMsg);
		static DWORD WINAPI startthread(LPVOID param);
	public:
		RemoteNaki(GameTable* const gStat);
		bool isFinished () {return finished;}
	};

	union RemoteActionPtr {
		RemoteDahai* dahai;
		RemoteNaki* naki;
	};

	extern RemoteActionPtr rDahaiProc;
	__declspec(dllexport) void remotedahai_begin (GameTable* const gameStat);
	__declspec(dllexport) int remotedahai_isfinished ();
	__declspec(dllexport) int remotedahai_getdiscard ();
	__declspec(dllexport) void remotedahai_end ();
	__declspec(dllexport) void remotenaki_begin (GameTable* const gameStat);
	__declspec(dllexport) int remotenaki_isfinished ();
	__declspec(dllexport) void remotenaki_end ();
}

#endif
