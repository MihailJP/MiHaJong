#ifndef PREPARE_H
#define PREPARE_H

#include <algorithm>
#include "../socket/socket.h"
#include "gametbl.h"
#include "envtbl.h"
#include "random.h"
#include "haifu.h"
#include "tileutil.h"

__declspec(dllexport) void shuffle(GameTable* const gameStat); // 牌をバッファに並べて、洗牌
__declspec(dllexport) void initdora(GameTable* const gameStat); // ドラの設定
#endif
