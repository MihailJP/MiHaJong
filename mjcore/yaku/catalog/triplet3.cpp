﻿#include "../catalog.h"

#include "../../func.h"
#include "../../../common/datetime.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_3() {
#ifndef GUOBIAO
	const auto countTilesOf =
		[](const MentsuAnalysis* const analysis, unsigned numeral) -> unsigned {
			return analysis->TileCount[composeNumberTile(TileSuit::characters, numeral)] +
				analysis->TileCount[composeNumberTile(TileSuit::circles, numeral)] +
				analysis->TileCount[composeNumberTile(TileSuit::bamboos, numeral)];
		};
	const auto countKangziOf =
		[](const MentsuAnalysis* const analysis, unsigned numeral) -> unsigned {
			return analysis->KangziCount[composeNumberTile(TileSuit::characters, numeral)] +
				analysis->KangziCount[composeNumberTile(TileSuit::circles, numeral)] +
				analysis->KangziCount[composeNumberTile(TileSuit::bamboos, numeral)];
		};
	const auto countKeziOf =
		[](const MentsuAnalysis* const analysis, unsigned numeral) -> unsigned {
			return analysis->KeziCount[composeNumberTile(TileSuit::characters, numeral)] +
				analysis->KeziCount[composeNumberTile(TileSuit::circles, numeral)] +
				analysis->KeziCount[composeNumberTile(TileSuit::bamboos, numeral)];
		};
	const auto countDuiziOf =
		[](const MentsuAnalysis* const analysis, unsigned numeral) -> unsigned {
			return analysis->DuiziCount[composeNumberTile(TileSuit::characters, numeral)] +
				analysis->DuiziCount[composeNumberTile(TileSuit::circles, numeral)] +
				analysis->DuiziCount[composeNumberTile(TileSuit::bamboos, numeral)];
		};

	// ---------------------------------------------------------------------

	/* イナバウアー */
	if (RuleData::chkRuleApplied("inabauer"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("イナバウアー"), get_yaku_han("inabauer"),
			_T("清一色"), _T("対々和"), _T("筋牌刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool flag = false;
				for (auto i : NumberTileSuits)
					if ((analysis->DuiziCount[composeNumberTile(i, 1)] >= 1) &&
						(analysis->DuiziCount[composeNumberTile(i, 7)] >= 1) &&
						(analysis->DuiziCount[composeNumberTile(i, 8)] >= 1) &&
						(analysis->DuiziCount[composeNumberTile(i, 5)] >= 1) &&
						(analysis->DuiziCount[composeNumberTile(i, 2)] >= 1))
						flag = true;
				return flag;
			}
		));
	/* 四月馬鹿 */
	if (RuleData::chkRuleApplied("april_fool")) {
		const auto isApril1st =
			[]() -> bool {
				const auto nowTime(DateTime::localTime());
				return (nowTime.month == DateTime::April) && (nowTime.day == 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四月馬鹿"), get_yaku_han("april_fool"),
			[isApril1st](const MentsuAnalysis* const analysis) -> bool {
				if (!isApril1st()) return false;
				bool flag = false;
				for (auto i : NumberTileSuits)
					if ((analysis->KeziCount[composeNumberTile(i, 4)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(i, 1)] >= 1))
						flag = true;
				return flag && (analysis->TileCount[TileCode::circleNine]);
			}
		));
	}
	/* 中東の笛 */
	if (RuleData::chkRuleApplied("chuutounofue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("中東の笛"), get_yaku_han("chuutounofue"),
			_T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1));
			}
		));
	/* 三国志 */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三国志"), get_yaku_han("sangokushi"),
			_T("対々和"),
			[countKeziOf](const MentsuAnalysis* const analysis) -> bool {
				return (bool)
					(countKeziOf(analysis, 3) * countKeziOf(analysis, 5) *
					countKeziOf(analysis, 9) * countKeziOf(analysis, 4));
			}
		));
	/* 一休さん */
	if (RuleData::chkRuleApplied("ikkyuusan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一休さん"), get_yaku_han("ikkyuusan"),
			_T("対々和"),
			[countDuiziOf](const MentsuAnalysis* const analysis) -> bool {
				return (countDuiziOf(analysis, 1) > 0) &&
					(countDuiziOf(analysis, 9) > 0) &&
					(countDuiziOf(analysis, 3) > 0) &&
					(countDuiziOf(analysis, 1) + countDuiziOf(analysis, 9) +
					countDuiziOf(analysis, 3) == SizeOfMeldBuffer);
			}
		));
	/* 大魔神 */
	if (RuleData::chkRuleApplied("daimajin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大魔神"), get_yaku_han("daimajin"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->AnKeziCount[TileCode::whiteDragon] +
					analysis->AnKeziCount[TileCode::greenDragon] +
					analysis->AnKeziCount[TileCode::redDragon]) >= 1) &&
					(analysis->TsumoHai().tile == TileCode::bambooFive) &&
					(analysis->Machi == yaku::yakuCalculator::MachiType::tanki);
			}
		));
	/* 松竹梅 */
	if (RuleData::chkRuleApplied("shouchikubai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("松竹梅"), get_yaku_han("shouchikubai"),
			_T("役牌・發"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool flag = false;
				for (int i = 2; i <= 9; ++i)
					if (analysis->KeziCount[composeNumberTile(TileSuit::bamboos, i)] >= 1)
						flag = true;
				return flag &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1);
			}
		));
	/* パンダ */
	if (RuleData::chkRuleApplied("panda"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("パンダ"), get_yaku_han("panda"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* 黒旗軍 */
	if (RuleData::chkRuleApplied("kokkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("黒旗軍"), get_yaku_han("kokkigun"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1);
			}
		));
	/* 白旗軍 */
	if (RuleData::chkRuleApplied("hakkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白旗軍"), get_yaku_han("hakkigun"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1);
			}
		));
	/* 紅旗軍 */
	if (RuleData::chkRuleApplied("kohkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("紅旗軍"), get_yaku_han("kohkigun"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1);
			}
		));
	/* 藍旗軍 */
	if (RuleData::chkRuleApplied("rankigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("藍旗軍"), get_yaku_han("rankigun"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* 東風燕剪柳 */
	if (RuleData::chkRuleApplied("dongfeng_yan_jian_liu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東風燕剪柳"), get_yaku_han("dongfeng_yan_jian_liu"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1);
			}
		));
	/* 橋本真也 */
	if (RuleData::chkRuleApplied("hashimoto_shin_ya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("橋本真也"), get_yaku_han("hashimoto_shin_ya"),
			_T("対々和"), _T("絶一門"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->DuiziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::redDragon] >= 1) &&
					(analysis->DuiziCount[TileCode::circleSeven] >= 1) &&
					(analysis->DuiziCount[TileCode::bambooTwo] >= 1) &&
					(analysis->DuiziCount[TileCode::bambooNine] >= 1);
			}
		));
	/* 大南西諸島 */
	if (RuleData::chkRuleApplied("dai_nansei_shotou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大南西諸島"), get_yaku_han("dai_nansei_shotou"),
			_T("対々和"), _T("三色同刻"), _T("混老頭"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->DuiziCount[TileCode::southWind] >= 1) &&
					(analysis->DuiziCount[TileCode::westWind] >= 1);
			}
		));
	/* 白衣の天使 */
	if (RuleData::chkRuleApplied("hakuinotenshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白衣の天使"), get_yaku_han("hakuinotenshi"),
			_T("役牌・白"),
			[countKeziOf, countKangziOf](const MentsuAnalysis* const analysis) -> bool {
				return (countKeziOf(analysis, 7) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(countKangziOf(analysis, 5) >= 1) &&
					(analysis->DuiziCount[TileCode::eastWind] + analysis->DuiziCount[TileCode::southWind] +
					analysis->DuiziCount[TileCode::westWind] + analysis->DuiziCount[TileCode::northWind] >= 1);
			}
		));
	/* 七龍珠 */
	if (RuleData::chkRuleApplied("dragon_ball"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("七龍珠"), get_yaku_han("dragon_ball"),
			_T("対々和"), _T("大三元"), _T("混一色"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] - analysis->KangziCount[TileCode::circleOne] >= 1) &&
					(analysis->DuiziCount[TileCode::circleTwo] >= 1);
			}
		));
	/* 七五三 */
	if (RuleData::chkRuleApplied("shichigosan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("七五三"), get_yaku_han("shichigosan"),
			_T("奇数対々和"), _T("奇数七対子"), _T("断幺九"),
			[countTilesOf](const MentsuAnalysis* const analysis) -> bool {
				return (countTilesOf(analysis, 7) + countTilesOf(analysis, 5) + countTilesOf(analysis, 3) == NumOfTilesInHand) &&
					(countTilesOf(analysis, 7) * countTilesOf(analysis, 5) * countTilesOf(analysis, 3));
			}
		));
	/* 御苦労さん */
	if (RuleData::chkRuleApplied("gokurousan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("御苦労さん"), get_yaku_han("gokurousan"),
			_T("対々和"), _T("筋牌刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool flag = false;
				for (auto i : NumberTileSuits)
					if ((analysis->KeziCount[composeNumberTile(i, 5)] >= 1) && (analysis->KeziCount[composeNumberTile(i, 9)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(i, 6)] >= 1) && (analysis->KeziCount[composeNumberTile(i, 3)] >= 1))
						flag = true;
				return flag;
			}
		));
	/* ニコニコ役満 */
	if (RuleData::chkRuleApplied("nikoniko_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ニコニコ役満"), get_yaku_han("nikoniko_yakuman"),
			_T("対々和"), _T("断幺九"),
			[countDuiziOf](const MentsuAnalysis* const analysis) -> bool {
				return (countDuiziOf(analysis, 2) + countDuiziOf(analysis, 5) == SizeOfMeldBuffer);
			}
		));
	/* 風雪宝双風 */
	if (RuleData::chkRuleApplied("feng_xue_bao_shangfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("風雪宝双風"), get_yaku_han("feng_xue_bao_shangfeng"),
			_T("対々和"), _T("混老頭"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (Wind2Tile(analysis->GameStat->prevailingwind()) != // ダブ風でなくて
					Wind2Tile(analysis->GameStat->playerwind(analysis->player))) &&
					(analysis->DuiziCount[Wind2Tile(analysis->GameStat->playerwind(analysis->player))] >= 1) && // 自風があり
					(analysis->DuiziCount[Wind2Tile(analysis->GameStat->prevailingwind())] >= 1) && // 場風があり
					(analysis->DuiziCount[TileCode::circleOne] >= 1) && (analysis->DuiziCount[TileCode::bambooOne] >= 1) &&
					(analysis->DuiziCount[TileCode::whiteDragon] >= 1);
			}
		));
	/* 遥か彼方 */
	if (RuleData::chkRuleApplied("haruka_kanata"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("遥か彼方"), get_yaku_han("haruka_kanata"),
			_T("役牌・中"), _T("役牌・發"), _T("三暗刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->AnKeziCount[TileCode::eastWind] >= 1) &&
					(analysis->AnKeziCount[TileCode::redDragon] >= 1) &&
					(analysis->AnKeziCount[TileCode::whiteDragon] >= 1);
			}
		));
	/* 激四暗刻 */
	if (RuleData::chkRuleApplied("geki_sianke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("激四暗刻"), get_yaku_han("geki_sianke"),
			_T("四暗刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::MachiType::shanpon) && (!analysis->TsumoAgariFlag())) ?
					1 : 0) == 4) && // 四暗刻を
					(analysis->TsumoHai().tile == TileCode::characterOne); // 一萬で和了る
			}
		));
	/* 激大三元 */
	if (RuleData::chkRuleApplied("geki_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("激大三元"), get_yaku_han("geki_daisangen"),
			_T("大三元"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::MachiType::penchan);
			}
		));
	/* 激北 */
	if (RuleData::chkRuleApplied("geki_north"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("激北"), get_yaku_han("geki_north"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] >= 1);
			}
		));
	/* 大九和 */
	if (RuleData::chkRuleApplied("daikyuuhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大九和"), get_yaku_han("daikyuuhoh"),
			_T("三色同刻"), _T("対々和"), _T("混老頭"), _T("五門斉"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->DuiziCount[TileCode::westWind] >= 1) &&
					(analysis->DuiziCount[TileCode::redDragon] >= 1);
			}
		));
	/* 大林間 */
	if (RuleData::chkRuleApplied("dairinkan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大林間"), get_yaku_han("dairinkan"),
			_T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1);
			}
		));
	/* 藤子スペシャル */
	if (RuleData::chkRuleApplied("fujikospecial"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("藤子スペシャル"), get_yaku_han("fujikospecial"),
			_T("役牌・白"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool flag = false;
				for (int i = 1; i < TileNonflowerMax; i++)
					if (analysis->GameStat->DoraFlag.Omote[i] && analysis->KeziCount[i])
						flag = true;
				return flag &&
					(analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1);
			}
		));
	/* フジテレビ */
	if (RuleData::chkRuleApplied("fujitv"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("フジテレビ"), get_yaku_han("fujitv"),
			_T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->MianziDat[0].tile == TileCode::circleOne) &&
					(analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1);
			}
		));
	/* 東アジア */
	if (RuleData::chkRuleApplied("eastasia"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東アジア"), get_yaku_han("eastasia"),
			_T("役牌・中"), _T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] >= 1) &&
					(analysis->KeziCount[TileCode::southWind] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1);
			}
		));
	/* バッハ */
	if (RuleData::chkRuleApplied("bach"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("バッハ"), get_yaku_han("bach"),
			_T("二色同刻"),
			[countKeziOf](const MentsuAnalysis* const analysis) -> bool {
				unsigned count = 0;
				for (int i = static_cast<int>(TileCode::circleOne); i <= static_cast<int>(TileCode::circleSeven); i++) {
					count += analysis->DuiziCount[i];
					count += analysis->ShunziCount[i];
				}
				count += analysis->DuiziCount[TileCode::circleNine];
				return (countKeziOf(analysis, 8) >= 2) &&
					(count >= 3);
			}
		));
	/* 北島三郎 */
	if (RuleData::chkRuleApplied("kitajima_saburoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北島三郎"), get_yaku_han("kitajima_saburoh"),
			_T("混一色"), _T("四帰一"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool pairIsHonor = false; bool flag = false;
				for (auto i : HonorTiles)
					if (analysis->MianziDat[0].tile == i)
						pairIsHonor = true;
				for (auto i : NumberTileSuits) {
					if ((analysis->KeziCount[composeNumberTile(i, 3)] >= 1) &&
						(analysis->KeziCount[composeNumberTile(i, 6)] >= 1) &&
						(analysis->KeziCount[TileCode::northWind] >= 1) &&
						(pairIsHonor)) {
							if ((analysis->ShunziCount[composeNumberTile(i, 4)] >= 1) &&
								(analysis->TsumoHai().tile == composeNumberTile(i, 6)))
								flag = true;
							if ((analysis->ShunziCount[composeNumberTile(i, 3)] >= 1) &&
								(analysis->TsumoHai().tile == composeNumberTile(i, 3)))
								flag = true;
					}
				}
				return flag;
			}
		));
	/* 大鎌の死神 */
	if (RuleData::chkRuleApplied("reaper"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大鎌の死神"), get_yaku_han("reaper"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1);
			}
		));
	/* 越谷 */
	if (RuleData::chkRuleApplied("koshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("越谷"), get_yaku_han("koshigaya"),
			[countKeziOf, countKangziOf](const MentsuAnalysis* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* 北越谷 */
	if (RuleData::chkRuleApplied("kitakoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北越谷"), get_yaku_han("kitakoshigaya"),
			_T("越谷"),
			[countKeziOf, countKangziOf](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::northWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* 南越谷 */
	if (RuleData::chkRuleApplied("minamikoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南越谷"), get_yaku_han("minamikoshigaya"),
			_T("越谷"),
			[countKeziOf, countKangziOf](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::southWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* 和洋中役満 */
	if (RuleData::chkRuleApplied("wayouchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("和洋中役満"), get_yaku_han("wayouchuu"),
			_T("対々和"), _T("役牌・中"),
			[countKeziOf](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1);
			}
		));
	/* 閏年役満 */
	if (RuleData::chkRuleApplied("leap_year"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("閏年役満"),
			yaku::yakuCalculator::Yaku::HANFUNC(
				[](const MentsuAnalysis* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
					const auto nowTime(DateTime::localTime());
					if ((nowTime.month == DateTime::February) && (nowTime.day == 29))
						return 2_yakuman;
					else return 1_yakuman;
				}),
			[](const MentsuAnalysis* const analysis) -> bool {
				const auto nowTime(DateTime::localTime());
				const bool isLeapYear = (nowTime.year % 400 == 0) || ((nowTime.year % 4 == 0) && (nowTime.year % 100 != 0));
				if (!isLeapYear) return false;
				for (const auto& k : parsedat_trichrome3)
					if ((analysis->KeziCount[(k[0] - _T('0')) * TileSuitStep + 2] >= 1) &&
						(analysis->KeziCount[(k[1] - _T('0')) * TileSuitStep + 2] >= 1) &&
						(analysis->KeziCount[(k[1] - _T('0')) * TileSuitStep + 9] >= 1))
						return true;
				return false;
			}
		));
	/* Windows8 */
	if (RuleData::chkRuleApplied("windows8"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Windows8"), get_yaku_han("windows8"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterEight] >= 1) &&
					(analysis->KeziCount[TileCode::circleEight] >= 1) &&
					(analysis->KeziCount[TileCode::bambooEight] >= 1) &&
					(analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::MachiType::shanpon) &&
					(!analysis->TsumoAgariFlag())) ? 1 : 0) >= 3);
			}
		));
	/* 侵略イカ娘 */
	if (RuleData::chkRuleApplied("ikamusume"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("侵略イカ娘"), get_yaku_han("ikamusume"),
			_T("役牌・白"), _T("役牌・發"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->AnKeziCount[TileCode::eastWind] - analysis->AnKangziCount[TileCode::eastWind] +
					analysis->AnKeziCount[TileCode::southWind] - analysis->AnKangziCount[TileCode::southWind] +
					analysis->AnKeziCount[TileCode::westWind] - analysis->AnKangziCount[TileCode::westWind] +
					analysis->AnKeziCount[TileCode::northWind] - analysis->AnKangziCount[TileCode::northWind]) >= 1) &&
					((analysis->AnKeziCount[TileCode::whiteDragon] - analysis->AnKangziCount[TileCode::whiteDragon]) >= 1) &&
					((analysis->AnKeziCount[TileCode::greenDragon] - analysis->AnKangziCount[TileCode::greenDragon]) >= 1);
			}
		));
	/* あずま寿し */
	if (RuleData::chkRuleApplied("azumazushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("あずま寿し"), get_yaku_han("azumazushi"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == TileCode::characterFour) ||
					(analysis->MianziDat[0].tile == TileCode::circleFour) ||
					(analysis->MianziDat[0].tile == TileCode::bambooFour)) &&
					(analysis->KeziCount[TileCode::eastWind] >= 1);
			}
		));
	/* 西新井 */
	if (RuleData::chkRuleApplied("nishiarai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("西新井"), get_yaku_han("nishiarai"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::westWind] >= 1) &&
					((analysis->TsumoHai().tile == TileCode::characterOne) ||
					(analysis->TsumoHai().tile == TileCode::circleOne) ||
					(analysis->TsumoHai().tile == TileCode::bambooOne) ||
					(analysis->TsumoHai().tile == TileCode::characterTwo) ||
					(analysis->TsumoHai().tile == TileCode::circleTwo) ||
					(analysis->TsumoHai().tile == TileCode::bambooTwo));
			}
		));
	/* 金剛組 */
	if (RuleData::chkRuleApplied("kongougumi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金剛組"), get_yaku_han("kongougumi"),
			_T("対々和"),
			[countKeziOf](const MentsuAnalysis* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 2) &&
					(countKeziOf(analysis, 9) >= 1) &&
					(countKeziOf(analysis, 3) >= 1);
			}
		));
	/* 東日本復興 */
	if (RuleData::chkRuleApplied("eastjapan_fukkou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東日本復興"), get_yaku_han("eastjapan_fukkou"),
			_T("対々和"),
			[countKeziOf](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(countKeziOf(analysis, 2) >= 1) &&
					(countKeziOf(analysis, 5) >= 1);
			}
		));
	/* 正露丸 */
	if (RuleData::chkRuleApplied("seirogan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("正露丸"), get_yaku_han("seirogan"),
			_T("役牌・發"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1) &&
					(analysis->KangziCount[TileCode::circleSix] >= 1);
			}
		));
	/* 朱雀門 */
	if (RuleData::chkRuleApplied("suzakumon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("朱雀門"), get_yaku_han("suzakumon"),
			_T("対々和"), _T("混一色"), _T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::bambooFour] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1) &&
					(analysis->MianziDat[0].tile == TileCode::southWind);
			}
		));
	/* 全六刻 */
	if (RuleData::chkRuleApplied("quanliuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("全六刻"), get_yaku_han("quanliuke"),
			_T("対々和"), _T("役牌・發"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterSix] >= 1) &&
					(analysis->KeziCount[TileCode::circleSix] >= 1) &&
					(analysis->KeziCount[TileCode::bambooSix] >= 1) &&
					(analysis->KeziCount[TileCode::greenDragon] >= 1);
			}
		));
	/* 全玖刻 */
	if (RuleData::chkRuleApplied("quanjiuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("全玖刻"), get_yaku_han("quanjiuke"),
			_T("対々和"), _T("役牌・中"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->KeziCount[TileCode::redDragon] >= 1);
			}
		));
	/* 白鳥湖 */
	if (RuleData::chkRuleApplied("swanlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白鳥湖"), get_yaku_han("swanlake"),
			_T("対々和"), _T("役牌・白"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterFour] >= 1) &&
					(analysis->KeziCount[TileCode::circleFive] >= 1) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1);
			}
		));
	/* 黒鳥湖 */
	if (RuleData::chkRuleApplied("blackbirdlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("黒鳥湖"), get_yaku_han("blackbirdlake"),
			_T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterFour] >= 1) &&
					(analysis->KeziCount[TileCode::circleFive] >= 1) &&
					(analysis->KeziCount[TileCode::bambooOne] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] >= 1);
			}
		));
	/* 大日本史 */
	if (RuleData::chkRuleApplied("dainihonshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大日本史"), get_yaku_han("dainihonshi"),
			_T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleFour] >= 1) &&
					(analysis->KeziCount[TileCode::bambooTwo] >= 1) &&
					(analysis->KeziCount[TileCode::bambooFour] >= 1);
			}
		));
	/* 大化の改新 */
	if (RuleData::chkRuleApplied("taikanokaishin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大化の改新"), get_yaku_han("taikanokaishin"),
			_T("対々和"), _T("役牌・中"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool flag = false;
				for (const auto& k : parsedat_trichrome3)
					if ((analysis->KeziCount[(k[0] - _T('0')) * TileSuitStep + 6] >= 1) &&
						(analysis->KeziCount[(k[1] - _T('0')) * TileSuitStep + 4] >= 1) &&
						(analysis->KeziCount[(k[2] - _T('0')) * TileSuitStep + 5] >= 1))
						flag = true;
				return flag &&
					(analysis->KeziCount[TileCode::redDragon] >= 1);
			}
		));
	/* 青山横北郭白水遶東城 */
	if (RuleData::chkRuleApplied("song_youren"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("青山横北郭白水遶東城"), get_yaku_han("song_youren"),
			_T("対々和"), _T("役牌・白"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::eastWind] >= 1) &&
					(analysis->KeziCount[TileCode::whiteDragon] >= 1) &&
					(analysis->KeziCount[TileCode::bambooThree] - analysis->AnKeziCount[TileCode::bambooThree] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] - analysis->AnKeziCount[TileCode::northWind] >= 1);
			}
		));
	/* 藍一色 */
	if (RuleData::chkRuleApplied("lanyise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("藍一色"), get_yaku_han("lanyise"),
			_T("対々和"), _T("混一色"), _T("小三風"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleOne] >= 1) &&
					(analysis->KeziCount[TileCode::circleTwo] >= 1) &&
					((analysis->DuiziCount[TileCode::eastWind] + analysis->DuiziCount[TileCode::southWind] +
					analysis->DuiziCount[TileCode::westWind] + analysis->DuiziCount[TileCode::northWind]) >= (SizeOfMeldBuffer - 2));
			}
		));
	/* 南大津波 */
	if (RuleData::chkRuleApplied("minami_ootsunami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("南大津波"), get_yaku_han("minami_ootsunami"),
			_T("対々和"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::circleTwo] >= 1) &&
					(analysis->KeziCount[TileCode::circleThree] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::southWind] >= 1);
			}
		));
	/* 西酒玖 */
	if (RuleData::chkRuleApplied("xijiujiu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("西酒玖"), get_yaku_han("xijiujiu"),
			_T("対々和"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterNine] >= 1) &&
					(analysis->KeziCount[TileCode::circleNine] >= 1) &&
					(analysis->KeziCount[TileCode::bambooNine] >= 1) &&
					(analysis->KeziCount[TileCode::westWind] >= 1) &&
					(analysis->MianziDat[0].tile == TileCode::bambooThree);
			}
		));
	/* 難波ミナミ */
	if (RuleData::chkRuleApplied("namba_minami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("難波ミナミ"), get_yaku_han("namba_minami"),
			_T("対々和"),
			[countKeziOf](const MentsuAnalysis* const analysis) -> bool {
				bool flag1 = false; bool flag2 = false;
				for (const auto& k : parsedat_trichrome3)
					if ((analysis->KeziCount[(k[0] - _T('0')) * TileSuitStep + 3] >= 1) &&
						(analysis->KeziCount[(k[1] - _T('0')) * TileSuitStep + 7] >= 1) &&
						(analysis->KeziCount[(k[2] - _T('0')) * TileSuitStep + 3] >= 1))
						flag1 = true;
				for (auto i : NumberTileSuits)
					if ((analysis->KeziCount[composeNumberTile(i, 8)] >= 1) &&
						(analysis->TsumoHai().tile == composeNumberTile(i, 8)))
						flag2 = true;
				return flag1 && flag2 &&
					(analysis->MianziDat[0].tile == TileCode::southWind);
			}
		));
	/* 七神戯北斗 */
	if (RuleData::chkRuleApplied("qishen_xi_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("七神戯北斗"), get_yaku_han("qishen_xi_beidou"),
			_T("対々和"), _T("三色同刻"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KeziCount[TileCode::characterSeven] >= 1) &&
					(analysis->KeziCount[TileCode::circleSeven] >= 1) &&
					(analysis->KeziCount[TileCode::bambooSeven] >= 1) &&
					(analysis->KeziCount[TileCode::northWind] >= 1);
			}
		));
#endif /* GUOBIAO */
}
