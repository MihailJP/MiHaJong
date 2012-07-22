#include "../catalog.h"

static const tileCode YaojiuShunCode[] = {
	CharacterOne, CharacterSeven, CircleOne, CircleSeven, BambooOne, BambooSeven,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
};
static const tileCode OneCode[] = {CharacterOne, CircleOne, BambooOne,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};
static const tileCode SevenCode[] = {CharacterSeven, CircleSeven, BambooSeven};
static const tileCode NineCode[] = {CharacterNine, CircleNine, BambooNine,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_yaojiu()
{
	/* タンヤオ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"断幺九", (getRule(RULE_KUITAN) == 2) ? /* 門前なら縛りを満たし、喰い断は縛りを満たさないルール */
		yaku::yakuCalculator::Yaku::HANFUNC(
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* analysis) {
				return gameStat->Player[analysis->player].MenzenFlag ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han);
			}) :
			((getRule(RULE_KUITAN) == 1) ? (yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen : // 喰い断なし
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han), // 喰い断あり
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == 0);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == 0);
			else return false;
		}
	));
	/* チャンタ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"混全帯幺九", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	if (getRule(RULE_HENCHANTAIYAO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"偏全帯幺九", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"混全帯幺九",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentz(analysis->MianziDat, OneCode, 10, OneCode, 3, false) == SIZE_OF_MELD_BUFFER) ||
					(yaku::countingFacility::countSpecMentz(analysis->MianziDat, NineCode, 10, SevenCode, 3, false) == SIZE_OF_MELD_BUFFER) );
				else return false;
			}
		));
	/* 純チャン */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"純全帯幺九", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		"混全帯幺九",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 6, YaojiuShunCode, 6, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	if (getRule(RULE_HENCHANTAIYAO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純偏全帯幺九", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"純全帯幺九", "偏全帯幺九",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentz(analysis->MianziDat, OneCode, 3, OneCode, 3, false) == SIZE_OF_MELD_BUFFER) ||
					(yaku::countingFacility::countSpecMentz(analysis->MianziDat, NineCode, 3, SevenCode, 3, false) == SIZE_OF_MELD_BUFFER) );
				else return false;
			}
		));
	/* ホンロー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"混老頭", (getRule(RULE_HONROUTOU) != 0) ? yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
		"混全帯幺九",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, NULL, 0, false) == SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == NUM_OF_TILES_IN_HAND / 2);
			else return false;
		}
	));
	/* チンロー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"清老頭", yaku::yakuCalculator::Yaku::yval_yakuman,
		"混全帯幺九", "純全帯幺九", "混老頭",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 6, NULL, 0, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	/* ツーイーソー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"字一色", yaku::yakuCalculator::Yaku::yval_yakuman,
		"混全帯幺九", "純全帯幺九", "混老頭", "混一色", "四字刻",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, NULL, 0, false) == SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NUM_OF_TILES_IN_HAND / 2);
			else return false;
		}
	));
	if (getRule(RULE_FOUR_HONORS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四字刻", yaku::yakuCalculator::Yaku::yval_yakuman,
			"混全帯幺九", "純全帯幺九", "混老頭", "混一色",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, NULL, 0, true) == (SIZE_OF_MELD_BUFFER - 1));
				else return false;
			}
		));
}
