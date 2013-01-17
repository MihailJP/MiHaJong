#include "envtbl.h"

EnvTable* EnvTable::Instantiate() {
	// Singleton instance accessor
	static EnvTable instance;
	return &instance;
}

void EnvTable::TableEnvInit() {
	for (int i = 0; i < PLAYERS; i++) {
		Instantiate()->PlayerDat[i].PlayerName = _T("");
		Instantiate()->PlayerDat[i].RemotePlayerFlag = false;
	}
	Instantiate()->GameMode = Unavailable;
	Instantiate()->WatchModeFlag = false;
}
