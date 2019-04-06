#pragma once

#include <cstdint>
#include <climits>
#include <type_traits>
#ifdef MJCORE_EXPORTS
#include "../mjcore/except.h"
#include "strcode.h"
#endif

namespace mihajong_structs {

// 牌の種類
constexpr unsigned int TileCodeMaximum    = 200; // 牌コード最大値
constexpr unsigned int TileSuitStep       =  10; // 色ごとの種類数
constexpr unsigned int TileNumeralColors  =   3; // 数牌の色の種類

enum TileSuit : uint8_t {
	TileSuitInvalid    = 0xffu,                            /* 無効データ */
	TileSuitCharacters = TileSuitStep * 0,                 /* 萬子 */
	TileSuitCircles    = TileSuitStep * 1,                 /* 筒子 */
	TileSuitBamboos    = TileSuitStep * 2,                 /* 索子 */
	TileSuitHonors     = TileSuitStep * TileNumeralColors, /* 字牌 */
	TileSuitFlowers    = TileSuitStep * 12,                /* 花牌 */
};

constexpr unsigned int TileNonflowerMax = TileSuitStep + TileSuitHonors;  // 普通牌の最大値
constexpr unsigned int TileFlowerMax    = TileSuitStep + TileSuitFlowers; // 花牌の最大値

enum TileCode : uint8_t { // 牌のコード
	NoTile = 0,                            // 牌なし
	/* 萬子 */
	CharacterOne = TileSuitCharacters + 1, // 一萬
	CharacterTwo,                          // 二萬
	CharacterThree,                        // 三萬
	CharacterFour,                         // 四萬
	CharacterFive,                         // 五萬
	CharacterSix,                          // 六萬
	CharacterSeven,                        // 七萬
	CharacterEight,                        // 八萬
	CharacterNine,                         // 九萬
	/* 筒子 */
	CircleOne = TileSuitCircles + 1,       // 一筒
	CircleTwo,                             // 二筒
	CircleThree,                           // 三筒
	CircleFour,                            // 四筒
	CircleFive,                            // 五筒
	CircleSix,                             // 六筒
	CircleSeven,                           // 七筒
	CircleEight,                           // 八筒
	CircleNine,                            // 九筒
	/* 索子 */
	BambooOne = TileSuitBamboos + 1,       // 一索
	BambooTwo,                             // 二索
	BambooThree,                           // 三索
	BambooFour,                            // 四索
	BambooFive,                            // 五索
	BambooSix,                             // 六索
	BambooSeven,                           // 七索
	BambooEight,                           // 八索
	BambooNine,                            // 九索
	/* 字牌 */
	EastWind = TileSuitHonors + 1,         // 東風牌
	SouthWind,                             // 南風牌
	WestWind,                              // 西風牌
	NorthWind,                             // 北風牌
	WhiteDragon,                           // 三元牌 白
	GreenDragon,                           // 三元牌 發
	RedDragon,                             // 三元牌 中
	/* 花牌 */
	Spring = TileSuitFlowers + 1,          // 季節牌 春
	Summer,                                // 季節牌 夏
	Autumn,                                // 季節牌 秋
	Winter,                                // 季節牌 冬
	Plum = TileSuitFlowers + 6,            // 草木牌 梅
	Orchid,                                // 草木牌 蘭
	Chrysanthemum,                         // 草木牌 菊
	Bamboo,                                // 草木牌 竹
	/* 特殊 */
	Flower = 38,                           // 判定用 花牌総称
	BackSide = 39,                         // 表示用 裏返し
	TilePad = UCHAR_MAX,                   // 理牌用 パディング
};

// -------------------------------------------------------------------------

template <class T> struct InfoByTile { // 牌ごとに指定した型による情報(テンプレート)
	T val[TileNonflowerMax];
	const T& operator[](const TileCode tile) const {
		if ((tile >= NoTile)&&(tile < TileNonflowerMax))
			return val[tile];
		else if (((tile >= Spring)&&(tile <= Winter)) ||
			((tile >= Plum)&&(tile <= Bamboo))) return val[Flower];
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByTile:添字が範囲外です (") << static_cast<int>(tile) << _T(")");
			RaiseTolerant(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			return val[NoTile];
		}
	}
	const T& operator[](const int tile) const {
		return InfoByTile::operator[](static_cast<TileCode>(tile));
	}
	T& operator[](const TileCode tile) {
		if ((tile >= NoTile)&&(tile < TileNonflowerMax))
			return val[tile];
		else if (((tile >= Spring)&&(tile <= Winter)) ||
			((tile >= Plum)&&(tile <= Bamboo))) return val[Flower];
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByTile:添字が範囲外です (") << static_cast<int>(tile) << _T(")");
			RaiseTolerant(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			return val[NoTile];
		}
	}
	T& operator[](const int tile) {
		return InfoByTile::operator[](static_cast<TileCode>(tile));
	}
};

template struct InfoByTile<bool>;
typedef InfoByTile<bool> FlagByTile;
static_assert(std::is_trivially_copyable<FlagByTile>::value, "FlagByTile is not trivially copyable");
static_assert(std::is_standard_layout<FlagByTile>::value, "FlagByTile is not standard layout");

template struct InfoByTile<int8_t>;
typedef InfoByTile<int8_t> Int8ByTile;
static_assert(std::is_trivially_copyable<Int8ByTile>::value, "Int8ByTile is not trivially copyable");
static_assert(std::is_standard_layout<Int8ByTile>::value, "Int8ByTile is not standard layout");

template struct InfoByTile<uint8_t>;
typedef InfoByTile<uint8_t> UInt8ByTile;
static_assert(std::is_trivially_copyable<UInt8ByTile>::value, "UInt8ByTile is not trivially copyable");
static_assert(std::is_standard_layout<UInt8ByTile>::value, "UInt8ByTile is not standard layout");

} /* namespace */
