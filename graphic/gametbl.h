#ifndef GRAPHIC_GAMETBL_H
#define GRAPHIC_GAMETBL_H

#include "../mjcore/mjimport.h"
#include "exports.h"

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

#endif
