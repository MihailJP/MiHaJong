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

	DiscardTileNum remotedahai (GameTable* const gameStat);
	void remotenaki (GameTable* const gameStat);
}

#endif
