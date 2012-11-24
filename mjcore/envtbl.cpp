#include "envtbl.h"

EnvTable* EnvTable::Instantiate() {
	// Singleton instance accessor
	static EnvTable instance;
	return &instance;
}

/* HSPからのアクセサ */

__declspec(dllexport) void EnvTable::setPlayerName(void *, int PlayerNum, LPCTSTR const NameStr) {
	Instantiate()->PlayerDat[PlayerNum].PlayerName = NameStr;
}
__declspec(dllexport) void EnvTable::getPlayerName(void *, int PlayerNum, LPTSTR const NameStr, int bufsize) {
	_tcscpy_s(NameStr, bufsize, Instantiate()->PlayerDat[PlayerNum].PlayerName.c_str());
}

__declspec(dllexport) void EnvTable::setRemotePlayer(void *, int PlayerNum, int Value) {
	Instantiate()->PlayerDat[PlayerNum].RemotePlayerFlag = Value;
}
__declspec(dllexport) int EnvTable::isRemotePlayer(void *, int PlayerNum) {
	return (int)Instantiate()->PlayerDat[PlayerNum].RemotePlayerFlag;
}

__declspec(dllexport) void EnvTable::setGameMode(void *, int Value) {
	Instantiate()->GameMode = (ClientType)Value;
}
__declspec(dllexport) int EnvTable::getGameMode(void *) {
	return (int)Instantiate()->GameMode;
}

__declspec(dllexport) void EnvTable::setWatchModeFlag(void *, int Value) {
	Instantiate()->WatchModeFlag = Value;
}
__declspec(dllexport) int EnvTable::getWatchModeFlag(void *) {
	return (int)Instantiate()->WatchModeFlag;
}

__declspec(dllexport) void EnvTable::setTableColor(void *, int R, int G, int B) {
	Instantiate()->bgColorR = R;
	Instantiate()->bgColorG = G;
	Instantiate()->bgColorB = B;
}
__declspec(dllexport) int EnvTable::getTableColorR(void *) {
	return (int)(Instantiate()->bgColorR);
}
__declspec(dllexport) int EnvTable::getTableColorG(void *) {
	return (int)(Instantiate()->bgColorG);
}
__declspec(dllexport) int EnvTable::getTableColorB(void *) {
	return (int)(Instantiate()->bgColorB);
}

void EnvTable::TableEnvInit() {
	for (int i = 0; i < PLAYERS; i++) {
		Instantiate()->PlayerDat[i].PlayerName = _T("");
		Instantiate()->PlayerDat[i].RemotePlayerFlag = false;
	}
	Instantiate()->GameMode = Unavailable;
	Instantiate()->WatchModeFlag = false;
}
__declspec(dllexport) void EnvTable::TableEnvInit(void *) {
	TableEnvInit();
}
__declspec(dllexport) void EnvTable::TableEnvInitAll(void *) {
	TableEnvInit(nullptr);
	Instantiate()->bgColorR =
		Instantiate()->bgColorG =
		Instantiate()->bgColorB = 0;
}
