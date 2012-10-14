#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* 一色双龍会 */
	if (RuleData::chkRuleApplied("isshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色双龍会", get_yaku_han("isshoku_souryuu"),
			"清一色", "二盃口", "鳴き二盃口",
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
	if (RuleData::chkRuleApplied("isshoku_suujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色四順", get_yaku_han("isshoku_suujun"),
			"一色三順", "一盃口", "二盃口", "鳴き二盃口", "重四帰四", "四帰四",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 一色三順 */
	if (RuleData::chkRuleApplied("isshoku_sanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色三順", get_yaku_han("isshoku_sanjun"),
			"一盃口",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 3) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 一盃口 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"一盃口", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 1);
		}
	));
	/* 二盃口 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"二盃口", get_yaku_han("ryanpeikou"),
		"一盃口",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 2);
		}
	));
	/* 大鏡 */
	if (RuleData::chkRuleApplied("ohkagami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大鏡", get_yaku_han("ohkagami"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return ((yakuFlagCount == 2)&&(analysis->MianziDat[0].tile == CircleOne));
			}
		));
	/* 清盃口 */
	if (RuleData::chkRuleApplied("chinpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"清盃口", get_yaku_han("chinpeikou"),
			"二盃口",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlagCount = false;
				for (int i = 1; i <= 7; i++) {
					if ((analysis->ShunziCount[i + TILE_SUIT_CHARACTERS] == 2) &&
						(analysis->ShunziCount[i + TILE_SUIT_CIRCLES] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TILE_SUIT_CHARACTERS] == 2) &&
						(analysis->ShunziCount[i + TILE_SUIT_BAMBOOS] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TILE_SUIT_CIRCLES] == 2) &&
						(analysis->ShunziCount[i + TILE_SUIT_BAMBOOS] == 2))
						yakuFlagCount = true;
				}
				return yakuFlagCount;
			}
		));
	/* 鳴き二盃口 */
	if (RuleData::chkRuleApplied("exposed_ryanpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鳴き二盃口", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return (yakuFlagCount == 2);
			}
		));

	// ---------------------------------------------------------------------

	/* サンショクともサンシキとも呼ばれる */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"三色同順", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 7; i++)
				if ((analysis->ShunziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
					(analysis->ShunziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
					(analysis->ShunziCount[i + TILE_SUIT_BAMBOOS] >= 1)) yakuFlag = true;
			return yakuFlag;
		}
	));

	// ---------------------------------------------------------------------

	/* イーチートンカン */
	auto ikki_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			*yakuFlag = false;
			for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
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
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
				if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) ==
					(*yakuCol / TILE_SUIT_STEP)) ++samecol;
				else if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_HONORS / TILE_SUIT_STEP)) ++samecol;
			}
			*yakuFlag = (samecol == SIZE_OF_MELD_BUFFER); // 一色になっているかどうか
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"一気通貫", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return yakuFlag;
			}
	));
	/* 20101204祝東京～新青森全通 */
	if (RuleData::chkRuleApplied("tohoku_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東北新幹線", get_yaku_han("tohoku_shinkansen"),
			"一気通貫", "混一色",
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
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
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
			"一気通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"一気通貫",
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, nullptr);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純一気通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"一気通貫",
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
			"五心通貫", get_yaku_han("uushintonkan"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // 嵌張待ちで
					(analysis->PlayerStat->Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // 和了牌が5
					);
			}
		));
	/* 竜戯珠 */
	if (RuleData::chkRuleApplied("longxizhu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"竜戯珠", get_yaku_han("longxizhu"),
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
			"金龍戯北斗", get_yaku_han("jinlong_dou_beidou"),
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
		const tileCode tmpTileCodeList[Rules][2] = {
			{EastWind, BambooEight}, {SouthWind, RedDragon}, {WestWind, CircleEight}, {NorthWind, CharacterEight},
			{WhiteDragon, BambooTwo}, {GreenDragon, CharacterTwo}, {RedDragon, CircleTwo},
			{WhiteDragon, CharacterOne}, {GreenDragon, CircleOne}, {RedDragon, BambooOne},
		};
		const int tmpStraightColorList[Rules] = {
			TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS, TILE_SUIT_CIRCLES, TILE_SUIT_CHARACTERS,
			TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS,
		};
		const char tmpYakuNameList[Rules][16] = {
			"双峰挿雲", "南屏晩鍾", "雷峰夕照", "柳浪聞鶯", "断橋残雪",
			"蘇堤春暁", "花港観魚", "三譚印月", "曲院風荷", "平湖秋月",
		};
		for (int i = 0; i < Rules; i++) {
			if (!RuleData::chkRuleApplied(tmpRuleCodeList[i])) continue;
			tileCode tmpTileCode1 = tmpTileCodeList[i][0];
			tileCode tmpTileCode2 = tmpTileCodeList[i][1];
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
			"東の青龍", get_yaku_han("azure_dragon"),
			"一気通貫", "混一色",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[EastWind] >= 1) && // 東の刻子があり
					(analysis->MianziDat[0].tile == WhiteDragon) && // アタマが白で
					(yakuCol == TILE_SUIT_CIRCLES) ); // イッツー部分が筒子
			}
		));
	/* 北の玄武 */
	if (RuleData::chkRuleApplied("black_tortoise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北の玄武", get_yaku_han("black_tortoise"),
			"一気通貫",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[NorthWind] >= 1) && // 北の刻子があり
					(analysis->MianziDat[0].tile == BambooSeven) && // アタマが七索で
					(yakuCol == TILE_SUIT_CIRCLES) ); // イッツー部分が筒子
			}
		));
	/* 秋田新幹線 */
	if (RuleData::chkRuleApplied("akita_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"秋田新幹線", get_yaku_han("akita_shinkansen"),
			"一気通貫", "混一色",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[NorthWind] >= 1) && // 北の刻子があり
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (yakuCol / TILE_SUIT_STEP)) && // 一色になっていて
					(analysis->PlayerStat->Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // 和了牌が5
					);
			}
		));
	/* 上越新幹線とき */
	if (RuleData::chkRuleApplied("joetsu_toki"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"上越新幹線とき", get_yaku_han("joetsu_toki"),
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
			"上越新幹線あさひ", get_yaku_han("joetsu_asahi"),
			"上越新幹線とき", "一気通貫", "混一色",
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
			"青函連絡船", get_yaku_han("seikan_ship"),
			"一気通貫", "混一色",
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
					(yakuCol == TILE_SUIT_CIRCLES) && // 筒子のイッツーで
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
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "2han_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush2
			));
		else if (RuleData::chkRule("goldrush", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_yakuman,
				"一気通貫", "混一色",
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "yakuman_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_yakuman,
				"一気通貫", "混一色",
				goldrush2
			));
	}
	/* ルート２４６ */
	if (RuleData::chkRuleApplied("route_246"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ルート２４６", get_yaku_han("route_246"),
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
			"九龍城落地", get_yaku_han("gaulungsing_lokdei"),
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
			"北海道新幹線", get_yaku_han("hokkaido_shinkansen"),
			"一気通貫", "混一色",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->AnKangziCount[WhiteDragon] >= 1) && // 白の暗槓があって
					(analysis->MianziDat[0].tile == NorthWind) ); // 雀頭が北
			}
		));

	// ---------------------------------------------------------------------

	/* ゴールデンゲートブリッジ */
	if (RuleData::chkRuleApplied("golden_gate_bridge"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金門橋", get_yaku_han("golden_gate_bridge"),
			"清連環套",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 3] >= 1) &&
						(analysis->ShunziCount[i + 5] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 1)) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* Arc de Triomphe */
	if (RuleData::chkRuleApplied("gaisenmon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"凱旋門", get_yaku_han("gaisenmon"),
			"清一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					int yakuFlagCount = 0;
					if ((analysis->ShunziCount[i + 1] >= 1) && (analysis->ShunziCount[i + 4] >= 1)) ++yakuFlag;
					if ((analysis->ShunziCount[i + 2] >= 1) && (analysis->ShunziCount[i + 5] >= 1)) ++yakuFlag;
					if ((analysis->ShunziCount[i + 3] >= 1) && (analysis->ShunziCount[i + 6] >= 1)) ++yakuFlag;
					if ((analysis->ShunziCount[i + 4] >= 1) && (analysis->ShunziCount[i + 7] >= 1)) ++yakuFlag;
					if (yakuFlagCount == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* 長城 */
	if (RuleData::chkRuleApplied("choujou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"長城", get_yaku_han("choujou"),
			"一気通貫", "清一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
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
			"青龍在天", get_yaku_han("qinglong_zaitian"),
			"一気通貫", "清一色", "一盃口",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫で
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (yakuCol / TILE_SUIT_STEP)) && // 一色で
					((analysis->ShunziCount[yakuCol + 1] >= 2) ||
					(analysis->ShunziCount[yakuCol + 4] >= 2) ||
					(analysis->ShunziCount[yakuCol + 7] >= 2)) // 一盃口になっている
					);
			}
		));
	/* 鏡同和 */
	if (RuleData::chkRuleApplied("kyantonhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鏡同和", get_yaku_han("kyantonhoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < (TILE_SUIT_HONORS * 2); i += TILE_SUIT_STEP * 2) {
					int yakuTmpFlag = 0;
					for (int k = 1; k <= 7; k++)
						if ((analysis->ShunziCount[i % TILE_SUIT_HONORS + k] >= 1) &&
							(analysis->ShunziCount[(i + TILE_SUIT_STEP) % TILE_SUIT_HONORS + k] >= 1))
								++yakuTmpFlag;
					if (yakuTmpFlag == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 四歩高 */
	auto puukao =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suupuukao) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (7 - step * (suupuukao ? 3 : 2)); k++)
					if ((analysis->ShunziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * 0 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * 1 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * 2 + k] >= 1) &&
						(suupuukao&&(analysis->ShunziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * 3 + k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四歩高", get_yaku_han("okasuujun"),
			"三歩高",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
			}
		));
	/* 三歩高 */
	if (RuleData::chkRuleApplied("okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三歩高", get_yaku_han("okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
			}
		));
	/* 山三順 */
	if (RuleData::chkRuleApplied("yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"清連環套", get_yaku_han("yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* 二色四歩高 */
	if (RuleData::chkRuleApplied("nishoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色四歩高", get_yaku_han("nishoku_okasuujun"),
			"二色三歩高",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* 二色三歩高 */
	if (RuleData::chkRuleApplied("nishoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色三歩高", get_yaku_han("nishoku_okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* 二色山四順 */
	if (RuleData::chkRuleApplied("nishoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大雑連套", get_yaku_han("nishoku_yamasuujun"),
			"雑連環套",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* 二色山三順 */
	if (RuleData::chkRuleApplied("nishoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"雑連環套", get_yaku_han("nishoku_yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* 三色四歩高 */
	if (RuleData::chkRuleApplied("sanshoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色四歩高", get_yaku_han("sanshoku_okasuujun"),
			"三色三歩高",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* 三色三歩高 */
	if (RuleData::chkRuleApplied("sanshoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色三歩高", get_yaku_han("sanshoku_okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* 三色山四順 */
	if (RuleData::chkRuleApplied("sanshoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大混連套", get_yaku_han("sanshoku_yamasuujun"),
			"混連環套",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* 三色山三順 */
	if (RuleData::chkRuleApplied("nishoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"混連環套", get_yaku_han("nishoku_yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 双龍争珠 */
	if (RuleData::chkRuleApplied("shanron_chonchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍争珠", get_yaku_han("shanron_chonchuu"),
			"鏡同和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 4; i++)
					if ((analysis->ShunziCount[TILE_SUIT_CHARACTERS + i] >= 1) &&
						(analysis->ShunziCount[TILE_SUIT_CHARACTERS + i + 3] >= 1) &&
						(analysis->ShunziCount[TILE_SUIT_BAMBOOS + i] >= 1) &&
						(analysis->ShunziCount[TILE_SUIT_BAMBOOS + i + 3] >= 1) &&
						((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP)))
							yakuFlag = true;
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 花龍 */
	auto sanshoku_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
				if ((analysis->ShunziCount[(int)((*k)[0] - '0') * TILE_SUIT_STEP + 1] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[1] - '0') * TILE_SUIT_STEP + 4] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[2] - '0') * TILE_SUIT_STEP + 7] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};

	if (RuleData::chkRuleApplied("hualong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色通貫", get_yaku_han("hualong"),
			sanshoku_tsuukan
		));
	/* 単龍 */
	if (RuleData::chkRuleApplied("tanron"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"単龍", get_yaku_han("tanron"),
			"三色通貫",
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
			"三色通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"三色通貫",
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting( analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純三色通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"三色通貫",
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}

	// ---------------------------------------------------------------------

	/* 双龍双亀 */
	auto ShuangLong =
		[](const MENTSU_ANALYSIS* const analysis, int suit1, int suit2, tileCode head) -> bool {
			return ((analysis->ShunziCount[suit1 + 1] >= 1) &&
				(analysis->ShunziCount[suit1 + 7] >= 1) &&
				(analysis->ShunziCount[suit2 + 1] >= 1) &&
				(analysis->ShunziCount[suit2 + 7] >= 1) &&
				(analysis->MianziDat[0].tile == head));
		};

	if (RuleData::chkRuleApplied("shanron_shankui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍双亀", get_yaku_han("shanron_shankui"),
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooThree);
			}
		));
	/* 双龍争珠(老少副タイプ) */
	if (RuleData::chkRuleApplied("shanron_chonchu_lsf"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍争珠 ", get_yaku_han("shanron_chonchu_lsf"),
			"鏡同和", "三色双龍会",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_BAMBOOS, CircleFive);
			}
		));
	/* 三色双龍会 */
	if (RuleData::chkRuleApplied("sanshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色双龍会", get_yaku_han("sanshoku_souryuu"),
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
					if (ShuangLong(analysis, (int)((*k)[0] - '0') * TILE_SUIT_STEP,
						(int)((*k)[1] - '0') * TILE_SUIT_STEP,
						(tileCode)((int)((*k)[1] - '0') * TILE_SUIT_STEP + 5))) return true;
				return false;
			}
		));
	/* 双龍戯鳳 */
	if (RuleData::chkRuleApplied("shanron_shiifon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍戯鳳", get_yaku_han("shanron_shiifon"),
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooOne);
			}
		));
	/* 双龍包柱 */
	if (RuleData::chkRuleApplied("shanron_paochuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍包柱", get_yaku_han("shanron_paochuu"),
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooTwo) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooFour) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSix));
			}
		));
	/* 双龍戯亀 */
	if (RuleData::chkRuleApplied("shanron_shiikui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍戯亀", get_yaku_han("shanron_shiikui"),
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSeven);
			}
		));
	/* 双龍闘蛇 */
	if (RuleData::chkRuleApplied("shanron_touja"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍戯亀", get_yaku_han("shanron_touja"),
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS, CharacterOne);
			}
		));

	// ---------------------------------------------------------------------

	/* 吃三嵌 */
	auto kanchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++)
				if (analysis->MianziDat[i].mstat == meldSequenceExposedMiddle) ++count;
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃三嵌", get_yaku_han("chii_sankan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 3);
			}
		));
	/* 吃四嵌 */
	if (RuleData::chkRuleApplied("chii_suukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃四嵌", get_yaku_han("chii_suukan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 4);
			}
		));
	/* 吃三辺 */
	auto penchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++) {
				for (int j = 0; j < TILE_SUIT_HONORS; j += TILE_SUIT_STEP) {
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedLower) &&
						(analysis->MianziDat[i].tile == TILE_SUIT_STEP + 7)) ++count;
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedUpper) &&
						(analysis->MianziDat[i].tile == TILE_SUIT_STEP + 1)) ++count;
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sanpen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃三辺", get_yaku_han("chii_sanpen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 3);
			}
		));
	/* 吃四辺 */
	if (RuleData::chkRuleApplied("chii_suupen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃四辺", get_yaku_han("chii_suupen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 4);
			}
		));
	/* 吃小順 */
	auto xiaoShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedLower) {
					++count;
					for (int j = 0; j < TILE_SUIT_HONORS; j += TILE_SUIT_STEP) {
						if (analysis->MianziDat[i].tile == TILE_SUIT_STEP + 7) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_shaoshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃小順", get_yaku_han("chii_shaoshun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 3);
			}
		));
	/* 吃小四順 */
	if (RuleData::chkRuleApplied("chii_shaosuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃小四順", get_yaku_han("chii_shaosuushun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 4);
			}
		));
	/* 吃大順 */
	auto daShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedUpper) {
					++count;
					for (int j = 0; j < TILE_SUIT_HONORS; j += TILE_SUIT_STEP) {
						if (analysis->MianziDat[i].tile == TILE_SUIT_STEP + 1) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_taashun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃大順", get_yaku_han("chii_taashun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 3);
			}
		));
	/* 吃大四順 */
	if (RuleData::chkRuleApplied("chii_taasuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃大四順", get_yaku_han("chii_taasuushun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 4);
			}
		));
}
