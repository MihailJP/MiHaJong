#include "../catalog.h"

bool yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, PlayerID targetPlayer) {
	// �\�O�s�����ǂ������肷��
#ifdef GUOBIAO
	return false; // ����ȃ��[������Ƃł��v�����H �c�O�I��y(ry
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("shiisan_puutaa")) return false; // ����ȃ��[������Ƃł��v�����H �c�O�I��y(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // �����Ȃ��Ĉꏄ�ڂł��邱�Ƃ��K�{����
		return ((ShantenAnalyzer::calcShanten(gameStat, targetPlayer, shantenRegular) == 7) &&
		// �Έ�ʌ`7�����Ƃ́A�Ύq�܂��͓��q��1���������ԂŁA�ʎq���Ȃ����
		(ShantenAnalyzer::calcShanten(gameStat, targetPlayer, shantenPairs) == 5)); // �Ύ��Ύq5�����Ƃ́A�Ύq��1����������
	else return false;
#endif /* GUOBIAO */
}

bool yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, PlayerID targetPlayer) {
	// �\�l�s�����ǂ������肷��
#ifdef GUOBIAO
	return false; // ����ȃ��[������Ƃł��v�����H �c�O�I��y(ry
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("shiisan_uushii")) return false; // ����ȃ��[������Ƃł��v�����H �c�O�I��y(ry
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // �����Ȃ��Ĉꏄ�ڂł��邱�Ƃ��K�{����
		return (ShantenAnalyzer::calcShanten(gameStat, targetPlayer, shantenRegular) == 8); // �Έ�ʌ`8�����Ƃ́A�Ύq�����q���Ȃ����
	else return false;
#endif /* GUOBIAO */
}

