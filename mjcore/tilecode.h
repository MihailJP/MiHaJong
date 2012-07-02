#ifndef TILECODE_H
#define TILECODE_H

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <type_traits>
#include <exception>
#include <stdexcept>
#include "logging.h"
#endif
#include "mjexport.h"

// 牌の種類
#define TILE_CODE_MAXIMUM 200
#define TILE_SUIT_STEP 10
#define TILE_NUMERAL_COLORS 3

#define TILE_SUIT_CHARACTERS (TILE_SUIT_STEP * 0)
#define TILE_SUIT_CIRCLES (TILE_SUIT_STEP * 1)
#define TILE_SUIT_BAMBOOS (TILE_SUIT_STEP * 2)
#define TILE_SUIT_HONORS (TILE_SUIT_STEP * TILE_NUMERAL_COLORS)
#define TILE_NONFLOWER_MAX (TILE_SUIT_HONORS + TILE_SUIT_STEP)
#define TILE_SUIT_FLOWERS (TILE_SUIT_STEP * 12)
#define TILE_FLOWER_MAX (TILE_SUIT_FLOWERS + TILE_SUIT_STEP)

enum tileCode { // 牌のコード
	NoTile = 0,
	/* 萬子 */
	CharacterOne = TILE_SUIT_CHARACTERS + 1,
	CharacterTwo,
	CharacterThree,
	CharacterFour,
	CharacterFive,
	CharacterSix,
	CharacterSeven,
	CharacterEight,
	CharacterNine,
	/* 筒子 */
	CircleOne = TILE_SUIT_CIRCLES + 1,
	CircleTwo,
	CircleThree,
	CircleFour,
	CircleFive,
	CircleSix,
	CircleSeven,
	CircleEight,
	CircleNine,
	/* 索子 */
	BambooOne = TILE_SUIT_BAMBOOS + 1,
	BambooTwo,
	BambooThree,
	BambooFour,
	BambooFive,
	BambooSix,
	BambooSeven,
	BambooEight,
	BambooNine,
	/* 字牌 */
	EastWind = TILE_SUIT_HONORS + 1,
	SouthWind,
	WestWind,
	NorthWind,
	WhiteDragon,
	GreenDragon,
	RedDragon,
	/* 花牌 */
	Spring = TILE_SUIT_FLOWERS + 1,
	Summer,
	Autumn,
	Winter,
	Plum = TILE_SUIT_FLOWERS + 6,
	Orchid,
	Chrysanthemum,
	Bamboo
};

// -------------------------------------------------------------------------

template <class T> struct InfoByTile { // 牌ごとに指定した型による情報(テンプレート)
	T val[TILE_NONFLOWER_MAX];
	const T& operator[](const tileCode tile) const {
		if ((tile >= NoTile)&&(tile < TILE_NONFLOWER_MAX)) {
			return val[tile];
		}
		else {
			error("InfoByTile::operator[] の引数が範囲外です");
			throw std::domain_error("Subscript out of range");
		}
	}
	const T& operator[](const int tile) const {
		return InfoByTile::operator[]((tileCode)tile);
	}
	T& operator[](const tileCode tile) {
		if ((tile >= NoTile)&&(tile < TILE_NONFLOWER_MAX)) {
			return val[tile];
		}
		else {
			error("InfoByTile::operator[] の引数が範囲外です");
			throw std::domain_error("Subscript out of range");
		}
	}
	T& operator[](const int tile) {
		return InfoByTile::operator[]((tileCode)tile);
	}
};

template struct MJCORE InfoByTile<bool>;
typedef InfoByTile<bool> FlagByTile;

template struct MJCORE InfoByTile<int8_t>;
typedef InfoByTile<int8_t> Int8ByTile;

#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<InfoByTile<int8_t> >::value, "Non-POD data type detected");
#endif

#endif
