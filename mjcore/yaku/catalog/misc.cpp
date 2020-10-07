#include "../catalog.h"

extern GameTable GameStat;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_misc() {
	const auto chktiles = // 面子手・七対子兼用の判定関数オブジェクト
		[](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetKez, int numOfKez,
		const TileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SizeOfMeldBuffer - (noDui ? 1 : 0));
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NumOfTilesInHand / 2);
			else return false;
		};
	const auto chktiles_nodup = // 面子手・七対子兼用の判定関数オブジェクト
		[](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetKez, int numOfKez,
		const TileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[ShantenType::regular] == -1)
				return (yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SizeOfMeldBuffer - (noDui ? 1 : 0));
			else if (analysis->shanten[ShantenType::pairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NumOfTilesInHand / 2);
			else return false;
		};

	// ---------------------------------------------------------------------

	/* 緑一色 */
	{
		const auto allgrean =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight, TileCode::greenDragon};
				return chktiles(analysis, kezi, 6, kezi, 1, false);
			};
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("緑一色"), 88_fenF,
			allgrean
		));
#else /* GUOBIAO */
		if (RuleData::chkRuleApplied("dragonless_all_green"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色"), 1_yakumanF,
				_T("混一色"), _T("茶一色"), _T("断紅和"), _T("混断紅"),
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[TileCode::greenDragon] > 0);
				}
			));
		else if (GameStat.chkGameType(GameTypeID::sanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色"), 1_yakumanF,
				_T("混一色"), _T("清一色"), _T("茶一色"), _T("断紅和"), _T("清断紅"),
				allgrean
			));
		else // 發なしでも良いルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色"), 1_yakumanF,
				_T("混一色"), _T("茶一色"), _T("断紅和"), _T("清断紅"), _T("混断紅"), // 清一色は複合可
				allgrean
			));
		if (RuleData::chkRule("dragonless_all_green", "double_yakuman")) // 發なしがダブル役満になるルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				GameStat.chkGameType(GameTypeID::sanmaS) ? _T("清緑一色") : _T("緑一色"), 2_yakumanF,
				_T("清一色"), _T("茶一色"), _T("断紅和"), _T("清断紅"),
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[TileCode::greenDragon] == 0);
				}
			));
#endif /* GUOBIAO */
	}

	// ---------------------------------------------------------------------

	/* 裸単騎１飜というローカル役 */
	{
		const auto chkHadakaTanki =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < SizeOfMeldBuffer; i++)
					if ((analysis->MianziDat[i].mstat != MeldStat::sequenceConcealed) &&
						(analysis->MianziDat[i].mstat != MeldStat::tripletConcealed) &&
						(analysis->MianziDat[i].mstat != MeldStat::quadConcealed)) ++count;
				return (count == SizeOfMeldBuffer - 1);
			};
#ifndef GUOBIAO
		if (RuleData::chkRuleApplied("shiiaru_raotai")) {
			if (!RuleData::chkRule("shiiaru_raotai", "1han_ron_only"))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					_T("全倒鋪"), 1_hanF,
					[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
						return chkHadakaTanki(analysis) && analysis->GameStat->TsumoAgariFlag;
					}
				));
#endif /* GUOBIAO */
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
				_T("全求人"), 6_fenF,
#else /* GUOBIAO */
				_T("全求人"), 1_hanF,
#endif /* GUOBIAO */
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (!analysis->GameStat->TsumoAgariFlag);
				}
			));
#ifndef GUOBIAO
		}
		// 金鶏独立…一索待ち
		if (RuleData::chkRuleApplied("kinkei_dokuritsu"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("金鶏独立"), get_yaku_han("kinkei_dokuritsu"),
				_T("全倒鋪"), _T("全求人"),
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == TileCode::bambooOne);
				}
			));
		// 独釣寒江雪…白待ち
		if (RuleData::chkRuleApplied("kankou_no_yuki"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("独釣寒江雪"), get_yaku_han("kankou_no_yuki"),
				_T("全倒鋪"), _T("全求人"),
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == TileCode::whiteDragon);
				}
			));
#endif /* GUOBIAO */
	}

	// ---------------------------------------------------------------------

	/* 推不倒 */
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("toipuutao")) {
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("推不倒"), 8_fenF,
			_T("缺一門"),
#else /* GUOBIAO */
			_T("推不倒"), get_yaku_han("toipuutao"),
			_T("絶一門"),
#endif /* GUOBIAO */
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::circleOne, TileCode::circleTwo, TileCode::circleThree, TileCode::bambooFour, TileCode::circleFour, TileCode::circleFive, TileCode::circleEight,
					TileCode::circleNine, TileCode::bambooTwo, TileCode::bambooFive, TileCode::bambooSix, TileCode::bambooEight, TileCode::bambooNine, TileCode::whiteDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 4, false);
			}
		));
