#include "../catalog.h"

bool yaku::yakuCalculator::YakuCatalog::catalogInit::isQiDui(const MentsuAnalysis* const analysis) {
#ifdef GUOBIAO
	return analysis->shanten[ShantenType::pairs] == -1;
#else /* GUOBIAO */
	return ((analysis->shanten[ShantenType::pairs] == -1) && (analysis->shanten[ShantenType::regular] >= 0));
#endif /* GUOBIAO */
};

#ifndef GUOBIAO
template <std::size_t dSize> bool yaku::yakuCalculator::YakuCatalog::catalogInit::chktiles(const MentsuAnalysis* const analysis, const std::array<TileCode, dSize>& targetDuiz) { // 判定関数
	if (isQiDui(analysis))
		return (yaku::countingFacility::countPairs(analysis->TileCount, targetDuiz)
			== (targetDuiz.size() > (NumOfTilesInHand / 2) ? (NumOfTilesInHand / 2) : targetDuiz.size()));
	else return false;
};
#endif /* GUOBIAO */

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* チートイ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("七対"), 24_fenF,
		_T("門前清"), _T("単調将"),
		_T("連六"), _T("連六x2"), _T("老少副"), _T("老少副x2"), _T("一般高"), _T("一般高x2"), _T("平和"),
#else /* GUOBIAO */
		_T("七対子"), get_yaku_han("seven_pairs"), /* 1翻50符のルールと2翻25符のルールがある。符はここでは設定できないです…… */
