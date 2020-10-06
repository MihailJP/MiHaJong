#include "../catalog.h"
#include "../../../common/strcode.h"

static constexpr TileCode YaojiuShunCode[] = {
	CharacterOne, CharacterSeven, CircleOne, CircleSeven, BambooOne, BambooSeven,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
};
static constexpr TileCode OneCode[] = {CharacterOne, CircleOne, BambooOne,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};
static constexpr TileCode SevenCode[] = {CharacterSeven, CircleSeven, BambooSeven};
static constexpr TileCode NineCode[] = {CharacterNine, CircleNine, BambooNine,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_yaojiu()
{
	/* タンヤオ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("断幺"), 2_fenF,
		_T("无字"),
#else /* GUOBIAO */
		_T("断幺九"), RuleData::chkRule("kuitan", "yes_but_unbound") ? /* 門前なら縛りを満たし、喰い断は縛りを満たさないルール */
		yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return *analysis->MenzenFlag ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(1_han,
					0_han) :
					yaku::yakuCalculator::Yaku::YAKU_HAN(0_han,
					1_han);
			}) :
			(RuleData::chkRule("kuitan", "no") ? (yaku::yakuCalculator::Yaku::HANFUNC)1_hanM : // 喰い断なし
			(yaku::yakuCalculator::Yaku::HANFUNC)1_hanF), // 喰い断あり
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, &Honor_Major_Tiles[0], 13, YaojiuShunCode, 13, false) == 0);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, &Honor_Major_Tiles[0], 13) == 0);
			else return false;
		}
	));
	/* チャンタ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("全帯幺"), 4_fenF,
#else /* GUOBIAO */
		_T("混全帯幺九"), 2_hanK,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, &Honor_Major_Tiles[0], 13, YaojiuShunCode, 13, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("偏全帯幺九"), 3_hanK,
			_T("混全帯幺九"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1)
					return ( (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, OneCode, 10, OneCode, 3, false) == SizeOfMeldBuffer) ||
					(yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, NineCode, 10, SevenCode, 3, false) == SizeOfMeldBuffer) );
				else return false;
			}
		));
	/* 純チャン */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("純全帯幺九"), 3_hanK,
		_T("混全帯幺九"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, &Honor_Major_Tiles[0], 6, YaojiuShunCode, 6, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純偏全帯幺九"), 3_hanK,
			_T("純全帯幺九"), _T("偏全帯幺九"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1)
					return ( (yaku::countingFacility::countSpecMentz(analysis->MianziDat, OneCode, 3, OneCode, 3, false) == SizeOfMeldBuffer) ||
					(yaku::countingFacility::countSpecMentz(analysis->MianziDat, NineCode, 3, SevenCode, 3, false) == SizeOfMeldBuffer) );
				else return false;
			}
		));
#endif /* GUOBIAO */
	/* ホンロー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("混幺九"), 32_fenF,
		_T("幺九刻"), _T("幺九刻x2"), _T("幺九刻x3"), _T("幺九刻x4"), PengPengHu, _T("全帯幺"),
#else /* GUOBIAO */
		_T("混老頭"), get_yaku_han("honroutou"),
		_T("混全帯幺九"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, &Honor_Major_Tiles[0], 13, nullptr, 0, false) == SizeOfMeldBuffer);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, &Honor_Major_Tiles[0], 13) == NumOfTilesInHand / 2);
			else return false;
		}
	));
	/* チンロー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("清幺九"), 32_fenF,
		_T("无字"), _T("双同刻"), _T("三同刻"), _T("幺九刻"), _T("幺九刻x2"), _T("幺九刻x3"), _T("幺九刻x4"), PengPengHu, _T("全帯幺"),
#else /* GUOBIAO */
		_T("清老頭"), 1_yakumanF,
		_T("混全帯幺九"), _T("純全帯幺九"), _T("混老頭"), _T("対々和"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, &Honor_Major_Tiles[0], 6, nullptr, 0, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
	/* ツーイーソー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("字一色"), 64_fenF,
		_T("幺九刻"), _T("幺九刻x2"), _T("幺九刻x3"), _T("幺九刻x4"), PengPengHu, _T("全帯幺"),
#else /* GUOBIAO */
		_T("字一色"), 1_yakumanF,
		_T("混全帯幺九"), _T("純全帯幺九"), _T("混老頭"), _T("混一色"), _T("四字刻"), _T("対々和"), _T("茶一色"), _T("三元七対子"), _T("四喜七対子"), _T("紅白七対"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, &Honor_Major_Tiles[6], 7, nullptr, 0, false) == SizeOfMeldBuffer);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, &Honor_Major_Tiles[6], 7) == NumOfTilesInHand / 2);
			else return false;
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("four_honors"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四字刻"), get_yaku_han("four_honors"),
			_T("混全帯幺九"), _T("純全帯幺九"), _T("混老頭"), _T("混一色"), _T("茶一色"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1)
					return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, &Honor_Major_Tiles[6], 7, nullptr, 0, true) == (SizeOfMeldBuffer - 1));
				else return false;
			}
		));
#endif /* GUOBIAO */
#ifdef GUOBIAO
	/* 无字 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("无字"), 1_fenF,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, &Honor_Major_Tiles[6], 7, nullptr, 0, false) == 0);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, &Honor_Major_Tiles[6], 7) == 0);
			else return false;
		}
	));
#endif /* GUOBIAO */
}
