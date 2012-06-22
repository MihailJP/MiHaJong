#ifndef GAMETBL_H
#define GAMETBL_H

#include <array>
#include <cstdint>
#include <exception>
#include <stdexcept>
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

#define SIZE_OF_MELD_BUFFER 5
#define MELD_TYPE_STEP 1000
enum meldTilePage { mTileCode, mRedTile };
enum meldStat {
	meldSequenceConcealed,      // 手の内の順子
	meldSequenceExposedLower,   // 小さい方をチー
	meldSequenceExposedMiddle,  // 嵌張をチー
	meldSequenceExposedUpper,   // 大きい方をチー
	meldTripletConcealed,       // 暗刻
	meldTripletExposedLeft,     // 上家からポン
	meldTripletExposedCenter,   // 対面からポン
	meldTripletExposedRight,    // 下家からポン
	meldQuadConcealed,          // 暗槓
	meldQuadExposedLeft,        // 上家から明槓
	meldQuadExposedCenter,      // 対面から明槓
	meldQuadExposedRight,       // 下家から明槓
	meldQuadAddedLeft,          // 上家からポンの後カン
	meldQuadAddedCenter,        // 対面からポンの後カン
	meldQuadAddedRight          // 下家からポンの後カン
};
struct meldCode {
	tileCode tcode;
	meldStat mstat;
};
typedef std::array<meldCode, SIZE_OF_MELD_BUFFER> MELD_BUF;
typedef std::array<MELD_BUF, PLAYERS> MELD_EACH_PLAYER;

struct RichiStat { // 立直フラグを格納
	bool RichiFlag;
	bool IppatsuFlag;
	bool DoubleFlag;
	bool OpenFlag;
};

#define KANG_PAGES 4
enum kangFlagPage {kfFlag, kfChainFlag, kfTopFlag, kfChankan};

struct PlayerTable { // プレイヤーの状態を格納
	LargeNum PlayerScore;
	int playerChip; // チップの収支
	bool SumaroFlag; // 四馬路解禁フラグ
	bool YakitoriFlag; // 焼き鳥フラグ
	std::array<HAND_TILES, 2> Hand; // 手牌の配列
	std::array<DISCARD_BUF, 3> Discard; // 捨牌の配列
	uint8_t DiscardPointer;
	std::array<MELD_BUF, 2> Meld; // 鳴き面子を格納
	uint8_t MeldPointer;
	bool MenzenFlag; // 門前フラグ
	int MenzenFlagAb;
	int8_t HandStat; // 手牌の状態（立てる・見せる・伏せる）
	int8_t NumberOfQuads; // 槓子の数（四槓流局、三槓子、四槓子などの判定に使う）
	RichiStat RichiFlag; // リーチしているかどうか
	bool FirstDrawFlag; // １巡目である（地和、ダブル立直の判定に使う）
	bool DoujunFuriten; // 同順振聴である
	bool AgariHouki; // 和了り放棄の罰則中かどうか
	uint8_t FlowerFlag; // 晒している花牌を格納するフラグ
	uint8_t NorthFlag; // 晒している北風牌を格納するフラグ
};

struct GameTable { // 卓の情報を格納する
public:
	std::array<PlayerTable, PLAYERS> Player;
	PLAYER_ID PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	int LastAgariPlayer;
	std::array<bool, TILE_NONFLOWER_MAX> OpenRichiWait; // プンリーの待ち牌(ＣＯＭに意図的な放銃を起こさせないために使用)
	std::array<uint8_t, KANG_PAGES> KangFlag; // 嶺上開花；連開花と槓振り；頭槓和；搶槓の判定に使う
	uint8_t TurnRound; // 現在の巡目
	uint8_t KangNum; // 四槓流局、四槓子などの判定に使う
	bool RichiCounter; // リーチをカウンター(宣言牌をロン)
	PLAYER_ID WaremePlayer; // 割れ目の位置(-1で割れ目なし)
	PLAYER_ID DoukasenPlayer; // 導火線の位置(-1で導火線なし)
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
	/*GameTable() {
	}*/
};

#endif
