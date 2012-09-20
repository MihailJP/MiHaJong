#ifndef PREPARE_H
#define PREPARE_H

#include "gametbl.h"
#include "envtbl.h"
#include "random.h"

__declspec(dllexport) void shuffle(GameTable* const gameStat); // 牌をバッファに並べて、洗牌

#endif
