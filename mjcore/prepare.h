#pragma once

#include <array>
#include "gametbl.h"

void shuffle(GameTable* const gameStat); // 牌をバッファに並べて、洗牌
void initdora(GameTable* const gameStat); // ドラの設定

class SeatShuffler { // 場決め処理
private:
	static int* posarry;
	static unsigned ClientNumber;
	static void shuffleSeat ();
public:
	static std::array<int, Players> shuffle(unsigned cNumber);
};

void gameinit(GameTable* gameStat, GameTypeID gameType, const std::string& ServerAddress,
	const std::array<int, 4>& PositionArray, unsigned ClientNumber);
void tableinit(GameTable* const gameStat);
