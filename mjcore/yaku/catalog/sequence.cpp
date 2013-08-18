#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* 一色双龍会 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色双龍会"), yaku::yakuCalculator::Yaku::yval_64,
			_T("清一色"), _T("平和"), _T("一般高"), _T("一般高x2"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("isshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色双龍会"), get_yaku_han("isshoku_souryuu"),
			_T("清一色"), _T("二盃口"), _T("鳴き二盃口"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[CharacterOne] >= 2) && (analysis->ShunziCount[CharacterSeven] >= 2) &&
					(analysis->MianziDat[0].tile == CharacterFive)) yakuFlag = true;
				if ((analysis->ShunziCount[CircleOne] >= 2) && (analysis->ShunziCount[CircleSeven] >= 2) &&
					(analysis->MianziDat[0].tile == CircleFive)) yakuFlag = true;
				if ((analysis->ShunziCount[BambooOne] >= 2) && (analysis->ShunziCount[BambooSeven] >= 2) &&
					(analysis->MianziDat[0].tile == BambooFive)) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 一色四順 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色四同順"), yaku::yakuCalculator::Yaku::yval_48,
			_T("一般高"), _T("一般高x2"), _T("一色三同順"), _T("四帰一"), _T("四帰一x2"), _T("四帰一x3"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("isshoku_suujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色四順"), get_yaku_han("isshoku_suujun"),
			_T("一色三順"), _T("一盃口"), _T("二盃口"), _T("鳴き二盃口"), _T("重四帰四"), _T("四帰四"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 一色三順 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色三同順"), yaku::yakuCalculator::Yaku::yval_24,
			_T("一般高"), _T("一般高x2"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("isshoku_sanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色三順"), get_yaku_han("isshoku_sanjun"),
			_T("一盃口"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 3) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 一盃口 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("一般高"), yaku::yakuCalculator::Yaku::yval_1,
#else /* GUOBIAO */
		_T("一盃口"), yaku::yakuCalculator::Yaku::yval_1han_menzen,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TileSuitHonors; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 1);
		}
	));
	/* 二盃口 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("一般高x2"), yaku::yakuCalculator::Yaku::yval_2,
		_T("一般高"),
#else /* GUOBIAO */
		_T("二盃口"), get_yaku_han("ryanpeikou"),
		_T("一盃口"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TileSuitHonors; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 2);
		}
	));
