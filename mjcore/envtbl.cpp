#include "envtbl.h"

EnvTable* EnvTable::Instantiate() {
	// Singleton instance accessor
	static EnvTable instance;
	return &instance;
}

EnvTable::EnvTable() {
	for (int i = 0; i < PLAYERS; i++) {
		PlayerDat[i].PlayerName = _T("");
		PlayerDat[i].RemotePlayerFlag = false;
	}
	GameMode = Unavailable;
	WatchModeFlag = false;
}
