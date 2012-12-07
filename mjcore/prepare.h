#pragma once

#include <cstdint>
#include <algorithm>
#include <functional>
#include "../socket/socket.h"
#include "gametbl.h"
#include "envtbl.h"
#include "random.h"
#include "haifu.h"
#include "tileutil.h"
#include "yaku/yaku.h"
#include "ai/class.h"
#include "../sound/sound.h"
#include "bgmid.h"
#include "random.h"

__declspec(dllexport) void shuffle(GameTable* const gameStat); // 牌をバッファに並べて、洗牌
__declspec(dllexport) void initdora(GameTable* const gameStat); // ドラの設定

class SeatShuffler { // 場決め処理
private:
	static int* posarry;
	static unsigned ClientNumber;
	static void shuffleSeat ();
public:
	static std::array<int, PLAYERS> shuffle(unsigned cNumber);
};

void gameinit(GameTable* gameStat, gameTypeID gameType);
void tableinit(GameTable* const gameStat);
