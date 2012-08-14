#include "../yaku.h"

/* �������񂵂ɂ�����̏���(����ȏ������K�v�Ȃ���) */
void yaku::yakuCalculator::CalculatorThread::checkPostponedYaku
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result,
	std::map<std::string, Yaku::YAKU_HAN> &yakuHan, std::set<std::string> &suppression,
	std::vector<std::string> &yakuOrd)
{
	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, NULL);

	// ---------------------------------------------------------------------

	/* �A���X�E�}�[�K�g���C�h */
	if (getRule(RULE_ALICE_MARGATROID) != 0) {
		/*	��������
			�Œ薞�і��A�Œ蒵�������������Ă���ꍇ�A���̖��͐������܂��� */
		if ((analysis->TotalAnKezi < 3) && (analysis->DuiziCount[EastWind] >= 1) &&
			(analysis->DuiziCount[WhiteDragon] + analysis->DuiziCount[GreenDragon] + analysis->DuiziCount[RedDragon] >= 1) &&
			(analysis->KeziCount[CharacterSeven] >= 1) &&
			(analysis->KeziCount[CircleSeven] >= 1) &&
			(analysis->KeziCount[BambooSeven] >= 1) &&
			(totalHan + totalBonusHan <= 7) &&
			(totalSemiMangan + totalBonusSemiMangan == 0)) {
				const char* name = "�A���X�E�}�[�K�g���C�h";
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_baiman(analysis);
				yakuOrd.push_back(name);
				suppression.insert("�΁X�a");
				suppression.insert("�O�F����");
				suppression.insert("�ܖ��");
		}
	}
}
