#include "../catalog.h"

bool yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, PlayerID targetPlayer) {
	// 十三不塔かどうか判定する
#ifdef GUOBIAO
	return false; // そんなルールあるとでも思った？ 残念！さy(ry
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("shiisan_puutaa")) return false; // そんなルールあるとでも思った？ 残念！さy(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // 鳴きがなくて一巡目であることが必須条件
		return ((ShantenAnalyzer::calcShanten(gameStat, targetPlayer, shantenRegular) == 7) &&
		// 対一般形7向聴とは、対子または塔子が1つだけある状態で、面子がない状態
		(ShantenAnalyzer::calcShanten(gameStat, targetPlayer, shantenPairs) == 5)); // 対七対子5向聴とは、対子が1つだけある状態
	else return false;
#endif /* GUOBIAO */
}

bool yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, PlayerID targetPlayer) {
	// 十四不塔かどうか判定する
#ifdef GUOBIAO
	return false; // そんなルールあるとでも思った？ 残念！さy(ry
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("shiisan_uushii")) return false; // そんなルールあるとでも思った？ 残念！さy(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // 鳴きがなくて一巡目であることが必須条件
		return (ShantenAnalyzer::calcShanten(gameStat, targetPlayer, shantenRegular) == 8); // 対一般形8向聴とは、対子も塔子もない状態
	else return false;
#endif /* GUOBIAO */
}

