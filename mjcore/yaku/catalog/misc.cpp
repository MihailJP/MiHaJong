#include "../catalog.h"

extern GameTable GameStat;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_misc() {
	auto chktiles = // –ÊŽqŽèEŽµ‘ÎŽqŒ“—p‚Ì”»’èŠÖ”ƒIƒuƒWƒFƒNƒg
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

	/* —ÎˆêF */
	{
		auto allgrean =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, GreenDragon};
				const tileCode shunzi[] = {BambooTwo};
				return chktiles(analysis, kezi, 6, shunzi, 1, false);
			};
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"—ÎˆêF", yaku::yakuCalculator::Yaku::yval_yakuman,
				"¬ˆêF", "’ƒˆêF", "’fg˜a", "¬’fg",
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] > 0);
				}
			));
		else if (chkGameType(&GameStat, SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"—ÎˆêF", yaku::yakuCalculator::Yaku::yval_yakuman,
				"¬ˆêF", "´ˆêF", "’ƒˆêF", "’fg˜a", "´’fg",
				allgrean
			));
		else // á¢‚È‚µ‚Å‚à—Ç‚¢ƒ‹[ƒ‹
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"—ÎˆêF", yaku::yakuCalculator::Yaku::yval_yakuman,
				"¬ˆêF", "’ƒˆêF", "’fg˜a", "´’fg", "¬’fg", // ´ˆêF‚Í•¡‡‰Â
				allgrean
			));
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) == 2) // á¢‚È‚µ‚ªƒ_ƒuƒ‹–ð–ž‚É‚È‚éƒ‹[ƒ‹
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				chkGameType(&GameStat, SanmaS) ? "´—ÎˆêF" : "—ÎˆêF", yaku::yakuCalculator::Yaku::yval_double_yakuman,
				"´ˆêF", "’ƒˆêF", "’fg˜a", "´’fg",
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] == 0);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* —‡’P‹R‚PãÊ‚Æ‚¢‚¤ƒ[ƒJƒ‹–ð */
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
				"‘S“|•Ý", yaku::yakuCalculator::Yaku::yval_1han,
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && analysis->GameStat->TsumoAgariFlag;
				}
			));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‘S‹l", yaku::yakuCalculator::Yaku::yval_1han,
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (!analysis->GameStat->TsumoAgariFlag);
				}
			));
		}
		// ‹àŒ{“Æ—§cˆêõ‘Ò‚¿
		if (getRule(RULE_KINKEI_DOKURITSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‹àŒ{“Æ—§", yaku::yakuCalculator::Yaku::yval_yakuman,
				"‘S“|•Ý", "‘S‹l",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == BambooOne);
				}
			));
		// “Æ’ÞŠ¦]ác”’‘Ò‚¿
		if (getRule(RULE_KANKOU_NO_YUKI) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"“Æ’ÞŠ¦]á", yaku::yakuCalculator::Yaku::yval_yakuman,
				"‘S“|•Ý", "‘S‹l",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == WhiteDragon);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* „•s“| */
	if (getRule(RULE_TOIPUUTAO) != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC han;
		switch (getRule(RULE_TOIPUUTAO)) {
			case 1: han = yaku::yakuCalculator::Yaku::yval_1han; break;
			case 2: han = yaku::yakuCalculator::Yaku::yval_4han; break;
			case 3: han = yaku::yakuCalculator::Yaku::yval_yakuman; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"„•s“|", han,
			"âˆê–å",
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
	/* gE */
	if (getRule(RULE_BENIKUJAKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"gE", yaku::yakuCalculator::Yaku::yval_yakuman,
			"¬ˆêF", "‘ÎX˜a",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooOne, BambooFive, BambooSeven, BambooNine, RedDragon,
				};
				return chktiles(analysis, kezi, 5, NULL, 0, false);
			}
		));
	/* äÝ—Î‘p’†ˆê“_g */
	if (getRule(RULE_KOUITTEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"gˆê“_", yaku::yakuCalculator::Yaku::yval_yakuman,
			"¬ˆêF",
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
	/* ”’—L‚è—ÎˆêF */
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
				"´“ñF", yaku::yakuCalculator::Yaku::yval_2han,
				allgreen_white
			));
		else if (getRule(RULE_ALL_GREEN_WITH_WHITE) == 2)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"”’—L‚è—ÎˆêF", yaku::yakuCalculator::Yaku::yval_yakuman,
				"¬ˆêF",
				allgreen_white
			));
	}
	/* —ÎˆêF—Ö */
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
				"—ÎˆêF—Ö", yaku::yakuCalculator::Yaku::yval_yakuman,
				"´ˆêF",
				allgreen_five
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"—ÎˆêF—Ö", yaku::yakuCalculator::Yaku::yval_yakuman,
				allgreen_five
			));
	}
	/* ‘å‘Œ´ */
	if (getRule(RULE_DAISOUGEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘å‘Œ´", yaku::yakuCalculator::Yaku::yval_yakuman,
			"´ˆêF",
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
	/* “ì‘å‘Œ´ */
	if (getRule(RULE_SOUTH_DAISOUGEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ì‘å‘Œ´", yaku::yakuCalculator::Yaku::yval_yakuman,
			"¬ˆêF",
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
	/* “ˆêF */
	if (getRule(RULE_TAOYISE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ˆêF", yaku::yakuCalculator::Yaku::yval_yakuman,
			"¬ˆêF",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleOne, CircleThree, CircleFive, CircleSix,
					CircleSeven, CircleNine, RedDragon,
				};
				const tileCode shunzi[] = {
					CircleFive,
				};
				return chktiles(analysis, kezi, 7, shunzi, 1, false) &&
					((countRedTilesInHand(analysis->GameStat, analysis->player, AkaDora))[CircleFive] > 0) &&
					(analysis->KeziCount[RedDragon] == 0);
			}
		));
	/* ’ƒˆêF */
	if (getRule(RULE_CHAYISE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’ƒˆêF", yaku::yakuCalculator::Yaku::yval_yakuman,
			"¬ˆêF",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon,
				};
				const tileCode shunzi[] = {
					BambooTwo,
				};
				return chktiles(analysis, kezi, 11, shunzi, 1, false);
			}
		));
	/* •ˆêF */
	if (getRule(RULE_HEIIISOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"•ˆêF", yaku::yakuCalculator::Yaku::yval_yakuman,
			"¬ˆêF", "‘ÎX˜a", "Â“´–å", "’fg˜a", "¬’fg",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleTwo, CircleFour, CircleEight,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return chktiles(analysis, kezi, 7, NULL, 0, false);
			}
		));
	/* Â“´–å */
	if (getRule(RULE_AO_NO_DOMON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Â“´–å", yaku::yakuCalculator::Yaku::yval_yakuman,
			"¬ˆêF", "‘ÎX˜a", "’fg˜a", "¬’fg",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1) {
					const tileCode kezi[] = {
						CircleTwo, CircleFour, CircleEight,
						EastWind, SouthWind, WestWind, NorthWind,
						GreenDragon,
					};
					const tileCode wind[] = {
						EastWind, SouthWind, WestWind, NorthWind,
					};
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 8, NULL, 0, false) == SIZE_OF_MELD_BUFFER) &&
						(yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, wind, 4, NULL, 0, false) == 1);
				}
				else return false;
			}
		));
	/* ’fg˜a */
	if (getRule(RULE_NO_RED) != 0) {
		auto hontanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleTwo, CircleFour, CircleEight,
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon,
				};
				const tileCode shunzi[] = { BambooTwo, };
				return chktiles(analysis, kezi, 14, shunzi, 1, false);
			};
		auto chintanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleTwo, CircleFour, CircleEight,
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
				};
				const tileCode shunzi[] = { BambooTwo, };
				return chktiles(analysis, kezi, 8, shunzi, 1, false);
			};
		if (getRule(RULE_NO_RED) == 2) {
			if (!chkGameType(&GameStat, SanmaS))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					"¬’fg", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
					hontanhon
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				chkGameType(&GameStat, SanmaS) ? "´’fg" : "’fg˜a", yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
				"¬’fg",
				chintanhon
			));
		} else {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"’fg˜a", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
				hontanhon
			));
		}
	}
	/* g”’é\“ª */
	if (getRule(RULE_KOHAKU_MANJU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"g”’é\“ª", (getRule(RULE_KOHAKU_MANJU) == 2) ?
			yaku::yakuCalculator::Yaku::yval_2han : yaku::yakuCalculator::Yaku::yval_1han,
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
}
