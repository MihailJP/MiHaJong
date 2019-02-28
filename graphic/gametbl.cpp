#include "gametbl.h"
#include <cstring>
#include "utils.h"
#include "event.h"

namespace mihajong_graphic {

GameTable GameStatus::myGameStat, GameStatus::myGameStat1;
GameStatus::GStatModFlag GameStatus::myModFlag;
bool GameStatus::isInitialized = false;

void GameStatus::updateGameStat(const GameTable* const gameStat) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(myModFlag.myCriticalSection);
	std::memcpy(&myGameStat, gameStat, sizeof(GameTable));
	myModFlag.myModificationFlag = true;
	if (utils::isWatchMode && utils::isWatchMode())
		ui::CheckIfDemoTerminated();
}

GameTable* GameStatus::gameStat() {
	if (isInitialized) return &myGameStat1;
	else return retrGameStat();
}

GameTable* GameStatus::retrGameStat() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(myModFlag.myCriticalSection);
	myModFlag.myModificationFlag = false;
	std::memcpy(&myGameStat1, &myGameStat, sizeof(GameTable));
	lock.unlock();
	isInitialized = true;
	return &myGameStat1;
}

bool GameStatus::isModified() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(myModFlag.myCriticalSection);
	return myModFlag.myModificationFlag;
}

GameStatus::GStatModFlag::GStatModFlag() {
	myModificationFlag = false;
}

GameStatus::GStatModFlag::~GStatModFlag() {
}

}