#endif /* GUOBIAO */
		isQiDui
	));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 七福星 */
	if (RuleData::chkRuleApplied("seven_honor_pairs") && !RuleData::chkRule("seven_honor_pairs", "yes"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("七福星"), get_yaku_han("seven_honor_pairs"),
			_T("混老頭"), _T("混一色"), _T("三元七対子"), _T("四喜七対子"), _T("紅白七対"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if (isQiDui(analysis))
					return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Tiles) == NumOfTilesInHand / 2);
				else return false;
			}
		));
	/* 三元七対子 */
	if (RuleData::chkRuleApplied("dragon_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三元七対子"), get_yaku_han("dragon_pairs"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[TileCode::whiteDragon] >= 2) &&
					(analysis->TileCount[TileCode::greenDragon] >= 2) &&
					(analysis->TileCount[TileCode::redDragon] >= 2));
			}
		));
	/* 四喜七対子 */
	if (RuleData::chkRuleApplied("wind_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四喜七対子"), get_yaku_han("wind_pairs"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[TileCode::eastWind] >= 2) &&
					(analysis->TileCount[TileCode::southWind] >= 2) &&
					(analysis->TileCount[TileCode::westWind] >= 2) &&
					(analysis->TileCount[TileCode::northWind] >= 2));
			}
		));
	/* 奇数七対子 */
	if (RuleData::chkRuleApplied("odd_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("奇数七対子"), get_yaku_han("odd_pairs"),
			[](const MentsuAnalysis* const analysis) -> bool {
				constexpr std::array<TileCode, 15> tc = {
					TileCode::characterOne, TileCode::characterThree, TileCode::characterFive, TileCode::characterSeven, TileCode::characterNine,
					TileCode::circleOne, TileCode::circleThree, TileCode::circleFive, TileCode::circleSeven, TileCode::circleNine,
					TileCode::bambooOne, TileCode::bambooThree, TileCode::bambooFive, TileCode::bambooSeven, TileCode::bambooNine,
				};
				return isQiDui(analysis) && chktiles(analysis, tc);
			}
		));
	/* 偶数七対子 */
	if (RuleData::chkRuleApplied("even_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("偶数七対子"), get_yaku_han("even_pairs"),
			_T("断幺九"),
			[](const MentsuAnalysis* const analysis) -> bool {
				constexpr std::array<TileCode, 12> tc = {
					TileCode::characterTwo, TileCode::characterFour, TileCode::characterSix, TileCode::characterEight,
					TileCode::circleTwo, TileCode::circleFour, TileCode::circleSix, TileCode::circleEight,
					TileCode::bambooTwo, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
				};
				return isQiDui(analysis) && chktiles(analysis, tc);
			}
		));

	// ---------------------------------------------------------------------

	/* 紅白七対 */
	if (RuleData::chkRuleApplied("kouhaku_chiitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("紅白七対"), get_yaku_han("kouhaku_chiitoi"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[TileCode::whiteDragon] >= 2) &&
					(analysis->TileCount[TileCode::redDragon] >= 2));
			}
		));
	/* 北の大地 */
	if (RuleData::chkRuleApplied("kita_no_daichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北の大地"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MentsuAnalysis* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
				switch (analysis->TsumoHai->tile) { /* 何で和了ったかで分岐 */
				case TileCode::northWind: /* 北で和了るとダブル役満 */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(2_yakuman);
				case TileCode::whiteDragon: /* 白で和了るとトリプル役満 */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(3_yakuman);
				default:
					return yaku::yakuCalculator::Yaku::YAKU_HAN(1_yakuman);
				}
			}),
			_T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				constexpr std::array<TileCode, 7> tc = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
					TileCode::northWind, TileCode::whiteDragon,
				};
				return isQiDui(analysis) && chktiles(analysis, tc);
			}
		));
	/* 世界一 */
	if (RuleData::chkRuleApplied("sekaiichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("世界一"), get_yaku_han("sekaiichi"),
			_T("四喜七対子"), _T("混老頭"),
			[](const MentsuAnalysis* const analysis) -> bool {
				constexpr std::array<TileCode, 7> tc = {
					TileCode::characterOne, TileCode::circleOne, TileCode::bambooOne,
					TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
				};
				return isQiDui(analysis) && chktiles(analysis, tc);
			}
		));
	/* ニコニコ七対子 */
	if (RuleData::chkRuleApplied("nikoniko_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ニコニコ七対子"), get_yaku_han("nikoniko_pairs"),
			[](const MentsuAnalysis* const analysis) -> bool {
				constexpr std::array<TileCode, 6> tc = {
					TileCode::characterTwo, TileCode::characterFive,
					TileCode::circleTwo, TileCode::circleFive, TileCode::bambooTwo, TileCode::bambooFive,
				};
				return isQiDui(analysis) && chktiles(analysis, tc);
			}
		));
	/* 一休さん七対子 */
	if (RuleData::chkRuleApplied("ikkyuusan_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一休さん七対子"), get_yaku_han("ikkyuusan_pairs"),
			[](const MentsuAnalysis* const analysis) -> bool {
				constexpr std::array<TileCode, 9> tc = {
					TileCode::characterOne, TileCode::characterThree, TileCode::characterNine,
					TileCode::circleOne, TileCode::circleThree, TileCode::circleNine,
					TileCode::bambooOne, TileCode::bambooThree, TileCode::bambooNine,
				};
				return isQiDui(analysis) && chktiles(analysis, tc);
			}
		));

	// ---------------------------------------------------------------------

	/* 三色同対 */
	if (RuleData::chkRuleApplied("sanshoku_doutoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色同対"), get_yaku_han("sanshoku_doutoi"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->TileCount[composeNumberTile(TileSuit::characters, i)] >= 2) &&
						(analysis->TileCount[composeNumberTile(TileSuit::circles, i)] >= 2) &&
						(analysis->TileCount[composeNumberTile(TileSuit::bamboos, i)] >= 2))
						yakuFlag = true;
				return isQiDui(analysis) && yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	{
		const auto gyakusharin =
			[](const MentsuAnalysis* const analysis, TileSuit suit) -> bool {
				std::array<TileCode, 7> honors = {
					TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
					TileCode::whiteDragon, TileCode::greenDragon, TileCode::redDragon,
				}; int count = 0;
				for (const auto& k : honors)
					if (analysis->TileCount[k] >= 2) ++count;
				return isQiDui(analysis) &&
					(analysis->TileCount[composeNumberTile(suit, 1)] >= 2) &&
					(analysis->TileCount[composeNumberTile(suit, 9)] >= 2) &&
					(count == 5);
			};
		/* 逆車輪 */
		if (RuleData::chkRuleApplied("gyakusharin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("逆車輪"), get_yaku_han("gyakusharin"),
				[gyakusharin](const MentsuAnalysis* const analysis) -> bool {
					return gyakusharin(analysis, TileSuit::circles);
				}
			));
		/* 逆竹林 */
		if (RuleData::chkRuleApplied("gyakuchikurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("逆竹林"), get_yaku_han("gyakuchikurin"),
				[gyakusharin](const MentsuAnalysis* const analysis) -> bool {
					return gyakusharin(analysis, TileSuit::bamboos);
				}
			));
		/* 逆数隣 */
		if (RuleData::chkRuleApplied("gyakusuurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("逆数隣"), get_yaku_han("gyakusuurin"),
				[gyakusharin](const MentsuAnalysis* const analysis) -> bool {
					return gyakusharin(analysis, TileSuit::characters);
				}
			));
	}
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 国士 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("十三幺"), 88_fenF,
		_T("五門斉"), _T("門前清"), _T("単調将"),
