#pragma once

#include "gametbl.h"
#include "endtype.h"
#include "discard.h"

DiscardTileNum getdahai(GameTable* const gameStat);
EndType procdahai(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex);
void tsumoproc(GameTable* const gameStat);
