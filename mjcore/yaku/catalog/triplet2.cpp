﻿#include "../catalog.h"
#include "../../../common/strcode.h"
#include "../../../common/datetime.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_2() {
#ifndef GUOBIAO
	/* 奇数対々和 */
	if (RuleData::chkRuleApplied("odd_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("奇数対々和"), get_yaku_han("odd_toitoi"),
			_T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < static_cast<int>(TileSuit::honors); i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SizeOfMeldBuffer);
			}
		));
	/* 偶数対々和 */
	if (RuleData::chkRuleApplied("even_toitoi"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("全双刻"), 24_fenF,
			PengPengHu, _T("断幺"),
#else /* GUOBIAO */
			_T("偶数対々和"), get_yaku_han("even_toitoi"),
			_T("対々和"), _T("断幺九"),
#endif /* GUOBIAO */
			[](const MentsuAnalysis* const analysis) -> bool {
				int count = 0;
				for (int i = 2; i < static_cast<int>(TileSuit::honors); i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SizeOfMeldBuffer);
			}
		));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 中原逐鹿 */
	if (RuleData::chkRuleApplied("chuugen_chikuroku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("中原逐鹿"), get_yaku_han("chuugen_chikuroku"),
			_T("対々和"), _T("役牌・發"), _T("役牌・中"), _T("絶一門"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::circleSix] >= 1) );
			}
		));
	/* 金亀孵蛋 */
	if (RuleData::chkRuleApplied("turtle_egg"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金亀孵蛋"), get_yaku_han("turtle_egg"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::bambooSeven] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) );
			}
		));
	/* 旭日東昇 */
	if (RuleData::chkRuleApplied("kyokujitsu_toushou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("旭日東昇"), get_yaku_han("kyokujitsu_toushou"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1) );
			}
		));
	/* 日落西山 */
	if (RuleData::chkRuleApplied("riirao_shiishan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("日落西山"), get_yaku_han("riirao_shiishan"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1) );
			}
		));
	/* 揺望北斗 */
	if (RuleData::chkRuleApplied("youbou_hokuto"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("揺望北斗"), get_yaku_han("youbou_hokuto"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::circleTwo] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] >= 1) );
			}
		));
	/* 北斗七星 */
	if (RuleData::chkRuleApplied("big_dipper"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北斗七星"), get_yaku_han("big_dipper"),
			_T("ビックボーナス"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterSeven] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::bambooSeven] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] >= 1) &&
					(analysis->DuiziCount[TileCode::whiteDragon] >= 1) );
			}
		));
	/* 西廂待月 */
	if (RuleData::chkRuleApplied("seisou_taigetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("西廂待月"), get_yaku_han("seisou_taigetsu"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::circleOne] >= 1) &&
					(analysis->TsumoHai().tile == TileCode::circleOne) );
			}
		));
	/* 南無八幡 */
	if (RuleData::chkRuleApplied("namu_hachiman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南無八幡"), get_yaku_han("namu_hachiman"),
			_T("役牌・白"), _T("南白つきもの"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::characterEight] >= 1) );
			}
		));
	/* 西湖十景 */
	if (RuleData::chkRuleApplied("seiko_jikkei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("西湖十景"), get_yaku_han("seiko_jikkei"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1 && analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleTwo] >= 1 && analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleThree] >= 1 && analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::circleFour] >= 1 && analysis->KeziCount[TileCode::circleSix] >= 1);
			}
		));
	/* 火山爆発 */
	if (RuleData::chkRuleApplied("volcanic_erruption"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("火山爆発"), get_yaku_han("volcanic_erruption"),
			_T("対々和"), _T("役牌・發"), _T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::bambooThree] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::characterNine] >= 1 ||
					analysis->KeziCount[TileCode::circleNine] >= 1 || analysis->KeziCount[TileCode::bambooNine] >= 1);
			}
		));
	/* 丹鳳朝陽 */
	if (RuleData::chkRuleApplied("tanfon_chouyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("丹鳳朝陽"), get_yaku_han("tanfon_chouyou"),
			_T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) );
			}
		));

	// ---------------------------------------------------------------------

	const auto haouben =
		[](const MentsuAnalysis* const analysis, TileCode tc, int step) -> bool {
			if ((analysis->KangziCount[tc] >= 1) &&
				(analysis->KeziCount[offsetTileNumber(tc, step)] >= 1) &&
				(analysis->KangziCount[offsetTileNumber(tc, step)] == 0) &&
				(analysis->MianziDat[0].tile == offsetTileNumber(tc, step * 2)) )
				return true;
			if ((analysis->KangziCount[offsetTileNumber(tc, step * 2)] >= 1) &&
				(analysis->KeziCount[offsetTileNumber(tc, step)] >= 1) &&
				(analysis->KangziCount[offsetTileNumber(tc, step)] == 0) &&
				(analysis->MianziDat[0].tile == tc) )
				return true;
			return false;
		};

	/* 覇王鞭 */
	if (RuleData::chkRule("haouben", "6han_bamboo_only"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("覇王鞭"), 6_hanF,
			[haouben](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (int k = 1; k <= 3; k++)
					yakuFlag = yakuFlag || haouben(analysis, composeNumberTile(TileSuit::bamboos, k), 3);
				return yakuFlag;
			}
		));
	else if (RuleData::chkRuleApplied("haouben"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("覇王鞭"), get_yaku_han("haouben"),
			[haouben](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i : NumberTileSuits)
					for (int k = 1; k <= 3; k++)
						yakuFlag = yakuFlag || haouben(analysis, composeNumberTile(i, k), 3);
				return yakuFlag;
			}
		));
	/* 劉関張 */
	if (RuleData::chkRuleApplied("ryuukanchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("劉関張"), get_yaku_han("ryuukanchan"),
			[haouben](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i : NumberTileSuits)
					for (int k = 1; k <= 7; k++)
						yakuFlag = yakuFlag || haouben(analysis, composeNumberTile(i, k), 1);
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 鵲橋会 */
	if (RuleData::chkRuleApplied("queqiaohui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鵲橋会"), get_yaku_han("queqiaohui"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if (analysis->KeziCount[TileCode::bambooOne] >= 1) {
					if ((analysis->KeziCount[TileCode::characterSeven] >= 1) &&
						(analysis->KeziCount[TileCode::circleSeven] >= 1)) return true;
					else if ((analysis->KeziCount[TileCode::characterSeven] >= 1) &&
						(analysis->KeziCount[TileCode::bambooSeven] >= 1)) return true;
					else if ((analysis->KeziCount[TileCode::circleSeven] >= 1) &&
						(analysis->KeziCount[TileCode::bambooSeven] >= 1)) return true;
					else return false;
				}
				else return false;
			}
		));
	/* 一竿風月 */
	if (RuleData::chkRuleApplied("yigan_fengyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一竿風月"), get_yaku_han("yigan_fengyue"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::bambooTwo] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					((analysis->KeziCount[TileCode::eastWind] >= 1) || (analysis->KeziCount[TileCode::southWind] >= 1) ||
					(analysis->KeziCount[TileCode::westWind] >= 1) || (analysis->KeziCount[TileCode::northWind] >= 1) ));
			}
		));
	/* 南の朱雀 */
	if (RuleData::chkRuleApplied("vermilion_bird"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南の朱雀"), get_yaku_han("vermilion_bird"),
			_T("対々和"), _T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[TileCode::southWind] >= 1) &&
					(analysis->KangziCount[TileCode::bambooOne] >= 1) &&
					((analysis->KeziCount[TileCode::bambooFive] >= 1 && analysis->KeziCount[TileCode::bambooSeven] >= 1) ||
					(analysis->KeziCount[TileCode::bambooFive] >= 1 && analysis->KeziCount[TileCode::bambooNine] >= 1) ||
					(analysis->KeziCount[TileCode::bambooSeven] >= 1 && analysis->KeziCount[TileCode::bambooNine] >= 1)) &&
					(analysis->MianziDat[0].tile == TileCode::redDragon));
			}
		));
	/* 西の白虎 */
	if (RuleData::chkRuleApplied("white_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("西の白虎"), get_yaku_han("white_tiger"),
			_T("対々和"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[TileCode::westWind] >= 1) &&
					(analysis->KangziCount[TileCode::characterThree] >= 1) &&
					(analysis->KangziCount[TileCode::circleThree] >= 1) &&
					(analysis->KangziCount[TileCode::bambooThree] >= 1) &&
					(analysis->MianziDat[0].tile == TileCode::whiteDragon));
			}
		));
	/* 三杯美酒敬梅花 */
	if (RuleData::chkRuleApplied("sanbei_meijiu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三杯美酒敬梅花"), get_yaku_han("sanbei_meijiu"),
			_T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleFive] >= 1) &&
					((analysis->KeziCount[TileCode::characterNine] >= 1) ||
					(analysis->KeziCount[TileCode::circleNine] >= 1) ||
					(analysis->KeziCount[TileCode::bambooNine] >= 1) ));
			}
		));
	/* 東風圧倒西風 */
	if (RuleData::chkRuleApplied("dongfeng_yadao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東風圧倒西風"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MentsuAnalysis* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->KangziCount[TileCode::eastWind] >= 1) ?
					3_han :
					2_han);
			}),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (((analysis->KangziCount[TileCode::eastWind] == 0) && // 東刻子・西対子
					(analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->MianziDat[0].tile == TileCode::westWind)) ||
					((analysis->KangziCount[TileCode::eastWind] >= 1) && // 東槓子・西刻子
					(analysis->KangziCount[TileCode::westWind] == 0) &&
					(analysis->KeziCount[TileCode::westWind] >= 1)));
			}
		));
	/* 太公釣金亀 */
	if (RuleData::chkRuleApplied("taikou_turtle"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("太公釣金亀"), get_yaku_han("taikou_turtle"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::bambooTwo] >= 1) &&
					(analysis->TsumoHai().tile == TileCode::bambooThree) &&
					(analysis->Machi == yaku::yakuCalculator::MachiType::tanki));
			}
		));
	/* 青函トンネル */
	if (RuleData::chkRuleApplied("seikan_tunnel"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("青函トンネル"), get_yaku_han("seikan_tunnel"),
			_T("役牌・發"), _T("東發つきもの"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->AnKeziCount[TileCode::eastWind] == 0) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1));
			}
		));
	/* 宝紅開花 */
	if (RuleData::chkRuleApplied("baohongkaihua"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("宝紅開花"), get_yaku_han("baohongkaihua"),
			_T("対々和"), _T("役牌・白"), _T("役牌・中"), _T("混老頭"), _T("三色小同刻"), _T("白鳥"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::characterOne] >= 1) &&
					(analysis->DuiziCount[TileCode::circleOne] >= 1) &&
					(analysis->DuiziCount[TileCode::bambooOne] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 東發つきもの */
	if (RuleData::chkRuleApplied("east_green"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東發つきもの"), get_yaku_han("east_green"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1));
			}
		));
	/* 南白つきもの */
	if (RuleData::chkRuleApplied("south_white"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南白つきもの"), get_yaku_han("south_white"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1));
			}
		));
	/* 中西君 */
	if (RuleData::chkRuleApplied("west_red"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("中西君"), get_yaku_han("west_red"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 白鳥 */
	if (RuleData::chkRuleApplied("hakucho"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白鳥"), get_yaku_han("hakucho"),
			_T("対々和"), _T("役牌・白"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->TotalKezi == SizeOfMeldBuffer - 1));
			}
		));
	/* 火山爆発(五索と七索) */
	if (RuleData::chkRuleApplied("volcanic_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("火山爆発 "), get_yaku_han("volcanic_toitoi"),
			_T("対々和"), _T("役牌・中"), _T("役牌・發"), _T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::bambooThree] >= 1) &&
					(analysis->DuiziCount[TileCode::bambooFive] >= 1) &&
					(analysis->DuiziCount[TileCode::bambooSeven] >= 1) &&
					(analysis->DuiziCount[TileCode::greenDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::redDragon] >= 1));
			}
		));
	/* 白一色の定義処理はmisc.cppに移動しました */

	/* 雪の銀閣 */
	if (RuleData::chkRuleApplied("snow_ginkaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("雪の銀閣"), get_yaku_han("snow_ginkaku"),
			_T("対々和"), _T("役牌・白"), _T("混一色"), _T("断紅和"), _T("混断紅"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::circleTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::circleFour] >= 1) &&
					(analysis->DuiziCount[TileCode::circleEight] >= 1) &&
					(analysis->DuiziCount[TileCode::eastWind] >= 1) &&
					(analysis->DuiziCount[TileCode::whiteDragon] >= 1));
			}
		));
	/* 北白川 */
	if (RuleData::chkRuleApplied("kitashirakawa"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北白川"), get_yaku_han("kitashirakawa"),
			_T("役牌・白"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::northWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::bambooSix] >= 1));
			}
		));
	/* 素数役満の定義処理はmisc.cppに移動しました */

	/* アル中 */
	if (RuleData::chkRuleApplied("alcoholism"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("アル中"), get_yaku_han("alcoholism"),
			_T("対々和"), _T("役牌・中"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::characterTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::circleTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::bambooTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::redDragon] >= 1));
			}
		));
	/* 救急車 */
	if (RuleData::chkRuleApplied("kyukyusha"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("救急車"), get_yaku_han("kyukyusha"),
			_T("対々和"), _T("役牌・白"), _T("三色同刻"), _T("混老頭"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::redDragon] >= 1));
			}
		));
	/* 盗難車 */
	if (RuleData::chkRuleApplied("tounansha"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("盗難車"), get_yaku_han("tounansha"),
			_T("三風刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1));
			}
		));
	/* 青春に悔いなし */
	if (RuleData::chkRuleApplied("seishun_kuinashi")) {
		const auto seishunnikuinashi =
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1));
			};
		if (RuleData::chkRule("seishun_kuinashi", "1han_menzen")) {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("青春に悔いなし"), 1_hanM,
				seishunnikuinashi
			));
		} else if (RuleData::chkRule("seishun_kuinashi", "2han_kuisagari")){
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("青春に悔いなし"), 2_hanM,
				_T("青春に悔いあり"),
				seishunnikuinashi
			));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("青春に悔いあり"), 1_hanF,
				seishunnikuinashi
			));
		}
	}
	/* スーパーマン */
	if (RuleData::chkRuleApplied("superman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("スーパーマン"), get_yaku_han("superman"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if ((analysis->KeziCount[TileCode::characterFour] >= 1) &&
					(analysis->KeziCount[TileCode::characterEight] >= 1)) return true;
				else if ((analysis->MianziDat[0].tile == TileCode::characterEight) &&
					(analysis->KeziCount[TileCode::characterFour] >= 1) &&
					(analysis->TsumoHai().tile == TileCode::characterFour)) return true;
				else if ((analysis->MianziDat[0].tile == TileCode::characterFour) &&
					(analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->TsumoHai().tile == TileCode::characterEight)) return true;
				else return false;
			}
		));
	/* 東花園 */
	if (RuleData::chkRuleApplied("higashi_hanazono_triplets"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東花園"), get_yaku_han("higashi_hanazono_triplets"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::eastWind] >= 1) &&
					((analysis->KeziCount[TileCode::characterEight] >= 1) || (analysis->KeziCount[TileCode::circleEight] >= 1) ||
					(analysis->KeziCount[TileCode::bambooEight] >= 1)) &&
					((analysis->KeziCount[TileCode::characterSeven] >= 1) || (analysis->KeziCount[TileCode::circleSeven] >= 1) ||
					(analysis->KeziCount[TileCode::bambooSeven] >= 1)));
			}
		));
	/* 琳両数 */
	if (RuleData::chkRuleApplied("rinryanshii"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("琳両数"), get_yaku_han("rinryanshii"),
			/* 対々和が必ず複合 */
			[](const MentsuAnalysis* const analysis) -> bool {
				constexpr char pattern[] = "121314151617181923242526272829343536373839454647484956575859676869787989";
				std::array<TileCode, static_cast<int>(TileSuit::honors) / TileSuitStep * 2> tCode;
				bool yakuFlag = false;
				for (int i = 0; i < 36; i++) {
					int num1 = static_cast<int>(pattern[i * 2] - _T('0'));
					int num2 = static_cast<int>(pattern[i * 2 + 1] - _T('0'));
					for (int k = 0; k < static_cast<int>(TileSuit::honors) / TileSuitStep * 2; k += 2) {
						tCode[k] = static_cast<TileCode>(k / 2 * TileSuitStep + num1);
						tCode[k + 1] = static_cast<TileCode>(k / 2 * TileSuitStep + num2);
					}
					if (yaku::countingFacility::countSpecMentz(
						analysis->MianziDat, tCode, std::array<TileCode, 0>(), false) == SizeOfMeldBuffer)
						yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ３分間待ってやる */
	const auto anysuit3 =
		[](const MentsuAnalysis* const analysis, int val1, int val2, int val3, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (auto suit1 : NumberTileSuits)
				for (auto suit2 : NumberTileSuits)
					for (auto suit3 : NumberTileSuits)
						if (((*count)[composeNumberTile(suit1, val1)] >= 1) && ((*count)[composeNumberTile(suit2, val2)] >= 1) &&
							((*count)[composeNumberTile(suit3, val3)] >= 1)) yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("colonel_muska"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ムスカ大佐"), get_yaku_han("colonel_muska"),
			[anysuit3](const MentsuAnalysis* const analysis) -> bool {
				return anysuit3(analysis, 6, 4, 3, true) && (analysis->Machi == yaku::yakuCalculator::MachiType::kanchan);
			}
		));
	/* 都の西北 */
	if (RuleData::chkRuleApplied("miyakonoseihoku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("都の西北"), get_yaku_han("miyakonoseihoku"),
			_T("対々和"),
			[anysuit3](const MentsuAnalysis* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 5, false) &&
					(analysis->DuiziCount[TileCode::westWind] >= 1) &&
					(analysis->DuiziCount[TileCode::northWind] >= 1);
			}
		));
	/* 東京二十三区 */
	if (RuleData::chkRuleApplied("tokyo")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東京二十三区"),  yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MentsuAnalysis* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(isshoku(analysis, false)) ?
					2_yakuman :
					1_yakuman);
			}),
			_T("対々和"),
			[anysuit3](const MentsuAnalysis* const analysis) -> bool {
				return anysuit3(analysis, 2, 3, 9, true) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1);
			}
		));
	}
	/* 初音ミク */
	const auto anysuit2 =
		[](const MentsuAnalysis* const analysis, int val1, int val2, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (auto suit1 : NumberTileSuits)
				for (auto suit2 : NumberTileSuits)
					if (((*count)[composeNumberTile(suit1, val1)] >= 1) && ((*count)[composeNumberTile(suit2, val2)] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("hatsune_miku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("初音ミク"), get_yaku_han("hatsune_miku"),
			[anysuit2](const MentsuAnalysis* const analysis) -> bool {
				return anysuit2(analysis, 3, 9, true) && (analysis->KeziCount[TileCode::greenDragon] >= 1);
			}
		));
	/* 白花 */
	if (RuleData::chkRuleApplied("baihua"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白花"), get_yaku_han("baihua"),
			/* 別途白で1飜 */
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleFive] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1);
			}
		));
	/* 撃鼓愕曹 */
	if (RuleData::chkRuleApplied("jigu_ecao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("撃鼓愕曹"), get_yaku_han("jigu_ecao"),
			_T("役牌・白"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleTwo] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ビックボーナス */
	if (RuleData::chkRuleApplied("777")) {
		const auto yaku777 =
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterSeven] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::bambooSeven] >= 1);
			};
		if (RuleData::chkRule("777", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("ビックボーナス"), 1_yakumanF,
				_T("三色同刻"),
				yaku777
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("ビックボーナス"), get_yaku_han("777"),
				yaku777
			));
	}
	/* クリスマス */
	if (RuleData::chkRuleApplied("christmas")) {
		const auto chris =
			[](const MentsuAnalysis* const analysis) -> bool {
				for (int i = 0; i < 6; i++)
					if ((analysis->KeziCount[(parsedat_trichrome3[i][0] - _T('0')) + 1] >= 1) &&
						(analysis->KeziCount[(parsedat_trichrome3[i][0] - _T('0')) + 2] >= 1) &&
						(analysis->KeziCount[(parsedat_trichrome3[i][1] - _T('0')) + 2] >= 1) &&
						(analysis->KeziCount[(parsedat_trichrome3[i][1] - _T('0')) + 5] >= 1))
						return true;
				return false;
			};
		const auto chrisday = 
			[]() -> bool {
				const auto nowTime(DateTime::localTime());
				return (nowTime.month == DateTime::December) && (nowTime.day == 25);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("クリスマス"), yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MentsuAnalysis* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					2_yakuman :
					1_yakuman);
			}),
			_T("対々和"),
			chris
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純正クリスマス"), yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MentsuAnalysis* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					3_yakuman :
					2_yakuman);
			}),
			_T("対々和"), _T("クリスマス"),
			[chris](const MentsuAnalysis* const analysis) -> bool {
				return chris(analysis) &&
					(analysis->MianziDat[0].tile == TileCode::bambooSeven);
			}
		));
	}
	/* 一富士二鷹三茄子 */
	if (RuleData::chkRuleApplied("newyear_dream")) {
		const auto isnewyeardays = 
			[]() -> bool {
				const auto nowTime(DateTime::localTime());
				return (nowTime.month == DateTime::January) && (nowTime.day <= 2);
			};
		const auto newyrdrm =
			[](const MentsuAnalysis* const analysis) -> bool {
				if (analysis->PlayerStat()->MeldPointer >= 3)
					return ((analysis->PlayerStat()->Meld[1].mstat > MeldStat::tripletConcealed) &&
						(analysis->PlayerStat()->Meld[1].tile == TileCode::bambooThree) &&
						(analysis->PlayerStat()->Meld[2].mstat > MeldStat::tripletConcealed) &&
						(analysis->PlayerStat()->Meld[2].tile == TileCode::bambooOne) &&
						(analysis->PlayerStat()->Meld[3].mstat > MeldStat::tripletConcealed) &&
						(analysis->PlayerStat()->Meld[3].tile == TileCode::circleEight));
				else return false;
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一富士二鷹三茄子"), 1_yakumanF,
			newyrdrm
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純正一富士二鷹三茄子"), 2_yakumanF,
			_T("一富士二鷹三茄子"),
			[newyrdrm, isnewyeardays](const MentsuAnalysis* const analysis) -> bool {
				return newyrdrm(analysis) && isnewyeardays();
			}
		));
	}
	/* 武蔵丸 */
	if (RuleData::chkRuleApplied("musashimaru")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("武蔵丸"), 1_yakumanF,
			[anysuit3](const MentsuAnalysis* const analysis) -> bool {
				return anysuit3(analysis, 6, 3, 4, true) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1);
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純正武蔵丸"), 2_yakumanF,
			_T("武蔵丸"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleSix] >= 1) &&
					(analysis->KeziCount[TileCode::circleThree] >= 1) &&
					(analysis->KeziCount[TileCode::circleFour] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(getTileSuit(analysis->MianziDat[0].tile) == TileSuit::circles);
			}
		));
	}

	// ---------------------------------------------------------------------

	/* 阪神 */
	if (RuleData::chkRuleApplied("hanshin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("阪神"), get_yaku_han("hanshin"),
			[anysuit2](const MentsuAnalysis* const analysis) -> bool {
				return anysuit2(analysis, 8, 4, true);
			}
		));
	/* 神戸 */
	if (RuleData::chkRuleApplied("kobe"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("神戸"), get_yaku_han("kobe"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterFive] >= 1) ||
					(analysis->KeziCount[TileCode::circleFive] >= 1) ||
					(analysis->KeziCount[TileCode::bambooFive] >= 1)) &&
					(analysis->KeziCount[TileCode::northWind] >= 1);
			}
		));
	/* サッポロ一番 */
	if (RuleData::chkRuleApplied("sapporo1"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("サッポロ一番"), get_yaku_han("sapporo1"),
			[anysuit3](const MentsuAnalysis* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 6, true) &&
					(analysis->GameStat->GameRound == 0) && // 東一局であって
					(analysis->GameStat->LoopRound == 0) && // 返り東場でもなくて
					(analysis->GameStat->Honba == 0); // 平場の時
			}
		));
	/* ポカリスエット */
	if (RuleData::chkRuleApplied("pcrswt"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ポカリスエット"), get_yaku_han("pcrswt"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::circleTwo] >= 1) ||
					(analysis->KeziCount[TileCode::circleFour] >= 1) ||
					(analysis->KeziCount[TileCode::circleEight] >= 1)) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1);
			}
		));
	/* アクエリアス */
	if (RuleData::chkRuleApplied("aqrs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("アクエリアス"), get_yaku_han("aqrs"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::circleOne] >= 1) ||
					(analysis->KeziCount[TileCode::circleThree] >= 1) ||
					(analysis->KeziCount[TileCode::circleSeven] >= 1) ||
					(analysis->KeziCount[TileCode::circleNine] >= 1)) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1);
			}
		));
	/* ＳＭＡＰ */
	if (RuleData::chkRuleApplied("smap"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ＳＭＡＰ"), get_yaku_han("smap"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterFour] -
					analysis->AnKeziCount[TileCode::characterFour] >= 1));
			}
		));
	/* モンスターハンター */
	if (RuleData::chkRuleApplied("monster_hunter"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("モンスターハンター"), get_yaku_han("monster_hunter"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::eastWind] >= 1) ||
					(analysis->KeziCount[TileCode::southWind] >= 1) ||
					(analysis->KeziCount[TileCode::westWind] >= 1) ||
					(analysis->KeziCount[TileCode::northWind] >= 1)) &&
					(analysis->GameStat->TurnRound <= 6);
			}
		));
	/* 旭川ラーメン */
	if (RuleData::chkRuleApplied("asahikawa_raamen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("旭川ラーメン"), get_yaku_han("asahikawa_raamen"),
			_T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::bambooThree] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 東方妖々夢 */
	if (RuleData::chkRuleApplied("perfect_cherry_blossom"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東方妖々夢"), get_yaku_han("perfect_cherry_blossom"),
			_T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1);
			}
		));
	/* 東方紅楼夢 */
	if (RuleData::chkRuleApplied("touhou_kouroumu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東方紅楼夢"), get_yaku_han("touhou_kouroumu"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::characterSix] >= 1) &&
					(analysis->TsumoHai().tile == TileCode::eastWind);
			}
		));
	/* Windows95 */
	if (RuleData::chkRuleApplied("windows95"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Windows95"), get_yaku_han("windows95"),
			_T("三色同刻"), _T("対々和"), _T("琳両数"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->DuiziCount[TileCode::characterFive] +
					analysis->DuiziCount[TileCode::circleFive] +
					analysis->DuiziCount[TileCode::bambooFive] >= 2);
			}
		));
	/* Windows98 */
	if (RuleData::chkRuleApplied("windows98"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Windows98"), get_yaku_han("windows98"),
			_T("三色同刻"), _T("対々和"), _T("琳両数"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->DuiziCount[TileCode::characterEight] +
					analysis->DuiziCount[TileCode::circleEight] +
					analysis->DuiziCount[TileCode::bambooEight] >= 2);
			}
		));
	/* Windows3.1 */
	if (RuleData::chkRuleApplied("windows31"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Windows3.1"), get_yaku_han("windows31"),
			_T("対々和"), _T("琳両数"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->DuiziCount[TileCode::characterThree] +
					analysis->DuiziCount[TileCode::circleThree] +
					analysis->DuiziCount[TileCode::bambooThree] +
					analysis->DuiziCount[TileCode::characterOne] +
					analysis->DuiziCount[TileCode::circleOne] +
					analysis->DuiziCount[TileCode::bambooOne] == SizeOfMeldBuffer);
			}
		));
	/* Windows7 */
	if (RuleData::chkRuleApplied("windows7"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Windows7"), get_yaku_han("windows7"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->TsumoHai().tile == TileCode::characterSeven) &&
					((analysis->KeziCount[TileCode::circleSeven] >= 1) ||
					(analysis->KeziCount[TileCode::bambooSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai().tile == TileCode::circleSeven) &&
					((analysis->KeziCount[TileCode::bambooSeven] >= 1) ||
					(analysis->KeziCount[TileCode::characterSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai().tile == TileCode::bambooSeven) &&
					((analysis->KeziCount[TileCode::characterSeven] >= 1) ||
					(analysis->KeziCount[TileCode::circleSeven] >= 1))) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 東方風神録 */
	if (RuleData::chkRuleApplied("mountain_of_faith"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東方風神録"), get_yaku_han("mountain_of_faith"),
			[anysuit2](const MentsuAnalysis* const analysis) -> bool {
				return anysuit2(analysis, 6, 7, true) &&
					((analysis->KeziCount[TileCode::eastWind] >= 1) ||
					(analysis->KeziCount[TileCode::southWind] >= 1) ||
					(analysis->KeziCount[TileCode::westWind] >= 1) ||
					(analysis->KeziCount[TileCode::northWind] >= 1)) &&
					(analysis->PlayerStat()->Tsumohai().tile == TileCode::eastWind);
			}
		));
	/* 博麗霊夢 */
	if (RuleData::chkRuleApplied("hakurei_reimu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("博麗霊夢"), get_yaku_han("hakurei_reimu"),
			_T("対々和"), _T("絶一門"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::characterEight] >= 1) &&
					(analysis->DuiziCount[TileCode::characterNine] >= 1) &&
					(analysis->DuiziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::eastWind] >= 1) &&
					(analysis->DuiziCount[TileCode::circleOne] >= 1));
			}
		));
	/* 霧雨魔理沙 */
	if (RuleData::chkRuleApplied("kirisame_marisa"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("霧雨魔理沙"), get_yaku_han("kirisame_marisa"),
			_T("対々和"), _T("絶一門"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::northWind] >= 1) &&
					(analysis->DuiziCount[TileCode::southWind] >= 1) &&
					(analysis->DuiziCount[TileCode::circleEight] >= 1) &&
					(analysis->DuiziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::characterThree] >= 1));
			}
		));
	/* 十六夜咲夜 */
	if (RuleData::chkRuleApplied("izayoi_sakuya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("十六夜咲夜"), get_yaku_han("izayoi_sakuya"),
			_T("対々和"), _T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::bambooThree] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					(analysis->DuiziCount[TileCode::redDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::MachiType::shanpon));
			}
		));
	/* 西南戦争 */
	if (RuleData::chkRuleApplied("seinansensou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("西南戦争"), get_yaku_han("seinansensou"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::bambooThree] >= 1));
			}
		));
	/* 純正西南戦争 */
	if (RuleData::chkRuleApplied("junsei_seinansensou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("純正西南戦争"), get_yaku_han("junsei_seinansensou"),
			_T("三色小同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->DuiziCount[TileCode::characterNine] >= 1) &&
					(analysis->DuiziCount[TileCode::circleNine] >= 1) &&
					(analysis->DuiziCount[TileCode::bambooNine] >= 1));
			}
		));
	/* 九州新幹線 */
	if (RuleData::chkRuleApplied("kyushu_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("九州新幹線"), get_yaku_han("kyushu_shinkansen"),
			_T("三色同刻"), _T("五門斎"), _T("混老頭"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->DuiziCount[TileCode::eastWind] + analysis->DuiziCount[TileCode::southWind] +
					analysis->DuiziCount[TileCode::westWind] + analysis->DuiziCount[TileCode::northWind] >= 1) &&
					(analysis->DuiziCount[TileCode::whiteDragon] + analysis->DuiziCount[TileCode::greenDragon] +
					analysis->DuiziCount[TileCode::redDragon] >= 1));
			}
		));
	/* 阿拉 */
	if (RuleData::chkRuleApplied("ara"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("阿拉"), get_yaku_han("ara"),
			_T("小三元"), _T("対々和"), _T("混老頭"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::whiteDragon] >= 1));
			}
		));
	/* 快馬加鞭 */
	if (RuleData::chkRuleApplied("kaibakaben"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("快馬加鞭"), get_yaku_han("kaibakaben"),
			_T("大三元"), _T("字一色"), _T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::northWind] >= 1));
			}
		));
	/* 酔江月 */
	if (RuleData::chkRuleApplied("suikougetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("酔江月"), get_yaku_han("suikougetsu"),
			_T("大三元"), _T("字一色"), _T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::northWind] >= 1));
			}
		));
	/* 中目黒 */
	if (RuleData::chkRuleApplied("nakameguro"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("中目黒"), get_yaku_han("nakameguro"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->DuiziCount[TileCode::redDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::circleTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::circleFour] >= 1) &&
					(analysis->DuiziCount[TileCode::circleEight] >= 1) &&
					(analysis->DuiziCount[TileCode::eastWind] + analysis->DuiziCount[TileCode::southWind] +
					analysis->DuiziCount[TileCode::westWind] + analysis->DuiziCount[TileCode::northWind]>= 1));
			}
		));
#endif /* GUOBIAO */
}
