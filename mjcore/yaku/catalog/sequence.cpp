#include "../catalog.h"

extern const std::array<char[8], 3> parsedat_monochrome4;
extern const std::array<char[4], 3> parsedat_monochrome3;
extern const std::array<char[8], 42> parsedat_bichrome4;
extern const std::array<char[4], 18> parsedat_bichrome3;
extern const std::array<char[8], 36> parsedat_trichrome4;
extern const std::array<char[4], 6> parsedat_trichrome3;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* 一色双龍会 */
	if (RuleData::getRule("isshoku_souryuu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色双龍会", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (RuleData::getRule("isshoku_suujun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色四順", yaku::yakuCalculator::Yaku::yval_yakuman,
			"一色三順", "一盃口", "二盃口", "鳴き二盃口", "重四帰四", "四帰四",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 一色三順 */
	if (RuleData::getRule("isshoku_sanjun") != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC hf = yaku::yakuCalculator::Yaku::HANFUNC();
		switch (RuleData::getRule("isshoku_sanjun")) {
			case 1: hf = yaku::yakuCalculator::Yaku::yval_2han_menzen; break;
			case 2: hf = yaku::yakuCalculator::Yaku::yval_2han_kuisagari; break;
			case 3: hf = yaku::yakuCalculator::Yaku::yval_3han_menzen; break;
			case 4: hf = yaku::yakuCalculator::Yaku::yval_3han_kuisagari; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色三順", hf,
			"一盃口",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 3) yakuFlag = true;
				return yakuFlag;
			}
		));
	}
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
		"二盃口", (RuleData::getRule("ryanpeikou") != 0) ?
		yaku::yakuCalculator::Yaku::yval_2han_menzen : yaku::yakuCalculator::Yaku::yval_3han_menzen,
		"一盃口",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 2);
		}
	));
	/* 大鏡 */
	if (RuleData::getRule("ohkagami") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大鏡", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return ((yakuFlagCount == 2)&&(analysis->MianziDat[0].tile == CircleOne));
			}
		));
	/* 清盃口 */
	if (RuleData::getRule("chinpeikou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"清盃口", (RuleData::getRule("chinpeikou") == 1) ? yaku::yakuCalculator::Yaku::yval_6han_menzen :
			((RuleData::getRule("chinpeikou") == 2) ? yaku::yakuCalculator::Yaku::yval_5han_menzen :
			yaku::yakuCalculator::Yaku::yval_4han_menzen),
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
	if (RuleData::getRule("exposed_ryanpeikou") != 0)
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
		"三色同順",yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
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
						if (yakuCol != NULL) *yakuCol = i;
				}
			}
		};
	auto ittsuu_monotonic =
		[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			int samecol;
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
		"一気通貫",yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->DuiziCount[EastWind] >= 1) && // 東と
					(analysis->DuiziCount[NorthWind] >= 1) ); // 北がある
			}
	));
	/* 20101204祝東京～新青森全通 */
	if (RuleData::getRule("tohoku_shinkansen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東北新幹線", (RuleData::getRule("tohoku_shinkansen") == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"一気通貫", "混一色",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
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
	if (RuleData::getRule("ittsuu_chanta") != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一気通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"一気通貫",
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, NULL);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純一気通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"一気通貫",
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, NULL);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}
	/* 五心通貫 */
	if (RuleData::getRule("uushintonkan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"五心通貫", yaku::yakuCalculator::Yaku::yval_1han,
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
	if (RuleData::getRule("longxizhu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"竜戯珠", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[WestWind] >= 1) && // 西の刻子があり
					(analysis->MianziDat[0].tile == CircleOne) ); // アタマが一筒
			}
		));
	/* 金龍戯北斗 */
	if (RuleData::getRule("jinlong_dou_beidou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金龍戯北斗", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
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
			if (RuleData::getRule(tmpRuleCodeList[i]) == 0) continue;
			tileCode tmpTileCode1 = tmpTileCodeList[i][0];
			tileCode tmpTileCode2 = tmpTileCodeList[i][1];
			int tmpStraightColor = tmpStraightColorList[i];
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				tmpYakuNameList[i], yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
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
	if (RuleData::getRule("azure_dragon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東の青龍", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
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
	if (RuleData::getRule("black_tortoise") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北の玄武", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
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
	if (RuleData::getRule("akita_shinkansen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"秋田新幹線", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (RuleData::getRule("joetsu_toki") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"上越新幹線とき", yaku::yakuCalculator::Yaku::yval_2han,
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(analysis->DuiziCount[NorthWind] >= 1) // 北の刻子または雀頭がある
					);
			}
		));
	/* 上越新幹線あさひ */
	if (RuleData::getRule("joetsu_asahi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"上越新幹線あさひ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"上越新幹線とき", "一気通貫", "混一色",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->DuiziCount[NorthWind] >= 1) && // 北と
					(analysis->DuiziCount[WhiteDragon] >= 1) // 白が刻子と雀頭として存在
					);
			}
		));
	/* 青函連絡船 */
	if (RuleData::getRule("seikan_ship") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"青函連絡船", yaku::yakuCalculator::Yaku::yval_yakuman,
			"一気通貫", "混一色",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->MianziDat[0].tile == NorthWind) && // 北が雀頭で
					(analysis->AnKangziCount[GreenDragon] >= 1) // 發の暗槓がある
					);
			}
		));
	/* ゴールドラッシュ */
	if (RuleData::getRule("goldrush") != 0) {
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
		switch (RuleData::getRule("goldrush")) {
		case 1:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush1
			)); break;
		case 2:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush2
			)); break;
		case 3:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_yakuman,
				"一気通貫", "混一色",
				goldrush1
			)); break;
		case 4:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ゴールドラッシュ", yaku::yakuCalculator::Yaku::yval_yakuman,
				"一気通貫", "混一色",
				goldrush2
			)); break;
		}
	}
	/* ルート２４６ */
	if (RuleData::getRule("route_246") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ルート２４６", yaku::yakuCalculator::Yaku::yval_1han,
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
	if (RuleData::getRule("gaulungsing_lokdei") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"九龍城落地", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (RuleData::getRule("hokkaido_shinkansen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北海道新幹線", yaku::yakuCalculator::Yaku::yval_yakuman,
			"一気通貫", "混一色",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->AnKangziCount[WhiteDragon] >= 1) && // 白の暗槓があって
					(analysis->MianziDat[0].tile == NorthWind) ); // 雀頭が北
			}
		));

	// ---------------------------------------------------------------------

	/* ゴールデンゲートブリッジ */
	if (RuleData::getRule("golden_gate_bridge") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金門橋", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (RuleData::getRule("gaisenmon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"凱旋門", RuleData::getRule("gaisenmon") == 1 ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
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
	if (RuleData::getRule("choujou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"長城", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (RuleData::getRule("qinglong_zaitian") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"青龍在天", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (RuleData::getRule("kyantonhoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鏡同和", (RuleData::getRule("kyantonhoh") == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < (TILE_SUIT_HONORS * 2); i += TILE_SUIT_STEP * 2) {
					int yakuTmpFlag = 0;
					for (int k = 1; k <= 7; k++)
						if ((analysis->ShunziCount[i % TILE_SUIT_HONORS + k] >= 1) &&
							(analysis->ShunziCount[(i + TILE_SUIT_HONORS) % TILE_SUIT_HONORS + k] >= 1))
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
					if ((analysis->ShunziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(suupuukao&&(analysis->ShunziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	if (RuleData::getRule("okasuujun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四歩高", (RuleData::getRule("okasuujun") == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"三歩高",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
			}
		));
	/* 三歩高 */
	if (RuleData::getRule("okasanjun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三歩高", (RuleData::getRule("okasanjun") == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((RuleData::getRule("okasanjun") == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
			}
		));
	/* 山三順 */
	if (RuleData::getRule("yamasanjun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"清連環套", (RuleData::getRule("yamasanjun") == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((RuleData::getRule("yamasanjun") == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* 二色四歩高 */
	if (RuleData::getRule("nishoku_okasuujun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色四歩高", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"二色三歩高",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* 二色三歩高 */
	if (RuleData::getRule("nishoku_okasanjun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二色三歩高", yaku::yakuCalculator::Yaku::yval_1han,
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* 二色山四順 */
	if (RuleData::getRule("nishoku_yamasuujun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大雑連套", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"雑連環套",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* 二色山三順 */
	if (RuleData::getRule("nishoku_yamasanjun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"雑連環套", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* 三色四歩高 */
	if (RuleData::getRule("sanshoku_okasuujun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色四歩高", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"三色三歩高",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* 三色三歩高 */
	if (RuleData::getRule("sanshoku_okasanjun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色三歩高", (RuleData::getRule("sanshoku_okasanjun") == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((RuleData::getRule("sanshoku_okasanjun") == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* 三色山四順 */
	if (RuleData::getRule("sanshoku_yamasuujun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大混連套", (RuleData::getRule("sanshoku_yamasuujun") == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
			"混連環套",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* 三色山三順 */
	if (RuleData::getRule("nishoku_yamasanjun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"混連環套", (RuleData::getRule("nishoku_yamasanjun") == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((RuleData::getRule("nishoku_yamasanjun") == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));

	// ---------------------------------------------------------------------

	/* 双龍争珠 */
	if (RuleData::getRule("shanron_chonchuu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍争珠", yaku::yakuCalculator::Yaku::yval_yakuman,
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
				if ((analysis->ShunziCount[(int)((&*k)[0] - '0') * TILE_SUIT_STEP + 1] >= 1) &&
					(analysis->ShunziCount[(int)((&*k)[1] - '0') * TILE_SUIT_STEP + 4] >= 1) &&
					(analysis->ShunziCount[(int)((&*k)[2] - '0') * TILE_SUIT_STEP + 7] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};

	if (RuleData::getRule("hualong") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色通貫", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			sanshoku_tsuukan
		));
	/* 単龍 */
	if (RuleData::getRule("tanron") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"単龍", yaku::yakuCalculator::Yaku::yval_6han_menzen,
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
	if (RuleData::getRule("hualong_chanta") != 0) {
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

	if (RuleData::getRule("shanron_shankui") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍双亀", yaku::yakuCalculator::Yaku::yval_4han,
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooThree);
			}
		));
	/* 双龍争珠(老少副タイプ) */
	if (RuleData::getRule("shanron_chonchu_lsf") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍争珠 ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"鏡同和", "三色双龍会",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_BAMBOOS, CircleFive);
			}
		));
	/* 三色双龍会 */
	if (RuleData::getRule("sanshoku_souryuu") != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC han;
		switch (RuleData::getRule("sanshoku_souryuu")) {
			case 1: han = yaku::yakuCalculator::Yaku::yval_2han_kuisagari; break;
			case 2: han = yaku::yakuCalculator::Yaku::yval_6han_kuisagari; break;
			case 3: han = yaku::yakuCalculator::Yaku::yval_6han; break;
			case 4: han = yaku::yakuCalculator::Yaku::yval_yakuman; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色双龍会", han,
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
					if (ShuangLong(analysis, (int)((&*k)[0] - '0') * TILE_SUIT_STEP,
						(int)((&*k)[1] - '0') * TILE_SUIT_STEP,
						(tileCode)((int)((&*k)[1] - '0') * TILE_SUIT_STEP + 5))) return true;
				return false;
			}
		));
	}
	/* 双龍戯鳳 */
	if (RuleData::getRule("shanron_shiifon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍戯鳳", yaku::yakuCalculator::Yaku::yval_4han,
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooOne);
			}
		));
	/* 双龍包柱 */
	if (RuleData::getRule("shanron_paochuu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍包柱", yaku::yakuCalculator::Yaku::yval_4han,
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooTwo) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooFour) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSix));
			}
		));
	/* 双龍戯亀 */
	if (RuleData::getRule("shanron_shiikui") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍戯亀", yaku::yakuCalculator::Yaku::yval_4han,
			"鏡同和",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSeven);
			}
		));
	/* 双龍闘蛇 */
	if (RuleData::getRule("shanron_touja") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"双龍戯亀", yaku::yakuCalculator::Yaku::yval_4han,
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
	if (RuleData::getRule("chii_sankan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃三嵌", yaku::yakuCalculator::Yaku::yval_1han,
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 3);
			}
		));
	/* 吃四嵌 */
	if (RuleData::getRule("chii_suukan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃四嵌", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (RuleData::getRule("chii_sanpen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃三辺", yaku::yakuCalculator::Yaku::yval_1han,
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 3);
			}
		));
	/* 吃四辺 */
	if (RuleData::getRule("chii_suupen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃四辺", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (RuleData::getRule("chii_shaoshun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃小順", yaku::yakuCalculator::Yaku::yval_1han,
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 3);
			}
		));
	/* 吃小四順 */
	if (RuleData::getRule("chii_shaosuushun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃小四順", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (RuleData::getRule("chii_taashun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃大順", yaku::yakuCalculator::Yaku::yval_1han,
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 3);
			}
		));
	/* 吃大四順 */
	if (RuleData::getRule("chii_taasuushun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"吃大四順", yaku::yakuCalculator::Yaku::yval_2han,
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 4);
			}
		));
}
