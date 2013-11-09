#pragma once

#include "gametbl.h"
#include "discard.h"

namespace RemoteAction {
	class RemoteDahai {
	private:
		volatile bool finished;
		volatile DiscardTileNum remoteDahai;
		GameTable* gameStat;
#ifdef _WIN32
		DWORD WINAPI thread();
		static DWORD WINAPI startthread(LPVOID param);
#else /*_WIN32*/
		void* thread();
		static void* startthread(void* param);
#endif /*_WIN32*/
	public:
		RemoteDahai(GameTable* const gStat);
		RemoteDahai(const RemoteDahai&) = delete; // Delete unexpected copy constructor
		RemoteDahai& operator= (const RemoteDahai&) = delete; // Delete unexpected assign operator
		bool isFinished() { return finished; }
		DiscardTileNum get () {return *const_cast<DiscardTileNum*>(&remoteDahai);}
	};

	class RemoteNaki {
	private:
		volatile bool finished;
		GameTable* gameStat;
		void thread_client();
		void thread_server();
#ifdef _WIN32
		DWORD WINAPI thread();
		static DWORD WINAPI startthread(LPVOID param);
#else /*_WIN32*/
		void* thread();
		static void* startthread(void* param);
#endif /*_WIN32*/
		void checkremotenaki(PlayerID player, int& ReceivedMsg);
	public:
		RemoteNaki(GameTable* const gStat);
		RemoteNaki(const RemoteNaki&) = delete; // Delete unexpected copy constructor
		RemoteNaki& operator= (const RemoteNaki&) = delete; // Delete unexpected assign operator
		bool isFinished() { return finished; }
	};

	DiscardTileNum remotedahai (GameTable* const gameStat);
	void remotenaki (GameTable* const gameStat);
}

namespace RemoteConnection {

void startServer(std::string& serverAddr, unsigned short gamePort);
void startClient(std::string& serverAddr, unsigned& ClientNumber, unsigned short gamePort);

}
