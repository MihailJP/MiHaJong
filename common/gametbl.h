#pragma once

#include <cstdint>
#include <type_traits>
#include "tilecode.h"
#include "gametype.h"
#include "largenum.h"

namespace mihajong_structs {

// -------------------------------------------------------------------------

enum doraCol : uint8_t { Normal, AkaDora, AoDora };
struct Tile { // 赤ドラデータを含めた牌のデータ
	TileCode tile;
	doraCol red;
};
static_assert(std::is_pod<Tile>::value, "Tile is not POD");

// -------------------------------------------------------------------------

const unsigned int Players = 4;
#ifdef MJCORE_EXPORTS
#define ACTUAL_PLAYERS (chkGameType(&GameStat, SanmaT) ? 3 : 4)
#endif
const unsigned int NumOfTilesInHand = 14;
const unsigned int SizeOfDiscardBuffer = 33;

typedef int8_t PlayerID; // プレイヤー番号

template <class T> struct InfoByPlayer { // プレイヤーごとに指定した型による情報(テンプレート)
	T val[Players];
	const T& operator[](const PlayerID playerID) const {
		if ((playerID >= 0)&&(playerID < Players)) {
			return val[playerID];
		}
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByPlayer:添字が範囲外です (") << (int)playerID << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			terminate();
		}
	}
	const T& operator[](const int playerID) const {
		return InfoByPlayer::operator[]((PlayerID)playerID);
	}
	T& operator[](const PlayerID playerID) {
		if ((playerID >= 0)&&(playerID < Players)) {
			return val[playerID];
		}
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByPlayer:添字が範囲外です (") << (int)playerID << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			terminate();
		}
	}
	T& operator[](const int playerID) {
		return InfoByPlayer::operator[]((PlayerID)playerID);
	}
};

// -------------------------------------------------------------------------

typedef Tile HandTiles[NumOfTilesInHand];

// -------------------------------------------------------------------------

const unsigned int SutehaiTypeStep = 200;
enum DiscardStat : uint8_t {
	discardNormal,
	discardTaken,
	discardRiichi,
	discardRiichiTaken,
};
struct DiscardTile {
	Tile tcode;
	DiscardStat dstat;
	bool isDiscardThrough; // ツモ切りフラグ
};
typedef DiscardTile DiscardBuf[SizeOfDiscardBuffer];
static_assert(std::is_pod<DiscardTile>::value, "DiscardTile is not POD");

// -------------------------------------------------------------------------

const unsigned int SizeOfMeldBuffer = 5;
const unsigned int MeldTypeStep = 1000;
enum MeldStat : uint8_t {
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
	meldQuadAddedRight,         // 下家からポンの後カン
};
struct MeldCode {
	TileCode tile;
	doraCol red[4];
	MeldStat mstat;
};
typedef MeldCode MeldBuf[SizeOfMeldBuffer];
static_assert(std::is_pod<MeldCode>::value, "MeldCode is not POD");

// -------------------------------------------------------------------------

struct RichiStat {bool RichiFlag, IppatsuFlag, DoubleFlag, OpenFlag;};
static_assert(std::is_pod<RichiStat>::value, "RichiStat is not POD");

// -------------------------------------------------------------------------

enum ChankanStat : uint8_t {
	chankanNone, chankanRegular, chankanOfAnkan, chankanOfNorth,
};
struct KangStat { bool kangFlag, topFlag; uint8_t chainFlag; ChankanStat chankanFlag; };
static_assert(std::is_pod<KangStat>::value, "KangStat is not POD");

// -------------------------------------------------------------------------

enum PaoYakuPage : uint8_t {pyDaisangen, pyDaisixi, pySikang, pyMinkan, PaoYakuPages};

struct PaoStat { PlayerID paoPlayer, agariPlayer; };
typedef PaoStat PaoStatBook[PaoYakuPages];
static_assert(std::is_pod<PaoStat>::value, "PaoStat is not POD");


// -------------------------------------------------------------------------

const unsigned SizeOfDeckBuf = 144;
typedef Tile DeckBuf[SizeOfDeckBuf]; // 最初はunionでやろうと思ったけどおかしくなるのでやめた

// -------------------------------------------------------------------------

struct PrevMeldBook { TileCode Discard, Stepped; };
static_assert(std::is_pod<PrevMeldBook>::value, "PrevMeldBook is not POD");

// -------------------------------------------------------------------------

struct DoraStatBook { UInt8ByTile Omote, Ura; };
static_assert(std::is_pod<DoraStatBook>::value, "DoraStatBook is not POD");

// -------------------------------------------------------------------------

enum ChiiType : int8_t {
	chiiNone, chiiLower, chiiMiddle, chiiUpper,
};
struct DeclFlag {
	bool Ron, Kan, Pon;
	ChiiType Chi;
};
static_assert(std::is_pod<DeclFlag>::value, "DeclFlag is not POD");

// -------------------------------------------------------------------------

struct CurrPlayer { PlayerID Active, Passive, Agari, Furikomi; };
static_assert(std::is_pod<CurrPlayer>::value, "CurrPlayer is not POD");

// -------------------------------------------------------------------------

struct Flowers { // 花牌フラグ用
	bool Spring, Summer, Autumn, Winter,
		Plum, Orchid, Chrys, Bamboo;
	/* Chrysanthemum は綴りが長いので省略…… */
};
static_assert(std::is_pod<Flowers>::value, "Flowers is not POD");

// -------------------------------------------------------------------------

struct Dice { // サイコロ
	uint8_t Number;
	uint8_t Direction;
};
static_assert(std::is_pod<Dice>::value, "Dice is not POD");

// -------------------------------------------------------------------------

enum handStatCode : int8_t {
	handUpright, handExposed, handHidden, handOpenRiichi,
};
struct PlayerTable { // プレイヤーの状態を格納
	LargeNum PlayerScore;
	int playerChip; // チップの収支
	HandTiles Hand; // 手牌の配列
	DiscardBuf Discard; // 捨牌の配列
	MeldBuf Meld; // 鳴き面子を格納
	uint8_t DiscardPointer;
	uint8_t MeldPointer;
	bool MenzenFlag; // 門前フラグ
	handStatCode HandStat; // 手牌の状態（立てる・見せる・伏せる）
	int8_t NumberOfQuads; // 槓子の数（四槓流局、三槓子、四槓子などの判定に使う）
	bool FirstDrawFlag; // １巡目である（地和、ダブル立直の判定に使う）
	bool DoujunFuriten; // 同順振聴である
	bool AgariHouki; // 和了り放棄の罰則中かどうか
	RichiStat RichiFlag; // リーチしているかどうか
	DeclFlag DeclarationFlag; // 鳴きの宣言
	Flowers FlowerFlag; // 晒している花牌を格納するフラグ
	uint8_t NorthFlag; // 晒している北風牌を格納するフラグ
	bool SumaroFlag; // 四馬路解禁フラグ
	bool YakitoriFlag; // 焼き鳥フラグ
	bool ConnectionLost;
};
static_assert(std::is_pod<PlayerTable>::value, "PlayerTable is not POD");

// -------------------------------------------------------------------------

template struct InfoByPlayer<PlayerTable>;
typedef InfoByPlayer<PlayerTable> StatusByPlayer;
struct GameTable { // 卓の情報を格納する
	StatusByPlayer Player;
	DeckBuf Deck; // 壁牌の配列
	DoraStatBook DoraFlag; // ドラ判定の配列
	FlagByTile OpenRichiWait; // プンリーの待ち牌(ＣＯＭに意図的な放銃を起こさせないために使用)
	PaoStatBook PaoFlag; // 包フラグ（-1…なし、0～3…該当プレイヤー）
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	CurrPlayer CurrentPlayer;
	PlayerID PlayerID;
	mihajong_structs::PlayerID LastAgariPlayer;
	GameTypeID gameType;
	bool TsumoAgariFlag; // ツモアガリ？
	int16_t AgariSpecialStat; // 今のところ食い変えでチョンボになる場合だけ使ってる？
	PrevMeldBook PreviousMeld; // 先ほど鳴いた牌（喰い替えの判定に使う）
	KangStat KangFlag; // 嶺上開花；連開花と槓振り；頭槓和；搶槓の判定に使う
	Dice Dice[2]; // サイコロ
	uint8_t TurnRound; // 現在の巡目
	uint8_t KangNum; // 四槓流局、四槓子などの判定に使う
	bool RichiCounter; // リーチをカウンター(宣言牌をロン)
	bool DoubleRichiCounter; // ダブルリーチをカウンター(宣言牌をロン)
	mihajong_structs::PlayerID WaremePlayer; // 割れ目の位置(-1で割れ目なし)
	mihajong_structs::PlayerID DoukasenPlayer; // 導火線の位置(-1で導火線なし)
	uint8_t DeadTiles; // 王牌の数
	uint8_t ExtraRinshan; // 追加の嶺上牌の数
	bool ShibariFlag; //二飜縛り
	uint8_t TilePointer; // ツモ牌のポインタ
	uint16_t DoraPointer;
	uint8_t RinshanPointer; // 嶺上牌のポインタ
	bool TianHuFlag; // 親の第一打牌がまだ（天和の判定などに使う）
	Tile CurrentDiscard;
};
static_assert(std::is_pod<GameTable>::value, "GameTable is not POD");

// -------------------------------------------------------------------------

// 食い変え判定用の gameStat->AgariSpecialStat 番号
const unsigned int agariKuikae = 999;

// -------------------------------------------------------------------------

inline bool chkGameType(const GameTable* const gameStat, GameTypeID gameType) {
	return ((gameStat->gameType) & gameType);
}

// -------------------------------------------------------------------------

} /* namespace */