#ifndef GUOBIAO
	/* 大鏡 */
	if (RuleData::chkRuleApplied("ohkagami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大鏡"), get_yaku_han("ohkagami"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return ((yakuFlagCount == 2)&&(analysis->MianziDat[0].tile == CircleOne));
			}
		));
	/* 清盃口 */
	if (RuleData::chkRuleApplied("chinpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("清盃口"), get_yaku_han("chinpeikou"),
			_T("二盃口"), _T("琵琶湖の妖精"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlagCount = false;
				for (int i = 1; i <= 7; i++) {
					if ((analysis->ShunziCount[i + TileSuitCharacters] == 2) &&
						(analysis->ShunziCount[i + TileSuitCircles] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TileSuitCharacters] == 2) &&
						(analysis->ShunziCount[i + TileSuitBamboos] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TileSuitCircles] == 2) &&
						(analysis->ShunziCount[i + TileSuitBamboos] == 2))
						yakuFlagCount = true;
				}
				return yakuFlagCount;
			}
		));
	/* 琵琶湖の妖精 */
	if (RuleData::chkRuleApplied("biwakonoyousei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("琵琶湖の妖精"), get_yaku_han("biwakonoyousei"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlagCount = false;
				if ((analysis->ShunziCount[CharacterOne] == 2) &&
					(analysis->ShunziCount[CircleOne] == 2))
					yakuFlagCount = true;
				if ((analysis->ShunziCount[CharacterOne] == 2) &&
					(analysis->ShunziCount[BambooOne] == 2))
					yakuFlagCount = true;
				if ((analysis->ShunziCount[CircleOne] == 2) &&
					(analysis->ShunziCount[BambooOne] == 2))
					yakuFlagCount = true;
				return yakuFlagCount;
			}
		));
	/* 鳴き二盃口 */
	if (RuleData::chkRuleApplied("exposed_ryanpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鳴き二盃口"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return (yakuFlagCount == 2);
			}
		));
	/* 北四巡(一色四順) */
	if (RuleData::chkRuleApplied("pei4jun_isshoku_suujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北四巡 "), get_yaku_han("pei4jun_isshoku_suujun"),
			_T("一色四順"), _T("一色三順"), _T("一盃口"), _T("二盃口"), _T("鳴き二盃口"), _T("重四帰四"), _T("四帰四"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag && (analysis->MianziDat[0].tile == NorthWind);
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* サンショクともサンシキとも呼ばれる */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("三色三同順"), yaku::yakuCalculator::Yaku::yval_8,
#else /* GUOBIAO */
		_T("三色同順"), yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 7; i++)
				if ((analysis->ShunziCount[i + TileSuitCharacters] >= 1) &&
					(analysis->ShunziCount[i + TileSuitCircles] >= 1) &&
					(analysis->ShunziCount[i + TileSuitBamboos] >= 1)) yakuFlag = true;
			return yakuFlag;
		}
	));

	// ---------------------------------------------------------------------

	/* イーチートンカン */
	auto ikki_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			*yakuFlag = false;
			for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
				if ((analysis->ShunziCount[i + 1] >= 1) &&
					(analysis->ShunziCount[i + 4] >= 1) &&
					(analysis->ShunziCount[i + 7] >= 1)) {
						*yakuFlag = true;
						if (yakuCol != nullptr) *yakuCol = i;
				}
			}
		};
	auto ittsuu_monotonic =
		[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			int samecol = 0;
			ikki_tsuukan(analysis, yakuFlag, yakuCol);
			if (!(*yakuFlag)) return;
			for (int i = 0; i < SizeOfMeldBuffer; i++) {
				if ((analysis->MianziDat[i].tile / TileSuitStep) ==
					(*yakuCol / TileSuitStep)) ++samecol;
				else if ((analysis->MianziDat[i].tile / TileSuitStep) ==
					(TileSuitHonors / TileSuitStep)) ++samecol;
			}
			*yakuFlag = (samecol == SizeOfMeldBuffer); // 一色になっているかどうか
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("清龍"), yaku::yakuCalculator::Yaku::yval_24,
#else /* GUOBIAO */
		_T("一気通貫"), yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
#endif /* GUOBIAO */
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return yakuFlag;
			}
	));
