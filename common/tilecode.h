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

enum class TileSuit : uint8_t {
	invalid    = 0xffu,                            /* 無効データ */
	characters = TileSuitStep * 0,                 /* 萬子 */
	circles    = TileSuitStep * 1,                 /* 筒子 */
	bamboos    = TileSuitStep * 2,                 /* 索子 */
	honors     = TileSuitStep * TileNumeralColors, /* 字牌 */
	flowers    = TileSuitStep * 12,                /* 花牌 */
};

constexpr unsigned int TileNonflowerMax = TileSuitStep + static_cast<int>(TileSuit::honors);  // 普通牌の最大値
constexpr unsigned int TileFlowerMax    = TileSuitStep + static_cast<int>(TileSuit::flowers); // 花牌の最大値

enum class TileCode : uint8_t { // 牌のコード
	noTile = 0,                            // 牌なし
	/* 萬子 */
	characterOne = static_cast<int>(TileSuit::characters) + 1, // 一萬
	characterTwo,                                              // 二萬
	characterThree,                                            // 三萬
	characterFour,                                             // 四萬
	characterFive,                                             // 五萬
	characterSix,                                              // 六萬
	characterSeven,                                            // 七萬
	characterEight,                                            // 八萬
	characterNine,                                             // 九萬
	/* 筒子 */
	circleOne = static_cast<int>(TileSuit::circles) + 1,       // 一筒
	circleTwo,                                                 // 二筒
	circleThree,                                               // 三筒
	circleFour,                                                // 四筒
	circleFive,                                                // 五筒
	circleSix,                                                 // 六筒
	circleSeven,                                               // 七筒
	circleEight,                                               // 八筒
	circleNine,                                                // 九筒
	/* 索子 */
	bambooOne = static_cast<int>(TileSuit::bamboos) + 1,       // 一索
	bambooTwo,                                                 // 二索
	bambooThree,                                               // 三索
	bambooFour,                                                // 四索
	bambooFive,                                                // 五索
	bambooSix,                                                 // 六索
	bambooSeven,                                               // 七索
	bambooEight,                                               // 八索
	bambooNine,                                                // 九索
	/* 字牌 */
	eastWind = static_cast<int>(TileSuit::honors) + 1,         // 東風牌
	southWind,                                                 // 南風牌
	westWind,                                                  // 西風牌
	northWind,                                                 // 北風牌
	whiteDragon,                                               // 三元牌 白
	greenDragon,                                               // 三元牌 發
	redDragon,                                                 // 三元牌 中
	/* 花牌 */
	spring = static_cast<int>(TileSuit::flowers) + 1,          // 季節牌 春
	summer,                                                    // 季節牌 夏
	autumn,                                                    // 季節牌 秋
	winter,                                                    // 季節牌 冬
	plum = static_cast<int>(TileSuit::flowers) + 6,            // 草木牌 梅
	orchid,                                                    // 草木牌 蘭
	chrysanthemum,                                             // 草木牌 菊
	bamboo,                                                    // 草木牌 竹
	/* 特殊 */
	flower = 38,                                               // 判定用 花牌総称
	backSide = 39,                                             // 表示用 裏返し
	tilePad = UCHAR_MAX,                                       // 理牌用 パディング
};

// -------------------------------------------------------------------------

template <class T> class InfoByTile { // 牌ごとに指定した型による情報(テンプレート)
private:
	T val[TileNonflowerMax];
public:
	const T& operator[](const TileCode tile) const {
		if ((tile >= TileCode::noTile)&&(tile < static_cast<TileCode>(TileNonflowerMax)))
			return val[static_cast<int>(tile)];
		else if (((tile >= TileCode::spring)&&(tile <= TileCode::winter)) ||
			((tile >= TileCode::plum)&&(tile <= TileCode::bamboo))) return val[static_cast<int>(TileCode::flower)];
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByTile:添字が範囲外です (") << static_cast<int>(tile) << _T(")");
			RaiseTolerant(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			return val[static_cast<int>(TileCode::noTile)];
		}
	}
	const T& operator[](const int tile) const {
		return InfoByTile::operator[](static_cast<TileCode>(tile));
	}
	T& operator[](const TileCode tile) {
		if ((tile >= TileCode::noTile)&&(tile < static_cast<TileCode>(TileNonflowerMax)))
			return val[static_cast<int>(tile)];
		else if (((tile >= TileCode::spring)&&(tile <= TileCode::winter)) ||
			((tile >= TileCode::plum)&&(tile <= TileCode::bamboo))) return val[static_cast<int>(TileCode::flower)];
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByTile:添字が範囲外です (") << static_cast<int>(tile) << _T(")");
			RaiseTolerant(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			return val[static_cast<int>(TileCode::noTile)];
		}
	}
	T& operator[](const int tile) {
		return InfoByTile::operator[](static_cast<TileCode>(tile));
	}
};

