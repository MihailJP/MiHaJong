#pragma once

#include "gametbl.h"
#include "discard.h"

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
		void checkremotenaki(PlayerID player, int& ReceivedMsg);
		static DWORD WINAPI startthread(LPVOID param);
	public:
		RemoteNaki(GameTable* const gStat);
		bool isFinished () {return finished;}
	};

	DiscardTileNum remotedahai (GameTable* const gameStat);
	void remotenaki (GameTable* const gameStat);
}
