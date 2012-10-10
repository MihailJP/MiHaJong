#ifndef REMOTE_H
#define REMOTE_H

#include "../socket/socket.h"
#include "gametbl.h"
#include "envtbl.h"
#include "discard.h"
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

	extern RemoteDahai* rDahaiProc;
	__declspec(dllexport) void remotedahai_begin (GameTable* const gameStat);
	__declspec(dllexport) int remotedahai_isfinished ();
	__declspec(dllexport) int remotedahai_getdiscard ();
	__declspec(dllexport) void remotedahai_end ();
}

#endif
