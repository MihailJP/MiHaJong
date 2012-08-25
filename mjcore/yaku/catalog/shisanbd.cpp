#include "../catalog.h"

bool yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// �\�O�s�����ǂ������肷��
	if (!RuleData::chkRuleApplied("shiisan_puutaa")) return false; // ����ȃ��[������Ƃł��v�����H �c�O�I��y(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // �����Ȃ��Ĉꏄ�ڂł��邱�Ƃ��K�{����
		return ((ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenRegular) == 7) &&
		// �Έ�ʌ`7�����Ƃ́A�Ύq�܂��͓��q��1���������ԂŁA�ʎq���Ȃ����
		(ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenPairs) == 5)); // �Ύ��Ύq5�����Ƃ́A�Ύq��1����������
	else return false;
}
__declspec(dllexport) int yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, int targetPlayer) {
	return chkShisanBuDa(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

bool yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// �\�l�s�����ǂ������肷��
	if (!RuleData::chkRuleApplied("shiisan_uushii")) return false; // ����ȃ��[������Ƃł��v�����H �c�O�I��y(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // �����Ȃ��Ĉꏄ�ڂł��邱�Ƃ��K�{����
		return (ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenRegular) == 8); // �Έ�ʌ`8�����Ƃ́A�Ύq�����q���Ȃ����
	else return false;
}
__declspec(dllexport) int yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, int targetPlayer) {
	return chkShisiBuDa(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

