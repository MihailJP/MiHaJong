#ifndef TILECODE_H
#define TILECODE_H

#include <cstdint>

// îvÇÃéÌóﬁ
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

enum tileCode { // îvÇÃÉRÅ[Éh
	NoTile = 0,
	/* ‰›éq */
	CharacterOne = TILE_SUIT_CHARACTERS + 1,
	CharacterTwo,
	CharacterThree,
	CharacterFour,
	CharacterFive,
	CharacterSix,
	CharacterSeven,
	CharacterEight,
	CharacterNine,
	/* ìõéq */
	CircleOne = TILE_SUIT_CIRCLES + 1,
	CircleTwo,
	CircleThree,
	CircleFour,
	CircleFive,
	CircleSix,
	CircleSeven,
	CircleEight,
	CircleNine,
	/* çıéq */
	BambooOne = TILE_SUIT_BAMBOOS + 1,
	BambooTwo,
	BambooThree,
	BambooFour,
	BambooFive,
	BambooSix,
	BambooSeven,
	BambooEight,
	BambooNine,
	/* éöîv */
	EastWind = TILE_SUIT_HONORS + 1,
	SouthWind,
	WestWind,
	NorthWind,
	WhiteDragon,
	GreenDragon,
	RedDragon,
	/* â‘îv */
	Spring = TILE_SUIT_FLOWERS + 1,
	Summer,
	Autumn,
	Winter,
	Plum = TILE_SUIT_FLOWERS + 6,
	Orchid,
	Chrysanthemum,
	Bamboo
};

#endif
