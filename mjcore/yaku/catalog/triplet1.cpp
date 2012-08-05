#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_1() {
	/* élà√çè */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"élà√çè", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"ëŒÅXòa", "éOà√çè",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
				1 : 0) == 4);
		}
	));
	/* élà√çèíPãR */
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"élà√çèíPãR", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"élà√çè", "ëŒÅXòa", "éOà√çè",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && (
					(analysis->GameStat->TianHuFlag) ||
					((analysis->Machi == yaku::yakuCalculator::machiTanki) && (!analysis->MachiInfo.FuritenFlag))
					);
			}
		));
	/* éOà√çè */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"éOà√çè", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
				1 : 0) == 3);
		}
	));

	// ---------------------------------------------------------------------

	/* å‹òAçè */
	if (getRule(RULE_GOLDEN_LADDER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ã‡íÚ", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"ê¥àÍêF", "ëŒÅXòa",
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
	/* élòAçè */
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
			"élòAçè", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ã‡íÚ", "ëŒÅXòa",
			suurenkoh
		));
	else if (getRule(RULE_SUURENKOH) == 2)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"élòAçè", yaku::yakuCalculator::Yaku::yval_6han /* ëŒÅXòaçûÇ›8„  */,
			"ã‡íÚ",
			suurenkoh
		));
	/* éOòAçè */
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
			"éOòAçè", (getRule(RULE_SANRENKOH) == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
			"ã‡íÚ", "élòAçè",
			sanrenkoh
		));
	/* ãæâπÉåÉì */
	if (getRule(RULE_KAGAMINE_LEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ãæâπÉåÉì", yaku::yakuCalculator::Yaku::yval_2han,
			/* éOòAçèÇ…í«â¡Ç∑ÇÈñ */
			[sanrenkoh](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));

	// ---------------------------------------------------------------------

	/* ÉåÉAÇ»ï˚ÇÃéOêF */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"éOêFìØçè", (getRule(RULE_SANSHOKU_DOUKOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
		"éOêFè¨ìØçè",
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
	/* élêFìØçèÅiÇ¶Ç¡ */
	if (getRule(RULE_YONSHOKU_DOUKOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"élêFìØçè", yaku::yakuCalculator::Yaku::yval_yakuman,
			"éOêFìØçè", "éOêFè¨ìØçè", "ëŒÅXòa",
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
	/* éOêFè¨ìØçè */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"éOêFè¨ìØçè", yaku::yakuCalculator::Yaku::yval_1han,
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

	/* éOå≥òaÅEéläÏòaîªíËóp */
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
	/* è¨éOå≥ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"è¨éOå≥", yaku::yakuCalculator::Yaku::yval_2han,
		/* ñîv2Ç¬ÇÕïKÇ∏ï°çáÇ∑ÇÈ */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* ëÂéOå≥ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ëÂéOå≥", yaku::yakuCalculator::Yaku::yval_yakuman,
		"ñîvÅEîí", "ñîvÅE·¢", "ñîvÅEíÜ",
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
	/* è¨éläÏ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"è¨éläÏ", yaku::yakuCalculator::Yaku::yval_yakuman,
		"ç¨àÍêF",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
	/* ìVå’ */
	if (getRule(RULE_HEAVEN_TIGER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ìVå’", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"ç¨àÍêF", "è¨éläÏ", "éöàÍêF", "ñîvÅE·¢",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* ëÂâˆåŒ */
	if (getRule(RULE_DAIKAIKO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ëÂâˆåŒ", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"ç¨àÍêF", "è¨éläÏ", "éöàÍêF", "ñîvÅEîí",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* êÁîNíé */
	if (getRule(RULE_Y2KBUG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"êÁîNíé", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"ç¨àÍêF", "è¨éläÏ", "ç¨òVì™",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[CharacterNine] >= 1)&&(analysis->KeziCount[SouthWind] == 1));
			}
		));
	/* ëÂéläÏ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ëÂéläÏ", (getRule(RULE_DOUBLE_YAKUMAN) == 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"ç¨àÍêF", "ëŒÅXòa", "ñîvÅEèÍïó", "ñîvÅEé©ïó", "ñîvÅEäJñÂïó", "ñîvÅEó†ïó",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
	/* íe‰ﬁ */
	if (getRule(RULE_DAN_YAKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"íe‰ﬁ", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"ç¨àÍêF", "ëŒÅXòa", "ñîvÅEèÍïó", "ñîvÅEé©ïó", "ñîvÅEäJñÂïó", "ñîvÅEó†ïó", "ëÂéläÏ", "ç¨òVì™",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[CharacterNine] >= 1));
			}
		));
	/* êÂêlè∂ */
	if (getRule(RULE_CACTUS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"êÂêlè∂", yaku::yakuCalculator::Yaku::yval_quad_yakuman,
			"ç¨àÍêF", "ëŒÅXòa", "ñîvÅEèÍïó", "ñîvÅEé©ïó", "ñîvÅEäJñÂïó", "ñîvÅEó†ïó", "ëÂéläÏ", "éöàÍêF",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[RedDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* ëŒÅXòa */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ëŒÅXòa", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1);
		}
	));

	// ---------------------------------------------------------------------

	/* ñîv */
	auto bakaze =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ñîvÅEèÍïó", yaku::yakuCalculator::Yaku::yval_1han,
		bakaze
	));
	auto jikaze =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
				analysis->player, analysis->GameStat->GameRound))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ñîvÅEé©ïó", yaku::yakuCalculator::Yaku::yval_1han,
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
			"ñîvÅEäJñÂïó", yaku::yakuCalculator::Yaku::yval_1han,
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
			"ñîvÅEó†ïó", yaku::yakuCalculator::Yaku::yval_1han,
			urakaze
		));
	} else {
		urakaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ñîvÅEîí", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[WhiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ñîvÅE·¢", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[GreenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ñîvÅEíÜ", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->KeziCount[RedDragon] >= 1);
		}
	));

	// ---------------------------------------------------------------------

	/* êVè¿ÇØÇÒÇ∂ */
	if (getRule(RULE_NIINUMA_KENJI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"êVè¿ÇØÇÒÇ∂", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* êwëæå€ */
	if (getRule(RULE_JINDAIKO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"êwëæå€", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* è¨éOïó */
	if (getRule(RULE_XIAOSANFENG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"è¨éOïó", yaku::yakuCalculator::Yaku::yval_1han,
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* éOïóçè */
	if (getRule(RULE_THREE_WINDS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"éOïóçè", yaku::yakuCalculator::Yaku::yval_2han,
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 6);
			}
		));
	/* ãqïóéOçè */
	if (getRule(RULE_THREE_OTAKAZE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ãqïóéOçè", yaku::yakuCalculator::Yaku::yval_3han,
			"éOïóçè",
			[WindCnt, bakaze, jikaze, kaimenkaze, urakaze]
			(const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 6) &&
					(!bakaze(analysis)) &&
					(!jikaze(analysis)) &&
					(!kaimenkaze(analysis)) &&
					(!urakaze(analysis)));
			}
		));
	/* ãqïóè¨éOçè */
	if (getRule(RULE_OTAKAZE_XIAOSANFENG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ãqïóè¨éOçè", yaku::yakuCalculator::Yaku::yval_2han,
			"è¨éOçè",
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

	/* êºíÜìáìÏï˚ */
	if (getRule(RULE_NISHINAKAJIMA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"êºíÜìáìÏï˚", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ñîvÅEèÍïó", "ñîvÅEé©ïó", "ñîvÅEíÜ",
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
	/* ê·åéâ‘ */
	if (getRule(RULE_SETSUGETSUKA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ê·åéâ‘", (getRule(RULE_SETSUGETSUKA) == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_2han,
			"ñîvÅEîí",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1));
			}
		));
	/* ïóâ‘ê·åé */
	if (getRule(RULE_FUUKA_SETSUGETSU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ïóâ‘ê·åé", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ê·åéâ‘", "ëŒÅXòa", "ñîvÅEèÍïó", "ñîvÅEé©ïó",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* â‘íπïóåé */
	if (getRule(RULE_KACHOU_FUUGETSU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"â‘íπïóåé", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ê·åéâ‘", "ëŒÅXòa", "ñîvÅEèÍïó", "ñîvÅEé©ïó",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* ìåñké©ìÆé‘ìπ */
	if (getRule(RULE_TOHOKU_EXPWY) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ìåñké©ìÆé‘ìπ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ëŒÅXòa", "ç¨àÍêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* ìåä÷ìåé©ìÆé‘ìπ */
	if (getRule(RULE_HIGASHIKANTO_EXPWY) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ìåä÷ìåé©ìÆé‘ìπ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ëŒÅXòa", "ç¨àÍêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1));
			}
		));
	/* íÜçëånÇÃÇ®ÇﬂÇ≈ÇΩÇ¢ñ */
	if (getRule(RULE_KOUKEI_HOUGYOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"çgå{ïÒã≈", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ëŒÅXòa", "ç¨àÍêF", "ê‚àÍñÂ", "ñîvÅEîí", "ñîvÅEíÜ", "ã‡å{ïÒã≈", "ã‚å{ïÒèt",
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
			"ã‡å{ïÒã≈", yaku::yakuCalculator::Yaku::yval_3han,
			"ñîvÅEíÜ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (getRule(RULE_GINKEI_HOUSHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ã‚å{ïÒèt", yaku::yakuCalculator::Yaku::yval_5han,
			"ñîvÅEîí",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->PlayerStat->FlowerFlag.Spring));
			}
		));

	// ---------------------------------------------------------------------

	/* élíµîvçè */
	if (getRule(RULE_SUUCHAOPAIKOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"élíµîvçè", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ëŒÅXòa", "íµîvçè",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 9-6; k++)
						if ((analysis->KeziCount[i + k] >= 1) &&
							(analysis->KeziCount[i + k + 2] >= 1) &&
							(analysis->KeziCount[i + k + 4] >= 1) &&
							(analysis->KeziCount[i + k + 6] >= 1))
							yakuFlag = true;
				return yakuFlag;
			}
		));
	/* íµîvçè */
	if (getRule(RULE_CHAOPAIKOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"íµîvçè", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 9-4; k++)
						if ((analysis->KeziCount[i + k] >= 1) &&
							(analysis->KeziCount[i + k + 2] >= 1) &&
							(analysis->KeziCount[i + k + 4] >= 1))
							yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ãÿîvçè */
	if (getRule(RULE_CHINPAIKOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ãÿîvçè", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 9-6; k++)
						if ((analysis->KeziCount[i + k] >= 1) &&
							(analysis->KeziCount[i + k + 3] >= 1) &&
							(analysis->KeziCount[i + k + 6] >= 1))
							yakuFlag = true;
				return yakuFlag;
			}
		));
	/* í∏éOçè */
	if (getRule(RULE_TINSANKOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"í∏éOçè", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 1] >= 1) &&
						(analysis->KeziCount[i + 5] >= 1) &&
						(analysis->KeziCount[i + 9] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* ìÒêFå‹òAçè */
	const std::array<char[8], 90> parsedat_bichrome5 = {
		"00001","00002","00010","00011","00020","00022","00100","00101","00110","00111",
		"00200","00202","00220","00222","01000","01001","01010","01011","01100","01101",
		"01110","01111","02000","02002","02020","02022","02200","02202","02220","02222",
		"10000","10001","10010","10011","10100","10101","10110","10111","11000","11001",
		"11010","11011","11100","11101","11110","11112","11121","11122","11211","11212",
		"11221","11222","12111","12112","12121","12122","12211","12212","12221","12222",
		"20000","20002","20020","20022","20200","20202","20220","20222","21111","21112",
		"21121","21122","21211","21212","21221","21222","22000","22002","22020","22022",
		"22111","22112","22121","22122","22200","22202","22211","22212","22220","22221",
	};
	auto wulianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * 4); k++)
					if ((analysis->DuiziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->DuiziCount[(int)(parsedat[i * fldsize + 4] - '0') * TILE_SUIT_STEP + step * k] >= 1) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	auto lianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suurnkoh) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * (suurnkoh ? 3 : 2)); k++)
					if ((analysis->KeziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(suurnkoh&&(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	if (getRule(RULE_SILVER_LADDER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ã‚íÚ", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"ìÒêFélòAçè", "ìÒêFéOòAçè", "ëŒÅXòa",
			[wulianke, parsedat_bichrome5](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_bichrome5[0], 90, 8, 1);
			}
		));
}
