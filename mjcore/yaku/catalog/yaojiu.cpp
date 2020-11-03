#include "../catalog.h"
#include "../../../common/strcode.h"

static constexpr std::array<TileCode, 6> YaojiuShunCode = {
	TileCode::characterOne, TileCode::characterSeven, TileCode::circleOne, TileCode::circleSeven, TileCode::bambooOne, TileCode::bambooSeven,
};
static constexpr std::array<TileCode, 10> OneHonorCode = {TileCode::characterOne, TileCode::circleOne, TileCode::bambooOne,
	TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind, TileCode::whiteDragon, TileCode::greenDragon, TileCode::redDragon};
static constexpr std::array<TileCode, 3> OneCode = {TileCode::characterOne, TileCode::circleOne, TileCode::bambooOne,};
static constexpr std::array<TileCode, 3> SevenCode = {TileCode::characterSeven, TileCode::circleSeven, TileCode::bambooSeven};
static constexpr std::array<TileCode, 10> NineHonorCode = {TileCode::characterNine, TileCode::circleNine, TileCode::bambooNine,
	TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind, TileCode::whiteDragon, TileCode::greenDragon, TileCode::redDragon};
static constexpr std::array<TileCode, 3> NineCode = {TileCode::characterNine, TileCode::circleNine, TileCode::bambooNine,};
static constexpr std::array<TileCode, 0> EmptyCode = {};

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
			[](const MentsuAnalysis* const analysis) {
				return analysis->MenzenFlag() ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(1_han,
					0_han) :
					yaku::yakuCalculator::Yaku::YAKU_HAN(0_han,
					1_han);
			}) :
			(RuleData::chkRule("kuitan", "no") ? (yaku::yakuCalculator::Yaku::HANFUNC)1_hanM : // 喰い断なし
			(yaku::yakuCalculator::Yaku::HANFUNC)1_hanF), // 喰い断あり
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles, YaojiuShunCode, false) == 0);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles) == 0);
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
		[](const MentsuAnalysis* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Honor_Major_Tiles, YaojiuShunCode, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("偏全帯幺九"), 3_hanK,
			_T("混全帯幺九"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1)
					return ( (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, OneHonorCode, OneCode, false) == SizeOfMeldBuffer) ||
					(yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, NineHonorCode, SevenCode, false) == SizeOfMeldBuffer) );
				else return false;
			}
		));
	/* 純チャン */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("純全帯幺九"), 3_hanK,
		_T("混全帯幺九"),
		[](const MentsuAnalysis* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Major_Tiles, YaojiuShunCode, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純偏全帯幺九"), 3_hanK,
			_T("純全帯幺九"), _T("偏全帯幺九"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1)
					return ( (yaku::countingFacility::countSpecMentz(analysis->MianziDat, OneCode, OneCode, false) == SizeOfMeldBuffer) ||
					(yaku::countingFacility::countSpecMentz(analysis->MianziDat, NineCode, SevenCode, false) == SizeOfMeldBuffer) );
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
		[](const MentsuAnalysis* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles, EmptyCode, false) == SizeOfMeldBuffer);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles) == NumOfTilesInHand / 2);
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
		[](const MentsuAnalysis* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Major_Tiles, EmptyCode, false) == SizeOfMeldBuffer);
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
		[](const MentsuAnalysis* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Tiles, EmptyCode, false) == SizeOfMeldBuffer);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Tiles) == NumOfTilesInHand / 2);
			else return false;
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("four_honors"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四字刻"), get_yaku_han("four_honors"),
			_T("混全帯幺九"), _T("純全帯幺九"), _T("混老頭"), _T("混一色"), _T("茶一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1)
					return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Tiles, EmptyCode, true) == (SizeOfMeldBuffer - 1));
				else return false;
			}
		));
#endif /* GUOBIAO */
#ifdef GUOBIAO
	/* 无字 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("无字"), 1_fenF,
		[](const MentsuAnalysis* const analysis) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Tiles, EmptyCode, false) == 0);
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Tiles) == 0);
			else return false;
		}
	));
#endif /* GUOBIAO */
}
