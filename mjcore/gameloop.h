#pragma once

#include "endround.h"
#include "gametbl.h"

EndType doTableTurn(GameTable* const gameStat);
bool doTableRound(GameTable* const gameStat);
void startgame(gameTypeID gameType);
