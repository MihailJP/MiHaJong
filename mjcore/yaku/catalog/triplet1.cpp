﻿#include "../catalog.h"

#include "../../func.h"
#include "../../../common/strcode.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_1() {
	const auto liankeSuit = [](const char suitNum) -> TileSuit {
		switch (suitNum) {
		case '0':
			return TileSuit::characters;
		case '1':
			return TileSuit::circles;
		case '2':
			return TileSuit::bamboos;
		default:
			return TileSuit::invalid;
		}
	};

	// ---------------------------------------------------------------------

	/* 四暗刻 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("四暗刻"), 64_fenF,
		PengPengHu, _T("三暗刻"), _T("双暗刻"), _T("門前清"),
#else /* GUOBIAO */
		_T("四暗刻"), 1_yakumanM,
		_T("対々和"), _T("三暗刻"),
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::MachiType::shanpon) && (!analysis->TsumoAgariFlag())) ?
				1 : 0) == 4);
		}
	));
#ifndef GUOBIAO
	/* 四暗刻単騎 */
	if (RuleData::chkRuleApplied("double_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四暗刻単騎"), 2_yakumanM,
			_T("四暗刻"), _T("対々和"), _T("三暗刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::MachiType::shanpon) && (!analysis->TsumoAgariFlag())) ?
					1 : 0) == 4) && (
					(analysis->GameStat->TianHuFlag) ||
					((analysis->Machi == yaku::yakuCalculator::MachiType::tanki) && (!analysis->MachiInfo.FuritenFlag))
					);
			}
		));
#endif /* GUOBIAO */
	/* 三暗刻 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("三暗刻"), 16_fenF,
		_T("双暗刻"),
#else /* GUOBIAO */
		_T("三暗刻"), 2_hanF,
		_T("二暗刻"),
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::MachiType::shanpon) && (!analysis->TsumoAgariFlag())) ?
				1 : 0) == 3);
		}
	));
	/* 双暗刻 */
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("ryanankoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("双暗刻"), 2_fenF,
#else /* GUOBIAO */
			_T("二暗刻"), get_yaku_han("ryanankoh"),
#endif /* GUOBIAO */
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::MachiType::shanpon) && (!analysis->TsumoAgariFlag())) ?
					1 : 0) == 2);
			}
		));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 五連刻 */
	const auto wulianke =
		[liankeSuit](const MentsuAnalysis* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * 4); k++)
					if ((analysis->DuiziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 0]), k + step * 0)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 1]), k + step * 1)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 2]), k + step * 2)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 3]), k + step * 3)] >= 1) &&
						(analysis->DuiziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 4]), k + step * 4)] >= 1) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#endif /* GUOBIAO */
	const auto lianke =
		[liankeSuit](const MentsuAnalysis* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suurnkoh) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * (suurnkoh ? 3 : 2)); k++)
					if ((analysis->KeziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 0]), k + step * 0)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 1]), k + step * 1)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 2]), k + step * 2)] >= 1) &&
						((!suurnkoh)||(analysis->KeziCount[composeNumberTile(liankeSuit(parsedat[i * fldsize + 3]), k + step * 3)] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("golden_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金梯"), get_yaku_han("golden_ladder"),
			_T("清一色"), _T("対々和"), _T("小四連刻"), _T("四連刻"), _T("三連刻"), _T("小三連刻"),
			[wulianke](const MentsuAnalysis* const analysis) -> bool {
				return wulianke(analysis, reinterpret_cast<const char*>(&parsedat_monochrome5[0]), 3, 8, 1);
			}
		));
#endif /* GUOBIAO */
	/* 四連刻 */
	const auto suurenkoh =
		[lianke](const MentsuAnalysis* const analysis) -> bool {
			return lianke(analysis, reinterpret_cast<const char*>(&parsedat_monochrome4[0]), 3, 8, 1, true);
		};
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("一色四節高"), 48_fenF,
		PengPengHu, _T("一色三節高"),
		suurenkoh
	));
