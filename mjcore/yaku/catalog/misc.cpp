#include "../catalog.h"

extern GameTable GameStat;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_misc() {
	auto chktiles = // 面子手・七対子兼用の判定関数オブジェクト
		[](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetKez, int numOfKez,
		const TileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SizeOfMeldBuffer - (noDui ? 1 : 0));
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NumOfTilesInHand / 2);
			else return false;
		};
	auto chktiles_nodup = // 面子手・七対子兼用の判定関数オブジェクト
		[](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetKez, int numOfKez,
		const TileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SizeOfMeldBuffer - (noDui ? 1 : 0));
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NumOfTilesInHand / 2);
			else return false;
		};

	// ---------------------------------------------------------------------

	/* 緑一色 */
	{
		auto allgrean =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, GreenDragon};
				return chktiles(analysis, kezi, 6, kezi, 1, false);
			};
		if (RuleData::chkRuleApplied("dragonless_all_green"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("混一色"), _T("茶一色"), _T("断紅和"), _T("混断紅"),
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] > 0);
				}
			));
		else if (GameStat.chkGameType(SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("混一色"), _T("清一色"), _T("茶一色"), _T("断紅和"), _T("清断紅"),
				allgrean
			));
		else // 發なしでも良いルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("緑一色"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("混一色"), _T("茶一色"), _T("断紅和"), _T("清断紅"), _T("混断紅"), // 清一色は複合可
				allgrean
			));
		if (RuleData::chkRule("dragonless_all_green", "double_yakuman")) // 發なしがダブル役満になるルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				GameStat.chkGameType(SanmaS) ? _T("清緑一色") : _T("緑一色"), yaku::yakuCalculator::Yaku::yval_double_yakuman,
				_T("清一色"), _T("茶一色"), _T("断紅和"), _T("清断紅"),
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
				for (int i = 1; i < SizeOfMeldBuffer; i++)
					if ((analysis->MianziDat[i].mstat != meldSequenceConcealed) &&
						(analysis->MianziDat[i].mstat != meldTripletConcealed) &&
						(analysis->MianziDat[i].mstat != meldQuadConcealed)) ++count;
				return (count == SizeOfMeldBuffer - 1);
			};
		if (RuleData::chkRuleApplied("shiiaru_raotai")) {
			if (!RuleData::chkRule("shiiaru_raotai", "1han_ron_only"))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					_T("全倒鋪"), yaku::yakuCalculator::Yaku::yval_1han,
					[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
						return chkHadakaTanki(analysis) && analysis->GameStat->TsumoAgariFlag;
					}
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("全求人"), yaku::yakuCalculator::Yaku::yval_1han,
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (!analysis->GameStat->TsumoAgariFlag);
				}
			));
		}
		// 金鶏独立…一索待ち
		if (RuleData::chkRuleApplied("kinkei_dokuritsu"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("金鶏独立"), get_yaku_han("kinkei_dokuritsu"),
				_T("全倒鋪"), _T("全求人"),
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == BambooOne);
				}
			));
		// 独釣寒江雪…白待ち
		if (RuleData::chkRuleApplied("kankou_no_yuki"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("独釣寒江雪"), get_yaku_han("kankou_no_yuki"),
				_T("全倒鋪"), _T("全求人"),
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == WhiteDragon);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* 推不倒 */
	if (RuleData::chkRuleApplied("toipuutao")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("推不倒"), get_yaku_han("toipuutao"),
			_T("絶一門"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("紅孔雀"), get_yaku_han("benikujaku"),
			_T("混一色"), _T("対々和"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooOne, BambooFive, BambooSeven, BambooNine, RedDragon,
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
				const TileCode kezi[] = {
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
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, WhiteDragon,
				};
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
					(analysis->TileCount[WhiteDragon] > 0);
			};
		if (RuleData::chkRule("all_green_with_white", "2han"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("清二色"), yaku::yakuCalculator::Yaku::yval_2han,
				allgreen_white
			));
		else if (RuleData::chkRule("all_green_with_white", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("白有り緑一色"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("混一色"),
				allgreen_white
			));
	}
	/* 緑一色輪 */
	if (RuleData::chkRuleApplied("ryuuiisohrin")) {
		auto allgreen_five =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
				};
				return chktiles(analysis, kezi, 5, kezi, 1, true) &&
					(analysis->MianziDat[0].tile == BambooFive);
			};
		if (GameStat.chkGameType(SanmaS))
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
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine,
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
				const TileCode kezi[] = {
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
			_T("桃一色"), get_yaku_han("taoyise"),
			_T("混一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->shanten[shantenRegular] == -1) && (analysis->KeziCount[RedDragon] >= 1))
					yakuFlag = true;
				else if ((analysis->shanten[shantenPairs] == -1) && (analysis->TileCount[RedDragon] >= 2))
					yakuFlag = true;
				const TileCode kezi[] = {
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
			_T("茶一色"), get_yaku_han("chayise"),
			_T("混一色"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("黒一色"), get_yaku_han("heiiisoh"),
			_T("混一色"), _T("対々和"), _T("青洞門"), _T("断紅和"), _T("混断紅"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleTwo, CircleFour, CircleEight,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return chktiles(analysis, kezi, 7, nullptr, 0, false);
			}
		));
	/* 青洞門 */
	if (RuleData::chkRuleApplied("ao_no_domon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("青洞門"), get_yaku_han("ao_no_domon"),
			_T("混一色"), _T("対々和"), _T("断紅和"), _T("混断紅"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1) {
					const TileCode kezi[] = {
						EastWind, SouthWind, WestWind, NorthWind,
						CircleTwo, CircleFour, CircleEight,
						GreenDragon,
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
		auto hontanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					CircleTwo, CircleFour, CircleEight,
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 1, false);
			};
		auto chintanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					CircleTwo, CircleFour, CircleEight,
				};
				return chktiles(analysis, kezi, 8, kezi, 1, false);
			};
		if (RuleData::chkRule("no_red", "hon3han_chin6han") || RuleData::chkRule("no_red", "6han")) {
			if (!GameStat.chkGameType(SanmaS))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					_T("混断紅"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
					hontanhon
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				GameStat.chkGameType(SanmaS) ? _T("清断紅") : _T("断紅和"), yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
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
				const TileCode kezi[] = {
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
			_T("素数役満"), get_yaku_han("prime_number"),
			_T("対々和"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("白一色"), get_yaku_han("hakuiisoo"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("大四帰四"), get_yaku_han("great_four_into_four"),
			_T("重四帰四"), _T("四帰四"), _T("四帰三一"), _T("四帰三"), _T("龍四帰一"), _T("虎四帰一"), _T("両四帰一"), _T("四帰一"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TileSuitHonors; i++) {
					// 123 123 234 234: 牌式24420
					if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 2) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3))))
						return true;
					// 123 123 234 345: 牌式23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 1) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3))))
						return true;
					// 123 123 345 345: 牌式22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 1)) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3)) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 4))))
						return true;
					// 123 234 345 345: 牌式12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3))))
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
				for (int i = 1; i < TileSuitHonors; i++) {
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
			_T("四帰四"), get_yaku_han("four_into_four"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TileSuitHonors; i++) {
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
				for (int i = 1; i < TileSuitHonors; i++) {
					if (analysis->MianziDat[0].tile == (TileCode)i) {
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
				_T("四帰三一"), get_yaku_han("four_into_three_one"),
				_T("四帰三"), _T("四帰一"),
				[suukuisan](const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag = false;
					for (int i = 1; i < TileSuitHonors; i++)
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
				for (int i = 1; i < TileSuitHonors; i++)
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
			_T("虎四帰一"), get_yaku_han("tiger_four_into_one"),
			_T("両四帰一"), _T("四帰一"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++) {
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
				for (int i = 1; i < TileSuitHonors; i++) {
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

	// ---------------------------------------------------------------------

	/* 九蓮花燈 */
	if (RuleData::chkRuleApplied("chuuren_hwaton"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("九蓮花燈"), get_yaku_han("chuuren_hwaton"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i <= 9; i++)
					if (analysis->TileCount[TileSuitCharacters + i] +
						analysis->TileCount[TileSuitCircles + i] +
						analysis->TileCount[TileSuitBamboos + i] >=
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
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
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
			_T("茴香ポン "), // 対々形と区別するために後ろにスペースを入れる
			get_yaku_han("uikyou_shuntsu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++) {
					if ((analysis->KeziCount[TileSuitCharacters + i] >= 1) &&
						(analysis->KeziCount[TileSuitCircles + i] >= 1) &&
						(analysis->KeziCount[TileSuitBamboos + i] >= 1)) {
							if (i <= 5)
								for (int k = 0; k < TileSuitHonors; k += TileSuitStep)
									if ((analysis->ShunziCount[k + i + 1] >= 1) &&
										(analysis->MianziDat[0].tile == k + i + 4))
										yakuFlag = true;
							/* ここにはelseをつけてはだめ */
							if (i >= 5)
								for (int k = 0; k < TileSuitHonors; k += TileSuitStep)
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
			_T("武太郎売焼餅"), get_yaku_han("uutaaran"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterFive, CircleFive, BambooFive,
				};
				const TileCode shunzi[] = {
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
			_T("愚濫怒苦露酢"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->TsumoHai->tile == CircleFive) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			_T("清一色"),
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleFour, CircleSix,
				};
				const TileCode shunzi[] = {
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
			_T("喜鵲閙梅"), get_yaku_han("xique_naomei"),
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleFive, BambooOne,
				};
				const TileCode shunzi[] = {
					CharacterOne, CircleOne,
				};
				return chktiles_nodup(analysis, kezi, 2, shunzi, 2, true) &&
					(analysis->MianziDat[0].tile == BambooFive);
			}
		));
	/* 花一色 */
	if (RuleData::chkRuleApplied("huaiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("花一色"), get_yaku_han("huaiisoh"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterOne, CharacterTwo, CharacterThree, CharacterFour, CharacterFive,
					CharacterSix, CharacterSeven, CharacterEight, CharacterNine, RedDragon,
					CircleOne, CircleThree, CircleFive, CircleSix, CircleSeven,
					CircleNine, BambooOne, BambooFive, BambooSeven, BambooNine,
				};
				const TileCode shunzi[] = {
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
			_T("赤一色"), get_yaku_han("akaiisoh"),
			_T("花一色"), _T("絶一門"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleFive, CircleOne, CircleThree, CircleSix, CircleSeven,
					CircleNine, BambooOne, BambooFive, BambooSeven, BambooNine,
				};
				return chktiles(analysis, kezi, 10, kezi, 1, false);
			}
		));
	/* 赤眼青龍 */
	if (RuleData::chkRuleApplied("sekigan_seiryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("赤眼青龍"), get_yaku_han("sekigan_seiryuu"),
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
		const TCHAR nameList[10][16] = { _T(""),
			_T("全帯一"), _T("全帯二"), _T("全帯三"), _T("全帯四"), _T("全帯五"), _T("全帯六"), _T("全帯七"), _T("全帯八"), _T("全帯九")};
		for (int i = 1; i <= 9; i++) {
			if (RuleData::chkRuleApplied(ruleCodeList[i])) {
				auto f =
					[chktiles, i](const MENTSU_ANALYSIS* const analysis) -> bool {
						const TileCode kezi[] = {
							(TileCode)(i+TileSuitCharacters),
							(TileCode)(i+TileSuitCircles),
							(TileCode)(i+TileSuitBamboos),
						};
						const TileCode shunzi[] = {
							(TileCode)(i+TileSuitCharacters), (TileCode)(i+TileSuitCircles),
							(TileCode)(i+TileSuitBamboos),
							(TileCode)(i+TileSuitCharacters-1), (TileCode)(i+TileSuitCircles-1),
							(TileCode)(i+TileSuitBamboos-1),
							(TileCode)(i+TileSuitCharacters-2), (TileCode)(i+TileSuitCircles-2),
							(TileCode)(i+TileSuitBamboos-2),
						};
						return chktiles(analysis, kezi, 3,
							&shunzi[i <= 7 ? 0 : 3 * (i - 7)],
							i > 7 ? 3 * (10 - i) : (i < 3 ? i * 3 : 9),
							false);
					};
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
	}

	// ---------------------------------------------------------------------

	/* 小於五 */
	if (RuleData::chkRuleApplied("xiaoyuwu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("小於五"), get_yaku_han("xiaoyuwu"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("大於五"), get_yaku_han("dayuwu"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("全小"), get_yaku_han("all_small"),
			_T("小於五"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("全中"), get_yaku_han("all_middle"),
			_T("断幺九"), _T("上級断幺九"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("全大"), get_yaku_han("all_large"),
			_T("大於五"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("黒衣騎士"), get_yaku_han("blackjack"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 21;
				else if (analysis->shanten[shantenPairs] == -1)
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
		const TileCode tileCodeList[rules] = {EastWind, NorthWind, RedDragon, WhiteDragon};
		const TCHAR yakuNameList[rules][16] = {_T("東大役満"), _T("北大役満"), _T("中大役満"), _T("白皇学院")};
		for (int i = 0; i < rules; i++) {
			const TileCode targetTile = tileCodeList[i];
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
			_T("東北大役満"), get_yaku_han("univ_tohoku"),
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
			_T("名大役満"), get_yaku_han("univ_nagoya"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					for (int j = 0; j < TileSuitHonors; j += TileSuitStep)
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
			_T("冬のソナタ"), get_yaku_han("fuyusona"),
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
			_T("冬のソナタ２"), get_yaku_han("fuyusona2"),
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
			_T("上級断幺九"), get_yaku_han("high_tanyao"),
			_T("断幺九"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("金一色"), get_yaku_han("allmoney"),
			_T("絶一門"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
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
			_T("鳳子刻"), get_yaku_han("fengzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if ((analysis->KeziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 4] >= 1) &&
						(analysis->KeziCount[i + 9] >= 1)) return true;
				return false;
			}
		));
	/* 竜子刻 */
	if (RuleData::chkRuleApplied("longzike"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("竜子刻"), get_yaku_han("longzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->KeziCount[i + 5] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 1)) return true;
				return false;
			}
		));
}
