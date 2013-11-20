#include "../catalog.h"

#include "../../func.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_1() {
	/* 四暗刻 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("四暗刻"), yaku::yakuCalculator::Yaku::yval_64,
		_T("碰碰和"), _T("三暗刻"), _T("双暗刻"), _T("門前清"),
#else /* GUOBIAO */
		_T("四暗刻"), yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		_T("対々和"), _T("三暗刻"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 4);
		}
	));
#ifndef GUOBIAO
	/* 四暗刻単騎 */
	if (RuleData::chkRuleApplied("double_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四暗刻単騎"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("四暗刻"), _T("対々和"), _T("三暗刻"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && (
					(analysis->GameStat->TianHuFlag) ||
					((analysis->Machi == yaku::yakuCalculator::machiTanki) && (!analysis->MachiInfo.FuritenFlag))
					);
			}
		));
#endif /* GUOBIAO */
	/* 三暗刻 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("三暗刻"), yaku::yakuCalculator::Yaku::yval_16,
		_T("双暗刻"),
#else /* GUOBIAO */
		_T("三暗刻"), yaku::yakuCalculator::Yaku::yval_2han,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 3);
		}
	));
#ifdef GUOBIAO
	/* 双暗刻 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("双暗刻"), yaku::yakuCalculator::Yaku::yval_2,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 2);
		}
	));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 五連刻 */
	auto wulianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * 4); k++)
					if ((analysis->DuiziCount[(int)(parsedat[i * fldsize + 0] - _T('0')) * TileSuitStep + k + step * 0] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - _T('0')) * TileSuitStep + k + step * 1] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - _T('0')) * TileSuitStep + k + step * 2] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - _T('0')) * TileSuitStep + k + step * 3] >= 1) &&
						(analysis->DuiziCount[(int)(parsedat[i * fldsize + 4] - _T('0')) * TileSuitStep + k + step * 4] >= 1) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#endif /* GUOBIAO */
	auto lianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suurnkoh) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * (suurnkoh ? 3 : 2)); k++)
					if ((analysis->KeziCount[(int)(parsedat[i * fldsize + 0] - _T('0')) * TileSuitStep + k + step * 0] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - _T('0')) * TileSuitStep + k + step * 1] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - _T('0')) * TileSuitStep + k + step * 2] >= 1) &&
						((!suurnkoh)||(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - _T('0')) * TileSuitStep + k + step * 3] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("golden_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金梯"), get_yaku_han("golden_ladder"),
			_T("清一色"), _T("対々和"), _T("小四連刻"), _T("四連刻"), _T("三連刻"), _T("小三連刻"),
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_monochrome5[0], 3, 8, 1);
			}
		));
#endif /* GUOBIAO */
	/* 四連刻 */
	auto suurenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
		};
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("一色四節高"), yaku::yakuCalculator::Yaku::yval_48,
		_T("碰碰和"), _T("一色三節高"),
		suurenkoh
	));