#else /* GUOBIAO */
	if (RuleData::chkRule("suurenkoh", "8han_incl_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四連刻"), 6_hanF /* 対々和込み8飜 */,
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
			[liankeSuit](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i = parsedat_monochrome5.begin(); i != parsedat_monochrome5.end(); i++) {
					for (int k = 1; k <= 5; k++)
						if ((analysis->DuiziCount[composeNumberTile(liankeSuit((*i)[0]), k + 0)] >= 1) &&
							(analysis->KeziCount[composeNumberTile(liankeSuit((*i)[1]), k + 1)] >= 1) &&
							(analysis->KeziCount[composeNumberTile(liankeSuit((*i)[2]), k + 2)] >= 1) &&
							(analysis->KeziCount[composeNumberTile(liankeSuit((*i)[3]), k + 3)] >= 1) &&
							(analysis->DuiziCount[composeNumberTile(liankeSuit((*i)[4]), k + 4)] >= 1) &&
							((analysis->TsumoHai().tile == composeNumberTile(liankeSuit((*i)[0]), k + 0)) ||
							(analysis->TsumoHai().tile == composeNumberTile(liankeSuit((*i)[4]), k + 4)))
							)
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */
	/* 三連刻 */
	const auto sanrenkoh =
		[lianke](const MentsuAnalysis* const analysis) -> bool {
			return lianke(analysis, reinterpret_cast<const char*>(&parsedat_monochrome3[0]), 3, 4, 1, false);
		};
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("sanrenkoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("一色三節高"), 24_fenF,
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
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i : NumberTileSuits) {
					for (int k = 1; k <= 6; k++)
						if ((analysis->DuiziCount[composeNumberTile(i, k + 0)] >= 1) && (analysis->KeziCount[composeNumberTile(i, k + 1)] >= 1) &&
							(analysis->KeziCount[composeNumberTile(i, k + 2)] >= 1) && (analysis->DuiziCount[composeNumberTile(i, k + 3)] >= 1))
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 小三連刻 */
	if (RuleData::chkRuleApplied("shou_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("小三連刻"), get_yaku_han("shou_sanrenkoh"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i : NumberTileSuits) {
					for (int k = 1; k <= 7; k++)
						if ((analysis->DuiziCount[composeNumberTile(i, k + 0)] >= 1) && (analysis->DuiziCount[composeNumberTile(i, k + 1)] >= 1) &&
							(analysis->DuiziCount[composeNumberTile(i, k + 2)] >= 1))
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
			[sanrenkoh](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::whiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* レアな方の三色 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("三同刻"), 16_fenF,
		_T("双同刻"),
#else /* GUOBIAO */
		_T("三色同刻"), get_yaku_han("sanshoku_doukoh"),
		_T("三色小同刻"),
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 9; i++)
				if ((analysis->KeziCount[composeNumberTile(TileSuit::characters, i)] >= 1) &&
					(analysis->KeziCount[composeNumberTile(TileSuit::circles,    i)] >= 1) &&
					(analysis->KeziCount[composeNumberTile(TileSuit::bamboos,    i)] >= 1))
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
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[composeNumberTile(TileSuit::characters, i)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(TileSuit::circles,    i)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(TileSuit::bamboos,    i)] >= 1) &&
						(analysis->KeziCount[TileCode::whiteDragon] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 三色小同刻 */
	if (RuleData::chkRuleApplied("sanshoku_shoudoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色小同刻"), get_yaku_han("sanshoku_shoudoukoh"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->DuiziCount[composeNumberTile(TileSuit::characters, i)] >= 1) &&
						(analysis->DuiziCount[composeNumberTile(TileSuit::circles,    i)] >= 1) &&
						(analysis->DuiziCount[composeNumberTile(TileSuit::bamboos,    i)] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 三元和・四喜和判定用 */
	const auto WindCnt =
		[](const MentsuAnalysis* const analysis) -> int {
			return analysis->DuiziCount[TileCode::eastWind] + analysis->KeziCount[TileCode::eastWind] +
				 analysis->DuiziCount[TileCode::southWind] + analysis->KeziCount[TileCode::southWind] +
				  analysis->DuiziCount[TileCode::westWind] + analysis->KeziCount[TileCode::westWind] +
				  analysis->DuiziCount[TileCode::northWind] + analysis->KeziCount[TileCode::northWind];
		};
	const auto DragonCnt =
		[](const MentsuAnalysis* const analysis) -> int {
			return analysis->DuiziCount[TileCode::whiteDragon] + analysis->KeziCount[TileCode::whiteDragon] +
				 analysis->DuiziCount[TileCode::greenDragon] + analysis->KeziCount[TileCode::greenDragon] +
				  analysis->DuiziCount[TileCode::redDragon] + analysis->KeziCount[TileCode::redDragon];
		};
	/* 小三元 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("小三元"), 64_fenF,
		_T("箭刻"), _T("双箭刻"),
#else /* GUOBIAO */
		_T("小三元"), 2_hanF,
		_T("青春に悔いなし"), _T("青春に悔いあり"), /* 役牌2つは必ず複合する */
#endif /* GUOBIAO */
		[DragonCnt](const MentsuAnalysis* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* 大三元 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("大三元"), 88_fenF,
		_T("箭刻"), _T("双箭刻"),
#else /* GUOBIAO */
		_T("大三元"), 1_yakumanF,
		_T("役牌・白"), _T("役牌・發"), _T("役牌・中"), _T("青春に悔いなし"), _T("青春に悔いあり"),
#endif /* GUOBIAO */
		[DragonCnt](const MentsuAnalysis* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
#ifndef GUOBIAO
	/* 門前大三元 */
	if (RuleData::chkRuleApplied("menzen_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("門前大三元"), 2_yakumanM,
			_T("大三元"),
			[DragonCnt](const MentsuAnalysis* const analysis) -> bool {
				return (DragonCnt(analysis) == 6);
			}
		));
#endif /* GUOBIAO */
	/* 小四喜 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("小四風会"), 64_fenF,
		_T("三風刻"), _T("幺九刻"), _T("幺九刻x2"), _T("幺九刻x3"),
#else /* GUOBIAO */
		_T("小四喜"), 1_yakumanF,
		_T("混一色"), _T("盗難車"),
#endif /* GUOBIAO */
		[WindCnt](const MentsuAnalysis* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
#ifndef GUOBIAO
	/* 天虎 */
	if (RuleData::chkRuleApplied("heaven_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("天虎"), get_yaku_han("heaven_tiger"),
			_T("混一色"), _T("小四喜"), _T("字一色"), _T("役牌・發"), _T("盗難車"),
			[WindCnt](const MentsuAnalysis* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[TileCode::greenDragon] >= 1));
			}
		));
	/* 大怪湖 */
	if (RuleData::chkRuleApplied("daikaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大怪湖"), get_yaku_han("daikaiko"),
			_T("混一色"), _T("小四喜"), _T("字一色"), _T("役牌・白"), _T("盗難車"),
			[WindCnt](const MentsuAnalysis* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[TileCode::whiteDragon] >= 1));
			}
		));
	/* 千年虫 */
	if (RuleData::chkRuleApplied("y2kbug"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("千年虫"), get_yaku_han("y2kbug"),
			_T("混一色"), _T("小四喜"), _T("混老頭"), _T("盗難車"),
			[WindCnt](const MentsuAnalysis* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[TileCode::characterNine] >= 1)&&(analysis->KeziCount[TileCode::southWind] == 1));
			}
		));
#endif /* GUOBIAO */
	/* 大四喜 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("大四風会"), 88_fenF,
		_T("圏風刻"), _T("門風刻"), _T("三風刻"), PengPengHu, _T("幺九刻"), _T("幺九刻x2"), _T("幺九刻x3"), _T("幺九刻x4"),
#else /* GUOBIAO */
		_T("大四喜"), (RuleData::chkRuleApplied("double_yakuman")) ?
		2_yakumanF : 1_yakumanF,
		_T("混一色"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"), _T("役牌・開門風"), _T("役牌・裏風"), _T("盗難車"),
#endif /* GUOBIAO */
		[WindCnt](const MentsuAnalysis* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
#ifndef GUOBIAO
	/* 弾葯 */
	if (RuleData::chkRuleApplied("dan_yaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("弾葯"), get_yaku_han("dan_yaku"),
			_T("混一色"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"), _T("役牌・開門風"), _T("役牌・裏風"), _T("大四喜"), _T("混老頭"), _T("盗難車"),
			[WindCnt](const MentsuAnalysis* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[TileCode::characterNine] >= 1));
			}
		));
	/* 仙人掌 */
	if (RuleData::chkRuleApplied("cactus"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("仙人掌"), get_yaku_han("cactus"),
			_T("混一色"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"), _T("役牌・開門風"), _T("役牌・裏風"), _T("大四喜"), _T("字一色"), _T("盗難車"),
			[WindCnt](const MentsuAnalysis* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[TileCode::redDragon] >= 1));
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 対々和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		PengPengHu, 6_fenF,
#else /* GUOBIAO */
		_T("対々和"), 2_hanF,
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->TotalKezi == SizeOfMeldBuffer - 1);
		}
	));

	// ---------------------------------------------------------------------

	/* 役牌 */
#ifdef GUOBIAO
	const auto chkYakuhai = [](const MentsuAnalysis* const analysis) -> const Int8ByTile& {return analysis->KeziCount;};
#else /* GUOBIAO */
	const auto chkYakuhai = RuleData::chkRuleApplied("exposed_yakuhai") ?
#ifdef _MSC_VER
		(std::function<const Int8ByTile& (const MentsuAnalysis* const)>)
#endif
		[](const MentsuAnalysis* const analysis) -> const Int8ByTile& {return analysis->KeziCount;} :
		[](const MentsuAnalysis* const analysis) -> const Int8ByTile& {return analysis->AnKeziCount;};
#endif /* GUOBIAO */
	const auto bakaze =
		[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->prevailingwind())] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("圏風刻"), 2_fenF,
#else /* GUOBIAO */
		_T("役牌・場風"), 1_hanF,
#endif /* GUOBIAO */
		bakaze
	));
	const auto jikaze =
		[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->playerwind(analysis->player))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("門風刻"), 2_fenF,
