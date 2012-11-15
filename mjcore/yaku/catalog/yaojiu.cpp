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
		_T("断幺九"), (RuleData::chkRule("kuitan") == "yes_but_unbound") ? /* 門前なら縛りを満たし、喰い断は縛りを満たさないルール */
		yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return *analysis->MenzenFlag ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han);
			}) :
			((RuleData::chkRule("kuitan") == "no") ? (yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen : // 喰い断なし
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han), // 喰い断あり
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == 0);
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == 0);
			else return false;
		}
	));
	/* チャンタ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("混全帯幺九"), yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("偏全帯幺九"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("混全帯幺九"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, OneCode, 10, OneCode, 3, false) == SIZE_OF_MELD_BUFFER) ||
					(yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, NineCode, 10, SevenCode, 3, false) == SIZE_OF_MELD_BUFFER) );
				else return false;
			}
		));
	/* 純チャン */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("純全帯幺九"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		_T("混全帯幺九"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Honor_Major_Tiles(), 6, YaojiuShunCode, 6, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純偏全帯幺九"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("純全帯幺九"), _T("偏全帯幺九"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentz(analysis->MianziDat, OneCode, 3, OneCode, 3, false) == SIZE_OF_MELD_BUFFER) ||
					(yaku::countingFacility::countSpecMentz(analysis->MianziDat, NineCode, 3, SevenCode, 3, false) == SIZE_OF_MELD_BUFFER) );
				else return false;
			}
		));
	/* ホンロー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("混老頭"), get_yaku_han("honroutou"),
		_T("混全帯幺九"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, nullptr, 0, false) == SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == NUM_OF_TILES_IN_HAND / 2);
			else return false;
		}
	));
	/* チンロー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("清老頭"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("混全帯幺九"), _T("純全帯幺九"), _T("混老頭"), _T("対々和"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 6, nullptr, 0, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	/* ツーイーソー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("字一色"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("混全帯幺九"), _T("純全帯幺九"), _T("混老頭"), _T("混一色"), _T("四字刻"), _T("対々和"), _T("茶一色"), _T("三元七対子"), _T("四喜七対子"), _T("紅白七対"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, nullptr, 0, false) == SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NUM_OF_TILES_IN_HAND / 2);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("four_honors"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四字刻"), get_yaku_han("four_honors"),
			_T("混全帯幺九"), _T("純全帯幺九"), _T("混老頭"), _T("混一色"), _T("茶一色"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, nullptr, 0, true) == (SIZE_OF_MELD_BUFFER - 1));
				else return false;
			}
		));
}
