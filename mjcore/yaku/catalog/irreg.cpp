#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* チートイ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"七対子", (getRule(RULE_SEVEN_PAIRS) == 1) ? /* 1翻50符のルールと2翻25符のルールがある。符はここでは設定できないです…… */
		yaku::yakuCalculator::Yaku::yval_1han_menzen : yaku::yakuCalculator::Yaku::yval_2han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		}
	));
	/* 国士 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"国士無双", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"十三不塔",
		[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0) /* 国士13面待ち（ダブル役満） */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"国士無双十三面", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"国士無双", "十三不塔",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // 国士無双になっていて、
					(/* フリテンかどうか判定する */0) && // フリテンではなくて、
					((/* 13面待ちか判定する */0)||(gameStat->TianHuFlag))); // 13面待ちか天和になっている
			}
		));
}
