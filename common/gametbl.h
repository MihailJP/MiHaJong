#pragma once

#ifdef _MSC_VER
#include <eh.h>
#endif
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <type_traits>
#include <functional>
#include "tilecode.h"
#include "gametype.h"
#include "largenum.h"

namespace mihajong_structs {

// -------------------------------------------------------------------------

enum doraCol : uint8_t { Normal, AkaDora, AoDora };
struct Tile { // 赤ドラデータを含めた牌のデータ
	TileCode tile;
	doraCol red;
	constexpr explicit operator bool() const {return tile != NoTile;}
	constexpr bool operator ! () const {return tile == NoTile;}
	constexpr bool operator == (const Tile& otherTile) const {return (tile == otherTile.tile) && (red == otherTile.red);}
	constexpr bool operator != (const Tile& otherTile) const {return !(*this == otherTile);}
	constexpr bool operator < (const Tile& otherTile) const {
		if (tile < otherTile.tile) return true;
		else if (tile > otherTile.tile) return false;
		else if (red == otherTile.red) return false;
		else if (red == Normal) return false;
		else if (otherTile.red == Normal) return true;
		else return red < otherTile.red;
	}
	constexpr bool operator > (const Tile& otherTile) const {return otherTile < *this;}
	constexpr bool operator <= (const Tile& otherTile) const {return !(otherTile < *this);}
	constexpr bool operator >= (const Tile& otherTile) const {return !(*this < otherTile);}
	constexpr TileSuit getSuit() const {
		switch (tile) {
		case CharacterOne: case CharacterTwo: case CharacterThree: case CharacterFour: case CharacterFive:
		case CharacterSix: case CharacterSeven: case CharacterEight: case CharacterNine:
			return TileSuitCharacters;
		case CircleOne: case CircleTwo: case CircleThree: case CircleFour: case CircleFive:
		case CircleSix: case CircleSeven: case CircleEight: case CircleNine:
			return TileSuitCircles;
		case BambooOne: case BambooTwo: case BambooThree: case BambooFour: case BambooFive:
		case BambooSix: case BambooSeven: case BambooEight: case BambooNine:
			return TileSuitBamboos;
		case EastWind: case SouthWind: case WestWind: case NorthWind:
		case WhiteDragon: case GreenDragon: case RedDragon:
			return TileSuitHonors;
		case Spring: case Summer: case Autumn: case Winter:
		case Plum: case Orchid: case Chrysanthemum: case Bamboo:
			return TileSuitFlowers;
		default:
			return TileSuitInvalid;
		}
	}
	constexpr bool isNumber() const {
		return (tile != NoTile) && (static_cast<unsigned int>(tile) < static_cast<unsigned int>(TileSuitHonors));		
	}
	constexpr bool isHonor() const {return getSuit() == TileSuitHonors;}
	constexpr bool isFlower() const {return getSuit() == TileSuitFlowers;}
	constexpr explicit Tile(TileCode tile = NoTile, doraCol red = Normal) : tile(tile), red(red) {}
};
static_assert(std::is_trivially_copyable<Tile>::value, "Tile is not trivially copyable");
static_assert(std::is_standard_layout<Tile>::value, "Tile is not standard layout");

// -------------------------------------------------------------------------

constexpr unsigned int Players = 4;
#ifdef MJCORE_EXPORTS
#define ACTUAL_PLAYERS (GameStat.chkGameType(GameTypeID::sanmaT) ? 3 : 4)
#endif
constexpr unsigned int NumOfTilesInHand = 14;
constexpr unsigned int TsumohaiIndex = NumOfTilesInHand - 1;
constexpr unsigned int SizeOfDiscardBuffer = 33;

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
			o << _T("InfoByPlayer:添字が範囲外です (") << static_cast<int>(playerID) << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
#ifdef _MSC_VER
			terminate();
#else
			abort();
#endif
		}
	}
	const T& operator[](const int playerID) const {
		return InfoByPlayer::operator[](static_cast<PlayerID>(playerID));
	}
	T& operator[](const PlayerID playerID) {
		if ((playerID >= 0)&&(playerID < Players)) {
			return val[playerID];
		}
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByPlayer:添字が範囲外です (") << static_cast<int>(playerID) << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
#ifdef _MSC_VER
			terminate();
#else
			abort();
#endif
		}
	}
	T& operator[](const int playerID) {
		return InfoByPlayer::operator[](static_cast<PlayerID>(playerID));
	}
};

