#pragma once

#include "../mjcore/mjimport.h"
#include "exports.h"

namespace mihajong_graphic {

class GameStatus {
#ifdef GRAPHIC_EXPORTS
private:
	static GameTable myGameStat;
#endif
public:
#ifdef GRAPHIC_EXPORTS
	static GameTable* gameStat() {return &myGameStat;}
#endif
	EXPORT static void updateGameStat(const GameTable* const gameStat);
};

}
