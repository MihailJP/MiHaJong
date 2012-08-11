#include "../catalog.h"

extern GameTable GameStat;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_misc() {
	auto chktiles = // 面子手・七対子兼用の判定関数オブジェクト
		[](const MENTSU_ANALYSIS* const analysis, const tileCode* const targetKez, int numOfKez,
		const tileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[shantenPairs] == -1)
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
				const tileCode shunzi[] = {BambooTwo};
				return chktiles(analysis, kezi, 6, shunzi, 1, false);
			};
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色",
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] > 0);
				}
			));
		else if (chkGameType(&GameStat, SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色", "清一色",
				allgrean
			));
		else // 發なしでも良いルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色", // 清一色は複合可
				allgrean
			));
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) == 2) // 發なしがダブル役満になるルール
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				chkGameType(&GameStat, SanmaS) ? "清緑一色" : "緑一色", yaku::yakuCalculator::Yaku::yval_double_yakuman,
				"清一色",
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
		if (getRule(RULE_SHIIARU_RAOTAI) != 0) {
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
		if (getRule(RULE_KINKEI_DOKURITSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"金鶏独立", yaku::yakuCalculator::Yaku::yval_yakuman,
				"全倒鋪", "全求人",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == BambooOne);
				}
			));
		// 独釣寒江雪…白待ち
		if (getRule(RULE_KANKOU_NO_YUKI) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"独釣寒江雪", yaku::yakuCalculator::Yaku::yval_yakuman,
				"全倒鋪", "全求人",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == WhiteDragon);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* 推不倒 */
	if (getRule(RULE_TOIPUUTAO) != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC han;
		switch (getRule(RULE_TOIPUUTAO)) {
			case 1: han = yaku::yakuCalculator::Yaku::yval_1han; break;
			case 2: han = yaku::yakuCalculator::Yaku::yval_4han; break;
			case 3: han = yaku::yakuCalculator::Yaku::yval_yakuman; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"推不倒", han,
			"絶一門",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleOne, CircleTwo, CircleThree, CircleFour, CircleFive, CircleEight, CircleNine,
					BambooTwo, BambooFour, BambooFive, BambooSix, BambooEight, BambooNine, WhiteDragon,
				};
				const tileCode shunzi[] = {
					CircleOne, CircleTwo, CircleThree, BambooFour,
				};
				return chktiles(analysis, kezi, 14, shunzi, 4, false);
			}
		));
	}
	/* 紅孔雀 */
	if (getRule(RULE_BENIKUJAKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紅孔雀", yaku::yakuCalculator::Yaku::yval_yakuman,
			"混一色", "対々和",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooOne, BambooFive, BambooSeven, BambooNine, RedDragon,
				};
				return chktiles(analysis, kezi, 5, NULL, 0, false);
			}
		));
	/* 萬緑叢中一点紅 */
	if (getRule(RULE_KOUITTEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紅一点", yaku::yakuCalculator::Yaku::yval_yakuman,
			"混一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, RedDragon,
				};
				const tileCode shunzi[] = {
					BambooTwo,
				};
				return chktiles(analysis, kezi, 6, shunzi, 1, false) &&
					(analysis->TileCount[RedDragon] > 0);
			}
		));
	/* 白有り緑一色 */
	if (getRule(RULE_ALL_GREEN_WITH_WHITE) != 0) {
		auto allgreen_white =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, WhiteDragon,
				};
				const tileCode shunzi[] = {
					BambooTwo,
				};
				return chktiles(analysis, kezi, 6, shunzi, 1, false) &&
					(analysis->TileCount[WhiteDragon] > 0);
			};
		if (getRule(RULE_ALL_GREEN_WITH_WHITE) == 1)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"清二色", yaku::yakuCalculator::Yaku::yval_2han,
				allgreen_white
			));
		else if (getRule(RULE_ALL_GREEN_WITH_WHITE) == 2)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"白有り緑一色", yaku::yakuCalculator::Yaku::yval_yakuman,
				"混一色",
				allgreen_white
			));
	}
	/* 緑一色輪 */
	if (getRule(RULE_RYUUIISOHRIN) != 0) {
		auto allgreen_five =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive, BambooSix, BambooEight,
				};
				const tileCode shunzi[] = {
					BambooTwo,
				};
				return chktiles(analysis, kezi, 6, shunzi, 1, false) &&
					(analysis->MianziDat[0].tile == BambooFive);
			};
		if (chkGameType(&GameStat, SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色輪", yaku::yakuCalculator::Yaku::yval_yakuman,
				"清一色",
				allgreen_five
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"緑一色輪", yaku::yakuCalculator::Yaku::yval_yakuman,
				allgreen_five
			));
	}
	/* 大草原 */
	if (getRule(RULE_DAISOUGEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大草原", yaku::yakuCalculator::Yaku::yval_yakuman,
			"清一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine,
				};
				const tileCode shunzi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
				};
				return chktiles(analysis, kezi, 7, shunzi, 4, false);
			}
		));
	/* 南大草原 */
	if (getRule(RULE_SOUTH_DAISOUGEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"南大草原", yaku::yakuCalculator::Yaku::yval_yakuman,
			"混一色",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine, SouthWind,
				};
				const tileCode shunzi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
				};
				return chktiles(analysis, kezi, 7, shunzi, 4, false) &&
					(analysis->TileCount[SouthWind] > 0);
			}
		));
}
