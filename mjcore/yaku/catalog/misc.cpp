#include "../catalog.h"

extern GameTable GameStat;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_misc() {
	auto chktiles = // 面子手・七対子兼用の判定関数オブジェクト
		[](const MENTSU_ANALYSIS* const analysis, const tileCode* const targetKez, int numOfKez,
		const tileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SIZE_OF_MELD_BUFFER - (noDui ? 1 : 0));
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NUM_OF_TILES_IN_HAND / 2);
			else return false;
		};
	auto chktiles_nodup = // 面子手・七対子兼用の判定関数オブジェクト
		[](const MENTSU_ANALYSIS* const analysis, const tileCode* const targetKez, int numOfKez,
		const tileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SIZE_OF_MELD_BUFFER - (noDui ? 1 : 0));
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NUM_OF_TILES_IN_HAND / 2);
			else return false;
		};

	// ---------------------------------------------------------------------

	/* 緑一色 */
	{
		auto allgrean =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, GreenDragon};
				return chktiles(analysis, kezi, 6, kezi, 1, false);
			};
		if (RuleData::chkRuleApplied("dragonless_all_green"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色", "茶一色", "断紅和", "混断紅",
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] > 0);
				}
			));
		else if (chkGameType(&GameStat, SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色", "清一色", "茶一色", "断紅和", "清断紅",
				allgrean
			));
		else // 發なしでも良いルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色", "茶一色", "断紅和", "清断紅", "混断紅", // 清一色は複合可
				allgrean
			));
		if (RuleData::chkRule("dragonless_all_green", "double_yakuman")) // 發なしがダブル役満になるルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				chkGameType(&GameStat, SanmaS) ? "清緑一色" : "緑一色", yaku::yakuCalculator::Yaku::yval_double_yakuman,
				"清一色", "茶一色", "断紅和", "清断紅",
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] == 0);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* 裸単騎１飜というローカル役 */
	{
		auto chkHadakaTanki =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++)
					if ((analysis->MianziDat[i].mstat != meldSequenceConcealed) &&
						(analysis->MianziDat[i].mstat != meldTripletConcealed) &&
						(analysis->MianziDat[i].mstat != meldQuadConcealed)) ++count;
				return (count == SIZE_OF_MELD_BUFFER - 1);
			};
		if (RuleData::chkRuleApplied("shiiaru_raotai")) {
			if (!RuleData::chkRule("shiiaru_raotai", "1han_ron_only"))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					"全倒鋪", yaku::yakuCalculator::Yaku::yval_1han,
					[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
						return chkHadakaTanki(analysis) && analysis->GameStat->TsumoAgariFlag;
					}
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"全求人", yaku::yakuCalculator::Yaku::yval_1han,
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (!analysis->GameStat->TsumoAgariFlag);
				}
			));
		}
		// 金鶏独立…一索待ち
		if (RuleData::chkRuleApplied("kinkei_dokuritsu"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"金鶏独立", get_yaku_han("kinkei_dokuritsu"),
				"全倒鋪", "全求人",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == BambooOne);
				}
			));
		// 独釣寒江雪…白待ち
		if (RuleData::chkRuleApplied("kankou_no_yuki"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"独釣寒江雪", get_yaku_han("kankou_no_yuki"),
				"全倒鋪", "全求人",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == WhiteDragon);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* 推不倒 */
	if (RuleData::chkRuleApplied("toipuutao")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"推不倒", get_yaku_han("toipuutao"),
			"絶一門",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleOne, CircleTwo, CircleThree, BambooFour, CircleFour, CircleFive, CircleEight,
					CircleNine, BambooTwo, BambooFive, BambooSix, BambooEight, BambooNine, WhiteDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 4, false);
			}
		));
	}
	/* 紅孔雀 */
	if (RuleData::chkRuleApplied("benikujaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紅孔雀", get_yaku_han("benikujaku"),
			"混一色", "対々和",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooOne, BambooFive, BambooSeven, BambooNine, RedDragon,
				};
				return chktiles(analysis, kezi, 5, nullptr, 0, false);
			}
		));
	/* 萬緑叢中一点紅 */
	if (RuleData::chkRuleApplied("kouitten"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紅一点", get_yaku_han("kouitten"),
			"混一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, RedDragon,
				};
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
					(analysis->TileCount[RedDragon] > 0);
			}
		));
	/* 白有り緑一色 */
	if (RuleData::chkRuleApplied("all_green_with_white")) {
		auto allgreen_white =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, WhiteDragon,
				};
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
					(analysis->TileCount[WhiteDragon] > 0);
			};
		if (RuleData::chkRule("all_green_with_white", "2han"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"清二色", yaku::yakuCalculator::Yaku::yval_2han,
				allgreen_white
			));
		else if (RuleData::chkRule("all_green_with_white", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"白有り緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色",
				allgreen_white
			));
	}
	/* 緑一色輪 */
	if (RuleData::chkRuleApplied("ryuuiisohrin")) {
		auto allgreen_five =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
				};
				return chktiles(analysis, kezi, 5, kezi, 1, true) &&
					(analysis->MianziDat[0].tile == BambooFive);
			};
		if (chkGameType(&GameStat, SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色輪", get_yaku_han("ryuuiisohrin"),
				"清一色",
				allgreen_five
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色輪", get_yaku_han("ryuuiisohrin"),
				allgreen_five
			));
	}
	/* 大草原 */
	if (RuleData::chkRuleApplied("daisougen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大草原", get_yaku_han("daisougen"),
			"清一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine,
				};
				return chktiles(analysis, kezi, 7, kezi, 4, false);
			}
		));
	/* 南大草原 */
	if (RuleData::chkRuleApplied("south_daisougen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"南大草原", get_yaku_han("south_daisougen"),
			"混一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine, SouthWind,
				};
				return chktiles(analysis, kezi, 8, kezi, 4, false) &&
					(analysis->TileCount[SouthWind] > 0);
			}
		));
	/* 桃一色 */
	if (RuleData::chkRuleApplied("taoyise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"桃一色", get_yaku_han("taoyise"),
			"混一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) && (analysis->KeziCount[RedDragon] >= 1))
					yakuFlag = true;
				else if ((analysis->shanten[ShantenAnalyzer::shantenPairs] == -1) && (analysis->TileCount[RedDragon] >= 2))
					yakuFlag = true;
				const tileCode kezi[] = {
					CircleFive, CircleOne, CircleThree, CircleSix,
					CircleSeven, CircleNine, RedDragon,
				};
				return chktiles(analysis, kezi, 7, kezi, 1, false) &&
					((countRedTilesInHand(analysis->GameStat, analysis->player, AkaDora))[CircleFive] > 0) &&
					yakuFlag;
			}
		));
	/* 茶一色 */
	if (RuleData::chkRuleApplied("chayise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"茶一色", get_yaku_han("chayise"),
			"混一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon,
				};
				return chktiles(analysis, kezi, 11, kezi, 1, false);
			}
		));
	/* 黒一色 */
	if (RuleData::chkRuleApplied("heiiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"黒一色", get_yaku_han("heiiisoh"),
			"混一色", "対々和", "青洞門", "断紅和", "混断紅",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleTwo, CircleFour, CircleEight,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return chktiles(analysis, kezi, 7, nullptr, 0, false);
			}
		));
	/* 青洞門 */
	if (RuleData::chkRuleApplied("ao_no_domon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"青洞門", get_yaku_han("ao_no_domon"),
			"混一色", "対々和", "断紅和", "混断紅",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) {
					const tileCode kezi[] = {
						EastWind, SouthWind, WestWind, NorthWind,
						CircleTwo, CircleFour, CircleEight,
						GreenDragon,
					};
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 8, nullptr, 0, false) == SIZE_OF_MELD_BUFFER) &&
						(yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 4, nullptr, 0, false) == 1);
				}
				else return false;
			}
		));
	/* 断紅和 */
	if (RuleData::chkRuleApplied("no_red")) {
		auto hontanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					CircleTwo, CircleFour, CircleEight,
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 1, false);
			};
		auto chintanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					CircleTwo, CircleFour, CircleEight,
				};
				return chktiles(analysis, kezi, 8, kezi, 1, false);
			};
		if (RuleData::chkRule("no_red", "hon3han_chin6han") || RuleData::chkRule("no_red", "6han")) {
			if (!chkGameType(&GameStat, SanmaS))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					"混断紅", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
					hontanhon
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				chkGameType(&GameStat, SanmaS) ? "清断紅" : "断紅和", yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
				"混断紅",
				chintanhon
			));
		} else {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"断紅和", get_yaku_han("no_red"),
				hontanhon
			));
		}
	}
	/* 紅白饅頭 */
	if (RuleData::chkRuleApplied("kohaku_manju"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紅白饅頭", get_yaku_han("kohaku_manju"),
			"紅白七対",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterOne, CharacterTwo, CharacterThree, CharacterFour,
					CharacterFive, CharacterSix, CharacterSeven, CharacterEight,
					CharacterNine, WhiteDragon, RedDragon,
				};
				return chktiles(analysis, kezi, 11, kezi, 7, false) &&
					(analysis->TileCount[WhiteDragon] > 0) &&
					(analysis->TileCount[RedDragon] > 0);
			}
		));
	/* 素数役満 (triplet2.cppより移動) */
	if (RuleData::chkRuleApplied("prime_number"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"素数役満", get_yaku_han("prime_number"),
			"対々和",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterTwo, CharacterThree, CharacterFive, CharacterSeven,
					CircleTwo, CircleThree, CircleFive, CircleSeven,
					BambooTwo, BambooThree, BambooFive, BambooSeven,
				};
				return chktiles(analysis, kezi, 12, nullptr, 0, false);
			}
		));
	/* 白一色 (triplet2.cppより移動) */
	if (RuleData::chkRuleApplied("hakuiisoo"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白一色", get_yaku_han("hakuiisoo"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterEight, CharacterNine, CircleEight, CircleNine,
					BambooEight, BambooNine, WhiteDragon,
				};
				return chktiles(analysis, kezi, 7, nullptr, 0, false);
			}
		));

	// ---------------------------------------------------------------------

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
			"大四帰四", get_yaku_han("great_four_into_four"),
			"重四帰四", "四帰四", "四帰三一", "四帰三", "龍四帰一", "虎四帰一", "両四帰一", "四帰一",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					// 123 123 234 234: 牌式24420
					if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 2) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3))))
						return true;
					// 123 123 234 345: 牌式23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 1) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3))))
						return true;
					// 123 123 345 345: 牌式22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 1)) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3)) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 4))))
						return true;
					// 123 234 345 345: 牌式12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3))))
						return true;
				}
				return false;
			}
		));
	/* 重四帰四 */
	if (RuleData::chkRuleApplied("double_four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"重四帰四", get_yaku_han("double_four_into_four"),
			"四帰四", "四帰三一", "四帰三", "龍四帰一", "虎四帰一", "両四帰一", "四帰一",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					// 123 123 123 234: 牌式34410
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[i+1] >= 1))
						return true;
					// 123 123 234 234: 牌式24420
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 2))
						return true;
					// 123 234 234 234: 牌式14430
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 3))
						return true;
				}
				return false;
			}
		));
	/* 四帰四 */
	if (RuleData::chkRuleApplied("four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四帰四", get_yaku_han("four_into_four"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					// 123 123 123 345: 牌式33411
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 123 234 345: 牌式23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 123 345 345: 牌式22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+2] >= 2))
						return true;
					// 123 234 234 345: 牌式13431
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 2) &&
						(analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 234 345 345: 牌式12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 2))
						return true;
					// 123 345 345 345: 牌式11433
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+2] >= 3))
						return true;
				}
				return false;
			}
		));

	// ---------------------------------------------------------------------

	{
		auto suukuisan =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					if (analysis->MianziDat[0].tile == (tileCode)i) {
						if (analysis->ShunziCount[i] >= 2)
							return true;
						else if (i < 1) continue;
						else if ((analysis->ShunziCount[i-1] >= 1) && (analysis->ShunziCount[i] >= 1))
							return true;
						else if (analysis->ShunziCount[i-1] >= 2)
							return true;
						else if (i < 2) continue;
						else if ((analysis->ShunziCount[i-2] >= 1) && (analysis->ShunziCount[i] >= 1))
							return true;
						else if ((analysis->ShunziCount[i-2] >= 1) && (analysis->ShunziCount[i-1] >= 1))
							return true;
						else if (analysis->ShunziCount[i-2] >= 2)
							return true;
					}
				}
				return false;
			};
		/* 四帰三一 */
		if (RuleData::chkRuleApplied("four_into_three_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"四帰三一", get_yaku_han("four_into_three_one"),
				"四帰三", "四帰一",
				[suukuisan](const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag = false;
					for (int i = 1; i < TILE_SUIT_HONORS; i++)
						if ((analysis->ShunziCount[i] >= 1) &&
							((analysis->KeziCount[i] >= 1) ||
							(analysis->KeziCount[i+1] >= 1) ||
							(analysis->KeziCount[i+2] >= 1))) yakuFlag = true;
					return suukuisan(analysis) && yakuFlag;
				}
			));
		/* 四帰三 */
		if (RuleData::chkRuleApplied("four_into_three"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"四帰三", get_yaku_han("four_into_three"),
				"四帰一",
				suukuisan
			));
	}

	// ---------------------------------------------------------------------

	/* 龍四帰一 */
	if (RuleData::chkRuleApplied("dragon_four_into_one"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"龍四帰一", get_yaku_han("dragon_four_into_one"),
			"三連刻", "虎四帰一", "両四帰一", "四帰一",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if ((analysis->ShunziCount[i] >= 1) &&
						(analysis->KeziCount[i] >= 1) &&
						(analysis->KeziCount[i+1] >= 1) &&
						(analysis->KeziCount[i+2] >= 1)) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 虎四帰一 */
	if (RuleData::chkRuleApplied("tiger_four_into_one"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"虎四帰一", get_yaku_han("tiger_four_into_one"),
			"両四帰一", "四帰一",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					if (analysis->ShunziCount[i] >= 1) {
						if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[i+1] >= 1)) yakuFlag = true;
						else if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[i+2] >= 1)) yakuFlag = true;
						else if ((analysis->KeziCount[i+1] >= 1) && (analysis->KeziCount[i+2] >= 1)) yakuFlag = true;
					}
				}
				return yakuFlag;
			}
		));
	{
		auto suukuiyii =
			[](const MENTSU_ANALYSIS* const analysis) -> int {
				int count = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					if (analysis->ShunziCount[i] >= 1) {
						if (analysis->KeziCount[i] >= 1) ++count;
						else if (analysis->KeziCount[i+1] >= 1) ++count;
						else if (analysis->KeziCount[i+2] >= 1) ++count;
					}
				}
				return count;
			};
		/* 両四帰一 */
		if (RuleData::chkRuleApplied("double_four_into_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"両四帰一", get_yaku_han("double_four_into_one"),
				"四帰一",
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (suukuiyii(analysis) == 2);
				}
			));
		/* 四帰一 */
		if (RuleData::chkRuleApplied("four_into_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"四帰一", get_yaku_han("four_into_one"),
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (suukuiyii(analysis) == 1);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* 九蓮花燈 */
	if (RuleData::chkRuleApplied("chuuren_hwaton"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"九蓮花燈", get_yaku_han("chuuren_hwaton"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i <= 9; i++)
					if (analysis->TileCount[TILE_SUIT_CHARACTERS + i] +
						analysis->TileCount[TILE_SUIT_CIRCLES + i] +
						analysis->TileCount[TILE_SUIT_BAMBOOS + i] >=
						(((i == 1)||(i == 9)) ? 3 : 1) )
						++count;
				return (count == 9);
			}
		));
	/* 鳴き九連宝燈 */
	if (RuleData::chkRuleApplied("exposed_chuuren"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"九蓮碧燈", get_yaku_han("exposed_chuuren"),
			/* 鳴きチンイツが複合し倍満以上が確定 */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				Int8ByTile tiles = analysis->TileCount; bool yakuFlag = false;
				// 鳴き面子も計算 槓子は無視
				for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) {
					switch (analysis->MianziDat[i].mstat) {
					case meldSequenceExposedLower: case meldSequenceExposedMiddle:
					case meldSequenceExposedUpper:
						++tiles[analysis->MianziDat[i].tile];
						++tiles[analysis->MianziDat[i].tile + 1];
						++tiles[analysis->MianziDat[i].tile + 2];
						break;
					case meldTripletExposedLeft: case meldTripletExposedCenter:
					case meldTripletExposedRight:
						tiles[analysis->MianziDat[i].tile] += 3;
						break;
					}
				}
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((tiles[i + 1] >= 3) && (tiles[i + 2] >= 1) && (tiles[i + 3] >= 1) &&
						(tiles[i + 4] >= 1) && (tiles[i + 5] >= 1) && (tiles[i + 6] >= 1) &&
						(tiles[i + 7] >= 1) && (tiles[i + 8] >= 1) && (tiles[i + 9] >= 3))
						yakuFlag = true;
				return yakuFlag && (!(*analysis->MenzenFlag));
			}
		));

	// ---------------------------------------------------------------------

	/* 茴香ポン(非対々) */
	if (RuleData::chkRuleApplied("uikyou_shuntsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"茴香ポン ", // 対々形と区別するために後ろにスペースを入れる
			get_yaku_han("uikyou_shuntsu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++) {
					if ((analysis->KeziCount[TILE_SUIT_CHARACTERS + i] >= 1) &&
						(analysis->KeziCount[TILE_SUIT_CIRCLES + i] >= 1) &&
						(analysis->KeziCount[TILE_SUIT_BAMBOOS + i] >= 1)) {
							if (i <= 5)
								for (int k = 0; k < TILE_SUIT_HONORS; k += TILE_SUIT_STEP)
									if ((analysis->ShunziCount[k + i + 1] >= 1) &&
										(analysis->MianziDat[0].tile == k + i + 4))
										yakuFlag = true;
							/* ここにはelseをつけてはだめ */
							if (i >= 5)
								for (int k = 0; k < TILE_SUIT_HONORS; k += TILE_SUIT_STEP)
									if ((analysis->ShunziCount[k + i - 3] >= 1) &&
										(analysis->MianziDat[0].tile == k + i - 4))
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
			"武太郎売焼餅", get_yaku_han("uutaaran"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterFive, CircleFive, BambooFive,
				};
				const tileCode shunzi[] = {
					CharacterThree, CharacterFour, CharacterFive,
					CircleThree, CircleFour, CircleFive,
					BambooThree, BambooFour, BambooFive,
				};
				return chktiles(analysis, kezi, 3, shunzi, 9, true) &&
					(analysis->MianziDat[0].tile == CircleOne);
			}
		));
	/* 愚濫怒苦露酢 */
	if (RuleData::chkRuleApplied("grand_cross"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"愚濫怒苦露酢", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->TsumoHai->tile == CircleFive) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"清一色",
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleFour, CircleSix,
				};
				const tileCode shunzi[] = {
					CircleOne, CircleSeven,
				};
				return chktiles_nodup(analysis, kezi, 2, shunzi, 2, true) &&
					(analysis->MianziDat[0].tile == CircleFive) &&
					(*analysis->MenzenFlag);
			}
		));
	/* 喜鵲閙梅 */
	if (RuleData::chkRuleApplied("xique_naomei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"喜鵲閙梅", get_yaku_han("xique_naomei"),
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleFive, BambooOne,
				};
				const tileCode shunzi[] = {
					CharacterOne, CircleOne,
				};
				return chktiles_nodup(analysis, kezi, 2, shunzi, 2, true) &&
					(analysis->MianziDat[0].tile == BambooFive);
			}
		));
	/* 花一色 */
	if (RuleData::chkRuleApplied("huaiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"花一色", get_yaku_han("huaiisoh"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterOne, CharacterTwo, CharacterThree, CharacterFour, CharacterFive,
					CharacterSix, CharacterSeven, CharacterEight, CharacterNine, RedDragon,
					CircleOne, CircleThree, CircleFive, CircleSix, CircleSeven,
					CircleNine, BambooOne, BambooFive, BambooSeven, BambooNine,
				};
				const tileCode shunzi[] = {
					CharacterOne, CharacterTwo, CharacterThree, CharacterFour, CharacterFive,
					CharacterSix, CharacterSeven, CircleFive,
				};
				return (chktiles(analysis, kezi, 20, shunzi, 8, false)) &&
					(!chktiles(analysis, kezi, 10, shunzi, 7, false));
			}
		));
	/* 赤一色 */
	if (RuleData::chkRuleApplied("akaiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"赤一色", get_yaku_han("akaiisoh"),
			"花一色", "絶一門",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleFive, CircleOne, CircleThree, CircleSix, CircleSeven,
					CircleNine, BambooOne, BambooFive, BambooSeven, BambooNine,
				};
				return chktiles(analysis, kezi, 10, kezi, 1, false);
			}
		));
	/* 赤眼青龍 */
	if (RuleData::chkRuleApplied("sekigan_seiryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"赤眼青龍", get_yaku_han("sekigan_seiryuu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->ShunziCount[CircleTwo] == 1) &&
					(analysis->KeziCount[WhiteDragon] + analysis->KeziCount[EastWind] == 1) &&
					(analysis->DuiziCount[CircleTwo] + analysis->DuiziCount[CircleFour] +
					analysis->DuiziCount[CircleEight] >= 3));
			}
		));

	// ---------------------------------------------------------------------

	/* 満園花 */
	{
		const char ruleCodeList[10][16] = { "",
			"all_one", "all_two", "all_three", "all_four", "all_five",
			"all_six", "all_seven", "all_eight", "all_nine"};
		const char nameList[10][16] = { "",
			"全帯一", "全帯二", "全帯三", "全帯四", "全帯五", "全帯六", "全帯七", "全帯八", "全帯九"};
		for (int i = 1; i <= 9; i++) {
			if (RuleData::chkRuleApplied(ruleCodeList[i])) {
				auto f =
					[chktiles, i](const MENTSU_ANALYSIS* const analysis) -> bool {
						const tileCode kezi[] = {
							(tileCode)(i+TILE_SUIT_CHARACTERS),
							(tileCode)(i+TILE_SUIT_CIRCLES),
							(tileCode)(i+TILE_SUIT_BAMBOOS),
						};
						const tileCode shunzi[] = {
							(tileCode)(i+TILE_SUIT_CHARACTERS), (tileCode)(i+TILE_SUIT_CIRCLES),
							(tileCode)(i+TILE_SUIT_BAMBOOS),
							(tileCode)(i+TILE_SUIT_CHARACTERS-1), (tileCode)(i+TILE_SUIT_CIRCLES-1),
							(tileCode)(i+TILE_SUIT_BAMBOOS-1),
							(tileCode)(i+TILE_SUIT_CHARACTERS-2), (tileCode)(i+TILE_SUIT_CIRCLES-2),
							(tileCode)(i+TILE_SUIT_BAMBOOS-2),
						};
						return chktiles(analysis, kezi, 3,
							&shunzi[i <= 7 ? 0 : 3 * (i - 7)],
							i > 7 ? 3 * (10 - i) : (i < 3 ? i * 3 : 9),
							false);
					};
				switch (i) {
				case 4: case 5: case 6:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), "断幺九", f));
					break;
				case 1: case 9:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), "純全帯幺九", f));
					break;
				default:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), f));
					break;
				}
			}
		}
	}

	// ---------------------------------------------------------------------

	/* 小於五 */
	if (RuleData::chkRuleApplied("xiaoyuwu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"小於五", get_yaku_han("xiaoyuwu"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterOne, CircleOne, BambooOne,
					CharacterTwo, CircleTwo, BambooTwo,
					CharacterThree, CircleThree, BambooThree,
					CharacterFour, CircleFour, BambooFour,
				};
				return chktiles(analysis, kezi, 12, kezi, 6, false);
			}
		));
	/* 大於五 */
	if (RuleData::chkRuleApplied("dayuwu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大於五", get_yaku_han("dayuwu"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterSix, CircleSix, BambooSix,
					CharacterSeven, CircleSeven, BambooSeven,
					CharacterEight, CircleEight, BambooEight,
					CharacterNine, CircleNine, BambooNine,
				};
				return chktiles(analysis, kezi, 12, kezi, 6, false);
			}
		));
	/* 全小 */
	if (RuleData::chkRuleApplied("all_small"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"全小", get_yaku_han("all_small"),
			"小於五",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterOne, CircleOne, BambooOne,
					CharacterTwo, CircleTwo, BambooTwo,
					CharacterThree, CircleThree, BambooThree,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));
	/* 全中 */
	if (RuleData::chkRuleApplied("all_middle"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"全中", get_yaku_han("all_middle"),
			"断幺九", "上級断幺九",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterFour, CircleFour, BambooFour,
					CharacterFive, CircleFive, BambooFive,
					CharacterSix, CircleSix, BambooSix,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));
	/* 全大 */
	if (RuleData::chkRuleApplied("all_large"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"全大", get_yaku_han("all_large"),
			"大於五",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterSeven, CircleSeven, BambooSeven,
					CharacterEight, CircleEight, BambooEight,
					CharacterNine, CircleNine, BambooNine,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 黒衣騎士 */
	if (RuleData::chkRuleApplied("blackjack"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"黒衣騎士", get_yaku_han("blackjack"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					return yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 21;
				else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
					return yaku::countingFacility::countTileNumerals(analysis->TileCount) == 21;
				else return false;
			}
		));
	/* 白衣騎士 */
	if (RuleData::chkRuleApplied("white_knight"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白衣騎士", get_yaku_han("white_knight"),
			"黒衣騎士",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->KeziCount[WhiteDragon] >= 1)
					return yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 21;
				else return false;
			}
		));

	// ---------------------------------------------------------------------

	/* 東大系 */
	{
		const int rules = 4;
		const char ruleCodeList[rules][16] = {"univ_tokyo", "univ_hokkaido", "univ_chuo", "univ_hakuoh"};
		const tileCode tileCodeList[rules] = {EastWind, NorthWind, RedDragon, WhiteDragon};
		const char yakuNameList[rules][16] = {"東大役満", "北大役満", "中大役満", "白皇学園"};
		for (int i = 0; i < rules; i++) {
			const tileCode targetTile = tileCodeList[i];
			if (RuleData::chkRuleApplied(ruleCodeList[i]))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					yakuNameList[i], get_yaku_han(ruleCodeList[i]),
					[targetTile](const MENTSU_ANALYSIS* const analysis) -> bool {
						return ((analysis->KangziCount[targetTile] >= 1) &&
							(analysis->Machi == yaku::yakuCalculator::machiRyanmen) &&
							(analysis->TotalKezi == 1) && (*analysis->MenzenFlag));
					}
				));
		}
	}
	/* 東北大 */
	if (RuleData::chkRuleApplied("univ_tohoku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東北大役満", get_yaku_han("univ_tohoku"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiRyanmen) &&
					(analysis->TotalKezi == 2) && (*analysis->MenzenFlag));
			}
		));
	/* 名大役満 */
	if (RuleData::chkRuleApplied("univ_nagoya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"名大役満", get_yaku_han("univ_nagoya"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int j = 0; j < TILE_SUIT_HONORS; j += TILE_SUIT_STEP)
						if ((analysis->KangziCount[i + 5] >= 1) &&
							(analysis->KangziCount[j + 8] >= 1))
							yakuFlag = true;
				return ((yakuFlag) &&
					(analysis->Machi == yaku::yakuCalculator::machiRyanmen) &&
					(analysis->TotalKezi == 2) && (*analysis->MenzenFlag));
			}
		));

	// ---------------------------------------------------------------------

	/* 冬のソナタ */
	if (RuleData::chkRuleApplied("fuyusona"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"冬のソナタ", get_yaku_han("fuyusona"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[CharacterOne] >= 1) &&
					(analysis->ShunziCount[CircleOne] >= 1) && (analysis->ShunziCount[BambooOne] >= 1))
					yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterSeven] >= 1) &&
					(analysis->ShunziCount[CircleSeven] >= 1) && (analysis->ShunziCount[BambooSeven] >= 1))
					yakuFlag = true;
				return (yakuFlag && (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->MianziDat[0].tile == NorthWind));
			}
		));
	/* 冬のソナタ２ */
	if (RuleData::chkRuleApplied("fuyusona2"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"冬のソナタ２", get_yaku_han("fuyusona2"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[CharacterOne] >= 1) && (analysis->ShunziCount[CircleOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterOne] >= 1) && (analysis->ShunziCount[BambooOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CircleOne] >= 1) && (analysis->ShunziCount[BambooOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterSeven] >= 1) && (analysis->ShunziCount[CircleSeven] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterSeven] >= 1) && (analysis->ShunziCount[BambooSeven] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CircleSeven] >= 1) && (analysis->ShunziCount[BambooSeven] >= 1)) yakuFlag = true;
				return (yakuFlag &&
					(analysis->KeziCount[CharacterTwo] + analysis->KeziCount[CircleTwo] + analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->MianziDat[0].tile == NorthWind));
			}
		));

	// ---------------------------------------------------------------------

	/* 上級断幺九 */
	if (RuleData::chkRuleApplied("high_tanyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"上級断幺九", get_yaku_han("high_tanyao"),
			"断幺九",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterThree, CircleThree, BambooThree,
					CharacterFour, CircleFour, BambooFour,
					CharacterFive, CircleFive, BambooFive,
					CharacterSix, CircleSix, BambooSix,
					CharacterSeven, CircleSeven, BambooSeven,
				};
				return chktiles(analysis, kezi, 15, kezi, 9, false);
			}
		));
	/* 金一色 */
	if (RuleData::chkRuleApplied("allmoney"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金一色", get_yaku_han("allmoney"),
			"絶一門",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleOne, CircleTwo, CircleThree, CircleFour, CircleFive,
					CircleSix, CircleSeven, CircleEight, CircleNine, CharacterOne,
				};
				return chktiles(analysis, kezi, 10, kezi, 7, false) &&
					(analysis->TileCount[CharacterOne] > 0);
			}
		));
	/* 鳳子刻 */
	if (RuleData::chkRuleApplied("fengzike"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鳳子刻", get_yaku_han("fengzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 4] >= 1) &&
						(analysis->KeziCount[i + 9] >= 1)) return true;
				return false;
			}
		));
	/* 竜子刻 */
	if (RuleData::chkRuleApplied("longzike"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"竜子刻", get_yaku_han("longzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->KeziCount[i + 5] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 1)) return true;
				return false;
			}
		));
}