#else /* GUOBIAO */
	if (RuleData::chkRule("suurenkoh", "8han_incl_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四連刻"), yaku::yakuCalculator::Yaku::yval_6han /* 対々和込み8飜 */,
			_T("小四連刻"), _T("三連刻"), _T("小三連刻"),
			suurenkoh
		));
	else if (RuleData::chkRuleApplied("suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四連刻"), get_yaku_han("suurenkoh"),
			_T("対々和"), _T("小四連刻"), _T("三連刻"), _T("小三連刻"),
			suurenkoh
		));
	/* 四連刻両面待ち */
	if (RuleData::chkRuleApplied("suurenkoh_double"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四連刻両面待ち"), get_yaku_han("suurenkoh_double"),
			_T("清一色"), _T("対々和"), _T("金梯"), _T("四連刻"), _T("小四連刻"), _T("三連刻"), _T("小三連刻"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i = parsedat_monochrome5.begin(); i != parsedat_monochrome5.end(); i++) {
					for (int k = 1; k <= 5; k++)
						if ((analysis->DuiziCount[(int)((*i)[0] - _T('0')) * TileSuitStep + k + 0] >= 1) &&
							(analysis->KeziCount[(int)((*i)[1] - _T('0')) * TileSuitStep + k + 1] >= 1) &&
							(analysis->KeziCount[(int)((*i)[2] - _T('0')) * TileSuitStep + k + 2] >= 1) &&
							(analysis->KeziCount[(int)((*i)[3] - _T('0')) * TileSuitStep + k + 3] >= 1) &&
							(analysis->DuiziCount[(int)((*i)[4] - _T('0')) * TileSuitStep + k + 4] >= 1) &&
							((analysis->TsumoHai->tile == (TileCode)((int)((*i)[0] - _T('0')) * TileSuitStep + k + 0)) ||
							(analysis->TsumoHai->tile == (TileCode)((int)((*i)[4] - _T('0')) * TileSuitStep + k + 4)))
							)
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */
	/* 三連刻 */
	auto sanrenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
		};
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("sanrenkoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("一色三節高"), yaku::yakuCalculator::Yaku::yval_24,
#else /* GUOBIAO */
			_T("三連刻"), get_yaku_han("sanrenkoh"),
			_T("小三連刻"),
#endif /* GUOBIAO */
			sanrenkoh
		));
#ifndef GUOBIAO
	/* 小四連刻 */
	if (RuleData::chkRuleApplied("shou_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("小四連刻"), get_yaku_han("shou_suurenkoh"),
			_T("三連刻"), _T("小三連刻"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
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
			_T("小三連刻"), get_yaku_han("shou_sanrenkoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
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
			_T("鏡音レン"), get_yaku_han("kagamine_len"),
			/* 三連刻に追加する役 */
			[sanrenkoh](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* レアな方の三色 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("三同刻"), yaku::yakuCalculator::Yaku::yval_16,
		_T("双同刻"),
#else /* GUOBIAO */
		_T("三色同刻"), get_yaku_han("sanshoku_doukoh"),
		_T("三色小同刻"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 9; i++)
				if ((analysis->KeziCount[i + TileSuitCharacters] >= 1) &&
					(analysis->KeziCount[i + TileSuitCircles] >= 1) &&
					(analysis->KeziCount[i + TileSuitBamboos] >= 1))
					yakuFlag = true;
			return yakuFlag;
		}
	));
#ifndef GUOBIAO
	/* 四色同刻（えっ */
	if (RuleData::chkRuleApplied("yonshoku_doukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四色同刻"), get_yaku_han("yonshoku_doukoh"),
			_T("三色同刻"), _T("三色小同刻"), _T("対々和"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TileSuitCharacters] >= 1) &&
						(analysis->KeziCount[i + TileSuitCircles] >= 1) &&
						(analysis->KeziCount[i + TileSuitBamboos] >= 1) &&
						(analysis->KeziCount[WhiteDragon] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 三色小同刻 */
	if (RuleData::chkRuleApplied("sanshoku_shoudoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色小同刻"), get_yaku_han("sanshoku_shoudoukoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->DuiziCount[i + TileSuitCharacters] >= 1) &&
						(analysis->DuiziCount[i + TileSuitCircles] >= 1) &&
						(analysis->DuiziCount[i + TileSuitBamboos] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */

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
#ifdef GUOBIAO
		_T("小三元"), yaku::yakuCalculator::Yaku::yval_64,
		_T("箭刻"), _T("双箭刻"),
#else /* GUOBIAO */
		_T("小三元"), yaku::yakuCalculator::Yaku::yval_2han,
		_T("青春に悔いなし"), _T("青春に悔いあり"), /* 役牌2つは必ず複合する */
#endif /* GUOBIAO */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* 大三元 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("大三元"), yaku::yakuCalculator::Yaku::yval_88,
		_T("箭刻"), _T("双箭刻"),
#else /* GUOBIAO */
		_T("大三元"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("役牌・白"), _T("役牌・發"), _T("役牌・中"), _T("青春に悔いなし"), _T("青春に悔いあり"),
#endif /* GUOBIAO */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
#ifndef GUOBIAO
	/* 門前大三元 */
	if (RuleData::chkRuleApplied("menzen_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("門前大三元"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("大三元"),
			[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (DragonCnt(analysis) == 6);
			}
		));
#endif /* GUOBIAO */
	/* 小四喜 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("小四風会"), yaku::yakuCalculator::Yaku::yval_64,
		_T("三風刻"), _T("幺九刻"), _T("幺九刻x2"), _T("幺九刻x3"),
#else /* GUOBIAO */
		_T("小四喜"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("混一色"), _T("盗難車"),
#endif /* GUOBIAO */
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
#ifndef GUOBIAO
	/* 天虎 */
	if (RuleData::chkRuleApplied("heaven_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("天虎"), get_yaku_han("heaven_tiger"),
			_T("混一色"), _T("小四喜"), _T("字一色"), _T("役牌・發"), _T("盗難車"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* 大怪湖 */
	if (RuleData::chkRuleApplied("daikaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大怪湖"), get_yaku_han("daikaiko"),
			_T("混一色"), _T("小四喜"), _T("字一色"), _T("役牌・白"), _T("盗難車"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* 千年虫 */
	if (RuleData::chkRuleApplied("y2kbug"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("千年虫"), get_yaku_han("y2kbug"),
			_T("混一色"), _T("小四喜"), _T("混老頭"), _T("盗難車"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[CharacterNine] >= 1)&&(analysis->KeziCount[SouthWind] == 1));
			}
		));
#endif /* GUOBIAO */
	/* 大四喜 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("大四風会"), yaku::yakuCalculator::Yaku::yval_88,
		_T("圏風刻"), _T("門風刻"), _T("三風刻"), _T("碰碰和"), _T("幺九刻"), _T("幺九刻x2"), _T("幺九刻x3"), _T("幺九刻x4"),
#else /* GUOBIAO */
		_T("大四喜"), (RuleData::chkRuleApplied("double_yakuman")) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("混一色"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"), _T("役牌・開門風"), _T("役牌・裏風"), _T("盗難車"),
#endif /* GUOBIAO */
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
#ifndef GUOBIAO
	/* 弾葯 */
	if (RuleData::chkRuleApplied("dan_yaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("弾葯"), get_yaku_han("dan_yaku"),
			_T("混一色"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"), _T("役牌・開門風"), _T("役牌・裏風"), _T("大四喜"), _T("混老頭"), _T("盗難車"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[CharacterNine] >= 1));
			}
		));
	/* 仙人掌 */
	if (RuleData::chkRuleApplied("cactus"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("仙人掌"), get_yaku_han("cactus"),
			_T("混一色"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"), _T("役牌・開門風"), _T("役牌・裏風"), _T("大四喜"), _T("字一色"), _T("盗難車"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 対々和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("碰碰和"), yaku::yakuCalculator::Yaku::yval_6,
#else /* GUOBIAO */
		_T("対々和"), yaku::yakuCalculator::Yaku::yval_2han,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKezi == SizeOfMeldBuffer - 1);
		}
	));

	// ---------------------------------------------------------------------

	/* 役牌 */
#ifdef GUOBIAO
	auto chkYakuhai = [](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->KeziCount;};
#else /* GUOBIAO */
	auto chkYakuhai = RuleData::chkRuleApplied("exposed_yakuhai") ?
#ifdef _MSC_VER
		(std::function<const Int8ByTile& (const MENTSU_ANALYSIS* const)>)
#endif
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->KeziCount;} :
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->AnKeziCount;};
#endif /* GUOBIAO */
	auto bakaze =
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("圏風刻"), yaku::yakuCalculator::Yaku::yval_2,
#else /* GUOBIAO */
		_T("役牌・場風"), yaku::yakuCalculator::Yaku::yval_1han,
#endif /* GUOBIAO */
		bakaze
	));
	auto jikaze =
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile((uint8_t)analysis->GameStat->playerwind(analysis->player))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("門風刻"), yaku::yakuCalculator::Yaku::yval_2,
#else /* GUOBIAO */
		_T("役牌・自風"), yaku::yakuCalculator::Yaku::yval_1han,
#endif /* GUOBIAO */
		jikaze
	));
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("双箭刻"), yaku::yakuCalculator::Yaku::yval_6,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkYakuhai(analysis)[WhiteDragon] +
				chkYakuhai(analysis)[GreenDragon] +
				chkYakuhai(analysis)[RedDragon] == 2;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("箭刻"), yaku::yakuCalculator::Yaku::yval_2,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkYakuhai(analysis)[WhiteDragon] +
				chkYakuhai(analysis)[GreenDragon] +
				chkYakuhai(analysis)[RedDragon] == 1;
		}
	));
#else /* GUOBIAO */
	YAKUFUNC kaimenkaze, urakaze;
	if (RuleData::chkRuleApplied("kaimenkaze")) {
		kaimenkaze =
			[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)analysis->GameStat->playerwind(analysis->GameStat->WaremePlayer))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("役牌・開門風"), get_yaku_han("kaimenkaze"),
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
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)analysis->GameStat->playerwind(
					RelativePositionOf(analysis->player, sOpposite)))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("役牌・裏風"), get_yaku_han("urakaze"),
			urakaze
		));
	} else {
		urakaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("役牌・白"), yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[WhiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("役牌・發"), yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[GreenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("役牌・中"), yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[RedDragon] >= 1);
		}
	));
