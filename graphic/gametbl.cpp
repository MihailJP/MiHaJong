#include "gametbl.h"
#include <cstring>

namespace mihajong_graphic {

GameTable GameStatus::myGameStat, GameStatus::myGameStat1;
GameStatus::GStatModFlag GameStatus::myModFlag;
bool GameStatus::isInitialized = false;

void GameStatus::updateGameStat(const GameTable* const gameStat) {
	std::unique_lock<std::recursive_mutex> lock(myModFlag.myCriticalSection);
	std::memcpy(&myGameStat, gameStat, sizeof(GameTable));
	myModFlag.myModificationFlag = true;
}

GameTable* GameStatus::gameStat() {
	if (isInitialized) return &myGameStat1;
	else return retrGameStat();
}

GameTable* GameStatus::retrGameStat() {
	std::unique_lock<std::recursive_mutex> lock(myModFlag.myCriticalSection);
	myModFlag.myModificationFlag = false;
	std::memcpy(&myGameStat1, &myGameStat, sizeof(GameTable));
	lock.unlock();
	isInitialized = true;
	return &myGameStat1;
}

bool GameStatus::isModified() {
	std::unique_lock<std::recursive_mutex> lock(myModFlag.myCriticalSection);
	return myModFlag.myModificationFlag;
}

GameStatus::GStatModFlag::GStatModFlag() {
	myModificationFlag = false;
}

GameStatus::GStatModFlag::~GStatModFlag() {
}

}