// -------------------------------------------------------------------------

typedef Tile HandTiles[NumOfTilesInHand];

// -------------------------------------------------------------------------

constexpr unsigned int SutehaiTypeStep = 200;
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
	explicit operator bool() const {return tcode.tile != NoTile;}
	bool operator !() const {return tcode.tile == NoTile;}
	DiscardTile(Tile tile = Tile(), DiscardStat dStat = discardNormal, bool discardThrough = false)
		: tcode(tile), dstat(dStat), isDiscardThrough(discardNormal) {}
};
typedef DiscardTile DiscardBuf[SizeOfDiscardBuffer];
static_assert(std::is_trivially_copyable<DiscardTile>::value, "DiscardTile is not trivially copyable");
static_assert(std::is_standard_layout<DiscardTile>::value, "DiscardTile is not standard layout");

// -------------------------------------------------------------------------

constexpr unsigned int SizeOfMeldBuffer = 5;
constexpr unsigned int MeldTypeStep = 1000;
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
	explicit operator bool() const {return tile != NoTile;}
};
typedef MeldCode MeldBuf[SizeOfMeldBuffer];
static_assert(std::is_trivially_copyable<MeldCode>::value, "MeldCode is not trivially copyable");
static_assert(std::is_standard_layout<MeldCode>::value, "MeldCode is not standard layout");

// -------------------------------------------------------------------------

struct RichiStat {bool RichiFlag, IppatsuFlag, DoubleFlag, OpenFlag;};
static_assert(std::is_trivially_copyable<RichiStat>::value, "RichiStat is not trivially copyable");
static_assert(std::is_standard_layout<RichiStat>::value, "RichiStat is not standard layout");

// -------------------------------------------------------------------------

enum ChankanStat : uint8_t {
	chankanNone, chankanRegular, chankanOfAnkan, chankanOfNorth,
};
struct KangStat { bool kangFlag, topFlag; uint8_t chainFlag; ChankanStat chankanFlag; };
static_assert(std::is_trivially_copyable<KangStat>::value, "KangStat is not trivially copyable");
static_assert(std::is_standard_layout<KangStat>::value, "KangStat is not standard layout");

// -------------------------------------------------------------------------

enum PaoYakuPage : uint8_t {pyDaisangen, pyDaisixi, pySikang, pyMinkan, PaoYakuPages};

struct PaoStat { PlayerID paoPlayer, agariPlayer; };
typedef PaoStat PaoStatBook[PaoYakuPages];
static_assert(std::is_trivially_copyable<PaoStat>::value, "PaoStat is not trivially copyable");
static_assert(std::is_standard_layout<PaoStat>::value, "PaoStat is not standard layout");


// -------------------------------------------------------------------------

constexpr unsigned SizeOfDeckBuf = 144;
typedef Tile DeckBuf[SizeOfDeckBuf]; // 最初はunionでやろうと思ったけどおかしくなるのでやめた

// -------------------------------------------------------------------------

struct PrevMeldBook { TileCode Discard, Stepped; };
static_assert(std::is_trivially_copyable<PrevMeldBook>::value, "PrevMeldBook is not trivially copyable");
static_assert(std::is_standard_layout<PrevMeldBook>::value, "PrevMeldBook is not standard layout");

// -------------------------------------------------------------------------

struct DoraStatBook { UInt8ByTile Omote, Ura; };
static_assert(std::is_trivially_copyable<DoraStatBook>::value, "DoraStatBook is not trivially copyable");
static_assert(std::is_standard_layout<DoraStatBook>::value, "DoraStatBook is not standard layout");

// -------------------------------------------------------------------------

