#ifndef GAMETBL_H
#define GAMETBL_H

#include <array>
#include <cstdint>
#include "tilecode.h"

// 青天ルール対策
// 一応21不可思議まで表現可能……

#define DIGIT_GROUPS 8

class LargeNum {
private:
	std::array<int32_t, DIGIT_GROUPS> digitGroup;
	void fix() { // 正規形に直す
		for (int i = 0; i < (DIGIT_GROUPS - 1); i++) {
			if ((this->digitGroup[i] > 99999999)||(this->digitGroup[i] < -99999999)) {
				this->digitGroup[i + 1] += this->digitGroup[i] / 100000000;
				this->digitGroup[i] %= 100000000;
			}
		}
	}
public:
	LargeNum() { // ±21不可思議まで表現可能な数のクラス
		this->digitGroup.fill(0);
	}
	LargeNum(int val) { // ±21不可思議まで表現可能な数のクラス
		this->digitGroup.fill(0);
		this->digitGroup[0] = (val % 100000000);
		this->digitGroup[1] = (val / 100000000);
	}
	/* ここから演算子をオーバーロード */
	const LargeNum operator+(const LargeNum& addend) {
		LargeNum ans;
		for (int i = 0; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i] + addend.digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator+(const int32_t addend) {
		LargeNum ans;
		ans.digitGroup[0] = digitGroup[0] + addend;
		for (int i = 1; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i];
		ans.fix();
		return ans;
	}
	LargeNum& operator+=(const LargeNum& addend) {
		for (int i = 0; i < DIGIT_GROUPS; i++)
			digitGroup[i] += addend.digitGroup[i];
		fix();
		return *this;
	}
	LargeNum& operator+=(const int32_t addend) {
		digitGroup[0] += addend;
		fix();
		return *this;
	}
	const LargeNum operator-(const LargeNum& subtrahend) {
		LargeNum ans;
		for (int i = 0; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i] - subtrahend.digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator-(const int32_t subtrahend) {
		LargeNum ans;
		ans.digitGroup[0] = digitGroup[0] - subtrahend;
		for (int i = 1; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator*(const int32_t multiplier) { // めんどくさいので32bit整数倍だけ……
		LargeNum ans = LargeNum();
		for (int i = 0; i < DIGIT_GROUPS; i++) {
			int64_t tmpdigit = digitGroup[i] * multiplier;
			ans.digitGroup[i] = (int32_t)(tmpdigit % 100000000L);
			if ((i == (DIGIT_GROUPS - 1))
				&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN)))
				throw std::overflow_error("オーバーフローしました");
			ans.digitGroup[i + 1] = (int32_t)(tmpdigit / 100000000L);
		}
		ans.fix();
		return ans;
	}
	const int32_t& operator[] (const int i) const { // const 配列アクセス……
		return digitGroup[i];
	}
	int32_t& operator[] (const int i) { // non-const 配列アクセス……
		return digitGroup[i];
	}
};

#define PLAYERS 4
#define NUM_OF_TILES_IN_HAND 14
#define SIZE_OF_DISCARD_BUFFER 33

typedef int8_t PLAYER_ID; // プレイヤー番号
typedef std::array<int, PLAYERS> INT_EACH_PLAYER;
typedef std::array<LargeNum, PLAYERS> LARGENUM_EACH_PLAYER;

enum handTilePage { tlCode, redTile };

typedef std::array<tileCode, NUM_OF_TILES_IN_HAND> HAND_TILES;
typedef std::array<HAND_TILES, PLAYERS> HAND_EACH_PLAYER;

#define SUTEHAI_TYPE_STEP 200
enum discardStat {
	discardNormal,
	discardTaken,
	discardRiichi,
	discardRiichiTaken,
};
struct discardTile {
	tileCode tcode;
	discardStat dstat;
};
enum discardTilePage { dTileCode, dRedTile, dThrough };

typedef std::array<discardTile, SIZE_OF_DISCARD_BUFFER> DISCARD_BUF;
typedef std::array<DISCARD_BUF, PLAYERS> DISCARD_EACH_PLAYER;

enum meldTilePage { mTileCode, mRedTile };



class GameTable { // 卓の情報を格納するためのクラス
public:
	LARGENUM_EACH_PLAYER PlayerScore;
	INT_EACH_PLAYER playerChip;
	INT_EACH_PLAYER SumaroFlag;
	INT_EACH_PLAYER YakitoriFlag;
	PLAYER_ID PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int TurnRound;
	int Deposit;
	int AgariChain;
	int LastAgariPlayer;
	std::array<HAND_EACH_PLAYER, 2> Hand;
	std::array<DISCARD_EACH_PLAYER, 3> Discard;
	INT_EACH_PLAYER DiscardPointer;
	int Meld;
	int MenzenFlag;
	int HandStat;
	int NumberOfQuads;
	int RichiFlag;
	int OpenRichiWait;
	int FirstDrawFlag;
	int DoujunFuriten;
	int AgariHouki;
	int FlowerFlag;
	int NorthFlag;
	int KangFlag;
	int KangNum;
	int RichiCounter;
	int WaremePlayer;
	int DoukasenPlayer;
	int Dice1;
	int Dice2;
	int Dice1Direction;
	int Dice2Direction;
	int PaoFlag;
	int Deck;
	int DeadTiles;
	int ExtraRinshan;
	int ShibariFlag;
	int DoraFlag;
	int TilePointer;
	int DoraPointer;
	int RinshanPointer;
	int TianHuFlag;
	int PreviousMeld;
	int ConnectionLost;
	int CurrentPlayer;
	int DeclarationFlag;
	int TsumoAgariFlag;
	int CurrentDiscard;
	// Constructor
	GameTable() {
	}
};

#endif
