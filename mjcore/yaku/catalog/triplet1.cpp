#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_1() {
	/* 四暗刻 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"四暗刻", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"対々和", "三暗刻",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 4);
		}
	));
	/* 四暗刻単騎 */
	if (RuleData::chkRuleApplied("double_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四暗刻単騎", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"四暗刻", "対々和", "三暗刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && (
					(analysis->GameStat->TianHuFlag) ||
					((analysis->Machi == yaku::yakuCalculator::machiTanki) && (!analysis->MachiInfo.FuritenFlag))
					);
			}
		));
	/* 三暗刻 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"三暗刻", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 3);
		}
	));

	// ---------------------------------------------------------------------

	/* 五連刻 */
	auto wulianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * 4); k++)
					if ((analysis->DuiziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + k + step * 0] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + k + step * 1] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + k + step * 2] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + k + step * 3] >= 1) &&
						(analysis->DuiziCount[(int)(parsedat[i * fldsize + 4] - '0') * TILE_SUIT_STEP + k + step * 4] >= 1) )
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
					if ((analysis->KeziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + k + step * 0] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + k + step * 1] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + k + step * 2] >= 1) &&
						((!suurnkoh)||(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + k + step * 3] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("golden_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金梯", get_yaku_han("golden_ladder"),
			"清一色", "対々和", "小四連刻", "四連刻", "三連刻", "小三連刻",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_monochrome5[0], 3, 8, 1);
			}
		));
	/* 四連刻 */
	auto suurenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
		};
	if (RuleData::chkRule("suurenkoh", "8han_incl_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四連刻", yaku::yakuCalculator::Yaku::yval_6han /* 対々和込み8飜 */,
			"小四連刻", "三連刻", "小三連刻",
			suurenkoh
		));
	else if (RuleData::chkRuleApplied("suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四連刻", get_yaku_han("suurenkoh"),
			"対々和", "小四連刻", "三連刻", "小三連刻",
			suurenkoh
		));
	/* 四連刻両面待ち */
	if (RuleData::chkRuleApplied("suurenkoh_double"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四連刻両面待ち", get_yaku_han("suurenkoh_double"),
			"清一色", "対々和", "金梯", "四連刻", "小四連刻", "三連刻", "小三連刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i = parsedat_monochrome5.begin(); i != parsedat_monochrome5.end(); i++) {
					for (int k = 1; k <= 5; k++)
						if ((analysis->DuiziCount[(int)((*i)[0] - '0') * TILE_SUIT_STEP + k + 0] >= 1) &&
							(analysis->KeziCount[(int)((*i)[1] - '0') * TILE_SUIT_STEP + k + 1] >= 1) &&
							(analysis->KeziCount[(int)((*i)[2] - '0') * TILE_SUIT_STEP + k + 2] >= 1) &&
							(analysis->KeziCount[(int)((*i)[3] - '0') * TILE_SUIT_STEP + k + 3] >= 1) &&
							(analysis->DuiziCount[(int)((*i)[4] - '0') * TILE_SUIT_STEP + k + 4] >= 1) &&
							((analysis->TsumoHai->tile == (tileCode)((int)((*i)[0] - '0') * TILE_SUIT_STEP + k + 0)) ||
							(analysis->TsumoHai->tile == (tileCode)((int)((*i)[4] - '0') * TILE_SUIT_STEP + k + 4)))
							)
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 三連刻 */
	auto sanrenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
		};
	if (RuleData::chkRuleApplied("sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三連刻", get_yaku_han("sanrenkoh"),
			"小三連刻",
			sanrenkoh
		));
	/* 小四連刻 */
	if (RuleData::chkRuleApplied("shou_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"小四連刻", get_yaku_han("shou_suurenkoh"),
			"三連刻", "小三連刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					for (int k = 1; k <= 6; k++)
						if ((analysis->DuiziCount[i + k + 0] >= 1) && (analysis->KeziCount[i + k + 1] >= 1) &&
							(analysis->KeziCount[i + k + 2] >= 1) && (analysis->DuiziCount[i + k + 3] >= 1))
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 小三連刻 */
	if (RuleData::chkRuleApplied("shou_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"小三連刻", get_yaku_han("shou_sanrenkoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					for (int k = 1; k <= 7; k++)
						if ((analysis->DuiziCount[i + k + 0] >= 1) && (analysis->DuiziCount[i + k + 1] >= 1) &&
							(analysis->DuiziCount[i + k + 2] >= 1))
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 鏡音レン */
	if (RuleData::chkRuleApplied("kagamine_len"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鏡音レン", get_yaku_han("kagamine_len"),
			/* 三連刻に追加する役 */
			[sanrenkoh](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));

	// ---------------------------------------------------------------------

	/* レアな方の三色 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"三色同刻", get_yaku_han("sanshoku_doukoh"),
		"三色小同刻",
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
	/* 四色同刻（えっ */
	if (RuleData::chkRuleApplied("yonshoku_doukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四色同刻", get_yaku_han("yonshoku_doukoh"),
			"三色同刻", "三色小同刻", "対々和",
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
	/* 三色小同刻 */
	if (RuleData::chkRuleApplied("sanshoku_shoudoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色小同刻", get_yaku_han("sanshoku_shoudoukoh"),
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

	/* 三元和・四喜和判定用 */
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
	/* 小三元 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"小三元", yaku::yakuCalculator::Yaku::yval_2han,
		"青春に悔いなし", "青春に悔いあり", /* 役牌2つは必ず複合する */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* 大三元 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"大三元", yaku::yakuCalculator::Yaku::yval_yakuman,
		"役牌・白", "役牌・發", "役牌・中", "青春に悔いなし", "青春に悔いあり",
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
	/* 門前大三元 */
	if (RuleData::chkRuleApplied("menzen_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"門前大三元", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"大三元",
			[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (DragonCnt(analysis) == 6);
			}
		));
	/* 小四喜 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"小四喜", yaku::yakuCalculator::Yaku::yval_yakuman,
		"混一色", "盗難車",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
	/* 天虎 */
	if (RuleData::chkRuleApplied("heaven_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"天虎", get_yaku_han("heaven_tiger"),
			"混一色", "小四喜", "字一色", "役牌・發", "盗難車",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* 大怪湖 */
	if (RuleData::chkRuleApplied("daikaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大怪湖", get_yaku_han("daikaiko"),
			"混一色", "小四喜", "字一色", "役牌・白", "盗難車",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* 千年虫 */
	if (RuleData::chkRuleApplied("y2kbug"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"千年虫", get_yaku_han("y2kbug"),
			"混一色", "小四喜", "混老頭", "盗難車",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[CharacterNine] >= 1)&&(analysis->KeziCount[SouthWind] == 1));
			}
		));
	/* 大四喜 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"大四喜", (RuleData::chkRuleApplied("double_yakuman")) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"混一色", "対々和", "役牌・場風", "役牌・自風", "役牌・開門風", "役牌・裏風", "盗難車",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
	/* 弾葯 */
	if (RuleData::chkRuleApplied("dan_yaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"弾葯", get_yaku_han("dan_yaku"),
			"混一色", "対々和", "役牌・場風", "役牌・自風", "役牌・開門風", "役牌・裏風", "大四喜", "混老頭", "盗難車",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[CharacterNine] >= 1));
			}
		));
	/* 仙人掌 */
	if (RuleData::chkRuleApplied("cactus"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"仙人掌", get_yaku_han("cactus"),
			"混一色", "対々和", "役牌・場風", "役牌・自風", "役牌・開門風", "役牌・裏風", "大四喜", "字一色", "盗難車",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[RedDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 対々和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"対々和", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1);
		}
	));

	// ---------------------------------------------------------------------

	/* 役牌 */
	auto chkYakuhai = RuleData::chkRuleApplied("exposed_yakuhai") ?
		(std::function<const Int8ByTile& (const MENTSU_ANALYSIS* const)>)
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->KeziCount;} :
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->AnKeziCount;};
	auto bakaze =
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"役牌・場風", yaku::yakuCalculator::Yaku::yval_1han,
		bakaze
	));
	auto jikaze =
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
				analysis->player, analysis->GameStat->GameRound))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"役牌・自風", yaku::yakuCalculator::Yaku::yval_1han,
		jikaze
	));
	YAKUFUNC kaimenkaze, urakaze;
	if (RuleData::chkRuleApplied("kaimenkaze")) {
		kaimenkaze =
			[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"役牌・開門風", get_yaku_han("kaimenkaze"),
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
			[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					RelativePositionOf(analysis->player, sOpposite), analysis->GameStat->GameRound))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"役牌・裏風", get_yaku_han("urakaze"),
			urakaze
		));
	} else {
		urakaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"役牌・白", yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[WhiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"役牌・發", yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[GreenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"役牌・中", yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[RedDragon] >= 1);
		}
	));

	// ---------------------------------------------------------------------

	/* 新沼けんじ */
	if (RuleData::chkRuleApplied("niinuma_kenji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"新沼けんじ", get_yaku_han("niinuma_kenji"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* 陣太鼓 */
	if (RuleData::chkRuleApplied("jindaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"陣太鼓", get_yaku_han("jindaiko"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* 小三風 */
	if (RuleData::chkRuleApplied("xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"小三風", get_yaku_han("xiaosanfeng"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* 三風刻 */
	if (RuleData::chkRuleApplied("three_winds"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三風刻", get_yaku_han("three_winds"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 6);
			}
		));
	/* 客風三刻 */
	if (RuleData::chkRuleApplied("three_otakaze"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"客風三刻", get_yaku_han("three_otakaze"),
			"三風刻",
			[WindCnt, bakaze, jikaze, kaimenkaze, urakaze]
			(const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 6) &&
					(!bakaze(analysis)) &&
					(!jikaze(analysis)) &&
					(!kaimenkaze(analysis)) &&
					(!urakaze(analysis)));
			}
		));
	/* 客風小三刻 */
	if (RuleData::chkRuleApplied("otakaze_xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"客風小三刻", get_yaku_han("otakaze_xiaosanfeng"),
			"小三刻",
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

	/* 西中島南方 */
	if (RuleData::chkRuleApplied("nishinakajima"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"西中島南方", get_yaku_han("nishinakajima"),
			"役牌・場風", "役牌・自風", "役牌・中",
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
	/* 雪月花 */
	if (RuleData::chkRuleApplied("setsugetsuka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"雪月花", get_yaku_han("setsugetsuka"),
			"役牌・白",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1));
			}
		));
	/* 風花雪月 */
	if (RuleData::chkRuleApplied("fuuka_setsugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"風花雪月", get_yaku_han("fuuka_setsugetsu"),
			"雪月花", "対々和", "役牌・場風", "役牌・自風",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* 花鳥風月 */
	if (RuleData::chkRuleApplied("kachou_fuugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"花鳥風月", get_yaku_han("kachou_fuugetsu"),
			"雪月花", "対々和", "役牌・場風", "役牌・自風",
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* 東北自動車道 */
	if (RuleData::chkRuleApplied("tohoku_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東北自動車道", get_yaku_han("tohoku_expwy"),
			"対々和", "混一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* 東関東自動車道 */
	if (RuleData::chkRuleApplied("higashikanto_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東関東自動車道", get_yaku_han("higashikanto_expwy"),
			"対々和", "混一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1));
			}
		));
	/* 中国系のおめでたい役 */
	if (RuleData::chkRuleApplied("koukei_hougyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"紅鶏報暁", get_yaku_han("koukei_hougyou"),
			"対々和", "混一色", "絶一門", "役牌・白", "役牌・中", "金鶏報暁", "銀鶏報春", "白鳥",
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
			"金鶏報暁", get_yaku_han("kinkei_hougyou"),
			"役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("ginkei_houshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"銀鶏報春", get_yaku_han("ginkei_houshun"),
			"役牌・白", "白鳥",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->PlayerStat->FlowerFlag.Spring));
			}
		));

	// ---------------------------------------------------------------------

	/* 四跳牌刻 */
	if (RuleData::chkRuleApplied("suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四跳牌刻", get_yaku_han("suuchaopaikoh"),
			"対々和", "跳牌刻",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* 跳牌刻 */
	if (RuleData::chkRuleApplied("chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"跳牌刻", get_yaku_han("chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 2, false);
			}
		));
	/* 筋牌刻 */
	if (RuleData::chkRuleApplied("chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"筋牌刻", get_yaku_han("chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 3, false);
			}
		));
	/* 頂三刻 */
	if (RuleData::chkRuleApplied("tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"頂三刻", get_yaku_han("tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 二色五連刻 */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"銀梯", get_yaku_han("silver_ladder"),
			"二色四連刻", "二色三連刻", "対々和",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_bichrome5[0], 90, 8, 1);
			}
		));
	/* 二色四連刻 */
	if (RuleData::chkRuleApplied("nishoku_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色四連刻", get_yaku_han("nishoku_sanrenkoh"),
			"二色三連刻", "対々和",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* 二色三連刻 */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色三連刻", get_yaku_han("silver_ladder"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* 二色四跳牌刻 */
	if (RuleData::chkRuleApplied("nishoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色四跳牌刻", get_yaku_han("nishoku_suuchaopaikoh"),
			"二色跳牌刻", "対々和",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* 二色跳牌刻 */
	if (RuleData::chkRuleApplied("nishoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色跳牌刻", get_yaku_han("nishoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* 二色筋牌刻 */
	if (RuleData::chkRuleApplied("nishoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色筋牌刻", get_yaku_han("nishoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 3, false);
			}
		));
	/* 二色頂三刻 */
	if (RuleData::chkRuleApplied("nishoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色頂三刻", get_yaku_han("nishoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 三色五連刻 */
	if (RuleData::chkRuleApplied("sky_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"雲梯", get_yaku_han("sky_ladder"),
			"三色四連刻", "三色連刻", "対々和",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_trichrome5[0], 150, 8, 1);
			}
		));
	/* 三色四連刻 */
	if (RuleData::chkRuleApplied("sanshoku_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色四連刻", get_yaku_han("sanshoku_suurenkoh"),
			"三色連刻", "対々和",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* 三色連刻 */
	if (RuleData::chkRuleApplied("sanshoku_renkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色連刻", get_yaku_han("sanshoku_renkoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* 三色四跳牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色四跳牌刻", get_yaku_han("sanshoku_suuchaopaikoh"),
			"三色跳牌刻", "対々和",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* 三色跳牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色跳牌刻", get_yaku_han("sanshoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));
	/* 三色筋牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色筋牌刻", get_yaku_han("sanshoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 3, false);
			}
		));
	/* 三色頂三刻 */
	if (RuleData::chkRuleApplied("sanshoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色頂三刻", get_yaku_han("sanshoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 二同刻 */
	if (RuleData::chkRuleApplied("ryandoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二同刻", get_yaku_han("ryandoukoh"),
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
	/* 茴香ポン */
	if (RuleData::chkRuleApplied("uikyou_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"茴香ポン", get_yaku_han("uikyou_toitoi"),
			"三色同刻", "対々和",
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

	/* 八仙乗風過海 */
	if (RuleData::chkRuleApplied("baxian_chengfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"八仙乗風過海", get_yaku_han("baxian_chengfeng"),
			"対々和", "三色同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1)) );
			}
		));
	/* 八仙戯鳳 */
	if (RuleData::chkRuleApplied("baxian_xifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"八仙戯鳳", get_yaku_han("baxian_xifeng"),
			"対々和", "三色同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* 八仙戯四鳳 */
	if (RuleData::chkRuleApplied("baxian_xisifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"八仙戯四鳳", get_yaku_han("baxian_xisifeng"),
			"対々和", "三色同刻", "八仙戯鳳",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KangziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* 八仙戯金亀 */
	if (RuleData::chkRuleApplied("baxian_xijingui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"八仙戯金亀", get_yaku_han("baxian_xijingui"),
			"対々和", "三色同刻", "茴香ポン",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* 八仙闘北斗 */
	if (RuleData::chkRuleApplied("baxian_dou_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"八仙闘北斗", get_yaku_han("baxian_dou_beidou"),
			"対々和", "三色同刻", "茴香ポン",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) );
			}
		));
	/* 八仙聚首 */
	if (RuleData::chkRuleApplied("baxian_jushou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"八仙聚首", get_yaku_han("baxian_jushou"),
			"対々和", "三色同刻", "八仙過海",
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