#endif /* GUOBIAO */

#ifdef GUOBIAO
	/* 幺九刻 */
	auto YaoJiuKe =
		[chkYakuhai, bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> int {
			return chkYakuhai(analysis)[CharacterOne] + chkYakuhai(analysis)[CharacterNine] +
				chkYakuhai(analysis)[CircleOne] + chkYakuhai(analysis)[CircleNine] +
				chkYakuhai(analysis)[BambooOne] + chkYakuhai(analysis)[BambooNine] +
				chkYakuhai(analysis)[EastWind] + chkYakuhai(analysis)[SouthWind] +
				chkYakuhai(analysis)[WestWind] + chkYakuhai(analysis)[NorthWind] -
				(bakaze(analysis) ? 1 : 0) - (jikaze(analysis) ? 1 : 0);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻"), yaku::yakuCalculator::Yaku::yval_1,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 1;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻x2"), yaku::yakuCalculator::Yaku::yval_2,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 2;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻x3"), yaku::yakuCalculator::Yaku::yval_3,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 3;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻x4"), yaku::yakuCalculator::Yaku::yval_4,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 4;
		}
	));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 新沼けんじ */
	if (RuleData::chkRuleApplied("niinuma_kenji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("新沼けんじ"), get_yaku_han("niinuma_kenji"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* 陣太鼓 */
	if (RuleData::chkRuleApplied("jindaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("陣太鼓"), get_yaku_han("jindaiko"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* 小三風 */
	if (RuleData::chkRuleApplied("xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("小三風"), get_yaku_han("xiaosanfeng"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* 三風刻 */
	if (RuleData::chkRuleApplied("three_winds"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("三風刻"), yaku::yakuCalculator::Yaku::yval_12,
#else /* GUOBIAO */
			_T("三風刻"), get_yaku_han("three_winds"),
#endif /* GUOBIAO */
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 6);
			}
		));
#ifndef GUOBIAO
	/* 客風三刻 */
	if (RuleData::chkRuleApplied("three_otakaze"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("客風三刻"), get_yaku_han("three_otakaze"),
			_T("三風刻"),
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
			_T("客風小三刻"), get_yaku_han("otakaze_xiaosanfeng"),
			_T("小三刻"),
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
			_T("西中島南方"), get_yaku_han("nishinakajima"),
			_T("役牌・場風"), _T("役牌・自風"), _T("役牌・中"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->GameStat->GameRound / 4 == sWest) &&
					(analysis->GameStat->playerwind(analysis->player) == sSouth))
					yakuFlag = true;
				if ((analysis->GameStat->GameRound / 4 == sSouth) &&
					(analysis->GameStat->playerwind(analysis->player) == sWest))
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
			_T("雪月花"), get_yaku_han("setsugetsuka"),
			_T("役牌・白"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1));
			}
		));
	/* 風花雪月 */
	if (RuleData::chkRuleApplied("fuuka_setsugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("風花雪月"), get_yaku_han("fuuka_setsugetsu"),
			_T("雪月花"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"),
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
			_T("花鳥風月"), get_yaku_han("kachou_fuugetsu"),
			_T("雪月花"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"),
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
			_T("東北自動車道"), get_yaku_han("tohoku_expwy"),
			_T("対々和"), _T("混一色"),
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
			_T("東関東自動車道"), get_yaku_han("higashikanto_expwy"),
			_T("対々和"), _T("混一色"),
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
			_T("紅鶏報暁"), get_yaku_han("koukei_hougyou"),
			_T("対々和"), _T("混一色"), _T("絶一門"), _T("役牌・白"), _T("役牌・中"), _T("金鶏報暁"), _T("銀鶏報春"), _T("白鳥"),
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
			_T("金鶏報暁"), get_yaku_han("kinkei_hougyou"),
			_T("役牌・中"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("ginkei_houshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("銀鶏報春"), get_yaku_han("ginkei_houshun"),
			_T("役牌・白"), _T("白鳥"),
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
			_T("四跳牌刻"), get_yaku_han("suuchaopaikoh"),
			_T("対々和"), _T("跳牌刻"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* 跳牌刻 */
	if (RuleData::chkRuleApplied("chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("跳牌刻"), get_yaku_han("chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 2, false);
			}
		));
	/* 筋牌刻 */
	if (RuleData::chkRuleApplied("chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("筋牌刻"), get_yaku_han("chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 3, false);
			}
		));
	/* 頂三刻 */
	if (RuleData::chkRuleApplied("tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("頂三刻"), get_yaku_han("tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 二色五連刻 */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("銀梯"), get_yaku_han("silver_ladder"),
			_T("二色四連刻"), _T("二色三連刻"), _T("対々和"),
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_bichrome5[0], 90, 8, 1);
			}
		));
	/* 二色四連刻 */
	if (RuleData::chkRuleApplied("nishoku_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色四連刻"), get_yaku_han("nishoku_sanrenkoh"),
			_T("二色三連刻"), _T("対々和"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* 二色三連刻 */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色三連刻"), get_yaku_han("silver_ladder"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* 二色四跳牌刻 */
	if (RuleData::chkRuleApplied("nishoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色四跳牌刻"), get_yaku_han("nishoku_suuchaopaikoh"),
			_T("二色跳牌刻"), _T("対々和"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* 二色跳牌刻 */
	if (RuleData::chkRuleApplied("nishoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色跳牌刻"), get_yaku_han("nishoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* 二色筋牌刻 */
	if (RuleData::chkRuleApplied("nishoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色筋牌刻"), get_yaku_han("nishoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 3, false);
			}
		));
	/* 二色頂三刻 */
	if (RuleData::chkRuleApplied("nishoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色頂三刻"), get_yaku_han("nishoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 三色五連刻 */
	if (RuleData::chkRuleApplied("sky_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("雲梯"), get_yaku_han("sky_ladder"),
			_T("三色四連刻"), _T("三色連刻"), _T("対々和"),
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_trichrome5[0], 150, 8, 1);
			}
		));
	/* 三色四連刻 */
	if (RuleData::chkRuleApplied("sanshoku_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色四連刻"), get_yaku_han("sanshoku_suurenkoh"),
			_T("三色連刻"), _T("対々和"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* 三色連刻 */
	if (RuleData::chkRuleApplied("sanshoku_renkoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("三色三節高"), yaku::yakuCalculator::Yaku::yval_8,
#else /* GUOBIAO */
			_T("三色連刻"), get_yaku_han("sanshoku_renkoh"),
#endif /* GUOBIAO */
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
#ifndef GUOBIAO
	/* 三色四跳牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色四跳牌刻"), get_yaku_han("sanshoku_suuchaopaikoh"),
			_T("三色跳牌刻"), _T("対々和"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* 三色跳牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色跳牌刻"), get_yaku_han("sanshoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));
	/* 三色筋牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色筋牌刻"), get_yaku_han("sanshoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 3, false);
			}
		));
	/* 三色頂三刻 */
	if (RuleData::chkRuleApplied("sanshoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色頂三刻"), get_yaku_han("sanshoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 二同刻 */
	if (RuleData::chkRuleApplied("ryandoukoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("双同刻"), yaku::yakuCalculator::Yaku::yval_2,
#else /* GUOBIAO */
			_T("二同刻"), get_yaku_han("ryandoukoh"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const int suit[3][2] = {
					{TileSuitCharacters, TileSuitCircles},
					{TileSuitCharacters, TileSuitBamboos},
					{TileSuitCircles, TileSuitBamboos},
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
#ifndef GUOBIAO
	/* 茴香ポン */
	if (RuleData::chkRuleApplied("uikyou_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("茴香") PON, get_yaku_han("uikyou_toitoi"),
			_T("三色同刻"), _T("対々和"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TileSuitCharacters] >= 1) &&
						(analysis->KeziCount[i + TileSuitCircles] >= 1) &&
						(analysis->KeziCount[i + TileSuitBamboos] >= 1))
						if ((analysis->KeziCount[i + TileSuitCharacters + 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitCircles + 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitBamboos + 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitCharacters - 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitCircles - 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitBamboos - 1] >= 1))
							yakuFlag = true;
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 八仙乗風過海 */
	if (RuleData::chkRuleApplied("baxian_chengfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八仙乗風過海"), get_yaku_han("baxian_chengfeng"),
			_T("対々和"), _T("三色同刻"),
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
			_T("八仙戯鳳"), get_yaku_han("baxian_xifeng"),
			_T("対々和"), _T("三色同刻"),
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
			_T("八仙戯四鳳"), get_yaku_han("baxian_xisifeng"),
			_T("対々和"), _T("三色同刻"), _T("八仙戯鳳"),
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
			_T("八仙戯金亀"), get_yaku_han("baxian_xijingui"),
			_T("対々和"), _T("三色同刻"), _T("茴香") PON,
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
			_T("八仙闘北斗"), get_yaku_han("baxian_dou_beidou"),
			_T("対々和"), _T("三色同刻"), _T("茴香") PON,
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
			_T("八仙聚首"), get_yaku_han("baxian_jushou"),
			_T("対々和"), _T("三色同刻"), _T("八仙過海"),
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
#endif /* GUOBIAO */
}
