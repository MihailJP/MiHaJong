#include "gametbl.h"
#include <cstring>

namespace mihajong_graphic {

GameTable GameStatus::myGameStat, GameStatus::myGameStat1;
GameStatus::GStatModFlag GameStatus::myModFlag = GameStatus::GStatModFlag();
bool GameStatus::isInitialized = false;

void GameStatus::updateGameStat(const GameTable* const gameStat) {
	myModFlag.myCriticalSection.syncDo<void>([gameStat]() -> void {
		std::memcpy(&myGameStat, gameStat, sizeof(GameTable));
		myModFlag.myModificationFlag = true;
	});
}

GameTable* GameStatus::gameStat() {
	if (isInitialized) return &myGameStat1;
	else return retrGameStat();
}

GameTable* GameStatus::retrGameStat() {
	myModFlag.myCriticalSection.syncDo<void>([]() -> void {
		myModFlag.myModificationFlag = false;
		std::memcpy(&myGameStat1, &myGameStat, sizeof(GameTable));
	});
	isInitialized = true;
	return &myGameStat1;
}

bool GameStatus::isModified() {
	return myModFlag.myCriticalSection.syncDo<bool>([]() {return myModFlag.myModificationFlag;});
}

GameStatus::GStatModFlag::GStatModFlag() {
	myModificationFlag = false;
}

GameStatus::GStatModFlag::~GStatModFlag() {
}

}