#ifndef GUOBIAO
	}
	/* 紅孔雀 */
	if (RuleData::chkRuleApplied("benikujaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("紅孔雀"), get_yaku_han("benikujaku"),
			_T("混一色"), _T("対々和"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooOne, TileCode::bambooFive, TileCode::bambooSeven, TileCode::bambooNine, TileCode::redDragon,
				};
				return chktiles(analysis, kezi, 5, nullptr, 0, false);
			}
		));
	/* 萬緑叢中一点紅 */
	if (RuleData::chkRuleApplied("kouitten"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("紅一点"), get_yaku_han("kouitten"),
			_T("混一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight, TileCode::redDragon,
				};
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
					(analysis->TileCount[TileCode::redDragon] > 0);
			}
		));
	/* 白有り緑一色 */
	if (RuleData::chkRuleApplied("all_green_with_white")) {
		const auto allgreen_white =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight, TileCode::whiteDragon,
				};
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
					(analysis->TileCount[TileCode::whiteDragon] > 0);
			};
		if (RuleData::chkRule("all_green_with_white", "2han"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("清二色"), 2_hanF,
				allgreen_white
			));
		else if (RuleData::chkRule("all_green_with_white", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("白有り緑一色"), 1_yakumanF,
				_T("混一色"),
				allgreen_white
			));
	}
	/* 緑一色輪 */
	if (RuleData::chkRuleApplied("ryuuiisohrin")) {
		const auto allgreen_five =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
				};
				return chktiles(analysis, kezi, 5, kezi, 1, true) &&
					(analysis->MianziDat[0].tile == TileCode::bambooFive);
			};
		if (GameStat.chkGameType(GameTypeID::sanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色輪"), get_yaku_han("ryuuiisohrin"),
				_T("清一色"),
				allgreen_five
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色輪"), get_yaku_han("ryuuiisohrin"),
				allgreen_five
			));
	}
	/* 大草原 */
	if (RuleData::chkRuleApplied("daisougen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大草原"), get_yaku_han("daisougen"),
			_T("清一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooFive,
					TileCode::bambooSix, TileCode::bambooSeven, TileCode::bambooNine,
				};
				return chktiles(analysis, kezi, 7, kezi, 4, false);
			}
		));
	/* 南大草原 */
	if (RuleData::chkRuleApplied("south_daisougen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南大草原"), get_yaku_han("south_daisougen"),
			_T("混一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooFive,
					TileCode::bambooSix, TileCode::bambooSeven, TileCode::bambooNine, TileCode::southWind,
				};
				return chktiles(analysis, kezi, 8, kezi, 4, false) &&
					(analysis->TileCount[TileCode::southWind] > 0);
			}
		));
	/* 桃一色 */
	if (RuleData::chkRuleApplied("taoyise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("桃一色"), get_yaku_han("taoyise"),
			_T("混一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->shanten[ShantenType::regular] == -1) && (analysis->KeziCount[TileCode::redDragon] >= 1))
					yakuFlag = true;
				else if ((analysis->shanten[ShantenType::pairs] == -1) && (analysis->TileCount[TileCode::redDragon] >= 2))
					yakuFlag = true;
				constexpr TileCode kezi[] = {
					TileCode::circleFive, TileCode::circleOne, TileCode::circleThree, TileCode::circleSix,
					TileCode::circleSeven, TileCode::circleNine, TileCode::redDragon,
				};
				return chktiles(analysis, kezi, 7, kezi, 1, false) &&
					((countRedTilesInHand(analysis->GameStat, analysis->player, DoraCol::akaDora))[TileCode::circleFive] > 0) &&
					yakuFlag;
			}
		));
	/* 茶一色 */
	if (RuleData::chkRuleApplied("chayise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("茶一色"), get_yaku_han("chayise"),
			_T("混一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
					TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
					TileCode::whiteDragon, TileCode::greenDragon,
				};
				return chktiles(analysis, kezi, 11, kezi, 1, false);
			}
		));
	/* 黒一色 */
	if (RuleData::chkRuleApplied("heiiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("黒一色"), get_yaku_han("heiiisoh"),
			_T("混一色"), _T("対々和"), _T("青洞門"), _T("断紅和"), _T("混断紅"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::circleTwo, TileCode::circleFour, TileCode::circleEight,
					TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
				};
				return chktiles(analysis, kezi, 7, nullptr, 0, false);
			}
		));
	/* 反緑一色 */
	if (RuleData::chkRuleApplied("reverse_green"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("反緑一色"), get_yaku_han("reverse_green"),
			_T("混一色"), _T("対々和"), _T("断紅和"), _T("混断紅"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1) {
					constexpr TileCode kezi[] = {
						TileCode::bambooSeven,
						TileCode::bambooTwo, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
						TileCode::greenDragon,
					};
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 6, nullptr, 0, false) == SizeOfMeldBuffer) &&
						(yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 1, nullptr, 0, false) == 1);
				}
				else return false;
			}
		));
	/* 反緑一色輪 */
	if (RuleData::chkRuleApplied("reverse_ryuuiisohrin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("反緑一色輪"), get_yaku_han("reverse_ryuuiisohrin"),
			_T("混一色"), _T("対々和"), _T("断紅和"), _T("混断紅"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1) {
					constexpr TileCode kezi[] = {
						TileCode::bambooSeven,
						TileCode::bambooTwo, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
					};
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 5, nullptr, 0, false) == SizeOfMeldBuffer) &&
						(yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 1, nullptr, 0, false) == 1) &&
						(analysis->MianziDat[0].tile == TileCode::bambooFive);
				}
				else return false;
			}
		));
	/* 反黒一色 */
	if (RuleData::chkRuleApplied("reverse_black"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("反黒一色"), get_yaku_han("reverse_black"),
			_T("混一色"), _T("対々和"), _T("断紅和"), _T("混断紅"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::circleSix,
					TileCode::circleTwo, TileCode::circleEight,
					TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
				};
				return (yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, kezi, 7, nullptr, 0, false) == SizeOfMeldBuffer) &&
					(yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, kezi, 1, nullptr, 0, false) == 1);
			}
		));
	/* 青洞門 */
	if (RuleData::chkRuleApplied("ao_no_domon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("青洞門"), get_yaku_han("ao_no_domon"),
			_T("混一色"), _T("対々和"), _T("断紅和"), _T("混断紅"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1) {
					constexpr TileCode kezi[] = {
						TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
						TileCode::circleTwo, TileCode::circleFour, TileCode::circleEight,
						TileCode::greenDragon,
					};
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 8, nullptr, 0, false) == SizeOfMeldBuffer) &&
						(yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 4, nullptr, 0, false) == 1);
				}
				else return false;
			}
		));
	/* 断紅和 */
	if (RuleData::chkRuleApplied("no_red")) {
		const auto hontanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
					TileCode::circleTwo, TileCode::circleFour, TileCode::circleEight,
					TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
					TileCode::whiteDragon, TileCode::greenDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 1, false);
			};
		const auto chintanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooSix, TileCode::bambooEight,
					TileCode::circleTwo, TileCode::circleFour, TileCode::circleEight,
				};
				return chktiles(analysis, kezi, 8, kezi, 1, false);
			};
		if (RuleData::chkRule("no_red", "hon3han_chin6han") || RuleData::chkRule("no_red", "6han")) {
			if (!GameStat.chkGameType(GameTypeID::sanmaS))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					_T("混断紅"), 3_hanK,
					hontanhon
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				GameStat.chkGameType(GameTypeID::sanmaS) ? _T("清断紅") : _T("断紅和"), 6_hanK,
				_T("混断紅"),
				chintanhon
			));
		} else {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("断紅和"), get_yaku_han("no_red"),
				hontanhon
			));
		}
	}
	/* 紅白饅頭 */
	if (RuleData::chkRuleApplied("kohaku_manju"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("紅白饅頭"), get_yaku_han("kohaku_manju"),
			_T("紅白七対"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterOne, TileCode::characterTwo, TileCode::characterThree, TileCode::characterFour,
					TileCode::characterFive, TileCode::characterSix, TileCode::characterSeven, TileCode::characterEight,
					TileCode::characterNine, TileCode::whiteDragon, TileCode::redDragon,
				};
				return chktiles(analysis, kezi, 11, kezi, 7, false) &&
					(analysis->TileCount[TileCode::whiteDragon] > 0) &&
					(analysis->TileCount[TileCode::redDragon] > 0);
			}
		));
	/* 素数役満 (triplet2.cppより移動) */
	if (RuleData::chkRuleApplied("prime_number"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("素数役満"), get_yaku_han("prime_number"),
			_T("対々和"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterTwo, TileCode::characterThree, TileCode::characterFive, TileCode::characterSeven,
					TileCode::circleTwo, TileCode::circleThree, TileCode::circleFive, TileCode::circleSeven,
					TileCode::bambooTwo, TileCode::bambooThree, TileCode::bambooFive, TileCode::bambooSeven,
				};
				return chktiles(analysis, kezi, 12, nullptr, 0, false);
			}
		));
	/* 白一色 (triplet2.cppより移動) */
	if (RuleData::chkRuleApplied("hakuiisoo"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白一色"), get_yaku_han("hakuiisoo"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterEight, TileCode::characterNine, TileCode::circleEight, TileCode::circleNine,
					TileCode::bambooEight, TileCode::bambooNine, TileCode::whiteDragon,
				};
				return chktiles(analysis, kezi, 7, nullptr, 0, false);
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifdef GUOBIAO
	{
		const auto suukuiyii =
			[](const MENTSU_ANALYSIS* const analysis) -> int {
				int count = 0;
				for (auto i : AllTiles)
					if ((analysis->TileCount[i] == 4)&&(analysis->KangziCount[i] == 0))
						++count;
				return count;
			};
		/* 四帰一 */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四帰一"), 2_fenF,
			[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (suukuiyii(analysis) == 1);
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四帰一x2"), 4_fenF,
			[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (suukuiyii(analysis) == 2);
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四帰一x3"), 6_fenF,
			[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (suukuiyii(analysis) == 3);
			}
		));
	}
#else /* GUOBIAO */
	// 四帰四絡みで考えられるパターンと牌式表記
	// 123 123 123 123: 牌式44440 ←これは一色四順
	// 123 123 123 234: 牌式34410
	// 123 123 123 345: 牌式33411
	// 123 123 234 234: 牌式24420
	// 123 123 234 345: 牌式23421
	// 123 123 345 345: 牌式22422
	// 123 234 234 234: 牌式14430
	// 123 234 234 345: 牌式13431
	// 123 234 345 345: 牌式12432
	// 123 345 345 345: 牌式11433

	/* 大四帰四 */
	if (RuleData::chkRuleApplied("great_four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大四帰四"), get_yaku_han("great_four_into_four"),
			_T("重四帰四"), _T("四帰四"), _T("四帰三一"), _T("四帰三"), _T("龍四帰一"), _T("虎四帰一"), _T("両四帰一"), _T("四帰一"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto i : NumberTiles) {
					// 123 123 234 234: 牌式24420
					if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 2) &&
						((analysis->MianziDat[0].tile == i) ||
						(analysis->MianziDat[0].tile == static_cast<TileCode>(static_cast<int>(i) + 3))))
						return true;
					// 123 123 234 345: 牌式23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 1) &&
						(analysis->ShunziCount[static_cast<int>(i) + 2] >= 1) &&
						((analysis->MianziDat[0].tile == i) ||
						(analysis->MianziDat[0].tile == static_cast<TileCode>(static_cast<int>(i) + 3))))
						return true;
					// 123 123 345 345: 牌式22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[static_cast<int>(i) + 2] >= 2) &&
						((analysis->MianziDat[0].tile == i) ||
						(analysis->MianziDat[0].tile == static_cast<TileCode>(static_cast<int>(i) + 1)) ||
						(analysis->MianziDat[0].tile == static_cast<TileCode>(static_cast<int>(i) + 3)) ||
						(analysis->MianziDat[0].tile == static_cast<TileCode>(static_cast<int>(i) + 4))))
						return true;
					// 123 234 345 345: 牌式12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 1) &&
						(analysis->ShunziCount[static_cast<int>(i) + 2] >= 2) &&
						((analysis->MianziDat[0].tile == i) ||
						(analysis->MianziDat[0].tile == static_cast<TileCode>(static_cast<int>(i) + 3))))
						return true;
				}
				return false;
			}
		));
	/* 重四帰四 */
	if (RuleData::chkRuleApplied("double_four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("重四帰四"), get_yaku_han("double_four_into_four"),
			_T("四帰四"), _T("四帰三一"), _T("四帰三"), _T("龍四帰一"), _T("虎四帰一"), _T("両四帰一"), _T("四帰一"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto i : NumberTiles) {
					// 123 123 123 234: 牌式34410
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 1))
						return true;
					// 123 123 234 234: 牌式24420
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 2))
						return true;
					// 123 234 234 234: 牌式14430
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 3))
						return true;
				}
				return false;
			}
		));
	/* 四帰四 */
	if (RuleData::chkRuleApplied("four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四帰四"), get_yaku_han("four_into_four"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto i : NumberTiles) {
					// 123 123 123 345: 牌式33411
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[static_cast<int>(i) + 2] >= 1))
						return true;
					// 123 123 234 345: 牌式23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 1) &&
						(analysis->ShunziCount[static_cast<int>(i) + 2] >= 1))
						return true;
					// 123 123 345 345: 牌式22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[static_cast<int>(i) + 2] >= 2))
						return true;
					// 123 234 234 345: 牌式13431
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 2) &&
						(analysis->ShunziCount[static_cast<int>(i) + 2] >= 1))
						return true;
					// 123 234 345 345: 牌式12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[static_cast<int>(i) + 1] >= 1) &&
						(analysis->ShunziCount[static_cast<int>(i) + 2] >= 2))
						return true;
					// 123 345 345 345: 牌式11433
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[static_cast<int>(i) + 2] >= 3))
						return true;
				}
				return false;
			}
		));

	// ---------------------------------------------------------------------

	{
		const auto suukuisan =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto i : NumberTiles) {
					if (analysis->MianziDat[0].tile == i) {
						if (analysis->ShunziCount[i] >= 2)
							return true;
						else if (static_cast<int>(i) < 1) continue;
						else if ((analysis->ShunziCount[static_cast<int>(i) - 1] >= 1) && (analysis->ShunziCount[i] >= 1))
							return true;
						else if (analysis->ShunziCount[static_cast<int>(i) - 1] >= 2)
							return true;
						else if (static_cast<int>(i) < 2) continue;
						else if ((analysis->ShunziCount[static_cast<int>(i) - 2] >= 1) && (analysis->ShunziCount[i] >= 1))
							return true;
						else if ((analysis->ShunziCount[static_cast<int>(i) - 2] >= 1) && (analysis->ShunziCount[static_cast<int>(i) - 1] >= 1))
							return true;
						else if (analysis->ShunziCount[static_cast<int>(i) - 2] >= 2)
							return true;
					}
				}
				return false;
			};
		/* 四帰三一 */
		if (RuleData::chkRuleApplied("four_into_three_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("四帰三一"), get_yaku_han("four_into_three_one"),
				_T("四帰三"), _T("四帰一"),
				[suukuisan](const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag = false;
					for (auto i : NumberTiles)
						if ((analysis->ShunziCount[i] >= 1) &&
							((analysis->KeziCount[i] >= 1) ||
							(analysis->KeziCount[static_cast<int>(i) + 1] >= 1) ||
							(analysis->KeziCount[static_cast<int>(i) + 2] >= 1))) yakuFlag = true;
					return suukuisan(analysis) && yakuFlag;
				}
			));
		/* 四帰三 */
		if (RuleData::chkRuleApplied("four_into_three"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("四帰三"), get_yaku_han("four_into_three"),
				_T("四帰一"),
				suukuisan
			));
	}

	// ---------------------------------------------------------------------

	/* 龍四帰一 */
	if (RuleData::chkRuleApplied("dragon_four_into_one"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("龍四帰一"), get_yaku_han("dragon_four_into_one"),
			_T("三連刻"), _T("虎四帰一"), _T("両四帰一"), _T("四帰一"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i : NumberTiles)
					if ((analysis->ShunziCount[i] >= 1) &&
						(analysis->KeziCount[i] >= 1) &&
						(analysis->KeziCount[static_cast<int>(i) + 1] >= 1) &&
						(analysis->KeziCount[static_cast<int>(i) + 2] >= 1)) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 虎四帰一 */
	if (RuleData::chkRuleApplied("tiger_four_into_one"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("虎四帰一"), get_yaku_han("tiger_four_into_one"),
			_T("両四帰一"), _T("四帰一"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i : NumberTiles) {
					if (analysis->ShunziCount[i] >= 1) {
						if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[static_cast<int>(i) + 1] >= 1)) yakuFlag = true;
						else if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[static_cast<int>(i) + 2] >= 1)) yakuFlag = true;
						else if ((analysis->KeziCount[static_cast<int>(i) + 1] >= 1) && (analysis->KeziCount[static_cast<int>(i) + 2] >= 1)) yakuFlag = true;
					}
				}
				return yakuFlag;
			}
		));
	{
		const auto suukuiyii =
			[](const MENTSU_ANALYSIS* const analysis) -> int {
				int count = 0;
				for (auto i : NumberTiles) {
					if (analysis->ShunziCount[i] >= 1) {
						if (analysis->KeziCount[i] >= 1) ++count;
						else if (analysis->KeziCount[static_cast<int>(i) + 1] >= 1) ++count;
						else if (analysis->KeziCount[static_cast<int>(i) + 2] >= 1) ++count;
					}
				}
				return count;
			};
		/* 両四帰一 */
		if (RuleData::chkRuleApplied("double_four_into_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("両四帰一"), get_yaku_han("double_four_into_one"),
				_T("四帰一"),
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (suukuiyii(analysis) == 2);
				}
			));
		/* 四帰一 */
		if (RuleData::chkRuleApplied("four_into_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("四帰一"), get_yaku_han("four_into_one"),
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (suukuiyii(analysis) == 1);
				}
			));
	}
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 九蓮花燈 */
	if (RuleData::chkRuleApplied("chuuren_hwaton"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("九蓮花燈"), get_yaku_han("chuuren_hwaton"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i <= 9; i++)
					if (analysis->TileCount[composeNumberTile(TileSuit::characters, i)] +
						analysis->TileCount[composeNumberTile(TileSuit::circles, i)] +
						analysis->TileCount[composeNumberTile(TileSuit::bamboos, i)] >=
						(((i == 1)||(i == 9)) ? 3 : 1) )
						++count;
				return (count == 9);
			}
		));
	/* 鳴き九連宝燈 */
	if (RuleData::chkRuleApplied("exposed_chuuren"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("九蓮碧燈"), get_yaku_han("exposed_chuuren"),
			/* 鳴きチンイツが複合し倍満以上が確定 */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				Int8ByTile tiles = analysis->TileCount; bool yakuFlag = false;
				// 鳴き面子も計算 槓子は無視
				for (int i = 1; i < SizeOfMeldBuffer; i++) {
					switch (analysis->MianziDat[i].mstat) {
					case MeldStat::sequenceExposedLower: case MeldStat::sequenceExposedMiddle:
					case MeldStat::sequenceExposedUpper:
						++tiles[analysis->MianziDat[i].tile];
						++tiles[static_cast<int>(analysis->MianziDat[i].tile) + 1];
						++tiles[static_cast<int>(analysis->MianziDat[i].tile) + 2];
						break;
					case MeldStat::tripletExposedLeft: case MeldStat::tripletExposedCenter:
					case MeldStat::tripletExposedRight:
						tiles[analysis->MianziDat[i].tile] += 3;
						break;
					}
				}
				for (auto i : NumberTileSuits)
					if ((tiles[composeNumberTile(i, 1)] >= 3) && (tiles[composeNumberTile(i, 2)] >= 1) && (tiles[composeNumberTile(i, 3)] >= 1) &&
						(tiles[composeNumberTile(i, 4)] >= 1) && (tiles[composeNumberTile(i, 5)] >= 1) && (tiles[composeNumberTile(i, 6)] >= 1) &&
						(tiles[composeNumberTile(i, 7)] >= 1) && (tiles[composeNumberTile(i, 8)] >= 1) && (tiles[composeNumberTile(i, 9)] >= 3))
						yakuFlag = true;
				return yakuFlag && (!(*analysis->MenzenFlag));
			}
		));

	// ---------------------------------------------------------------------

	/* 茴香ポン(非対々) */
	if (RuleData::chkRuleApplied("uikyou_shuntsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("茴香") PON _T(" "), // 対々形と区別するために後ろにスペースを入れる
			get_yaku_han("uikyou_shuntsu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++) {
					if ((analysis->KeziCount[composeNumberTile(TileSuit::characters, i)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(TileSuit::circles, i)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(TileSuit::bamboos, i)] >= 1)) {
							if (i <= 5)
								for (auto k : NumberTileSuits)
									if ((analysis->ShunziCount[composeNumberTile(k, i + 1)] >= 1) &&
										(analysis->MianziDat[0].tile == composeNumberTile(k, i + 4)))
										yakuFlag = true;
							/* ここにはelseをつけてはだめ */
							if (i >= 5)
								for (auto k : NumberTileSuits)
									if ((analysis->ShunziCount[composeNumberTile(k, i - 3)] >= 1) &&
										(analysis->MianziDat[0].tile == composeNumberTile(k, i - 4)))
										yakuFlag = true;
					}
				}
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 武太郎売焼餅 */
	if (RuleData::chkRuleApplied("uutaaran"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("武太郎売焼餅"), get_yaku_han("uutaaran"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterFive, TileCode::circleFive, TileCode::bambooFive,
				};
				constexpr TileCode shunzi[] = {
					TileCode::characterThree, TileCode::characterFour, TileCode::characterFive,
					TileCode::circleThree, TileCode::circleFour, TileCode::circleFive,
					TileCode::bambooThree, TileCode::bambooFour, TileCode::bambooFive,
				};
				return chktiles(analysis, kezi, 3, shunzi, 9, true) &&
					(analysis->MianziDat[0].tile == TileCode::circleOne);
			}
		));
	/* 北の漁場 */
	if (RuleData::chkRuleApplied("kitanogyojou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北の漁場"), get_yaku_han("kitanogyojou"),
			_T("四帰一"), _T("混一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (auto i : NumberTileSuits) {
					const TileCode kezi[] = {
						composeNumberTile(i, 3), composeNumberTile(i, 6), TileCode::northWind,
					};
					const TileCode shunzi[] = {
						composeNumberTile(i, 3), composeNumberTile(i, 4)
					};
					if (chktiles(analysis, kezi, 3, shunzi, 1, true) &&
						(analysis->TsumoHai->tile == composeNumberTile(i, 3)) &&
						(Tile(analysis->MianziDat[0].tile).isHonor()))
						flag = true;
					if (chktiles(analysis, kezi, 3, shunzi + 1, 1, true) &&
						(analysis->TsumoHai->tile == composeNumberTile(i, 6)) &&
						(Tile(analysis->MianziDat[0].tile).isHonor()))
						flag = true;
				}
				return flag;
			}
		));
	/* 愚濫怒苦露酢 */
	if (RuleData::chkRuleApplied("grand_cross"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("愚濫怒苦露酢"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->TsumoHai->tile == TileCode::circleFive) ?
					2_yakuman :
					1_yakuman);
			}),
			_T("清一色"),
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::circleFour, TileCode::circleSix,
				};
				constexpr TileCode shunzi[] = {
					TileCode::circleOne, TileCode::circleSeven,
				};
				return chktiles_nodup(analysis, kezi, 2, shunzi, 2, true) &&
					(analysis->MianziDat[0].tile == TileCode::circleFive) &&
					(*analysis->MenzenFlag);
			}
		));
	/* 喜鵲閙梅 */
	if (RuleData::chkRuleApplied("xique_naomei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("喜鵲閙梅"), get_yaku_han("xique_naomei"),
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::circleFive, TileCode::bambooOne,
				};
				constexpr TileCode shunzi[] = {
					TileCode::characterOne, TileCode::circleOne,
				};
				return chktiles_nodup(analysis, kezi, 2, shunzi, 2, true) &&
					(analysis->MianziDat[0].tile == TileCode::bambooFive);
			}
		));
	/* 花一色 */
	if (RuleData::chkRuleApplied("huaiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("花一色"), get_yaku_han("huaiisoh"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterOne, TileCode::characterTwo, TileCode::characterThree, TileCode::characterFour, TileCode::characterFive,
					TileCode::characterSix, TileCode::characterSeven, TileCode::characterEight, TileCode::characterNine, TileCode::redDragon,
					TileCode::circleOne, TileCode::circleThree, TileCode::circleFive, TileCode::circleSix, TileCode::circleSeven,
					TileCode::circleNine, TileCode::bambooOne, TileCode::bambooFive, TileCode::bambooSeven, TileCode::bambooNine,
				};
				constexpr TileCode shunzi[] = {
					TileCode::characterOne, TileCode::characterTwo, TileCode::characterThree, TileCode::characterFour, TileCode::characterFive,
					TileCode::characterSix, TileCode::characterSeven, TileCode::circleFive,
				};
				return (chktiles(analysis, kezi, 20, shunzi, 8, false)) &&
					(!chktiles(analysis, kezi, 10, shunzi, 7, false));
			}
		));
	/* 赤一色 */
	if (RuleData::chkRuleApplied("akaiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("赤一色"), get_yaku_han("akaiisoh"),
			_T("花一色"), _T("絶一門"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::circleFive, TileCode::circleOne, TileCode::circleThree, TileCode::circleSix, TileCode::circleSeven,
					TileCode::circleNine, TileCode::bambooOne, TileCode::bambooFive, TileCode::bambooSeven, TileCode::bambooNine,
				};
				return chktiles(analysis, kezi, 10, kezi, 1, false);
			}
		));
	/* 赤眼青龍 */
	if (RuleData::chkRuleApplied("sekigan_seiryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("赤眼青龍"), get_yaku_han("sekigan_seiryuu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->ShunziCount[TileCode::circleTwo] == 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] + analysis->KeziCount[TileCode::eastWind] == 1) &&
					(analysis->DuiziCount[TileCode::circleTwo] + analysis->DuiziCount[TileCode::circleFour] +
					analysis->DuiziCount[TileCode::circleEight] >= 3));
			}
		));
	/* 合格祈願(東刻子) */
	if (RuleData::chkRuleApplied("goukaku_kigan_east"))
		for (int i = 1; i <= 4; ++i)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("合格祈願 "), yaku::yakuCalculator::Yaku::FixedHan(
				yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(i, HanUnit::han),
				0_han),
				[i](const MENTSU_ANALYSIS* const analysis) -> bool {
					unsigned tileCount = static_cast<unsigned>(analysis->TileCount[TileCode::circleFive]);
					tileCount += 3 * (analysis->KeziCount[TileCode::circleFive] - analysis->AnKeziCount[TileCode::circleFive]);
					tileCount += (analysis->KangziCount[TileCode::circleFive] - analysis->AnKangziCount[TileCode::circleFive]);
					for (unsigned int j = 3; j <= 5; ++j)
						tileCount += (analysis->ShunziCount[composeNumberTile(TileSuit::circles, j)] - analysis->AnShunziCount[composeNumberTile(TileSuit::circles, j)]);
					return ((analysis->KeziCount[TileCode::eastWind] >= 1) && (tileCount == i));
				}
			));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 満園花 */
	{
#ifdef GUOBIAO
		constexpr int i = 5;
#else /* GUOBIAO */
		constexpr char ruleCodeList[10][16] = { "",
			"all_one", "all_two", "all_three", "all_four", "all_five",
			"all_six", "all_seven", "all_eight", "all_nine"};
		constexpr TCHAR nameList[10][16] = { _T(""),
			_T("全帯一"), _T("全帯二"), _T("全帯三"), _T("全帯四"), _T("全帯五"), _T("全帯六"), _T("全帯七"), _T("全帯八"), _T("全帯九")};
		for (int i = 1; i <= 9; i++) {
			if (RuleData::chkRuleApplied(ruleCodeList[i])) {
#endif /* GUOBIAO */
				const auto f =
					[chktiles, i](const MENTSU_ANALYSIS* const analysis) -> bool {
						const TileCode kezi[] = {
							composeNumberTile(TileSuit::characters, i),
							composeNumberTile(TileSuit::circles,    i),
							composeNumberTile(TileSuit::bamboos,    i),
						};
						const TileCode shunzi[] = {
							composeNumberTile(TileSuit::characters, i),     composeNumberTile(TileSuit::circles, i),
							composeNumberTile(TileSuit::bamboos,    i),
							composeNumberTile(TileSuit::characters, i - 1), composeNumberTile(TileSuit::circles, i - 1),
							composeNumberTile(TileSuit::bamboos,    i - 1),
							composeNumberTile(TileSuit::characters, i - 2), composeNumberTile(TileSuit::circles, i - 2),
							composeNumberTile(TileSuit::bamboos,    i - 2),
						};
						return chktiles(analysis, kezi, 3,
							&shunzi[i <= 7 ? 0 : 3 * (i - 7)],
							i > 7 ? 3 * (10 - i) : (i < 3 ? i * 3 : 9),
							false);
					};
#ifdef GUOBIAO
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					_T("全帯五"), 16_fenF, _T("断幺"), f));
#else /* GUOBIAO */
				switch (i) {
				case 4: case 5: case 6:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), _T("断幺九"), f));
					break;
				case 1: case 9:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), _T("純全帯幺九"), f));
					break;
				default:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), f));
					break;
				}
			}
		}
