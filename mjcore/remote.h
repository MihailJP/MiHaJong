#pragma once

#include "gametbl.h"
#include "discard.h"
#include <thread>

namespace RemoteAction {
	class RemoteDahai {
	private:
		volatile bool finished;
		volatile DiscardTileNum remoteDahai;
		GameTable* gameStat;
		std::thread myThread;
		void thread();
		static void startthread(RemoteDahai* inst);
	public:
		RemoteDahai(GameTable* const gStat);
		RemoteDahai(const RemoteDahai&) = delete; // Delete unexpected copy constructor
		RemoteDahai& operator= (const RemoteDahai&) = delete; // Delete unexpected assign operator
		~RemoteDahai();
		bool isFinished() { return finished; }
		DiscardTileNum get () {return *const_cast<DiscardTileNum*>(&remoteDahai);}
	};

	class RemoteNaki {
	private:
		volatile bool finished;
		GameTable* gameStat;
		void thread_client();
		void thread_server();
		std::thread myThread;
		void thread();
		static void startthread(RemoteNaki* inst);
		void checkremotenaki(PlayerID player, int& ReceivedMsg);
	public:
		RemoteNaki(GameTable* const gStat);
		RemoteNaki(const RemoteNaki&) = delete; // Delete unexpected copy constructor
		RemoteNaki& operator= (const RemoteNaki&) = delete; // Delete unexpected assign operator
		~RemoteNaki();
		bool isFinished() { return finished; }
	};

	DiscardTileNum remotedahai (GameTable* const gameStat);
	void remotenaki (GameTable* const gameStat);
}

namespace RemoteConnection {

void startServer(std::string& serverAddr, unsigned short gamePort);
void startClient(std::string& serverAddr, unsigned& ClientNumber, unsigned short gamePort);

}