enum class ChiiType : int8_t {
	none, lower, middle, upper,
};
struct DeclFlag {
	bool Ron, Kan, Pon;
	ChiiType Chi;
};
static_assert(std::is_trivially_copyable<DeclFlag>::value, "DeclFlag is not trivially copyable");
static_assert(std::is_standard_layout<DeclFlag>::value, "DeclFlag is not standard layout");

// -------------------------------------------------------------------------

struct CurrPlayer { PlayerID Active, Passive, Agari, Furikomi; };
static_assert(std::is_trivially_copyable<CurrPlayer>::value, "CurrPlayer is not trivially copyable");
static_assert(std::is_standard_layout<CurrPlayer>::value, "CurrPlayer is not standard layout");

// -------------------------------------------------------------------------

struct Flowers { // 花牌フラグ用
	bool Spring, Summer, Autumn, Winter,
		Plum, Orchid, Chrys, Bamboo;
	/* Chrysanthemum は綴りが長いので省略…… */
};
static_assert(std::is_trivially_copyable<Flowers>::value, "Flowers is not trivially copyable");
static_assert(std::is_standard_layout<Flowers>::value, "Flowers is not standard layout");

// -------------------------------------------------------------------------

struct Dice { // サイコロ
	uint8_t Number;
	uint8_t Direction;
};
static_assert(std::is_trivially_copyable<Dice>::value, "Dice is not trivially copyable");
static_assert(std::is_standard_layout<Dice>::value, "Dice is not standard layout");

// -------------------------------------------------------------------------

enum class HandStatCode : int8_t {
	upright, exposed, hidden, openRiichi,
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
	HandStatCode HandStat; // 手牌の状態（立てる・見せる・伏せる）
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
	bool shokanFlag; // 初槓フラグ
	bool kansanjunFlag; // 槓三巡フラグ
	int8_t renpaiTenhohStat; // 戻牌天和判定用
	bool ConnectionLost;

	const Tile& Tsumohai() const {return Hand[TsumohaiIndex];} /* 自摸牌 (immutable) */
	      Tile& Tsumohai()       {return Hand[TsumohaiIndex];} /* 自摸牌 (mutable) */

	PlayerTable() { memset(this, 0, sizeof(PlayerTable)); }
};
static_assert(std::is_trivially_copyable<PlayerTable>::value, "PlayerTable is not trivially copyable");
static_assert(std::is_standard_layout<PlayerTable>::value, "PlayerTable is not standard layout");

// -------------------------------------------------------------------------

enum class SeatAbsolute : uint8_t { east, south, west, north, white, green, red };
enum class SeatRelative : uint8_t { self, right, opposite, left };

// -------------------------------------------------------------------------

template struct InfoByPlayer<PlayerTable>;
typedef InfoByPlayer<PlayerTable> StatusByPlayer;
typedef PlayerID Player_ID;
typedef Dice Dice_Struct;
struct GameTable { // 卓の情報を格納する
	StatusByPlayer Player;
	DeckBuf Deck; // 壁牌の配列
	DoraStatBook DoraFlag; // ドラ判定の配列
	FlagByTile OpenRichiWait; // プンリーの待ち牌(ＣＯＭに意図的な放銃を起こさせないために使用)
	PaoStatBook PaoFlag; // 包フラグ（-1…なし、0〜3…該当プレイヤー）
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	CurrPlayer CurrentPlayer;
	Player_ID PlayerID;
	Player_ID LastAgariPlayer;
	GameTypeID gameType;
	bool TsumoAgariFlag; // ツモアガリ？
	int16_t AgariSpecialStat; // 今のところ食い変えでチョンボになる場合だけ使ってる？
	PrevMeldBook PreviousMeld; // 先ほど鳴いた牌（喰い替えの判定に使う）
	KangStat KangFlag; // 嶺上開花；連開花と槓振り；頭槓和；搶槓の判定に使う
	Dice_Struct Dice[4]; // サイコロ
	uint8_t TurnRound; // 現在の巡目
	uint8_t KangNum; // 四槓流局、四槓子などの判定に使う
	bool RichiCounter; // リーチをカウンター(宣言牌をロン)
	bool DoubleRichiCounter; // ダブルリーチをカウンター(宣言牌をロン)
	Player_ID WaremePlayer; // 割れ目の位置(-1で割れ目なし)
	Player_ID DoukasenPlayer; // 導火線の位置(-1で導火線なし)
	uint8_t DeadTiles; // 王牌の数
	uint8_t ExtraRinshan; // 追加の嶺上牌の数
	bool ShibariFlag; //二飜縛り
	uint8_t TilePointer; // ツモ牌のポインタ
	uint16_t DoraPointer;
	uint8_t RinshanPointer; // 嶺上牌のポインタ
	bool TianHuFlag; // 親の第一打牌がまだ（天和の判定などに使う）
	Tile CurrentDiscard;