#else /* GUOBIAO */
		_T("役牌・自風"), 1_hanF,
#endif /* GUOBIAO */
		jikaze
	));
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("双箭刻"), 6_fenF,
		[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
			return chkYakuhai(analysis)[TileCode::whiteDragon] +
				chkYakuhai(analysis)[TileCode::greenDragon] +
				chkYakuhai(analysis)[TileCode::redDragon] == 2;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("箭刻"), 2_fenF,
		[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
			return chkYakuhai(analysis)[TileCode::whiteDragon] +
				chkYakuhai(analysis)[TileCode::greenDragon] +
				chkYakuhai(analysis)[TileCode::redDragon] == 1;
		}
	));
#else /* GUOBIAO */
	YAKUFUNC kaimenkaze, urakaze;
	if (RuleData::chkRuleApplied("kaimenkaze")) {
		kaimenkaze =
			[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->playerwind(analysis->GameStat->WaremePlayer))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("役牌・開門風"), get_yaku_han("kaimenkaze"),
			kaimenkaze
		));
	} else {
		kaimenkaze = 
			[](const MentsuAnalysis* const analysis) -> bool {
				return false;
			};
	}
	if (RuleData::chkRuleApplied("urakaze")) {
		urakaze =
			[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->playerwind(
					RelativePositionOf(analysis->player, SeatRelative::opposite)))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("役牌・裏風"), get_yaku_han("urakaze"),
			urakaze
		));
	} else {
		urakaze = 
			[](const MentsuAnalysis* const analysis) -> bool {
				return false;
			};
	}
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("役牌・白"), 1_hanF,
		[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
			return (chkYakuhai(analysis)[TileCode::whiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("役牌・發"), 1_hanF,
		[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
			return (chkYakuhai(analysis)[TileCode::greenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("役牌・中"), 1_hanF,
		[chkYakuhai](const MentsuAnalysis* const analysis) -> bool {
			return (chkYakuhai(analysis)[TileCode::redDragon] >= 1);
		}
	));
#endif /* GUOBIAO */

#ifdef GUOBIAO
	/* 幺九刻 */
	const auto YaoJiuKe =
		[chkYakuhai, bakaze, jikaze](const MentsuAnalysis* const analysis) -> int {
			return chkYakuhai(analysis)[TileCode::characterOne] + chkYakuhai(analysis)[TileCode::characterNine] +
				chkYakuhai(analysis)[TileCode::circleOne] + chkYakuhai(analysis)[TileCode::circleNine] +
				chkYakuhai(analysis)[TileCode::bambooOne] + chkYakuhai(analysis)[TileCode::bambooNine] +
				chkYakuhai(analysis)[TileCode::eastWind] + chkYakuhai(analysis)[TileCode::southWind] +
				chkYakuhai(analysis)[TileCode::westWind] + chkYakuhai(analysis)[TileCode::northWind] -
				(bakaze(analysis) ? 1 : 0) - (jikaze(analysis) ? 1 : 0);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻"), 1_fenF,
		[YaoJiuKe](const MentsuAnalysis* const analysis) -> bool {
			return YaoJiuKe(analysis) == 1;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻x2"), 2_fenF,
		[YaoJiuKe](const MentsuAnalysis* const analysis) -> bool {
			return YaoJiuKe(analysis) == 2;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻x3"), 3_fenF,
		[YaoJiuKe](const MentsuAnalysis* const analysis) -> bool {
			return YaoJiuKe(analysis) == 3;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("幺九刻x4"), 4_fenF,
		[YaoJiuKe](const MentsuAnalysis* const analysis) -> bool {
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
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::bambooOne] >= 1);
			}
		));
	/* 陣太鼓 */
	if (RuleData::chkRuleApplied("jindaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("陣太鼓"), get_yaku_han("jindaiko"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* 小三風 */
	if (RuleData::chkRuleApplied("xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("小三風"), get_yaku_han("xiaosanfeng"),
			[WindCnt](const MentsuAnalysis* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* 三風刻 */
	if (RuleData::chkRuleApplied("three_winds"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("三風刻"), 12_fenF,
#else /* GUOBIAO */
			_T("三風刻"), get_yaku_han("three_winds"),
#endif /* GUOBIAO */
			[WindCnt](const MentsuAnalysis* const analysis) -> bool {
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
			(const MentsuAnalysis* const analysis) -> bool {
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
			(const MentsuAnalysis* const analysis) -> bool {
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
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->GameStat->prevailingwind() == SeatAbsolute::west) &&
					(analysis->GameStat->playerwind(analysis->player) == SeatAbsolute::south))
					yakuFlag = true;
				if ((analysis->GameStat->prevailingwind() == SeatAbsolute::south) &&
					(analysis->GameStat->playerwind(analysis->player) == SeatAbsolute::west))
					yakuFlag = true;
				return ((analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(yakuFlag));
			}
		));
	/* 雪月花 */
	if (RuleData::chkRuleApplied("setsugetsuka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("雪月花"), get_yaku_han("setsugetsuka"),
			_T("役牌・白"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleFive] >= 1));
			}
		));
	/* 風花雪月 */
	if (RuleData::chkRuleApplied("fuuka_setsugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("風花雪月"), get_yaku_han("fuuka_setsugetsu"),
			_T("雪月花"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"),
			[bakaze, jikaze](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* 花鳥風月 */
	if (RuleData::chkRuleApplied("kachou_fuugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("花鳥風月"), get_yaku_han("kachou_fuugetsu"),
			_T("雪月花"), _T("対々和"), _T("役牌・場風"), _T("役牌・自風"),
			[bakaze, jikaze](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* 東北自動車道 */
	if (RuleData::chkRuleApplied("tohoku_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東北自動車道"), get_yaku_han("tohoku_expwy"),
			_T("対々和"), _T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::circleTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::circleFour] >= 1) &&
					(analysis->DuiziCount[TileCode::circleSix] >= 1) &&
					(analysis->DuiziCount[TileCode::eastWind] >= 1) &&
					(analysis->DuiziCount[TileCode::northWind] >= 1));
			}
		));
	/* 東関東自動車道 */
	if (RuleData::chkRuleApplied("higashikanto_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東関東自動車道"), get_yaku_han("higashikanto_expwy"),
			_T("対々和"), _T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::circleTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::circleFour] >= 1) &&
					(analysis->DuiziCount[TileCode::circleSix] >= 1) &&
					(analysis->DuiziCount[TileCode::eastWind] >= 1) &&
					(analysis->DuiziCount[TileCode::westWind] >= 1));
			}
		));
	/* 中国系のおめでたい役 */
	if (RuleData::chkRuleApplied("koukei_hougyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("紅鶏報暁"), get_yaku_han("koukei_hougyou"),
			_T("対々和"), _T("混一色"), _T("絶一門"), _T("役牌・白"), _T("役牌・中"), _T("金鶏報暁"), _T("銀鶏報春"), _T("白鳥"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == TileCode::circleOne) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("kinkei_hougyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金鶏報暁"), get_yaku_han("kinkei_hougyou"),
			_T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("ginkei_houshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("銀鶏報春"), get_yaku_han("ginkei_houshun"),
			_T("役牌・白"), _T("白鳥"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->PlayerStat()->FlowerFlag.Spring));
			}
		));

	// ---------------------------------------------------------------------

	/* 四跳牌刻 */
	if (RuleData::chkRuleApplied("suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四跳牌刻"), get_yaku_han("suuchaopaikoh"),
			_T("対々和"), _T("跳牌刻"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_monochrome4[0]), 3, 8, 2, true);
			}
		));
	/* 跳牌刻 */
	if (RuleData::chkRuleApplied("chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("跳牌刻"), get_yaku_han("chaopaikoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_monochrome3[0]), 3, 4, 2, false);
			}
		));
	/* 筋牌刻 */
	if (RuleData::chkRuleApplied("chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("筋牌刻"), get_yaku_han("chinpaikoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_monochrome3[0]), 3, 4, 3, false);
			}
		));
	/* 頂三刻 */
	if (RuleData::chkRuleApplied("tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("頂三刻"), get_yaku_han("tinsankoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_monochrome3[0]), 3, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 二色五連刻 */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("銀梯"), get_yaku_han("silver_ladder"),
			_T("二色四連刻"), _T("二色三連刻"), _T("対々和"),
			[wulianke](const MentsuAnalysis* const analysis) -> bool {
				return wulianke(analysis, reinterpret_cast<const char*>(&parsedat_bichrome5[0]), 90, 8, 1);
			}
		));
	/* 二色四連刻 */
	if (RuleData::chkRuleApplied("nishoku_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色四連刻"), get_yaku_han("nishoku_sanrenkoh"),
			_T("二色三連刻"), _T("対々和"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_bichrome4[0]), 42, 8, 1, true);
			}
		));
	/* 二色三連刻 */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色三連刻"), get_yaku_han("silver_ladder"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_bichrome3[0]), 18, 4, 1, false);
			}
		));
	/* 二色四跳牌刻 */
	if (RuleData::chkRuleApplied("nishoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色四跳牌刻"), get_yaku_han("nishoku_suuchaopaikoh"),
			_T("二色跳牌刻"), _T("対々和"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_bichrome4[0]), 42, 8, 2, true);
			}
		));
	/* 二色跳牌刻 */
	if (RuleData::chkRuleApplied("nishoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色跳牌刻"), get_yaku_han("nishoku_chaopaikoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_bichrome3[0]), 18, 4, 2, false);
			}
		));
	/* 二色筋牌刻 */
	if (RuleData::chkRuleApplied("nishoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色筋牌刻"), get_yaku_han("nishoku_chinpaikoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_bichrome3[0]), 18, 4, 3, false);
			}
		));
	/* 二色頂三刻 */
	if (RuleData::chkRuleApplied("nishoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色頂三刻"), get_yaku_han("nishoku_tinsankoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_bichrome3[0]), 18, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 三色五連刻 */
	if (RuleData::chkRuleApplied("sky_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("雲梯"), get_yaku_han("sky_ladder"),
			_T("三色四連刻"), _T("三色連刻"), _T("対々和"),
			[wulianke](const MentsuAnalysis* const analysis) -> bool {
				return wulianke(analysis, reinterpret_cast<const char*>(&parsedat_trichrome5[0]), 150, 8, 1);
			}
		));
	/* 三色四連刻 */
	if (RuleData::chkRuleApplied("sanshoku_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色四連刻"), get_yaku_han("sanshoku_suurenkoh"),
			_T("三色連刻"), _T("対々和"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_trichrome4[0]), 36, 8, 1, true);
			}
		));
	/* 三色連刻 */
	if (RuleData::chkRuleApplied("sanshoku_renkoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("三色三節高"), 8_fenF,
#else /* GUOBIAO */
			_T("三色連刻"), get_yaku_han("sanshoku_renkoh"),
#endif /* GUOBIAO */
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_trichrome3[0]), 6, 4, 1, false);
			}
		));
