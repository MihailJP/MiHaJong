#include "../catalog.h"

bool yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// 十三不塔かどうか判定する
	if (!RuleData::chkRuleApplied("shiisan_puutaa")) return false; // そんなルールあるとでも思った？ 残念！さy(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // 鳴きがなくて一巡目であることが必須条件
		return ((ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenRegular) == 7) &&
		// 対一般形7向聴とは、対子または塔子が1つだけある状態で、面子がない状態
		(ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenPairs) == 5)); // 対七対子5向聴とは、対子が1つだけある状態
	else return false;
}
__declspec(dllexport) int yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, int targetPlayer) {
	return chkShisanBuDa(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

bool yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// 十四不塔かどうか判定する
	if (!RuleData::chkRuleApplied("shiisan_uushii")) return false; // そんなルールあるとでも思った？ 残念！さy(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // 鳴きがなくて一巡目であることが必須条件
		return (ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenRegular) == 8); // 対一般形8向聴とは、対子も塔子もない状態
	else return false;
}
__declspec(dllexport) int yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, int targetPlayer) {
	return chkShisiBuDa(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

