#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_1() {
	/* ŽlˆÃ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ŽlˆÃ", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"‘ÎX˜a", "ŽOˆÃ",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
				1 : 0) == 4);
		}
	));
	/* ŽlˆÃ’P‹R */
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽlˆÃ’P‹R", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"ŽlˆÃ", "‘ÎX˜a", "ŽOˆÃ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && (
					(analysis->GameStat->TianHuFlag) ||
					((analysis->Machi == yaku::yakuCalculator::machiTanki) && (!analysis->MachiInfo.FuritenFlag))
					);
			}
		));
	/* ŽOˆÃ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ŽOˆÃ", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
				1 : 0) == 3);
		}
	));

	// ---------------------------------------------------------------------

	/* ŒÜ˜A */
	if (getRule(RULE_GOLDEN_LADDER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹à’ò", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"´ˆêF", "‘ÎX˜a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if ((analysis->KeziCount[i] >= 1) &&
						(analysis->KeziCount[i + 1] >= 1) &&
						(analysis->KeziCount[i + 2] >= 1) &&
						(analysis->KeziCount[i + 3] >= 1) &&
						((analysis->MianziDat[0].tile == (tileCode)(i - 1)) ||
						(analysis->MianziDat[0].tile == (tileCode)(i + 4))) )
						yakuFlag = true;
				return yakuFlag;
			}
		));
	/* Žl˜A */
	auto suurenkoh =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if ((analysis->KeziCount[i] >= 1) &&
					(analysis->KeziCount[i + 1] >= 1) &&
					(analysis->KeziCount[i + 2] >= 1) &&
					(analysis->KeziCount[i + 3] >= 1))
					yakuFlag = true;
			return yakuFlag;
		};
	if (getRule(RULE_SUURENKOH) == 1)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žl˜A", yaku::yakuCalculator::Yaku::yval_yakuman,
			"‹à’ò", "‘ÎX˜a",
			suurenkoh
		));
	else if (getRule(RULE_SUURENKOH) == 2)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žl˜A", yaku::yakuCalculator::Yaku::yval_6han /* ‘ÎX˜až‚Ý8ãÊ */,
			"‹à’ò",
			suurenkoh
		));
	/* ŽO˜A */
	auto sanrenkoh =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if ((analysis->KeziCount[i] >= 1) &&
					(analysis->KeziCount[i + 1] >= 1) &&
					(analysis->KeziCount[i + 2] >= 1))
					yakuFlag = true;
			return yakuFlag;
		};
	if (getRule(RULE_SANRENKOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽO˜A", (getRule(RULE_SANRENKOH) == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
			"‹à’ò", "Žl˜A",
			sanrenkoh
		));
	/* ‹¾‰¹ƒŒƒ“ */
	if (getRule(RULE_KAGAMINE_LEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹¾‰¹ƒŒƒ“", yaku::yakuCalculator::Yaku::yval_2han,
			/* ŽO˜A‚É’Ç‰Á‚·‚é–ð */
			[sanrenkoh](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));

	// ---------------------------------------------------------------------

	/* ƒŒƒA‚È•û‚ÌŽOF */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ŽOF“¯", (getRule(RULE_SANSHOKU_DOUKOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
		"ŽOF¬“¯",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 9; i++)
				if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
					(analysis->KeziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
					(analysis->KeziCount[i + TILE_SUIT_BAMBOOS] >= 1))
					yakuFlag = true;
			return yakuFlag;
		}
	));
	/* ŽlF“¯i‚¦‚Á */
	if (getRule(RULE_YONSHOKU_DOUKOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽlF“¯", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ŽOF“¯", "ŽOF¬“¯", "‘ÎX˜a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_BAMBOOS] >= 1) &&
						(analysis->KeziCount[WhiteDragon] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ŽOF¬“¯ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ŽOF¬“¯", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 9; i++)
				if ((analysis->DuiziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
					(analysis->DuiziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
					(analysis->DuiziCount[i + TILE_SUIT_BAMBOOS] >= 1))
					yakuFlag = true;
			return yakuFlag;
		}
	));

	// ---------------------------------------------------------------------

	/* ŽOŒ³˜aEŽlŠì˜a”»’è—p */
	auto WindCnt =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			return analysis->DuiziCount[EastWind] + analysis->KeziCount[EastWind] +
				 analysis->DuiziCount[SouthWind] + analysis->KeziCount[SouthWind] +
				  analysis->DuiziCount[WestWind] + analysis->KeziCount[WestWind] +
				  analysis->DuiziCount[NorthWind] + analysis->KeziCount[NorthWind];
		};
	auto DragonCnt =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			return analysis->DuiziCount[WhiteDragon] + analysis->KeziCount[WhiteDragon] +
				 analysis->DuiziCount[GreenDragon] + analysis->KeziCount[GreenDragon] +
				  analysis->DuiziCount[RedDragon] + analysis->KeziCount[RedDragon];
		};
	/* ¬ŽOŒ³ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"¬ŽOŒ³", yaku::yakuCalculator::Yaku::yval_2han,
		/* –ð”v2‚Â‚Í•K‚¸•¡‡‚·‚é */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* ‘åŽOŒ³ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘åŽOŒ³", yaku::yakuCalculator::Yaku::yval_yakuman,
		"–ð”vE”’", "–ð”vEá¢", "–ð”vE’†",
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
	/* ¬ŽlŠì */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"¬ŽlŠì", yaku::yakuCalculator::Yaku::yval_yakuman,
		"¬ˆêF",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
	/* “VŒÕ */
	if (getRule(RULE_HEAVEN_TIGER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“VŒÕ", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"¬ˆêF", "¬ŽlŠì", "ŽšˆêF", "–ð”vEá¢",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* ‘å‰öŒÎ */
	if (getRule(RULE_DAIKAIKO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘å‰öŒÎ", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"¬ˆêF", "¬ŽlŠì", "ŽšˆêF", "–ð”vE”’",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* ç”N’Ž */
	if (getRule(RULE_Y2KBUG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ç”N’Ž", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"¬ˆêF", "¬ŽlŠì", "¬˜V“ª",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[CharacterNine] >= 1)&&(analysis->KeziCount[SouthWind] == 1));
			}
		));
	/* ‘åŽlŠì */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘åŽlŠì", (getRule(RULE_DOUBLE_YAKUMAN) == 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"¬ˆêF", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—", "–ð”vEŠJ–å•—", "–ð”vE— •—",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
	/* ’eäÞ */
	if (getRule(RULE_DAN_YAKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’eäÞ", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"¬ˆêF", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—", "–ð”vEŠJ–å•—", "–ð”vE— •—", "‘åŽlŠì", "¬˜V“ª",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[CharacterNine] >= 1));
			}
		));
	/* ål¶ */
	if (getRule(RULE_CACTUS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ål¶", yaku::yakuCalculator::Yaku::yval_quad_yakuman,
			"¬ˆêF", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—", "–ð”vEŠJ–å•—", "–ð”vE— •—", "‘åŽlŠì", "ŽšˆêF",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[RedDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* ‘ÎX˜a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘ÎX˜a", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1);
		}
	));

	// ---------------------------------------------------------------------

	/* –ð”v */
	auto bakaze =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vEê•—", yaku::yakuCalculator::Yaku::yval_1han,
		bakaze
	));
	auto jikaze =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
				analysis->player, analysis->GameStat->GameRound))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vEŽ©•—", yaku::yakuCalculator::Yaku::yval_1han,
		jikaze
	));
	YAKUFUNC kaimenkaze, urakaze;
	if (getRule(RULE_KAIMENKAZE) != 0) {
		kaimenkaze =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound)))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–ð”vEŠJ–å•—", yaku::yakuCalculator::Yaku::yval_1han,
			kaimenkaze
		));
	} else {
		kaimenkaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	if (getRule(RULE_URAKAZE) != 0) {
		urakaze =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					RelativePositionOf(analysis->player, sOpposite), analysis->GameStat->GameRound)))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–ð”vE— •—", yaku::yakuCalculator::Yaku::yval_1han,
			urakaze
		));
	} else {
		urakaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vE”’", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[WhiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vEá¢", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[GreenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vE’†", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[RedDragon] >= 1);
		}
	));

	// ---------------------------------------------------------------------

	/* VÀ‚¯‚ñ‚¶ */
	if (getRule(RULE_NIINUMA_KENJI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"VÀ‚¯‚ñ‚¶", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* w‘¾ŒÛ */
	if (getRule(RULE_JINDAIKO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"w‘¾ŒÛ", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ¬ŽO•— */
	if (getRule(RULE_XIAOSANFENG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¬ŽO•—", yaku::yakuCalculator::Yaku::yval_1han,
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* ŽO•— */
	if (getRule(RULE_THREE_WINDS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽO•—", yaku::yakuCalculator::Yaku::yval_2han,
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 6);
			}
		));
	/* ‹q•—ŽO */
	if (getRule(RULE_THREE_OTAKAZE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹q•—ŽO", yaku::yakuCalculator::Yaku::yval_3han,
			"ŽO•—",
			[WindCnt, bakaze, jikaze, kaimenkaze, urakaze]
			(const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 6) &&
					(!bakaze(analysis)) &&
					(!jikaze(analysis)) &&
					(!kaimenkaze(analysis)) &&
					(!urakaze(analysis)));
			}
		));
	/* ‹q•—¬ŽO */
	if (getRule(RULE_OTAKAZE_XIAOSANFENG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹q•—¬ŽO", yaku::yakuCalculator::Yaku::yval_2han,
			"¬ŽO",
			[WindCnt, bakaze, jikaze, kaimenkaze, urakaze]
			(const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 5) &&
					(!bakaze(analysis)) &&
					(!jikaze(analysis)) &&
					(!kaimenkaze(analysis)) &&
					(!urakaze(analysis)));
			}
		));

	// ---------------------------------------------------------------------

	/* ¼’†“‡“ì•û */
	if (getRule(RULE_NISHINAKAJIMA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¼’†“‡“ì•û", yaku::yakuCalculator::Yaku::yval_yakuman,
			"–ð”vEê•—", "–ð”vEŽ©•—", "–ð”vE’†",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->GameStat->GameRound / 4 == sWest) &&
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sSouth))
					yakuFlag = true;
				if ((analysis->GameStat->GameRound / 4 == sSouth) &&
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sWest))
					yakuFlag = true;
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(yakuFlag));
			}
		));
	/* áŒŽ‰Ô */
	if (getRule(RULE_SETSUGETSUKA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"áŒŽ‰Ô", (getRule(RULE_SETSUGETSUKA) == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
			"–ð”vE”’",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1));
			}
		));
	/* •—‰ÔáŒŽ */
	if (getRule(RULE_FUUKA_SETSUGETSU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"•—‰ÔáŒŽ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"áŒŽ‰Ô", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* ‰Ô’¹•—ŒŽ */
	if (getRule(RULE_KACHOU_FUUGETSU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰Ô’¹•—ŒŽ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"áŒŽ‰Ô", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* “Œ–kŽ©“®ŽÔ“¹ */
	if (getRule(RULE_TOHOKU_EXPWY) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ–kŽ©“®ŽÔ“¹", yaku::yakuCalculator::Yaku::yval_yakuman,
			"‘ÎX˜a", "¬ˆêF",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* “ŒŠÖ“ŒŽ©“®ŽÔ“¹ */
	if (getRule(RULE_HIGASHIKANTO_EXPWY) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ŒŠÖ“ŒŽ©“®ŽÔ“¹", yaku::yakuCalculator::Yaku::yval_yakuman,
			"‘ÎX˜a", "¬ˆêF",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1));
			}
		));
	/* ’†‘Œn‚Ì‚¨‚ß‚Å‚½‚¢–ð */
	if (getRule(RULE_KOUKEI_HOUGYOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"gŒ{•ñ‹Å", yaku::yakuCalculator::Yaku::yval_yakuman,
			"‘ÎX˜a", "¬ˆêF", "âˆê–å", "–ð”vE”’", "–ð”vE’†", "‹àŒ{•ñ‹Å", "‹âŒ{•ñt",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->analysis->MianziDat[0].tile == CircleOne) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (getRule(RULE_KINKEI_HOUGYOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹àŒ{•ñ‹Å", yaku::yakuCalculator::Yaku::yval_3han,
			"–ð”vE’†",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (getRule(RULE_GINKEI_HOUSHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹âŒ{•ñt", yaku::yakuCalculator::Yaku::yval_5han,
			"–ð”vE”’",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->PlayerStat->FlowerFlag.Spring));
			}
		));
}