#ifndef GUOBIAO
	/* 三色四跳牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色四跳牌刻"), get_yaku_han("sanshoku_suuchaopaikoh"),
			_T("三色跳牌刻"), _T("対々和"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_trichrome4[0]), 36, 8, 2, true);
			}
		));
	/* 三色跳牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色跳牌刻"), get_yaku_han("sanshoku_chaopaikoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_trichrome3[0]), 6, 4, 2, false);
			}
		));
	/* 三色筋牌刻 */
	if (RuleData::chkRuleApplied("sanshoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色筋牌刻"), get_yaku_han("sanshoku_chinpaikoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_trichrome3[0]), 6, 4, 3, false);
			}
		));
	/* 三色頂三刻 */
	if (RuleData::chkRuleApplied("sanshoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色頂三刻"), get_yaku_han("sanshoku_tinsankoh"),
			[lianke](const MentsuAnalysis* const analysis) -> bool {
				return lianke(analysis, reinterpret_cast<const char*>(&parsedat_trichrome3[0]), 6, 4, 4, false);
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 二色同刻 */
	const auto ShuangTongKe =
		[](const MentsuAnalysis* const analysis) -> int {
			constexpr std::array<std::pair<TileSuit, TileSuit>, 3> suit = {
				std::make_pair(TileSuit::characters, TileSuit::circles),
				std::make_pair(TileSuit::characters, TileSuit::bamboos),
				std::make_pair(TileSuit::circles, TileSuit::bamboos),
			};
			int j = 0;
			for (auto& k : suit) {
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[composeNumberTile(k.first, i)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(k.second, i)] >= 1)) ++j;
			}
			return j;
		};
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("双同刻"), 2_fenF,
		[ShuangTongKe](const MentsuAnalysis* const analysis) -> bool {
			return ShuangTongKe(analysis) == 1;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("双同刻x2"), 4_fenF,
		[ShuangTongKe](const MentsuAnalysis* const analysis) -> bool {
			return ShuangTongKe(analysis) == 2;
		}
	));
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("nishoku_doukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色同刻"), get_yaku_han("nishoku_doukoh"),
			[ShuangTongKe](const MentsuAnalysis* const analysis) -> bool {
				return ShuangTongKe(analysis) >= 1;
			}
	));

	/* 二同刻 */
	if (RuleData::chkRuleApplied("ryandoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二同刻"), get_yaku_han("ryandoukoh"),
			_T("二色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				constexpr std::array<std::pair<TileSuit, TileSuit>, 3> suit = {
					std::make_pair(TileSuit::characters, TileSuit::circles),
					std::make_pair(TileSuit::characters, TileSuit::bamboos),
					std::make_pair(TileSuit::circles, TileSuit::bamboos),
				};
				for (auto& k : suit) {
					int j = 0;
					for (int i = 1; i <= 9; i++)
						if ((analysis->KeziCount[composeNumberTile(k.first,  i)] >= 1) &&
							(analysis->KeziCount[composeNumberTile(k.second, i)] >= 1)) ++j;
					if (j == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 茴香ポン */
	if (RuleData::chkRuleApplied("uikyou_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("茴香") PON, get_yaku_han("uikyou_toitoi"),
			_T("三色同刻"), _T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[composeNumberTile(TileSuit::characters, i)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(TileSuit::circles,    i)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(TileSuit::bamboos,    i)] >= 1))
						if ((analysis->KeziCount[composeNumberTile(TileSuit::characters, i + 1)] >= 1) ||
							(analysis->KeziCount[composeNumberTile(TileSuit::circles,    i + 1)] >= 1) ||
							(analysis->KeziCount[composeNumberTile(TileSuit::bamboos,    i + 1)] >= 1) ||
							(analysis->KeziCount[composeNumberTile(TileSuit::characters, i - 1)] >= 1) ||
							(analysis->KeziCount[composeNumberTile(TileSuit::circles,    i - 1)] >= 1) ||
							(analysis->KeziCount[composeNumberTile(TileSuit::bamboos,    i - 1)] >= 1))
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
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					((analysis->KeziCount[TileCode::eastWind] >= 1) ||
					(analysis->KeziCount[TileCode::southWind] >= 1)) );
			}
		));
	/* 八仙戯鳳 */
	if (RuleData::chkRuleApplied("baxian_xifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八仙戯鳳"), get_yaku_han("baxian_xifeng"),
			_T("対々和"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1) );
			}
		));
	/* 八仙戯四鳳 */
	if (RuleData::chkRuleApplied("baxian_xisifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八仙戯四鳳"), get_yaku_han("baxian_xisifeng"),
			_T("対々和"), _T("三色同刻"), _T("八仙戯鳳"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					(analysis->KangziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1) );
			}
		));
	/* 八仙戯金亀 */
	if (RuleData::chkRuleApplied("baxian_xijingui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八仙戯金亀"), get_yaku_han("baxian_xijingui"),
			_T("対々和"), _T("三色同刻"), _T("茴香") PON,
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooSeven] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1) );
			}
		));
	/* 八仙闘北斗 */
	if (RuleData::chkRuleApplied("baxian_dou_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八仙闘北斗"), get_yaku_han("baxian_dou_beidou"),
			_T("対々和"), _T("三色同刻"), _T("茴香") PON,
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1) );
			}
		));
	/* 八仙聚首 */
	if (RuleData::chkRuleApplied("baxian_jushou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八仙聚首"), get_yaku_han("baxian_jushou"),
			_T("対々和"), _T("三色同刻"), _T("八仙過海"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					analysis->PlayerStat()->FlowerFlag.Spring && analysis->PlayerStat()->FlowerFlag.Summer &&
					analysis->PlayerStat()->FlowerFlag.Autumn && analysis->PlayerStat()->FlowerFlag.Winter &&
					analysis->PlayerStat()->FlowerFlag.Plum && analysis->PlayerStat()->FlowerFlag.Orchid &&
					analysis->PlayerStat()->FlowerFlag.Chrys && analysis->PlayerStat()->FlowerFlag.Bamboo );
			}
		));
#endif /* GUOBIAO */
}
