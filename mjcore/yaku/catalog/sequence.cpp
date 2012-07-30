#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* 一色双龍会 */
	if (getRule(RULE_ISSHOKU_SOURYUU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色双龍会", yaku::yakuCalculator::Yaku::yval_yakuman,
			"清一色", "二盃口", "鳴き二盃口",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
	if (getRule(RULE_ISSHOKU_SUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色四順", yaku::yakuCalculator::Yaku::yval_yakuman,
			"一色三順", "一盃口", "二盃口", "鳴き二盃口",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 一色三順 */
	if (getRule(RULE_ISSHOKU_SANJUN) != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC hf = yaku::yakuCalculator::Yaku::HANFUNC();
		switch (getRule(RULE_ISSHOKU_SANJUN)) {
			case 1: hf = yaku::yakuCalculator::Yaku::yval_2han_menzen; break;
			case 2: hf = yaku::yakuCalculator::Yaku::yval_2han_kuisagari; break;
			case 3: hf = yaku::yakuCalculator::Yaku::yval_3han_menzen; break;
			case 4: hf = yaku::yakuCalculator::Yaku::yval_3han_kuisagari; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一色三順", hf,
			"一盃口",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 1);
		}
	));
	/* 二盃口 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"二盃口", (getRule(RULE_RYANPEIKOU) != 0) ?
		yaku::yakuCalculator::Yaku::yval_2han_menzen : yaku::yakuCalculator::Yaku::yval_3han_menzen,
		"一盃口",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 2);
		}
	));
	/* 大鏡 */
	if (getRule(RULE_OHKAGAMI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大鏡", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return ((yakuFlagCount == 2)&&(analysis->MianziDat[0].tile == CircleOne));
			}
		));
	/* 清盃口 */
	if (getRule(RULE_CHINPEIKOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"清盃口", (getRule(RULE_CHINPEIKOU) == 1) ? yaku::yakuCalculator::Yaku::yval_6han_menzen :
			((getRule(RULE_CHINPEIKOU) == 2) ? yaku::yakuCalculator::Yaku::yval_5han_menzen :
			yaku::yakuCalculator::Yaku::yval_4han_menzen),
			"二盃口",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
	if (getRule(RULE_EXPOSED_RYANPEIKOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鳴き二盃口", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(gameStat->Player[analysis->player].MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
			}),
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis,
		bool* const yakuFlag, int* const yakuCol) -> void {
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
		[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis,
		bool* const yakuFlag, int* const yakuCol) -> void {
			int samecol;
			ikki_tsuukan(gameStat, analysis, yakuFlag, yakuCol);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->DuiziCount[EastWind] >= 1) && // 東と
					(analysis->DuiziCount[NorthWind] >= 1) ); // 北がある
			}
	));
	/* 20101204祝東京～新青森全通 */
	if (getRule(RULE_TOHOKU_SHINKANSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東北新幹線", (getRule(RULE_TOHOKU_SHINKANSEN) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"一気通貫", "混一色",
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->DuiziCount[EastWind] >= 1) && // 東と
					(analysis->DuiziCount[NorthWind] >= 1) ); // 北がある
			}
		));
	/* 一気通貫全帯幺九 */
	if (getRule(RULE_ITTSUU_CHANTA) != 0) {
		auto ittsuu_chanta_counting =
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis,
			int* const yaojiu, int* const zipai) -> void {
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
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一気通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"一気通貫",
			[ittsuu_chanta_counting](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai;
				ittsuu_chanta_counting(gameStat, analysis, &yaojiu, &zipai);
				return ((yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純一気通貫全帯幺九", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"一気通貫",
			[ittsuu_chanta_counting](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai;
				ittsuu_chanta_counting(gameStat, analysis, &yaojiu, &zipai);
				return ((yaojiu == 4) && (zipai == 0));
			}
		));
	}
	/* 五心通貫 */
	if (getRule(RULE_UUSHINTONKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"五心通貫", yaku::yakuCalculator::Yaku::yval_1han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // 嵌張待ちで
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // 和了牌が5
					);
			}
		));
	/* 竜戯珠 */
	if (getRule(RULE_LONGXIZHU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"竜戯珠", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[WestWind] >= 1) && // 西の刻子があり
					(analysis->MianziDat[0].tile == CircleOne) ); // アタマが一筒
			}
		));
	/* 金龍戯北斗 */
	if (getRule(RULE_JINLONG_DOU_BEIDOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金龍戯北斗", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[CircleSeven] >= 1) && // 七筒の刻子があり
					(analysis->MianziDat[0].tile == NorthWind) ); // アタマが北
			}
		));
	/* 西湖十景系列の役 */
	{
		const int Rules = 10;
		const RuleCode tmpRuleCodeList[Rules] = {
			RULE_SHUANGFENG_ZHAYUN, RULE_NANBING_WANZHONG, RULE_LEIFENG_XIZHAO, RULE_LIULANG_WENYING, RULE_DUANQIAO_CANXUE,
			RULE_SUDI_CHUNXIAO, RULE_HUAGANG_GUANYU, RULE_SANTAN_YINYUE, RULE_QUYUAN_FENGHE, RULE_PINGHU_QIUYUE,
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
			if (getRule(tmpRuleCodeList[i]) == 0) continue;
			tileCode tmpTileCode1 = tmpTileCodeList[i][0];
			tileCode tmpTileCode2 = tmpTileCodeList[i][1];
			int tmpStraightColor = tmpStraightColorList[i];
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				tmpYakuNameList[i], yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
				[ikki_tsuukan, tmpTileCode1, tmpTileCode2, tmpStraightColor]
				(const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag; int yakuCol;
					ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
					return (yakuFlag && // 一気通貫が成立していて
						(analysis->DuiziCount[tmpTileCode1] >= 1) && // 指定の牌が刻子と雀頭として存在し
						(analysis->DuiziCount[tmpTileCode2] >= 1) &&
						(yakuCol == tmpStraightColor) ); // イッツー部分が指定された色
				}
			));
		}
	}
	/* 東の青龍 */
	if (getRule(RULE_AZURE_DRAGON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東の青龍", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"一気通貫", "混一色",
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[EastWind] >= 1) && // 東の刻子があり
					(analysis->MianziDat[0].tile == WhiteDragon) && // アタマが白で
					(yakuCol == TILE_SUIT_CIRCLES) ); // イッツー部分が筒子
			}
		));
	/* 北の玄武 */
	if (getRule(RULE_BLACK_TORTOISE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北の玄武", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"一気通貫",
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[NorthWind] >= 1) && // 北の刻子があり
					(analysis->MianziDat[0].tile == BambooSeven) && // アタマが七索で
					(yakuCol == TILE_SUIT_CIRCLES) ); // イッツー部分が筒子
			}
		));
	/* 秋田新幹線 */
	if (getRule(RULE_AKITA_SHINKANSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"秋田新幹線", yaku::yakuCalculator::Yaku::yval_yakuman,
			"一気通貫", "混一色",
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->KeziCount[NorthWind] >= 1) && // 北の刻子があり
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (yakuCol / TILE_SUIT_STEP)) && // 一色になっていて
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // 和了牌が5
					);
			}
		));
	/* 上越新幹線とき */
	if (getRule(RULE_JOETSU_TOKI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"上越新幹線とき", yaku::yakuCalculator::Yaku::yval_2han,
			[ittsuu_monotonic](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(analysis->DuiziCount[NorthWind] >= 1) // 北の刻子または雀頭がある
					);
			}
		));
	/* 上越新幹線あさひ */
	if (getRule(RULE_JOETSU_ASAHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"上越新幹線あさひ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"上越新幹線とき", "一気通貫", "混一色",
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->DuiziCount[NorthWind] >= 1) && // 北と
					(analysis->DuiziCount[WhiteDragon] >= 1) // 白が刻子と雀頭として存在
					);
			}
		));
	/* 青函連絡船 */
	if (getRule(RULE_SEIKAN_SHIP) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"青函連絡船", yaku::yakuCalculator::Yaku::yval_yakuman,
			"一気通貫", "混一色",
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->MianziDat[0].tile == NorthWind) && // 北が雀頭で
					(analysis->AnKangziCount[GreenDragon] >= 1) // 發の暗槓がある
					);
			}
		));
	/* ゴールドラッシュ */
	if (getRule(RULE_GOLDRUSH) != 0) {
		auto goldrush1 =
			[ittsuu_monotonic](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫かつ一色で
					(yakuCol == TILE_SUIT_CIRCLES) && // 筒子のイッツーで
					(analysis->DuiziCount[GreenDragon] >= 1) // 發の刻子か雀頭がある
					);
			};
		auto goldrush2 =
			[goldrush1](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (goldrush1(gameStat, analysis) && // goldrush1の条件を満たし
					(analysis->DuiziCount[WestWind] >= 1) // 西の刻子か雀頭がある
					);
			};
		switch (getRule(RULE_GOLDRUSH)) {
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
	if (getRule(RULE_ROUTE_246) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ルート２４６", yaku::yakuCalculator::Yaku::yval_1han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // 嵌張待ちで
					((gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == yakuCol + 2) || // 2か
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == yakuCol + 4) || // 4か
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == yakuCol + 6)) // 6で和了
					);
			}
		));
	/* 九龍城落地 */
	if (getRule(RULE_GAULUNGSING_LOKDEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"九龍城落地", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->ShunziCount[yakuCol + 1] - analysis->AnShunziCount[yakuCol + 1] >= 1) &&
					(analysis->ShunziCount[yakuCol + 4] - analysis->AnShunziCount[yakuCol + 4] >= 1) &&
					(analysis->ShunziCount[yakuCol + 7] - analysis->AnShunziCount[yakuCol + 7] >= 1) // 一気通貫の牌は全部チー
					);
			}
		));
	/* 北海道新幹線 */
	if (getRule(RULE_HOKKAIDO_SHINKANSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北海道新幹線", yaku::yakuCalculator::Yaku::yval_yakuman,
			"一気通貫", "混一色",
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // 一気通貫が成立していて
					(analysis->AnKangziCount[WhiteDragon] >= 1) && // 白の暗槓があって
					(analysis->MianziDat[0].tile == NorthWind) ); // 雀頭が北
			}
		));
}
