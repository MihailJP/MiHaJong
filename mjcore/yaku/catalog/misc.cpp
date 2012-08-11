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
				return chktiles(analysis, kezi, 6, kezi, 1, false);
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
					CircleOne, CircleTwo, CircleThree, BambooFour, CircleFour, CircleFive, CircleEight,
					CircleNine, BambooTwo, BambooFive, BambooSix, BambooEight, BambooNine, WhiteDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 4, false);
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
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
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
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
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
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
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
				return chktiles(analysis, kezi, 7, kezi, 4, false);
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
				return chktiles(analysis, kezi, 7, kezi, 4, false) &&
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
					CircleFive, CircleOne, CircleThree, CircleSix,
					CircleSeven, CircleNine, RedDragon,
				};
				return chktiles(analysis, kezi, 7, kezi, 1, false) &&
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
				return chktiles(analysis, kezi, 11, kezi, 1, false);
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
						EastWind, SouthWind, WestWind, NorthWind,
						CircleTwo, CircleFour, CircleEight,
						GreenDragon,
					};
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 8, NULL, 0, false) == SIZE_OF_MELD_BUFFER) &&
						(yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 4, NULL, 0, false) == 1);
				}
				else return false;
			}
		));
	/* ’fg˜a */
	if (getRule(RULE_NO_RED) != 0) {
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

	// ---------------------------------------------------------------------

	// Žl‹AŽl—‚Ý‚Ål‚¦‚ç‚ê‚éƒpƒ^[ƒ“‚Æ”vŽ®•\‹L
	// 123 123 123 123: ”vŽ®44440 ©‚±‚ê‚ÍˆêFŽl‡
	// 123 123 123 234: ”vŽ®34410
	// 123 123 123 345: ”vŽ®33411
	// 123 123 234 234: ”vŽ®24420
	// 123 123 234 345: ”vŽ®23421
	// 123 123 345 345: ”vŽ®22422
	// 123 234 234 234: ”vŽ®14430
	// 123 234 234 345: ”vŽ®13431
	// 123 234 345 345: ”vŽ®12432
	// 123 345 345 345: ”vŽ®11433

	/* ‘åŽl‹AŽl */
	if (getRule(RULE_GREAT_FOUR_INTO_FOUR) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘åŽl‹AŽl", yaku::yakuCalculator::Yaku::yval_yakuman,
			"dŽl‹AŽl", "Žl‹AŽl", "Žl‹AŽOˆê", "Žl‹AŽO", "—´Žl‹Aˆê", "ŒÕŽl‹Aˆê", "—¼Žl‹Aˆê", "Žl‹Aˆê",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					// 123 123 234 234: ”vŽ®24420
					if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 2) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3))))
						return true;
					// 123 123 234 345: ”vŽ®23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 1) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3))))
						return true;
					// 123 123 345 345: ”vŽ®22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 1)) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3)) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 4))))
						return true;
					// 123 234 345 345: ”vŽ®12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (tileCode)i) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 3))))
						return true;
				}
				return false;
			}
		));
	/* dŽl‹AŽl */
	if (getRule(RULE_DOUBLE_FOUR_INTO_FOUR) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"dŽl‹AŽl", yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_8han),
			"Žl‹AŽl", "Žl‹AŽOˆê", "Žl‹AŽO", "—´Žl‹Aˆê", "ŒÕŽl‹Aˆê", "—¼Žl‹Aˆê", "Žl‹Aˆê",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					// 123 123 123 234: ”vŽ®34410
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[i+1] >= 1))
						return true;
					// 123 123 234 234: ”vŽ®24420
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 2))
						return true;
					// 123 234 234 234: ”vŽ®14430
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 3))
						return true;
				}
				return false;
			}
		));
	/* Žl‹AŽl */
	if (getRule(RULE_FOUR_INTO_FOUR) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žl‹AŽl", yaku::yakuCalculator::Yaku::yval_6han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TILE_SUIT_HONORS; i++) {
					// 123 123 123 345: ”vŽ®33411
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 123 234 345: ”vŽ®23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 123 345 345: ”vŽ®22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+2] >= 2))
						return true;
					// 123 234 234 345: ”vŽ®13431
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 2) &&
						(analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 234 345 345: ”vŽ®12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 2))
						return true;
					// 123 345 345 345: ”vŽ®11433
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
		/* Žl‹AŽOˆê */
		if (getRule(RULE_FOUR_INTO_THREE_ONE) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"Žl‹AŽOˆê", yaku::yakuCalculator::Yaku::yval_4han,
				"Žl‹AŽO", "Žl‹Aˆê",
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
		/* Žl‹AŽO */
		if (getRule(RULE_FOUR_INTO_THREE) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"Žl‹AŽO", yaku::yakuCalculator::Yaku::yval_3han,
				"Žl‹Aˆê",
				suukuisan
			));
	}

	// ---------------------------------------------------------------------

	/* —´Žl‹Aˆê */
	if (getRule(RULE_DRAGON_FOUR_INTO_ONE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"—´Žl‹Aˆê", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ŽO˜A", "ŒÕŽl‹Aˆê", "—¼Žl‹Aˆê", "Žl‹Aˆê",
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
	/* ŒÕŽl‹Aˆê */
	if (getRule(RULE_TIGER_FOUR_INTO_ONE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŒÕŽl‹Aˆê", (getRule(RULE_TIGER_FOUR_INTO_ONE) == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
			"—¼Žl‹Aˆê", "Žl‹Aˆê",
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
				return ++count;
			};
		/* —¼Žl‹Aˆê */
		if (getRule(RULE_DOUBLE_FOUR_INTO_ONE) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"—¼Žl‹Aˆê", (getRule(RULE_DOUBLE_FOUR_INTO_ONE) == 2) ?
				yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
				"Žl‹Aˆê",
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag = false;
					return (suukuiyii(analysis) == 2);
				}
			));
		/* Žl‹Aˆê */
		if (getRule(RULE_FOUR_INTO_ONE) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"Žl‹Aˆê", yaku::yakuCalculator::Yaku::yval_1han,
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag = false;
					return (suukuiyii(analysis) == 1);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ‹ã˜@‰Ô“• */
	if (getRule(RULE_CHUUREN_HWATON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ã˜@‰Ô“•", (getRule(RULE_CHUUREN_HWATON) == 2) ?
			yaku::yakuCalculator::Yaku::yval_2han : yaku::yakuCalculator::Yaku::yval_4han,
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
	/* –Â‚«‹ã˜A•ó“• */
	if (getRule(RULE_EXPOSED_CHUUREN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ã˜@•É“•", yaku::yakuCalculator::Yaku::yval_3han,
			/* –Â‚«ƒ`ƒ“ƒCƒc‚ª•¡‡‚µ”{–žˆÈã‚ªŠm’è */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				Int8ByTile tiles = analysis->TileCount; bool yakuFlag = false;
				// –Â‚«–ÊŽq‚àŒvŽZ žÈŽq‚Í–³Ž‹
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

	/* ä ƒ|ƒ“(”ñ‘ÎX) */
	if (getRule(RULE_UIKYOU_SHUNTSU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ä ƒ|ƒ“ ", // ‘ÎXŒ`‚Æ‹æ•Ê‚·‚é‚½‚ß‚ÉŒã‚ë‚ÉƒXƒy[ƒX‚ð“ü‚ê‚é
			(getRule(RULE_UIKYOU_SHUNTSU) == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_4han,
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
							/* ‚±‚±‚É‚Íelse‚ð‚Â‚¯‚Ä‚Í‚¾‚ß */
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

	/* •‘¾˜Y”„Ä–Ý */
	if (getRule(RULE_UUTAARAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"•‘¾˜Y”„Ä–Ý", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CharacterFive, CircleFive, BambooFive, CircleOne,
				};
				const tileCode shunzi[] = {
					CharacterThree, CharacterFour, CharacterFive,
					CircleThree, CircleFour, CircleFive,
					BambooThree, BambooFour, BambooFive,
				};
				return chktiles(analysis, kezi, 4, shunzi, 9, false) &&
					(analysis->MianziDat[0].tile == CircleOne);
			}
		));
	/* ‹ð—”“{‹ê˜I| */
	if (getRule(RULE_GRAND_CROSS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ð—”“{‹ê˜I|", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->TsumoHai->tile == CircleFive) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"´ˆêF",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleFour, CircleFive, CircleSix,
				};
				const tileCode shunzi[] = {
					CircleOne, CircleSeven,
				};
				return chktiles(analysis, kezi, 3, shunzi, 2, false) &&
					(analysis->MianziDat[0].tile == CircleFive) &&
					(*analysis->MenzenFlag);
			}
		));
	/* ŠìêFè~”~ */
	if (getRule(RULE_XIQUE_NAOMEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŠìêFè~”~", yaku::yakuCalculator::Yaku::yval_6han,
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleFive, BambooOne, BambooFive,
				};
				const tileCode shunzi[] = {
					CharacterOne, CircleOne,
				};
				return chktiles(analysis, kezi, 3, shunzi, 2, false) &&
					(analysis->MianziDat[0].tile == BambooFive);
			}
		));
	/* ‰ÔˆêF */
	if (getRule(RULE_HUAIISOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰ÔˆêF", yaku::yakuCalculator::Yaku::yval_1han,
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
	/* ÔˆêF */
	if (getRule(RULE_AKAIISOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ÔˆêF", yaku::yakuCalculator::Yaku::yval_2han,
			"‰ÔˆêF", "âˆê–å",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleFive, CircleOne, CircleThree, CircleSix, CircleSeven,
					CircleNine, BambooOne, BambooFive, BambooSeven, BambooNine,
				};
				return chktiles(analysis, kezi, 10, kezi, 1, false);
			}
		));
	/* ÔŠáÂ—´ */
	if (getRule(RULE_SEKIGAN_SEIRYUU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ÔŠáÂ—´", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->ShunziCount[CircleTwo] == 1) &&
					(analysis->KeziCount[WhiteDragon] + analysis->KeziCount[EastWind] == 1) &&
					(analysis->DuiziCount[CircleTwo] + analysis->DuiziCount[CircleFour] +
					analysis->DuiziCount[CircleEight] >= 3));
			}
		));

	// ---------------------------------------------------------------------

	/* –ž‰€‰Ô */
	{
		RuleCode ruleCodeList[10] = { (RuleCode)0,
			RULE_ALL_ONE, RULE_ALL_TWO, RULE_ALL_THREE, RULE_ALL_FOUR, RULE_ALL_FIVE,
			RULE_ALL_SIX, RULE_ALL_SEVEN, RULE_ALL_EIGHT, RULE_ALL_NINE};
		const char nameList[10][16] = { "",
			"‘S‘Ñˆê", "‘S‘Ñ“ñ", "‘S‘ÑŽO", "‘S‘ÑŽl", "‘S‘ÑŒÜ", "‘S‘Ñ˜Z", "‘S‘ÑŽµ", "‘S‘Ñ”ª", "‘S‘Ñ‹ã"};
		for (int i = 1; i <= 9; i++) {
			if (getRule(ruleCodeList[i]) != 0) {
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
				yaku::yakuCalculator::Yaku::HANFUNC han;
				switch (i) {
				case 5:
					switch (getRule(ruleCodeList[i])) {
						case 1: han = yaku::yakuCalculator::Yaku::yval_2han; break;
						case 2: han = yaku::yakuCalculator::Yaku::yval_3han_kuisagari; break;
						case 3: han = yaku::yakuCalculator::Yaku::yval_4han; break;
						case 4: han = yaku::yakuCalculator::Yaku::yval_6han_kuisagari; break;
					}
					goto four_five_six; // ‚±‚±‚ÍŠ¸‚¦‚Ägoto‚ðŽg‚¤
				case 4: case 6:
					han = yaku::yakuCalculator::Yaku::yval_4han;
					/* FALLTHRU */
				four_five_six:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], han, "’f›ô‹ã", f));
					break;
				case 1: case 9:
					switch (getRule(ruleCodeList[i])) {
						case 1: han = yaku::yakuCalculator::Yaku::yval_4han; break;
						case 2: han = yaku::yakuCalculator::Yaku::yval_6han_kuisagari; break;
					}
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], han, "ƒ‘S‘Ñ›ô‹ã", f));
					break;
				default:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], yaku::yakuCalculator::Yaku::yval_4han, f));
					break;
				}
			}
		}
	}
}
