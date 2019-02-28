#include "envtbl.h"

EnvTable* EnvTable::Instantiate() {
	// Singleton instance accessor
	static EnvTable instance;
	return &instance;
}

EnvTable::EnvTable() {
	for (int i = 0; i < Players; i++) {
		PlayerDat[i].PlayerName = _T("");
		PlayerDat[i].RemotePlayerFlag = false;
	}
	GameMode = Unavailable;
	WatchModeFlag = false;
	bgColorR = bgColorG = bgColorB = 0;
}

LPCTSTR getName(PlayerID playerID) {
	return EnvTable::Instantiate()->PlayerDat[playerID].PlayerName.c_str();
}

bool isStandAlone() {
	return EnvTable::Instantiate()->GameMode == EnvTable::Standalone;
}

bool isWatchMode() {
	return EnvTable::Instantiate()->WatchModeFlag;
}