#else /* GUOBIAO */
		_T("国士無双"), 1_yakumanM,
		_T("十三不塔"), _T("五門斎"),
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->shanten[ShantenType::orphans] == -1);
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("double_yakuman")) /* 国士13面待ち（ダブル役満） */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("国士無双十三面"), 2_yakumanM,
			_T("国士無双"), _T("十三不塔"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::orphans] == -1) && // 国士無双になっていて、
					(!analysis->MachiInfo.FuritenFlag) && // フリテンではなくて、
					((analysis->MachiInfo.MachiMen == 13)||(analysis->GameStat->TianHuFlag))); // 13面待ちか天和になっている
			}
		));
	/* 七星無靠 */
	if (RuleData::chkRuleApplied("stellar_uushii"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("七星不靠"), 24_fenF,
			_T("全不靠"), _T("五門斉"), _T("門前清"), _T("単調将"),
#else /* GUOBIAO */
			_T("七星無靠"), get_yaku_han("stellar_uushii"),
			_T("全不靠"),
#endif /* GUOBIAO */
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::stellar] == -1);
			}
		));
#ifndef GUOBIAO
	/* 南北戦争 */
	if (RuleData::chkRuleApplied("civil_war"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南北戦争"), get_yaku_han("civil_war"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::civilWar] == -1);
			}
		));
	/* 東北新幹線グリーン車 */
	if (RuleData::chkRuleApplied("tohoku_shinkansen_green"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東北新幹線グリーン車"), get_yaku_han("tohoku_shinkansen_green"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::tohokuGreen] == -1);
			}
		));
	/* 惑星直列 */
	if (RuleData::chkRuleApplied("syzygy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("惑星直列"), get_yaku_han("syzygy"),
			_T("清一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::syzygy] == -1);
			}
		));
	/* 全不靠 */
	if (RuleData::chkRuleApplied("quanbukao"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("全不靠"), 12_fenF,
			_T("五門斉"), _T("門前清"), _T("単調将"), _T("七星不靠"),
#else /* GUOBIAO */
			_T("全不靠"), get_yaku_han("quanbukao"),
#endif /* GUOBIAO */
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::quanbukao] == -1);
			}
		));
#ifndef GUOBIAO
	/* セブンアップ */
	if (RuleData::chkRuleApplied("sevenup"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("セブンアップ"), get_yaku_han("sevenup"),
			_T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::sevenup] == -1);
			}
		));
	/* 組合龍 */
	if (RuleData::chkRuleApplied("zuhelong"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("組合龍"), 12_fenF,
#else /* GUOBIAO */
			_T("組合龍"), get_yaku_han("zuhelong"),
#endif /* GUOBIAO */
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::zuhelong] == -1);
			}
		));
#ifndef GUOBIAO
	/* 仁和寺 */
	if (RuleData::chkRuleApplied("ninnaji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("仁和寺"), get_yaku_han("ninnaji"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->shanten[ShantenType::ninnaji] == -1);
			}
		));
#endif /* GUOBIAO */
}
