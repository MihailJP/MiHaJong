#include "../catalog.h"

extern const std::array<char[8], 3> parsedat_monochrome5, parsedat_monochrome4;
extern const std::array<char[4], 3> parsedat_monochrome3;
extern const std::array<char[8], 90> parsedat_bichrome5;
extern const std::array<char[8], 42> parsedat_bichrome4;
extern const std::array<char[4], 18> parsedat_bichrome3;
extern const std::array<char[8], 150> parsedat_trichrome5;
extern const std::array<char[8], 36> parsedat_trichrome4;
extern const std::array<char[4], 6> parsedat_trichrome3;

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
	if (RuleData::chkRuleApplied("double_yakuman"))
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
	if (RuleData::chkRuleApplied("golden_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹à’ò", get_yaku_han("golden_ladder"),
			"´ˆêF", "‘ÎX˜a",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_monochrome5[0], 3, 8, 1);
			}
		));
	/* Žl˜A */
	auto suurenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
		};
	if (RuleData::chkRule("suurenkoh", "8han_incl_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žl˜A", yaku::yakuCalculator::Yaku::yval_6han /* ‘ÎX˜až‚Ý8ãÊ */,
			"‹à’ò",
			suurenkoh
		));
	else if (RuleData::chkRuleApplied("suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žl˜A", get_yaku_han("suurenkoh"),
			"‹à’ò", "‘ÎX˜a",
			suurenkoh
		));
	/* ŽO˜A */
	auto sanrenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
		};
	if (RuleData::chkRuleApplied("sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽO˜A", get_yaku_han("sanrenkoh"),
			"‹à’ò", "Žl˜A",
			sanrenkoh
		));
	/* ‹¾‰¹ƒŒƒ“ */
	if (RuleData::chkRuleApplied("kagamine_len"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹¾‰¹ƒŒƒ“", get_yaku_han("kagamine_len"),
			/* ŽO˜A‚É’Ç‰Á‚·‚é–ð */
			[sanrenkoh](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));

	// ---------------------------------------------------------------------

	/* ƒŒƒA‚È•û‚ÌŽOF */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ŽOF“¯", get_yaku_han("sanshoku_doukoh"),
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
	if (RuleData::chkRuleApplied("yonshoku_doukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽlF“¯", get_yaku_han("yonshoku_doukoh"),
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
	if (RuleData::chkRuleApplied("sanshoku_shoudoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOF¬“¯", get_yaku_han("sanshoku_shoudoukoh"),
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
		"Ât‚É‰÷‚¢‚È‚µ", "Ât‚É‰÷‚¢‚ ‚è", /* –ð”v2‚Â‚Í•K‚¸•¡‡‚·‚é */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* ‘åŽOŒ³ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘åŽOŒ³", yaku::yakuCalculator::Yaku::yval_yakuman,
		"–ð”vE”’", "–ð”vEá¢", "–ð”vE’†", "Ât‚É‰÷‚¢‚È‚µ", "Ât‚É‰÷‚¢‚ ‚è",
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
	/* –å‘O‘åŽOŒ³ */
	if (RuleData::chkRuleApplied("menzen_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–å‘O‘åŽOŒ³", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"‘åŽOŒ³",
			[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (DragonCnt(analysis) == 6);
			}
		));
	/* ¬ŽlŠì */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"¬ŽlŠì", yaku::yakuCalculator::Yaku::yval_yakuman,
		"¬ˆêF", "““ïŽÔ",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
	/* “VŒÕ */
	if (RuleData::chkRuleApplied("heaven_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“VŒÕ", get_yaku_han("heaven_tiger"),
			"¬ˆêF", "¬ŽlŠì", "ŽšˆêF", "–ð”vEá¢", "““ïŽÔ",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* ‘å‰öŒÎ */
	if (RuleData::chkRuleApplied("daikaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘å‰öŒÎ", get_yaku_han("daikaiko"),
			"¬ˆêF", "¬ŽlŠì", "ŽšˆêF", "–ð”vE”’", "““ïŽÔ",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* ç”N’Ž */
	if (RuleData::chkRuleApplied("y2kbug"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ç”N’Ž", get_yaku_han("y2kbug"),
			"¬ˆêF", "¬ŽlŠì", "¬˜V“ª", "““ïŽÔ",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[CharacterNine] >= 1)&&(analysis->KeziCount[SouthWind] == 1));
			}
		));
	/* ‘åŽlŠì */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘åŽlŠì", (RuleData::chkRuleApplied("double_yakuman")) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"¬ˆêF", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—", "–ð”vEŠJ–å•—", "–ð”vE— •—", "““ïŽÔ",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
	/* ’eäÞ */
	if (RuleData::chkRuleApplied("dan_yaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’eäÞ", get_yaku_han("dan_yaku"),
			"¬ˆêF", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—", "–ð”vEŠJ–å•—", "–ð”vE— •—", "‘åŽlŠì", "¬˜V“ª", "““ïŽÔ",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[CharacterNine] >= 1));
			}
		));
	/* ål¶ */
	if (RuleData::chkRuleApplied("cactus"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ål¶", get_yaku_han("cactus"),
			"¬ˆêF", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—", "–ð”vEŠJ–å•—", "–ð”vE— •—", "‘åŽlŠì", "ŽšˆêF", "““ïŽÔ",
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
	auto chkYakuhai = RuleData::chkRuleApplied("exposed_yakuhai") ?
		(std::function<const Int8ByTile& (const MENTSU_ANALYSIS* const)>)
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->KeziCount;} :
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->AnKeziCount;};
	auto bakaze =
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vEê•—", yaku::yakuCalculator::Yaku::yval_1han,
		bakaze
	));
	auto jikaze =
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
				analysis->player, analysis->GameStat->GameRound))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vEŽ©•—", yaku::yakuCalculator::Yaku::yval_1han,
		jikaze
	));
	YAKUFUNC kaimenkaze, urakaze;
	if (RuleData::chkRuleApplied("kaimenkaze")) {
		kaimenkaze =
			[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–ð”vEŠJ–å•—", get_yaku_han("kaimenkaze"),
			kaimenkaze
		));
	} else {
		kaimenkaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	if (RuleData::chkRuleApplied("urakaze")) {
		urakaze =
			[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					RelativePositionOf(analysis->player, sOpposite), analysis->GameStat->GameRound))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–ð”vE— •—", get_yaku_han("urakaze"),
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
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[WhiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vEá¢", yaku::yakuCalculator::Yaku::yval_1han,
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[GreenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–ð”vE’†", yaku::yakuCalculator::Yaku::yval_1han,
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[RedDragon] >= 1);
		}
	));

	// ---------------------------------------------------------------------

	/* VÀ‚¯‚ñ‚¶ */
	if (RuleData::chkRuleApplied("niinuma_kenji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"VÀ‚¯‚ñ‚¶", get_yaku_han("niinuma_kenji"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* w‘¾ŒÛ */
	if (RuleData::chkRuleApplied("jindaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"w‘¾ŒÛ", get_yaku_han("jindaiko"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ¬ŽO•— */
	if (RuleData::chkRuleApplied("xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¬ŽO•—", get_yaku_han("xiaosanfeng"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* ŽO•— */
	if (RuleData::chkRuleApplied("three_winds"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽO•—", get_yaku_han("three_winds"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 6);
			}
		));
	/* ‹q•—ŽO */
	if (RuleData::chkRuleApplied("three_otakaze"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹q•—ŽO", get_yaku_han("three_otakaze"),
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
	if (RuleData::chkRuleApplied("otakaze_xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹q•—¬ŽO", get_yaku_han("otakaze_xiaosanfeng"),
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
	if (RuleData::chkRuleApplied("nishinakajima"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¼’†“‡“ì•û", get_yaku_han("nishinakajima"),
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
	if (RuleData::chkRuleApplied("setsugetsuka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"áŒŽ‰Ô", get_yaku_han("setsugetsuka"),
			"–ð”vE”’",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1));
			}
		));
	/* •—‰ÔáŒŽ */
	if (RuleData::chkRuleApplied("fuuka_setsugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"•—‰ÔáŒŽ", get_yaku_han("fuuka_setsugetsu"),
			"áŒŽ‰Ô", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* ‰Ô’¹•—ŒŽ */
	if (RuleData::chkRuleApplied("kachou_fuugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰Ô’¹•—ŒŽ", get_yaku_han("kachou_fuugetsu"),
			"áŒŽ‰Ô", "‘ÎX˜a", "–ð”vEê•—", "–ð”vEŽ©•—",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* “Œ–kŽ©“®ŽÔ“¹ */
	if (RuleData::chkRuleApplied("tohoku_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ–kŽ©“®ŽÔ“¹", get_yaku_han("tohoku_expwy"),
			"‘ÎX˜a", "¬ˆêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* “ŒŠÖ“ŒŽ©“®ŽÔ“¹ */
	if (RuleData::chkRuleApplied("higashikanto_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ŒŠÖ“ŒŽ©“®ŽÔ“¹", get_yaku_han("higashikanto_expwy"),
			"‘ÎX˜a", "¬ˆêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1));
			}
		));
	/* ’†‘Œn‚Ì‚¨‚ß‚Å‚½‚¢–ð */
	if (RuleData::chkRuleApplied("koukei_hougyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"gŒ{•ñ‹Å", get_yaku_han("koukei_hougyou"),
			"‘ÎX˜a", "¬ˆêF", "âˆê–å", "–ð”vE”’", "–ð”vE’†", "‹àŒ{•ñ‹Å", "‹âŒ{•ñt", "”’’¹",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == CircleOne) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("kinkei_hougyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹àŒ{•ñ‹Å", get_yaku_han("kinkei_hougyou"),
			"–ð”vE’†",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("ginkei_houshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹âŒ{•ñt", get_yaku_han("ginkei_houshun"),
			"–ð”vE”’", "”’’¹",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->PlayerStat->FlowerFlag.Spring));
			}
		));

	// ---------------------------------------------------------------------

	/* Žl’µ”v */
	if (RuleData::chkRuleApplied("suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žl’µ”v", get_yaku_han("suuchaopaikoh"),
			"‘ÎX˜a", "’µ”v",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* ’µ”v */
	if (RuleData::chkRuleApplied("chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’µ”v", get_yaku_han("chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 2, false);
			}
		));
	/* ‹Ø”v */
	if (RuleData::chkRuleApplied("chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹Ø”v", get_yaku_han("chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 3, false);
			}
		));
	/* ’¸ŽO */
	if (RuleData::chkRuleApplied("tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’¸ŽO", get_yaku_han("tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* “ñFŒÜ˜A */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹â’ò", get_yaku_han("silver_ladder"),
			"“ñFŽl˜A", "“ñFŽO˜A", "‘ÎX˜a",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_bichrome5[0], 90, 8, 1);
			}
		));
	/* “ñFŽl˜A */
	if (RuleData::chkRuleApplied("nishoku_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ñFŽl˜A", get_yaku_han("nishoku_sanrenkoh"),
			"“ñFŽO˜A", "‘ÎX˜a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* “ñFŽO˜A */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ñFŽO˜A", get_yaku_han("silver_ladder"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* “ñFŽl’µ”v */
	if (RuleData::chkRuleApplied("nishoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ñFŽl’µ”v", get_yaku_han("nishoku_suuchaopaikoh"),
			"“ñF’µ”v", "‘ÎX˜a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* “ñF’µ”v */
	if (RuleData::chkRuleApplied("nishoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ñF’µ”v", get_yaku_han("nishoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* “ñF‹Ø”v */
	if (RuleData::chkRuleApplied("nishoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ñF‹Ø”v", get_yaku_han("nishoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 3, false);
			}
		));
	/* “ñF’¸ŽO */
	if (RuleData::chkRuleApplied("nishoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ñF’¸ŽO", get_yaku_han("nishoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* ŽOFŒÜ˜A */
	if (RuleData::chkRuleApplied("sky_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰_’ò", get_yaku_han("sky_ladder"),
			"ŽOFŽl˜A", "ŽOF˜A", "‘ÎX˜a",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_trichrome5[0], 150, 8, 1);
			}
		));
	/* ŽOFŽl˜A */
	if (RuleData::chkRuleApplied("sanshoku_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOFŽl˜A", get_yaku_han("sanshoku_suurenkoh"),
			"ŽOF˜A", "‘ÎX˜a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* ŽOF˜A */
	if (RuleData::chkRuleApplied("sanshoku_renkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOF˜A", get_yaku_han("sanshoku_renkoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* ŽOFŽl’µ”v */
	if (RuleData::chkRuleApplied("sanshoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOFŽl’µ”v", get_yaku_han("sanshoku_suuchaopaikoh"),
			"ŽOF’µ”v", "‘ÎX˜a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* ŽOF’µ”v */
	if (RuleData::chkRuleApplied("sanshoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOF’µ”v", get_yaku_han("sanshoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));
	/* ŽOF‹Ø”v */
	if (RuleData::chkRuleApplied("sanshoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOF‹Ø”v", get_yaku_han("sanshoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 3, false);
			}
		));
	/* ŽOF’¸ŽO */
	if (RuleData::chkRuleApplied("sanshoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOF’¸ŽO", get_yaku_han("sanshoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* “ñ“¯ */
	if (RuleData::chkRuleApplied("ryandoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ñ“¯", get_yaku_han("ryandoukoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const int suit[3][2] = {
					{TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES},
					{TILE_SUIT_CHARACTERS, TILE_SUIT_BAMBOOS},
					{TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS},
				};
				for (int k = 0; k < 3; k++) {
					int j = 0;
					for (int i = 1; i <= 9; i++)
						if ((analysis->KeziCount[i + suit[k][0]] >= 1) &&
							(analysis->KeziCount[i + suit[k][1]] >= 1)) ++j;
					if (j == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ä ƒ|ƒ“ */
	if (RuleData::chkRuleApplied("uikyou_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ä ƒ|ƒ“", get_yaku_han("uikyou_toitoi"),
			"ŽOF“¯", "‘ÎX˜a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_BAMBOOS] >= 1))
						if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS + 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_CIRCLES + 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_BAMBOOS + 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_CHARACTERS - 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_CIRCLES - 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_BAMBOOS - 1] >= 1))
							yakuFlag = true;
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* ”ªåæ•—‰ßŠC */
	if (RuleData::chkRuleApplied("baxian_chengfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”ªåæ•—‰ßŠC", get_yaku_han("baxian_chengfeng"),
			"‘ÎX˜a", "ŽOF“¯",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1)) );
			}
		));
	/* ”ªå‹Y–P */
	if (RuleData::chkRuleApplied("baxian_xifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”ªå‹Y–P", get_yaku_han("baxian_xifeng"),
			"‘ÎX˜a", "ŽOF“¯",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* ”ªå‹YŽl–P */
	if (RuleData::chkRuleApplied("baxian_xisifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”ªå‹YŽl–P", get_yaku_han("baxian_xisifeng"),
			"‘ÎX˜a", "ŽOF“¯", "”ªå‹Y–P",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KangziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* ”ªå‹Y‹à‹T */
	if (RuleData::chkRuleApplied("baxian_xijingui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”ªå‹Y‹à‹T", get_yaku_han("baxian_xijingui"),
			"‘ÎX˜a", "ŽOF“¯", "ä ƒ|ƒ“",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* ”ªå“¬–k“l */
	if (RuleData::chkRuleApplied("baxian_dou_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”ªå“¬–k“l", get_yaku_han("baxian_dou_beidou"),
			"‘ÎX˜a", "ŽOF“¯", "ä ƒ|ƒ“",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) );
			}
		));
	/* ”ªåãÚŽñ */
	if (RuleData::chkRuleApplied("baxian_jushou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”ªåãÚŽñ", get_yaku_han("baxian_jushou"),
			"‘ÎX˜a", "ŽOF“¯", "”ªå‰ßŠC",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					analysis->PlayerStat->FlowerFlag.Spring && analysis->PlayerStat->FlowerFlag.Summer &&
					analysis->PlayerStat->FlowerFlag.Autumn && analysis->PlayerStat->FlowerFlag.Winter &&
					analysis->PlayerStat->FlowerFlag.Plum && analysis->PlayerStat->FlowerFlag.Orchid &&
					analysis->PlayerStat->FlowerFlag.Chrys && analysis->PlayerStat->FlowerFlag.Bamboo );
			}
		));
}
