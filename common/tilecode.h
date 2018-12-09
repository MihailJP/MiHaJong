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
const unsigned int TileCodeMaximum    = 200;
const unsigned int TileSuitStep       =  10;
const unsigned int TileNumeralColors  =   3;

const unsigned int TileSuitCharacters = TileSuitStep * 0;
const unsigned int TileSuitCircles    = TileSuitStep * 1;
const unsigned int TileSuitBamboos    = TileSuitStep * 2;
const unsigned int TileSuitHonors     = TileSuitStep * TileNumeralColors;
const unsigned int TileNonflowerMax   = TileSuitStep   + TileSuitHonors;
const unsigned int TileSuitFlowers    = TileSuitStep * 12;
const unsigned int TileFlowerMax      = TileSuitStep   + TileSuitFlowers;

enum TileCode : uint8_t { // 牌のコード
	NoTile = 0,
	/* 萬子 */
	CharacterOne = TileSuitCharacters + 1,
	CharacterTwo,
	CharacterThree,
	CharacterFour,
	CharacterFive,
	CharacterSix,
	CharacterSeven,
	CharacterEight,
	CharacterNine,
	/* 筒子 */
	CircleOne = TileSuitCircles + 1,
	CircleTwo,
	CircleThree,
	CircleFour,
	CircleFive,
	CircleSix,
	CircleSeven,
	CircleEight,
	CircleNine,
	/* 索子 */
	BambooOne = TileSuitBamboos + 1,
	BambooTwo,
	BambooThree,
	BambooFour,
	BambooFive,
	BambooSix,
	BambooSeven,
	BambooEight,
	BambooNine,
	/* 字牌 */
	EastWind = TileSuitHonors + 1,
	SouthWind,
	WestWind,
	NorthWind,
	WhiteDragon,
	GreenDragon,
	RedDragon,
	/* 花牌 */
	Spring = TileSuitFlowers + 1,
	Summer,
	Autumn,
	Winter,
	Plum = TileSuitFlowers + 6,
	Orchid,
	Chrysanthemum,
	Bamboo,
	/* 特殊 */
	Flower = 38,
	BackSide = 39,
	TilePad = UCHAR_MAX,
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
			o << _T("InfoByTile:添字が範囲外です (") << (int)tile << _T(")");
			RaiseTolerant(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			return val[NoTile];
		}
	}
	const T& operator[](const int tile) const {
		return InfoByTile::operator[]((TileCode)tile);
	}
	T& operator[](const TileCode tile) {
		if ((tile >= NoTile)&&(tile < TileNonflowerMax))
			return val[tile];
		else if (((tile >= Spring)&&(tile <= Winter)) ||
			((tile >= Plum)&&(tile <= Bamboo))) return val[Flower];
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByTile:添字が範囲外です (") << (int)tile << _T(")");
			RaiseTolerant(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			return val[NoTile];
		}
	}
	T& operator[](const int tile) {
		return InfoByTile::operator[]((TileCode)tile);
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
