#ifndef GAMETBL_H
#define GAMETBL_H

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <algorithm>
#include <cassert>
#include <type_traits>
#endif
#include "mjexport.h"
#include "tilecode.h"
#include "ruletbl.h"
#include "largenum.h"
#include "except.h"

// -------------------------------------------------------------------------

enum doraCol : uint8_t { Normal, AkaDora, AoDora };
EXPORT_STRUCT TILE { // 赤ドラデータを含めた牌のデータ
	tileCode tile;
	doraCol red;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<TILE>::value, "TILE is not POD");
#endif

// -------------------------------------------------------------------------

#define PLAYERS 4
#ifdef MJCORE_EXPORTS
#define ACTUAL_PLAYERS (chkGameType(&GameStat, SanmaT) ? 3 : 4)
#endif
#define NUM_OF_TILES_IN_HAND 14
#define SIZE_OF_DISCARD_BUFFER 33

typedef int8_t PLAYER_ID; // プレイヤー番号

template <class T> struct InfoByPlayer { // プレイヤーごとに指定した型による情報(テンプレート)
	T val[PLAYERS];
	const T& operator[](const PLAYER_ID playerID) const {
		if ((playerID >= 0)&&(playerID < PLAYERS)) {
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
		return InfoByPlayer::operator[]((PLAYER_ID)playerID);
	}
	T& operator[](const PLAYER_ID playerID) {
		if ((playerID >= 0)&&(playerID < PLAYERS)) {
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
		return InfoByPlayer::operator[]((PLAYER_ID)playerID);
	}
};

// -------------------------------------------------------------------------

typedef TILE HAND_TILES[NUM_OF_TILES_IN_HAND];

// -------------------------------------------------------------------------

#define SUTEHAI_TYPE_STEP 200
enum discardStat : uint8_t {
	discardNormal,
	discardTaken,
	discardRiichi,
	discardRiichiTaken,
};
EXPORT_STRUCT discardTile {
	TILE tcode;
	discardStat dstat;
	bool isDiscardThrough; // ツモ切りフラグ
};
typedef discardTile DISCARD_BUF[SIZE_OF_DISCARD_BUFFER];
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<discardTile>::value, "discardTile is not POD");
#endif

// -------------------------------------------------------------------------

#define SIZE_OF_MELD_BUFFER 5
#define MELD_TYPE_STEP 1000
enum meldStat : uint8_t {
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
EXPORT_STRUCT meldCode {
	tileCode tile;
	doraCol red[4];
	meldStat mstat;
};
typedef meldCode MELD_BUF[SIZE_OF_MELD_BUFFER];
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<meldCode>::value, "meldCode is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT RichiStat {bool RichiFlag, IppatsuFlag, DoubleFlag, OpenFlag;};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<RichiStat>::value, "RichiStat is not POD");
#endif

// -------------------------------------------------------------------------

enum ChankanStat : uint8_t {
	chankanNone, chankanRegular, chankanOfAnkan, chankanOfNorth,
};
EXPORT_STRUCT KANGSTAT { bool kangFlag, topFlag; uint8_t chainFlag; ChankanStat chankanFlag; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<KANGSTAT>::value, "KANGSTAT is not POD");
#endif

// -------------------------------------------------------------------------

#define PAO_YAKU_PAGES 4
enum paoYakuPage : uint8_t {pyDaisangen, pyDaisixi, pySikang, pyMinkan};

EXPORT_STRUCT PAOSTAT { PLAYER_ID paoPlayer, agariPlayer; };
typedef PAOSTAT paoStatBook[PAO_YAKU_PAGES];
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<PAOSTAT>::value, "PAOSTAT is not POD");
#endif


// -------------------------------------------------------------------------

#define SIZE_OF_DECKBUF 144
typedef TILE DeckBuf[SIZE_OF_DECKBUF]; // 最初はunionでやろうと思ったけどおかしくなるのでやめた

// -------------------------------------------------------------------------

EXPORT_STRUCT prevMeldBook { tileCode Discard, Stepped; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<prevMeldBook>::value, "prevMeldBook is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT doraStatBook { UInt8ByTile Omote, Ura; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<doraStatBook>::value, "doraStatBook is not POD");
#endif

// -------------------------------------------------------------------------

enum ChiiType : int8_t {
	chiiNone, chiiLower, chiiMiddle, chiiUpper,
};
EXPORT_STRUCT DECLFLAG {
	bool Ron, Kan, Pon;
	ChiiType Chi;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<DECLFLAG>::value, "DECLFLAG is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT CURRPLAYER { PLAYER_ID Active, Passive, Agari, Furikomi; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<CURRPLAYER>::value, "CURRPLAYER is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT FLOWERS { // 花牌フラグ用
	bool Spring, Summer, Autumn, Winter,
		Plum, Orchid, Chrys, Bamboo;
	/* Chrysanthemum は綴りが長いので省略…… */
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<FLOWERS>::value, "FLOWERS is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT DICE { // サイコロ
	uint8_t Number;
	bool Direction;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<DICE>::value, "DICE is not POD");
#endif

// -------------------------------------------------------------------------

enum handStatCode : int8_t {
	handUpright, handExposed, handHidden, handOpenRiichi,
};
EXPORT_STRUCT PlayerTable { // プレイヤーの状態を格納
	LargeNum PlayerScore;
	int playerChip; // チップの収支
	bool SumaroFlag; // 四馬路解禁フラグ
	bool YakitoriFlag; // 焼き鳥フラグ
	HAND_TILES Hand; // 手牌の配列
	DISCARD_BUF Discard; // 捨牌の配列
	uint8_t DiscardPointer;
	MELD_BUF Meld; // 鳴き面子を格納
	uint8_t MeldPointer;
	bool MenzenFlag; // 門前フラグ
	handStatCode HandStat; // 手牌の状態（立てる・見せる・伏せる）
	int8_t NumberOfQuads; // 槓子の数（四槓流局、三槓子、四槓子などの判定に使う）
	RichiStat RichiFlag; // リーチしているかどうか
	bool FirstDrawFlag; // １巡目である（地和、ダブル立直の判定に使う）
	bool DoujunFuriten; // 同順振聴である
	bool AgariHouki; // 和了り放棄の罰則中かどうか
	FLOWERS FlowerFlag; // 晒している花牌を格納するフラグ
	uint8_t NorthFlag; // 晒している北風牌を格納するフラグ
	bool ConnectionLost;
	DECLFLAG DeclarationFlag; // 鳴きの宣言
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<PlayerTable>::value, "PlayerTable is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_TEMPLATE_STRUCT InfoByPlayer<PlayerTable>;
typedef InfoByPlayer<PlayerTable> StatusByPlayer;
EXPORT_STRUCT GameTable { // 卓の情報を格納する
	gameTypeID gameType;
	StatusByPlayer Player;
	PLAYER_ID PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	PLAYER_ID LastAgariPlayer;
	FlagByTile OpenRichiWait; // プンリーの待ち牌(ＣＯＭに意図的な放銃を起こさせないために使用)
	KANGSTAT KangFlag; // 嶺上開花；連開花と槓振り；頭槓和；搶槓の判定に使う
	uint8_t TurnRound; // 現在の巡目
	uint8_t KangNum; // 四槓流局、四槓子などの判定に使う
	bool RichiCounter; // リーチをカウンター(宣言牌をロン)
	bool DoubleRichiCounter; // ダブルリーチをカウンター(宣言牌をロン)
	PLAYER_ID WaremePlayer; // 割れ目の位置(-1で割れ目なし)
	PLAYER_ID DoukasenPlayer; // 導火線の位置(-1で導火線なし)
	DICE Dice[2]; // サイコロ
	paoStatBook PaoFlag; // 包フラグ（-1…なし、0～3…該当プレイヤー）
	DeckBuf Deck; // 壁牌の配列
	uint8_t DeadTiles; // 王牌の数
	uint8_t ExtraRinshan; // 追加の嶺上牌の数
	bool ShibariFlag; //二飜縛り
	doraStatBook DoraFlag; // ドラ判定の配列
	uint8_t TilePointer; // ツモ牌のポインタ
	uint16_t DoraPointer;
	uint8_t RinshanPointer; // 嶺上牌のポインタ
	bool TianHuFlag; // 親の第一打牌がまだ（天和の判定などに使う）
	prevMeldBook PreviousMeld; // 先ほど鳴いた牌（喰い替えの判定に使う）
	CURRPLAYER CurrentPlayer;
	bool TsumoAgariFlag; // ツモアガリ？
	int16_t AgariSpecialStat; // 今のところ食い変えでチョンボになる場合だけ使ってる？
	TILE CurrentDiscard;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<GameTable>::value, "GameTable is not POD");
#endif

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
/* 先行宣言 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat);
PLAYER_ID tobePlayed(const GameTable* const gameStat, int id);

/* gametbl.cpp */
extern GameTable GameStat, StatSandBox;
inline bool chkGameType(const GameTable* const gameStat, gameTypeID gameType);
extern "C" __declspec(dllexport) void calcWareme(GameTable* const gameStat);
extern "C" GameTable* makesandBox(const GameTable* const gameStat, PLAYER_ID targetPlayer);
extern "C" __declspec(dllexport) void inittable(GameTable* const gameStat);
extern "C" GameTable* initializeGameTable(gameTypeID gameType);
#endif

// -------------------------------------------------------------------------

#endif
