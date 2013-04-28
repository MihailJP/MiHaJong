#pragma once

#include "../common/gametbl.h"

using namespace mihajong_structs;

#ifdef MJCORE_EXPORTS
extern GameTable GameStat, StatSandBox;
void calcWareme(GameTable* const gameStat);
GameTable* makesandBox(const GameTable* const gameStat, PlayerID targetPlayer);
void inittable(GameTable* const gameStat);
GameTable* initializeGameTable(GameTypeID gameType);
#endif
MJCORE GameTable* setGameType(GameTypeID gameType);

// -------------------------------------------------------------------------
