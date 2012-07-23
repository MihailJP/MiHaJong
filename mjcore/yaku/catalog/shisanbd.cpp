#include "../catalog.h"

bool yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// �\�O�s�����ǂ������肷��
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // �����Ȃ��Ĉꏄ�ڂł��邱�Ƃ��K�{����
		return ((calcShanten(gameStat, targetPlayer, shantenRegular) == 7) &&
		// �Έ�ʌ`7�����Ƃ́A�Ύq�܂��͓��q��1���������ԂŁA�ʎq���Ȃ����
		(calcShanten(gameStat, targetPlayer, shantenPairs) == 5)); // �Ύ��Ύq5�����Ƃ́A�Ύq��1����������
	else return false;
}
__declspec(dllexport) int yaku::yakuCalculator::chkShisanBuDa(const GameTable* const gameStat, int targetPlayer) {
	return chkShisanBuDa(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

bool yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// �\�l�s�����ǂ������肷��
	if (gameStat->Player[targetPlayer].FirstDrawFlag) // �����Ȃ��Ĉꏄ�ڂł��邱�Ƃ��K�{����
		return (calcShanten(gameStat, targetPlayer, shantenRegular) == 8); // �Έ�ʌ`8�����Ƃ́A�Ύq�����q���Ȃ����
	else return false;
}
__declspec(dllexport) int yaku::yakuCalculator::chkShisiBuDa(const GameTable* const gameStat, int targetPlayer) {
	return chkShisiBuDa(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

