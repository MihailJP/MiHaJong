#pragma once

#include "../common/gametbl.h"

using mihajong_structs::GameTable;
using mihajong_structs::GameTypeID;
using mihajong_structs::PlayerID;

#ifdef MJCORE_EXPORTS
extern GameTable GameStat, StatSandBox;
inline bool chkGameType(const GameTable* const gameStat, GameTypeID gameType);
__declspec(dllexport) void calcWareme(GameTable* const gameStat);
GameTable* makesandBox(const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) void inittable(GameTable* const gameStat);
GameTable* initializeGameTable(GameTypeID gameType);
#endif
MJCORE GameTable* setGameType(GameTypeID gameType);

// -------------------------------------------------------------------------
