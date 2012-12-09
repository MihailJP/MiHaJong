#include "gametbl.h"
#include <cstring>

namespace mihajong_graphic {

GameTable GameStatus::myGameStat, GameStatus::myGameStat1;
GameStatus::GStatModFlag GameStatus::myModFlag = GameStatus::GStatModFlag();

void GameStatus::updateGameStat(const GameTable* const gameStat) {
	EnterCriticalSection(&myModFlag.myCriticalSection);
	std::memcpy(&myGameStat, gameStat, sizeof(GameTable));
	myModFlag.myModificationFlag = true;
	LeaveCriticalSection(&myModFlag.myCriticalSection);
}

GameTable* GameStatus::gameStat() {
	EnterCriticalSection(&myModFlag.myCriticalSection);
	myModFlag.myModificationFlag = false;
	std::memcpy(&myGameStat1, &myGameStat, sizeof(GameTable));
	LeaveCriticalSection(&myModFlag.myCriticalSection);
	return &myGameStat1;
}

bool GameStatus::isModified() {
	EnterCriticalSection(&myModFlag.myCriticalSection);
	bool flag = myModFlag.myModificationFlag;
	LeaveCriticalSection(&myModFlag.myCriticalSection);
	return flag;
}

GameStatus::GStatModFlag::GStatModFlag() {
	InitializeCriticalSection(&myCriticalSection);
	myModificationFlag = false;
}

GameStatus::GStatModFlag::~GStatModFlag() {
	DeleteCriticalSection(&myCriticalSection);
}

}