template class InfoByTile<bool>;
typedef InfoByTile<bool> FlagByTile;
static_assert(std::is_trivially_copyable<FlagByTile>::value, "FlagByTile is not trivially copyable");
static_assert(std::is_standard_layout<FlagByTile>::value, "FlagByTile is not standard layout");

template class InfoByTile<int8_t>;
typedef InfoByTile<int8_t> Int8ByTile;
static_assert(std::is_trivially_copyable<Int8ByTile>::value, "Int8ByTile is not trivially copyable");
static_assert(std::is_standard_layout<Int8ByTile>::value, "Int8ByTile is not standard layout");

template class InfoByTile<uint8_t>;
typedef InfoByTile<uint8_t> UInt8ByTile;
static_assert(std::is_trivially_copyable<UInt8ByTile>::value, "UInt8ByTile is not trivially copyable");
static_assert(std::is_standard_layout<UInt8ByTile>::value, "UInt8ByTile is not standard layout");

// -------------------------------------------------------------------------

inline constexpr TileCode composeNumberTile(TileSuit suit, unsigned int num) {
	if ((suit != TileSuit::characters) && (suit != TileSuit::circles) && (suit != TileSuit::bamboos))
		return TileCode::noTile;
	else if ((num == 0) || (num > 9))
		return TileCode::noTile;
	else
		return static_cast<TileCode>(static_cast<int>(suit) + num);
}

inline constexpr TileSuit getTileSuit(TileCode tile) {
	switch (tile) {
	case TileCode::characterOne:   case TileCode::characterTwo:   case TileCode::characterThree:
	case TileCode::characterFour:  case TileCode::characterFive:  case TileCode::characterSix:
	case TileCode::characterSeven: case TileCode::characterEight: case TileCode::characterNine:
		return TileSuit::characters;
	case TileCode::circleOne:   case TileCode::circleTwo:   case TileCode::circleThree:
	case TileCode::circleFour:  case TileCode::circleFive:  case TileCode::circleSix:
	case TileCode::circleSeven: case TileCode::circleEight: case TileCode::circleNine:
		return TileSuit::circles;
	case TileCode::bambooOne:   case TileCode::bambooTwo:   case TileCode::bambooThree:
	case TileCode::bambooFour:  case TileCode::bambooFive:  case TileCode::bambooSix:
	case TileCode::bambooSeven: case TileCode::bambooEight: case TileCode::bambooNine:
		return TileSuit::bamboos;
	case TileCode::eastWind: case TileCode::southWind: case TileCode::westWind: case TileCode::northWind:
	case TileCode::whiteDragon: case TileCode::greenDragon: case TileCode::redDragon:
		return TileSuit::honors;
	case TileCode::spring: case TileCode::summer: case TileCode::autumn: case TileCode::winter:
	case TileCode::plum: case TileCode::orchid: case TileCode::chrysanthemum: case TileCode::bamboo:
		return TileSuit::flowers;
	default:
		return TileSuit::invalid;
	}
}

inline constexpr unsigned int getTileNumber(TileCode tile) {
	switch (tile) {
	case TileCode::characterOne: case TileCode::circleOne: case TileCode::bambooOne:
		return 1;
	case TileCode::characterTwo: case TileCode::circleTwo: case TileCode::bambooTwo:
		return 2;
	case TileCode::characterThree: case TileCode::circleThree: case TileCode::bambooThree:
		return 3;
	case TileCode::characterFour: case TileCode::circleFour: case TileCode::bambooFour:
		return 4;
	case TileCode::characterFive: case TileCode::circleFive: case TileCode::bambooFive:
		return 5;
	case TileCode::characterSix: case TileCode::circleSix: case TileCode::bambooSix:
		return 6;
	case TileCode::characterSeven: case TileCode::circleSeven: case TileCode::bambooSeven:
		return 7;
	case TileCode::characterEight: case TileCode::circleEight: case TileCode::bambooEight:
		return 8;
	case TileCode::characterNine: case TileCode::circleNine: case TileCode::bambooNine:
		return 9;
	default:
		return 0;
	}
}

inline constexpr TileCode offsetTileNumber(TileCode tile, int offset) {
	const int newNum = static_cast<int>(getTileNumber(tile)) + offset;
	if (getTileNumber(tile) > 0)
		return composeNumberTile(getTileSuit(tile), newNum > 0 ? static_cast<unsigned int>(newNum) : 0u);
	else return TileCode::noTile;
}

} /* namespace */