#ifndef GUOBIAO
	/* 20101204祝東京～新青森全通 */
	if (RuleData::chkRuleApplied("tohoku_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東北新幹線"), get_yaku_han("tohoku_shinkansen"),
			_T("一気通貫"), _T("混一色"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->DuiziCount[EastWind] >= 1) && // 東と
					(analysis->DuiziCount[NorthWind] >= 1) ); // 北がある
			}
		));
	/* 一気通貫全帯幺九 */
	auto ittsuu_chanta_counting =
		[](const MENTSU_ANALYSIS* const analysis, int* const yaojiu, int* const zipai) -> void {
			*yaojiu = *zipai = 0;
			for (int i = 0; i < SizeOfMeldBuffer; i++) {
				switch (analysis->MianziDat[i].tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind:
				case WhiteDragon: case GreenDragon: case RedDragon:
					++*zipai;
					/* FALLTHRU */
				case CharacterOne: case CharacterNine: case CircleOne: case CircleNine:
				case BambooOne: case BambooNine:
					++*yaojiu; break;
				case CharacterSeven: case CircleSeven: case BambooSeven:
					if ((i > 0) && (analysis->MianziDat[i].mstat < meldTripletConcealed))
						++*yaojiu;
					break;
				}
			}
		};
	if (RuleData::chkRuleApplied("ittsuu_chanta")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一気通貫全帯幺九"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("一気通貫"),
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, nullptr);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純一気通貫全帯幺九"), yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			_T("一気通貫"),
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, nullptr);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}
	/* 五心通貫 */
	if (RuleData::chkRuleApplied("uushintonkan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("五心通貫"), get_yaku_han("uushintonkan"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // 嵌張待ちで
					(analysis->PlayerStat->Hand[NumOfTilesInHand].tile == (yakuCol + 5)) // 和了牌が5
					);
			}
		));
	/* 竜戯珠 */
	if (RuleData::chkRuleApplied("longxizhu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("竜戯珠"), get_yaku_han("longxizhu"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[WestWind] >= 1) && // 西の刻子があり
					(analysis->MianziDat[0].tile == CircleOne) ); // アタマが一筒
			}
		));
	/* 金龍戯北斗 */
	if (RuleData::chkRuleApplied("jinlong_dou_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金龍戯北斗"), get_yaku_han("jinlong_dou_beidou"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[CircleSeven] >= 1) && // 七筒の刻子があり
					(analysis->MianziDat[0].tile == NorthWind) ); // アタマが北
			}
		));
	/* 西湖十景系列の役 */
	{
		const int Rules = 10;
		const char tmpRuleCodeList[Rules][32] = {
			"shuangfeng_zhayun", "nanbing_wanzhong", "leifeng_xizhao", "liulang_wenying", "duanqiao_canxue",
			"sudi_chunxiao", "huagang_guanyu", "santan_yinyue", "quyuan_fenghe", "pinghu_qiuyue",
		};
		const TileCode tmpTileCodeList[Rules][2] = {
			{EastWind, BambooEight}, {SouthWind, RedDragon}, {WestWind, CircleEight}, {NorthWind, CharacterEight},
			{WhiteDragon, BambooTwo}, {GreenDragon, CharacterTwo}, {RedDragon, CircleTwo},
			{WhiteDragon, CharacterOne}, {GreenDragon, CircleOne}, {RedDragon, BambooOne},
		};
		const int tmpStraightColorList[Rules] = {
			TileSuitCharacters, TileSuitCircles, TileSuitBamboos, TileSuitCircles, TileSuitCharacters,
			TileSuitCircles, TileSuitBamboos, TileSuitCharacters, TileSuitCircles, TileSuitBamboos,
		};
		const TCHAR tmpYakuNameList[Rules][16] = {
			_T("双峰挿雲"), _T("南屏晩鍾"), _T("雷峰夕照"), _T("柳浪聞鶯"), _T("断橋残雪"),
			_T("蘇堤春暁"), _T("花港観魚"), _T("三譚印月"), _T("曲院風荷"), _T("平湖秋月"),
		};
		for (int i = 0; i < Rules; i++) {
			if (!RuleData::chkRuleApplied(tmpRuleCodeList[i])) continue;
			TileCode tmpTileCode1 = tmpTileCodeList[i][0];
			TileCode tmpTileCode2 = tmpTileCodeList[i][1];
			int tmpStraightColor = tmpStraightColorList[i];
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				tmpYakuNameList[i], get_yaku_han(tmpRuleCodeList[i]),
				[ikki_tsuukan, tmpTileCode1, tmpTileCode2, tmpStraightColor]
				(const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag; int yakuCol;
					ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
					return (yakuFlag && // 一気通貫が成立していて
						(analysis->DuiziCount[tmpTileCode1] >= 1) && // 指定の牌が刻子と雀頭として存在し
						(analysis->DuiziCount[tmpTileCode2] >= 1) &&
						(yakuCol == tmpStraightColor) ); // イッツー部分が指定された色
				}
			));
		}
	}
	/* 東の青龍 */
	if (RuleData::chkRuleApplied("azure_dragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東の青龍"), get_yaku_han("azure_dragon"),
			_T("一気通貫"), _T("混一色"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[EastWind] >= 1) && // 東の刻子があり
					(analysis->MianziDat[0].tile == WhiteDragon) && // アタマが白で
					(yakuCol == TileSuitCircles) ); // イッツー部分が筒子
			}
		));
	/* 北の玄武 */
	if (RuleData::chkRuleApplied("black_tortoise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北の玄武"), get_yaku_han("black_tortoise"),
			_T("一気通貫"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[NorthWind] >= 1) && // 北の刻子があり
					(analysis->MianziDat[0].tile == BambooSeven) && // アタマが七索で
					(yakuCol == TileSuitCircles) ); // イッツー部分が筒子
			}
		));
	/* 秋田新幹線 */
	if (RuleData::chkRuleApplied("akita_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("秋田新幹線"), get_yaku_han("akita_shinkansen"),
			_T("一気通貫"), _T("混一色"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[NorthWind] >= 1) && // 北の刻子があり
					((analysis->MianziDat[0].tile / TileSuitStep) == (yakuCol / TileSuitStep)) && // 一色になっていて
					(analysis->PlayerStat->Hand[NumOfTilesInHand].tile == (yakuCol + 5)) // 和了牌が5
					);
			}
		));
	/* 上越新幹線とき */
	if (RuleData::chkRuleApplied("joetsu_toki"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("上越新幹線とき"), get_yaku_han("joetsu_toki"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(analysis->DuiziCount[NorthWind] >= 1) // 北の刻子または雀頭がある
					);
			}
		));
	/* 上越新幹線あさひ */
	if (RuleData::chkRuleApplied("joetsu_asahi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("上越新幹線あさひ"), get_yaku_han("joetsu_asahi"),
			_T("上越新幹線とき"), _T("一気通貫"), _T("混一色"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->DuiziCount[NorthWind] >= 1) && // 北と
					(analysis->DuiziCount[WhiteDragon] >= 1) // 白が刻子と雀頭として存在
					);
			}
		));
	/* 青函連絡船 */
	if (RuleData::chkRuleApplied("seikan_ship"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("青函連絡船"), get_yaku_han("seikan_ship"),
			_T("一気通貫"), _T("混一色"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->MianziDat[0].tile == NorthWind) && // 北が雀頭で
					(analysis->AnKangziCount[GreenDragon] >= 1) // 發の暗槓がある
					);
			}
		));
	/* ゴールドラッシュ */
	if (RuleData::chkRuleApplied("goldrush")) {
		auto goldrush1 =
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(yakuCol == TileSuitCircles) && // 筒子のイッツーで
					(analysis->DuiziCount[GreenDragon] >= 1) // 發の刻子か雀頭がある
					);
			};
		auto goldrush2 =
			[goldrush1](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (goldrush1(analysis) && // goldrush1の条件を満たし
					(analysis->DuiziCount[WestWind] >= 1) // 西の刻子か雀頭がある
					);
			};
		if (RuleData::chkRule("goldrush", "2han"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("ゴールドラッシュ"), yaku::yakuCalculator::Yaku::yval_2han,
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "2han_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("ゴールドラッシュ"), yaku::yakuCalculator::Yaku::yval_2han,
				goldrush2
			));
		else if (RuleData::chkRule("goldrush", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("ゴールドラッシュ"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("一気通貫"), _T("混一色"),
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "yakuman_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("ゴールドラッシュ"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("一気通貫"), _T("混一色"),
				goldrush2
			));
	}
	/* ルート２４６ */
	if (RuleData::chkRuleApplied("route_246"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ルート２４６"), get_yaku_han("route_246"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // 嵌張待ちで
					((analysis->TsumoHai->tile == yakuCol + 2) || // 2か
					(analysis->TsumoHai->tile == yakuCol + 4) || // 4か
					(analysis->TsumoHai->tile == yakuCol + 6)) // 6で和了
					);
			}
		));
	/* 九龍城落地 */
	if (RuleData::chkRuleApplied("gaulungsing_lokdei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("九龍城落地"), get_yaku_han("gaulungsing_lokdei"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->ShunziCount[yakuCol + 1] - analysis->AnShunziCount[yakuCol + 1] >= 1) &&
					(analysis->ShunziCount[yakuCol + 4] - analysis->AnShunziCount[yakuCol + 4] >= 1) &&
					(analysis->ShunziCount[yakuCol + 7] - analysis->AnShunziCount[yakuCol + 7] >= 1) // 一気通貫の牌は全部チー
					);
			}
		));
	/* 北海道新幹線 */
	if (RuleData::chkRuleApplied("hokkaido_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北海道新幹線"), get_yaku_han("hokkaido_shinkansen"),
			_T("一気通貫"), _T("混一色"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->AnKangziCount[WhiteDragon] >= 1) && // 白の暗槓があって
					(analysis->MianziDat[0].tile == NorthWind) ); // 雀頭が北
			}
		));
	/* 五爪の龍 */
	if (RuleData::chkRuleApplied("five_claw_dragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("五爪の龍"), get_yaku_han("five_claw_dragon"),
			_T("一気通貫"), _T("混一色"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(analysis->DuiziCount[RedDragon] >= 1) && // 中の雀頭
					(analysis->KeziCount[BambooFive] >= 1) && // 五索の刻子
					(analysis->Machi == yaku::yakuCalculator::machiShanpon) // シャンポン待ち
					);
			}
		));
	/* 琉球四爪龍 */
	if (RuleData::chkRuleApplied("ryukyu_four_claw_dragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("琉球四爪龍"), get_yaku_han("ryukyu_four_claw_dragon"),
			_T("一気通貫"), _T("清一色"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(analysis->DuiziCount[BambooNine] >= 1) && // 九索の雀頭
					(analysis->KeziCount[BambooFour] >= 1) && // 四索の刻子
					(analysis->Machi == yaku::yakuCalculator::machiShanpon) // シャンポン待ち
					);
			}
		));
	/* 南天全星 */
	if (RuleData::chkRuleApplied("southern_stars"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南天全星"), get_yaku_han("southern_stars"),
			_T("一気通貫"), _T("混一色"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(analysis->DuiziCount[CircleThree] >= 1) && // 三筒の雀頭
					(analysis->KeziCount[SouthWind] >= 1) // 南の刻子
					);
			}
		));
	/* 東海道新幹線二階建 */
	if (RuleData::chkRuleApplied("tokaido_shinkansen_double"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東海道新幹線二階建"), get_yaku_han("tokaido_shinkansen_double"),
			_T("一気通貫"), _T("混一色"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(analysis->DuiziCount[EastWind] >= 1) && // 東の雀頭
					((analysis->ShunziCount[CharacterTwo] >= 1) || // ２３４の順子
					(analysis->ShunziCount[CircleTwo] >= 1) || // ２３４の順子
					(analysis->ShunziCount[BambooTwo] >= 1)) // ２３４の順子
					);
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* ゴールデンゲートブリッジ */
	auto chkGoldenGateBridge =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
				if ((analysis->ShunziCount[i + 1] >= 1) &&
					(analysis->ShunziCount[i + 3] >= 1) &&
					(analysis->ShunziCount[i + 5] >= 1) &&
					(analysis->ShunziCount[i + 7] >= 1)) yakuFlag = true;
			return yakuFlag;
		};
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色四歩高"), yaku::yakuCalculator::Yaku::yval_32,
			_T("一色三歩高"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("golden_gate_bridge"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金門橋"), get_yaku_han("golden_gate_bridge"),
			_T("清連環套"),
#endif /* GUOBIAO */
			chkGoldenGateBridge
		));
#ifndef GUOBIAO
	/* ゴールデンゲートブリッジ */
	if (RuleData::chkRuleApplied("naruto_bridge"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鳴門大橋"), get_yaku_han("naruto_bridge"),
			_T("清連環套"), _T("金門橋"),
			[chkGoldenGateBridge](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkGoldenGateBridge(analysis) && (analysis->MianziDat[0].tile == CircleOne);
			}
		));
	/* Arc de Triomphe */
	if (RuleData::chkRuleApplied("gaisenmon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("凱旋門"), get_yaku_han("gaisenmon"),
			_T("清一色"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
					int yakuFlagCount = 0;
					if ((analysis->ShunziCount[i + 0] >= 1) && (analysis->ShunziCount[i + 3] >= 2) && (analysis->ShunziCount[i + 6] >= 1)) {
						yakuFlag = true;
					} else if ((analysis->ShunziCount[i + 0] >= 1) && (analysis->ShunziCount[i + 3] == 1) && (analysis->ShunziCount[i + 6] >= 1)) {
						continue;
					} else {
						for (int j = 1; j <= 4; j++)
							for (int k = 1; k <= 2; k++)
								if ((analysis->ShunziCount[i + j] >= k) && (analysis->ShunziCount[i + j + 3] >= k))
									++yakuFlagCount;
					}
					if ((yakuFlagCount == 2) && (analysis->MianziDat[0].tile == i + 7))
						yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 長城 */
	if (RuleData::chkRuleApplied("choujou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("長城"), get_yaku_han("choujou"),
			_T("一気通貫"), _T("清一色"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
					int yakuFlagCount = 0;
					if ((analysis->ShunziCount[i + 1] >= 2) &&
						(analysis->ShunziCount[i + 4] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 1)) yakuFlagCount += 2;
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 4] >= 2) &&
						(analysis->ShunziCount[i + 7] >= 1)) yakuFlagCount += 2;
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 4] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 2)) yakuFlagCount += 2;
					if ((analysis->ShunziCount[i + 1] == 1) &&
						(analysis->ShunziCount[i + 4] == 1) &&
						(analysis->ShunziCount[i + 7] == 1)) yakuFlagCount += 1;
					if ((analysis->ShunziCount[i + 2] >= 1) ||
						(analysis->ShunziCount[i + 3] >= 1) ||
						(analysis->ShunziCount[i + 5] >= 1) ||
						(analysis->ShunziCount[i + 6] >= 1)) yakuFlagCount += 1;
					if ((yakuFlagCount == 2) &&
						((analysis->MianziDat[0].tile == i + 2) ||
						(analysis->MianziDat[0].tile == i + 5) ||
						(analysis->MianziDat[0].tile == i + 8))) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 青龍在天 */
	if (RuleData::chkRuleApplied("qinglong_zaitian"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("青龍在天"), get_yaku_han("qinglong_zaitian"),
			_T("一気通貫"), _T("清一色"), _T("一盃口"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫で
					((analysis->MianziDat[0].tile / TileSuitStep) == (yakuCol / TileSuitStep)) && // 一色で
					((analysis->ShunziCount[yakuCol + 1] >= 2) ||
					(analysis->ShunziCount[yakuCol + 4] >= 2) ||
					(analysis->ShunziCount[yakuCol + 7] >= 2)) // 一盃口になっている
					);
			}
		));
	/* 鏡同和 */
	if (RuleData::chkRuleApplied("kyantonhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鏡同和"), get_yaku_han("kyantonhoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < (TileSuitHonors * 2); i += TileSuitStep * 2) {
					int yakuTmpFlag = 0;
					for (int k = 1; k <= 7; k++)
						if ((analysis->ShunziCount[i % TileSuitHonors + k] >= 1) &&
							(analysis->ShunziCount[(i + TileSuitStep) % TileSuitHonors + k] >= 1))
								++yakuTmpFlag;
					if (yakuTmpFlag == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 四歩高 */
	auto puukao =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suupuukao) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (7 - step * (suupuukao ? 3 : 2)); k++)
					if ((analysis->ShunziCount[(int)(parsedat[i * fldsize + 0] - _T('0')) * TileSuitStep + step * 0 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 1] - _T('0')) * TileSuitStep + step * 1 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 2] - _T('0')) * TileSuitStep + step * 2 + k] >= 1) &&
						((!suupuukao)||(analysis->ShunziCount[(int)(parsedat[i * fldsize + 3] - _T('0')) * TileSuitStep + step * 3 + k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色四歩高"), yaku::yakuCalculator::Yaku::yval_32,
			_T("一色三歩高"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四歩高"), get_yaku_han("okasuujun"),
			_T("三歩高"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
			}
		));
	/* 三歩高 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色三歩高"), yaku::yakuCalculator::Yaku::yval_16,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色三歩高"), get_yaku_han("okasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
			}
		));
	/* 山三順 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一色三歩高"), yaku::yakuCalculator::Yaku::yval_16,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("清連環套"), get_yaku_han("yamasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, false);
			}
		));
#ifndef GUOBIAO
	/* 二色四歩高 */
	if (RuleData::chkRuleApplied("nishoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色四歩高"), get_yaku_han("nishoku_okasuujun"),
			_T("二色三歩高"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* 二色三歩高 */
	if (RuleData::chkRuleApplied("nishoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二色三歩高"), get_yaku_han("nishoku_okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* 二色山四順 */
	if (RuleData::chkRuleApplied("nishoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大雑連套"), get_yaku_han("nishoku_yamasuujun"),
			_T("雑連環套"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* 二色山三順 */
	if (RuleData::chkRuleApplied("nishoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("雑連環套"), get_yaku_han("nishoku_yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* 三色四歩高 */
	if (RuleData::chkRuleApplied("sanshoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色四歩高"), get_yaku_han("sanshoku_okasuujun"),
			_T("三色三歩高"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
#endif /* GUOBIAO */
	/* 三色三歩高 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色三歩高"), yaku::yakuCalculator::Yaku::yval_6,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("sanshoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色三歩高"), get_yaku_han("sanshoku_okasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
#ifndef GUOBIAO
	/* 三色山四順 */
	if (RuleData::chkRuleApplied("sanshoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大混連套"), get_yaku_han("sanshoku_yamasuujun"),
			_T("混連環套"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
#endif /* GUOBIAO */
	/* 三色山三順 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色三歩高"), yaku::yakuCalculator::Yaku::yval_6,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("sanshoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("混連環套"), get_yaku_han("sanshoku_yamasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 双龍争珠 */
	if (RuleData::chkRuleApplied("shanron_chonchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("双龍争珠"), get_yaku_han("shanron_chonchuu"),
			_T("鏡同和"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 4; i++)
					if ((analysis->ShunziCount[TileSuitCharacters + i] >= 1) &&
						(analysis->ShunziCount[TileSuitCharacters + i + 3] >= 1) &&
						(analysis->ShunziCount[TileSuitBamboos + i] >= 1) &&
						(analysis->ShunziCount[TileSuitBamboos + i + 3] >= 1) &&
						((analysis->MianziDat[0].tile / TileSuitStep) == (TileSuitCircles / TileSuitStep)))
							yakuFlag = true;
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 花龍 */
	auto sanshoku_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
				if ((analysis->ShunziCount[(int)((*k)[0] - _T('0')) * TileSuitStep + 1] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[1] - _T('0')) * TileSuitStep + 4] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[2] - _T('0')) * TileSuitStep + 7] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};

#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("花龍"), yaku::yakuCalculator::Yaku::yval_8,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("hualong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色通貫"), get_yaku_han("hualong"),
#endif /* GUOBIAO */
			sanshoku_tsuukan
		));
#ifndef GUOBIAO
	/* 単龍 */
	if (RuleData::chkRuleApplied("tanron"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("単龍"), get_yaku_han("tanron"),
			_T("三色通貫"),
			[sanshoku_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (sanshoku_tsuukan(analysis) && // 三色通貫で
					(*analysis->MenzenFlag) && // 門前で
					(analysis->Machi == yaku::yakuCalculator::machiTanki) && // 単騎待ちで
					(analysis->TsumoHai->tile != WhiteDragon) && // 白待ちでなく
					(analysis->TsumoHai->tile != GreenDragon) ); // 發待ちでない
			}
		));
	/* 三色通貫全帯幺九 */
	if (RuleData::chkRuleApplied("hualong_chanta")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色通貫全帯幺九"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("三色通貫"),
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting( analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純三色通貫全帯幺九"), yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			_T("三色通貫"),
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}

	// ---------------------------------------------------------------------

	/* 双龍双亀 */
#endif /* GUOBIAO */
	auto ShuangLong =
		[](const MENTSU_ANALYSIS* const analysis, int suit1, int suit2, TileCode head) -> bool {
			return ((analysis->ShunziCount[suit1 + 1] >= 1) &&
				(analysis->ShunziCount[suit1 + 7] >= 1) &&
				(analysis->ShunziCount[suit2 + 1] >= 1) &&
				(analysis->ShunziCount[suit2 + 7] >= 1) &&
				(analysis->MianziDat[0].tile == head));
		};

#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("shanron_shankui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("双龍双亀"), get_yaku_han("shanron_shankui"),
			_T("鏡同和"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooThree);
			}
		));
	/* 双龍争珠(老少副タイプ) */
	if (RuleData::chkRuleApplied("shanron_chonchu_lsf"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("双龍争珠 "), get_yaku_han("shanron_chonchu_lsf"),
			_T("鏡同和"), _T("三色双龍会"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitBamboos, CircleFive);
			}
		));
#endif /* GUOBIAO */
	/* 三色双龍会 */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色双龍会"), yaku::yakuCalculator::Yaku::yval_16,
			_T("平和"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("sanshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色双龍会"), get_yaku_han("sanshoku_souryuu"),
			_T("鏡同和"),
#endif /* GUOBIAO */
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
					if (ShuangLong(analysis, (int)((*k)[0] - _T('0')) * TileSuitStep,
						(int)((*k)[1] - _T('0')) * TileSuitStep,
						(TileCode)((int)((*k)[1] - _T('0')) * TileSuitStep + 5))) return true;
				return false;
			}
		));
#ifndef GUOBIAO
	/* 双龍戯鳳 */
	if (RuleData::chkRuleApplied("shanron_shiifon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("双龍戯鳳"), get_yaku_han("shanron_shiifon"),
			_T("鏡同和"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooOne);
			}
		));
	/* 双龍包柱 */
	if (RuleData::chkRuleApplied("shanron_paochuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("双龍包柱"), get_yaku_han("shanron_paochuu"),
			_T("鏡同和"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooTwo) ||
					ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooFour) ||
					ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooSix));
			}
		));
	/* 双龍戯亀 */
	if (RuleData::chkRuleApplied("shanron_shiikui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("双龍戯亀"), get_yaku_han("shanron_shiikui"),
			_T("鏡同和"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooSeven);
			}
		));
	/* 双龍闘蛇 */
	if (RuleData::chkRuleApplied("shanron_touja"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("双龍戯亀"), get_yaku_han("shanron_touja"),
			_T("鏡同和"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCircles, TileSuitBamboos, CharacterOne);
			}
		));

	// ---------------------------------------------------------------------

	/* 吃三嵌 */
	auto kanchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++)
				if (analysis->MianziDat[i].mstat == meldSequenceExposedMiddle) ++count;
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃三嵌"), get_yaku_han("chii_sankan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 3);
			}
		));
	/* 吃四嵌 */
	if (RuleData::chkRuleApplied("chii_suukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃四嵌"), get_yaku_han("chii_suukan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 4);
			}
		));
	/* 吃三辺 */
	auto penchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++) {
				for (int j = 0; j < TileSuitHonors; j += TileSuitStep) {
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedLower) &&
						(analysis->MianziDat[i].tile == TileSuitStep + 7)) ++count;
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedUpper) &&
						(analysis->MianziDat[i].tile == TileSuitStep + 1)) ++count;
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sanpen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃三辺"), get_yaku_han("chii_sanpen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 3);
			}
		));
	/* 吃四辺 */
	if (RuleData::chkRuleApplied("chii_suupen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃四辺"), get_yaku_han("chii_suupen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 4);
			}
		));
	/* 吃小順 */
	auto xiaoShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedLower) {
					++count;
					for (int j = 0; j < TileSuitHonors; j += TileSuitStep) {
						if (analysis->MianziDat[i].tile == TileSuitStep + 7) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_shaoshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃小順"), get_yaku_han("chii_shaoshun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 3);
			}
		));
	/* 吃小四順 */
	if (RuleData::chkRuleApplied("chii_shaosuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃小四順"), get_yaku_han("chii_shaosuushun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 4);
			}
		));
	/* 吃大順 */
	auto daShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedUpper) {
					++count;
					for (int j = 0; j < TileSuitHonors; j += TileSuitStep) {
						if (analysis->MianziDat[i].tile == TileSuitStep + 1) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_taashun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃大順"), get_yaku_han("chii_taashun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 3);
			}
		));
	/* 吃大四順 */
	if (RuleData::chkRuleApplied("chii_taasuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("吃大四順"), get_yaku_han("chii_taasuushun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 4);
			}
		));
#endif /* GUOBIAO */
}
