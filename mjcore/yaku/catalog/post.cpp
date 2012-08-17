#include "../yaku.h"

/* �������񂵂ɂ�����̏���(����ȏ������K�v�Ȃ���) */
void yaku::yakuCalculator::CalculatorThread::checkPostponedYaku
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result,
	std::map<std::string, Yaku::YAKU_HAN> &yakuHan, std::set<std::string> &suppression,
	std::vector<std::string> &yakuOrd)
{
	if (analysis->shanten[ShantenAnalyzer::shantenAll] >= 0) return; // �a�����ĂȂ��Ȃ�߂�(�ꉞ�c�c)

	// ---------------------------------------------------------------------

	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, NULL);

	// ---------------------------------------------------------------------

	/* �A���X�E�}�[�K�g���C�h */
	if (RuleData::getRule("alice_margatroid") != 0) {
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

	// ---------------------------------------------------------------------

	/* �J���X */
	if (RuleData::getRule("karasu") != 0) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(! analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan <= 0) && (totalSemiMangan == 0)) {
				const char* name = "�J���X";
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_1han(analysis);
				yakuOrd.push_back(name);
		}
	}
	/* �J���X���� */
	if (RuleData::getRule("karasu_riichi") != 0) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan == 1) && (totalSemiMangan == 0)) {
				const char* name = "�J���X����";
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_1han(analysis);
				yakuOrd.push_back(name);
		}
	}

	// ---------------------------------------------------------------------

	/* �k�� */
	if (RuleData::getRule("kitamakura") != 0) {
		if ((totalHan >= 2) && /* 2�ʈȏ゠�邩�H */
			(totalSemiMangan == 0) && /* �𖞖������H */
			(analysis->GameStat->DoraFlag.Omote[NorthWind] == 0) && /* �k�̓h���ł͂Ȃ����H */
			((!*analysis->MenzenFlag) || (!analysis->PlayerStat->RichiFlag.RichiFlag) ||
			(RuleData::getRule("uradora") == 1) ||
			(analysis->GameStat->DoraFlag.Ura[NorthWind] == 0)) && /* �k�͗��h���ł͂Ȃ����H */
			(analysis->MianziDat[0].tile == NorthWind)) { /* �������k���H */
				const char* name = "�k��";
				yakuHan[name] = yaku::yakuCalculator::Yaku::YAKU_HAN(
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(-1));
				yakuOrd.push_back(name);
		}
	}
}