#endif /* GUOBIAO */
	}

	// ---------------------------------------------------------------------

	/* 小於五 */
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("xiaoyuwu"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("小于五"), 12_fenF,
			_T("无字"),
#else /* GUOBIAO */
			_T("小於五"), get_yaku_han("xiaoyuwu"),
#endif /* GUOBIAO */
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterOne, TileCode::circleOne, TileCode::bambooOne,
					TileCode::characterTwo, TileCode::circleTwo, TileCode::bambooTwo,
					TileCode::characterThree, TileCode::circleThree, TileCode::bambooThree,
					TileCode::characterFour, TileCode::circleFour, TileCode::bambooFour,
				};
				return chktiles(analysis, kezi, 12, kezi, 6, false);
			}
		));
	/* 大於五 */
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("dayuwu"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("大于五"), 12_fenF,
			_T("无字"),
#else /* GUOBIAO */
			_T("大於五"), get_yaku_han("dayuwu"),
#endif /* GUOBIAO */
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterSix, TileCode::circleSix, TileCode::bambooSix,
					TileCode::characterSeven, TileCode::circleSeven, TileCode::bambooSeven,
					TileCode::characterEight, TileCode::circleEight, TileCode::bambooEight,
					TileCode::characterNine, TileCode::circleNine, TileCode::bambooNine,
				};
				return chktiles(analysis, kezi, 12, kezi, 6, false);
			}
		));
	/* 全小 */
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("all_small"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("全小"), 24_fenF,
			_T("无字"), _T("小于五"),
#else /* GUOBIAO */
			_T("全小"), get_yaku_han("all_small"),
			_T("小於五"),
#endif /* GUOBIAO */
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterOne, TileCode::circleOne, TileCode::bambooOne,
					TileCode::characterTwo, TileCode::circleTwo, TileCode::bambooTwo,
					TileCode::characterThree, TileCode::circleThree, TileCode::bambooThree,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));
	/* 全中 */
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("all_middle"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("全中"), 24_fenF,
			_T("无字"), _T("断幺"),
#else /* GUOBIAO */
			_T("全中"), get_yaku_han("all_middle"),
			_T("断幺九"), _T("上級断幺九"),
#endif /* GUOBIAO */
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterFour, TileCode::circleFour, TileCode::bambooFour,
					TileCode::characterFive, TileCode::circleFive, TileCode::bambooFive,
					TileCode::characterSix, TileCode::circleSix, TileCode::bambooSix,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));
	/* 全大 */
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("all_large"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("全大"), 24_fenF,
			_T("无字"), _T("大于五"),
#else /* GUOBIAO */
			_T("全大"), get_yaku_han("all_large"),
			_T("大於五"),
#endif /* GUOBIAO */
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterSeven, TileCode::circleSeven, TileCode::bambooSeven,
					TileCode::characterEight, TileCode::circleEight, TileCode::bambooEight,
					TileCode::characterNine, TileCode::circleNine, TileCode::bambooNine,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 黒衣騎士 */
	if (RuleData::chkRuleApplied("blackjack"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("黒衣騎士"), get_yaku_han("blackjack"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenType::regular] == -1)
					return yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 21;
				else if (analysis->shanten[ShantenType::pairs] == -1)
					return yaku::countingFacility::countTileNumerals(analysis->TileCount) == 21;
				else return false;
			}
		));
	/* 白衣騎士 */
	if (RuleData::chkRuleApplied("white_knight"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白衣騎士"), get_yaku_han("white_knight"),
			_T("黒衣騎士"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->KeziCount[TileCode::whiteDragon] >= 1)
					return yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 21;
				else return false;
			}
		));

	// ---------------------------------------------------------------------

	/* 東大系 */
	{
		constexpr int rules = 10;
		constexpr char ruleCodeList[rules][16] = {
			"univ_tokyo", "univ_hokkaido", "univ_chuo", "hakuoh_gakuen",
			"univ_osaka", "univ_osaka", "univ_osaka",
			"univ_kyushu", "univ_kyushu", "univ_kyushu",
		};
		constexpr TileCode tileCodeList[rules] = {
			TileCode::eastWind, TileCode::northWind, TileCode::redDragon, TileCode::whiteDragon,
			TileCode::characterEight, TileCode::circleEight, TileCode::bambooEight,
			TileCode::characterNine, TileCode::circleNine, TileCode::bambooNine,
		};
		constexpr TCHAR yakuNameList[rules][16] = {
			_T("東大役満"), _T("北大役満"), _T("中大役満"), _T("白皇学院"),
			_T("阪大役満"), _T("阪大役満"), _T("阪大役満"),
			_T("九大役満"), _T("九大役満"), _T("九大役満"),
		};
		for (int i = 0; i < rules; i++) {
			const TileCode targetTile = tileCodeList[i];
			if (RuleData::chkRuleApplied(ruleCodeList[i]))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					yakuNameList[i], get_yaku_han(ruleCodeList[i]),
					[targetTile](const MENTSU_ANALYSIS* const analysis) -> bool {
						return ((analysis->KangziCount[targetTile] >= 1) &&
							(analysis->Machi == yaku::yakuCalculator::MachiType::ryanmen) &&
							(analysis->TotalKezi == 1) && (*analysis->MenzenFlag));
					}
				));
		}
	}
	/* 東北大 */
	if (RuleData::chkRuleApplied("univ_tohoku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東北大役満"), get_yaku_han("univ_tohoku"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[TileCode::eastWind] >= 1) &&
					(analysis->KangziCount[TileCode::northWind] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::MachiType::ryanmen) &&
					(analysis->TotalKezi == 2) && (*analysis->MenzenFlag));
			}
		));
	/* 名大役満 */
	if (RuleData::chkRuleApplied("univ_nagoya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("名大役満"), get_yaku_han("univ_nagoya"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i : NumberTileSuits)
					for (auto j : NumberTileSuits)
						if ((analysis->KangziCount[composeNumberTile(i, 5)] >= 1) &&
							(analysis->KangziCount[composeNumberTile(j, 8)] >= 1))
							yakuFlag = true;
				return ((yakuFlag) &&
					(analysis->Machi == yaku::yakuCalculator::MachiType::ryanmen) &&
					(analysis->TotalKezi == 2) && (*analysis->MenzenFlag));
			}
		));

	// ---------------------------------------------------------------------

	/* 冬のソナタ */
	if (RuleData::chkRuleApplied("fuyusona"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("冬のソナタ"), get_yaku_han("fuyusona"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[TileCode::characterOne] >= 1) &&
					(analysis->ShunziCount[TileCode::circleOne] >= 1) && (analysis->ShunziCount[TileCode::bambooOne] >= 1))
					yakuFlag = true;
				else if ((analysis->ShunziCount[TileCode::characterSeven] >= 1) &&
					(analysis->ShunziCount[TileCode::circleSeven] >= 1) && (analysis->ShunziCount[TileCode::bambooSeven] >= 1))
					yakuFlag = true;
				return (yakuFlag && (analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->MianziDat[0].tile == TileCode::northWind));
			}
		));
	/* 冬のソナタ２ */
	if (RuleData::chkRuleApplied("fuyusona2"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("冬のソナタ２"), get_yaku_han("fuyusona2"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[TileCode::characterOne] >= 1) && (analysis->ShunziCount[TileCode::circleOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[TileCode::characterOne] >= 1) && (analysis->ShunziCount[TileCode::bambooOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[TileCode::circleOne] >= 1) && (analysis->ShunziCount[TileCode::bambooOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[TileCode::characterSeven] >= 1) && (analysis->ShunziCount[TileCode::circleSeven] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[TileCode::characterSeven] >= 1) && (analysis->ShunziCount[TileCode::bambooSeven] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[TileCode::circleSeven] >= 1) && (analysis->ShunziCount[TileCode::bambooSeven] >= 1)) yakuFlag = true;
				return (yakuFlag &&
					(analysis->KeziCount[TileCode::characterTwo] + analysis->KeziCount[TileCode::circleTwo] + analysis->KeziCount[TileCode::bambooTwo] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->MianziDat[0].tile == TileCode::northWind));
			}
		));

	// ---------------------------------------------------------------------

	/* 南西諸島 */
	if (RuleData::chkRuleApplied("nansei_islands"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南西諸島"), get_yaku_han("nansei_islands"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[TileCode::characterOne] >= 1) &&
					(analysis->ShunziCount[TileCode::circleOne] >= 1) && (analysis->ShunziCount[TileCode::bambooOne] >= 1))
					yakuFlag = true;
				else if ((analysis->ShunziCount[TileCode::characterSeven] >= 1) &&
					(analysis->ShunziCount[TileCode::circleSeven] >= 1) && (analysis->ShunziCount[TileCode::bambooSeven] >= 1))
					yakuFlag = true;
				return (yakuFlag && (analysis->DuiziCount[TileCode::southWind] >= 1) &&
					(analysis->DuiziCount[TileCode::westWind] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 上級断幺九 */
	if (RuleData::chkRuleApplied("high_tanyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("上級断幺九"), get_yaku_han("high_tanyao"),
			_T("断幺九"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::characterThree, TileCode::circleThree, TileCode::bambooThree,
					TileCode::characterFour, TileCode::circleFour, TileCode::bambooFour,
					TileCode::characterFive, TileCode::circleFive, TileCode::bambooFive,
					TileCode::characterSix, TileCode::circleSix, TileCode::bambooSix,
					TileCode::characterSeven, TileCode::circleSeven, TileCode::bambooSeven,
				};
				return chktiles(analysis, kezi, 15, kezi, 9, false);
			}
		));
	/* 金一色 */
	if (RuleData::chkRuleApplied("allmoney"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金一色"), get_yaku_han("allmoney"),
			_T("絶一門"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				constexpr TileCode kezi[] = {
					TileCode::circleOne, TileCode::circleTwo, TileCode::circleThree, TileCode::circleFour, TileCode::circleFive,
					TileCode::circleSix, TileCode::circleSeven, TileCode::circleEight, TileCode::circleNine, TileCode::characterOne,
				};
				return chktiles(analysis, kezi, 10, kezi, 7, false) &&
					(analysis->TileCount[TileCode::characterOne] > 0);
			}
		));
	/* 鳳子刻 */
	if (RuleData::chkRuleApplied("fengzike"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鳳子刻"), get_yaku_han("fengzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto i : NumberTileSuits)
					if ((analysis->KeziCount[composeNumberTile(i, 1)] >= 1) &&
						(analysis->ShunziCount[composeNumberTile(i, 4)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(i, 9)] >= 1)) return true;
				return false;
			}
		));
	/* 竜子刻 */
	if (RuleData::chkRuleApplied("longzike"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("竜子刻"), get_yaku_han("longzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto i : NumberTileSuits)
					if ((analysis->ShunziCount[composeNumberTile(i, 1)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(i, 5)] >= 1) &&
						(analysis->ShunziCount[composeNumberTile(i, 7)] >= 1)) return true;
				return false;
			}
		));
#endif /* GUOBIAO */
}