	const PlayerTable& statOfActive () const {return Player[CurrentPlayer.Active ];} /* 自摸番のプレイヤーの情報 (immutable) */
	      PlayerTable& statOfActive ()       {return Player[CurrentPlayer.Active ];} /* 自摸番のプレイヤーの情報 (mutable) */
	const PlayerTable& statOfPassive() const {return Player[CurrentPlayer.Passive];} /* 鳴き判定中のプレイヤーの情報 (immutable) */
	      PlayerTable& statOfPassive()       {return Player[CurrentPlayer.Passive];} /* 鳴き判定中のプレイヤーの情報 (mutable) */
	const PlayerTable& statOfAgari  () const {return Player[CurrentPlayer.Agari  ];} /* 和了ったプレイヤーの情報 (immutable) */
	      PlayerTable& statOfAgari  ()       {return Player[CurrentPlayer.Agari  ];} /* 和了ったプレイヤーの情報 (mutable) */
	const PlayerTable& statOfMine   () const {return Player[PlayerID             ];} /* 自分のプレイヤーの情報 (immutable) */
	      PlayerTable& statOfMine   ()       {return Player[PlayerID             ];} /* 自分のプレイヤーの情報 (mutable) */

	bool chkGameType(GameTypeID gameType) const {return (static_cast<int>(this->gameType) & static_cast<int>(gameType));}
	uint8_t diceSum() { // サイコロの出目を取得
		return Dice[0].Number + Dice[1].Number;
	}
	uint8_t diceSum2() { // 二度振りの2回目のサイコロの出目を取得
		return Dice[2].Number + Dice[3].Number;
	}

	SeatAbsolute prevailingwind() const { // 場風
		return static_cast<SeatAbsolute>(GameRound / 4);
	}

	SeatAbsolute playerwind(Player_ID player, int currentRound) const { // プレイヤーの自風がどれか調べる
		if (chkGameType(GameTypeID::sanmaT))
			return static_cast<SeatAbsolute>((player + 24 - (currentRound - ( currentRound / 4))) % 3);
		else return static_cast<SeatAbsolute>((player + 32 - currentRound) % 4);
	}
	SeatAbsolute playerwind(Player_ID player) const { // プレイヤーの自風がどれか調べる
		return playerwind(player, GameRound);
	}

	int tilesLeft() const { // 王牌を除いた山牌の残り枚数
		return (static_cast<int>(RinshanPointer) - (static_cast<int>(DeadTiles) - 1) - static_cast<int>(TilePointer));
	}

	GameTable() { memset(this, 0, sizeof(GameTable)); }
};
static_assert(std::is_trivially_copyable<GameTable>::value, "GameTable is not trivially copyable");
static_assert(std::is_standard_layout<GameTable>::value, "GameTable is not standard layout");

// -------------------------------------------------------------------------

// 食い変え判定用の gameStat->AgariSpecialStat 番号
constexpr unsigned int agariKuikae = 999;

// -------------------------------------------------------------------------

} /* namespace */

template<> struct std::hash<mihajong_structs::Tile> {
	typedef mihajong_structs::Tile argument_type;
	typedef std::size_t result_type;
	result_type operator() (argument_type const& s) const noexcept {
		return std::hash<std::uint16_t>{} (static_cast<std::uint16_t>(s.tile) << 8 | static_cast<std::uint16_t>(s.red));
	}
};
