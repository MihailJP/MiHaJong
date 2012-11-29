#include "gametbl.h"
#include <cstring>

namespace mihajong_graphic {

GameTable GameStatus::myGameStat;

void GameStatus::updateGameStat(const GameTable* const gameStat) {
	std::memcpy(&myGameStat, gameStat, sizeof(GameTable));
}

}
