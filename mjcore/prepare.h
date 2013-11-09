#pragma once

#include <array>
#include "gametbl.h"

void shuffle(GameTable* const gameStat); // �v���o�b�t�@�ɕ��ׂāA���v
#ifndef GUOBIAO
void initdora(GameTable* const gameStat); // �h���̐ݒ�
#endif /* GUOBIAO */

class SeatShuffler { // �ꌈ�ߏ���
private:
	static int* posarry;
	static unsigned ClientNumber;
	static void shuffleSeat ();
public:
	static std::array<int, Players> shuffle(unsigned cNumber);
public: /* Monostate class: cannot be instantiated */
	SeatShuffler() = delete;
	SeatShuffler(const SeatShuffler&) = delete;
	SeatShuffler& operator= (const SeatShuffler&) = delete;
	~SeatShuffler() = delete;
};

void gameinit(GameTable* gameStat, GameTypeID gameType, const std::string& ServerAddress,
	const std::array<int, 4>& PositionArray, unsigned ClientNumber);
void tableinit(GameTable* const gameStat);
