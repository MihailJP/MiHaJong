#include "../yaku.h"
#include "../../ruletbl.h"

/* �������񂵂ɂ�����̏���(����ȏ������K�v�Ȃ���) */
void yaku::yakuCalculator::CalculatorThread::checkPostponedYaku
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result,
	std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::set<CodeConv::tstring> &suppression,
	std::vector<CodeConv::tstring> &yakuOrd)
{
	if (analysis->shanten[ShantenAnalyzer::shantenAll] >= 0) return; // �a�����ĂȂ��Ȃ�߂�(�ꉞ�c�c)

	// ---------------------------------------------------------------------

	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, nullptr);

	// ---------------------------------------------------------------------

	/* �A���X�E�}�[�K�g���C�h */
	if (RuleData::chkRuleApplied("alice_margatroid")) {
		/*	��������
			�Œ薞�і��A�Œ蒵�������������Ă���ꍇ�A���̖��͐������܂��� */
		if ((analysis->TotalAnKezi < 3) && (analysis->DuiziCount[EastWind] >= 1) &&
			(analysis->DuiziCount[WhiteDragon] + analysis->DuiziCount[GreenDragon] + analysis->DuiziCount[RedDragon] >= 1) &&
			(analysis->KeziCount[CharacterSeven] >= 1) &&
			(analysis->KeziCount[CircleSeven] >= 1) &&
			(analysis->KeziCount[BambooSeven] >= 1) &&
			(totalHan + totalBonusHan <= 7) &&
			(totalSemiMangan + totalBonusSemiMangan == 0)) {
				LPCTSTR name = _T("�A���X�E�}�[�K�g���C�h");
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_baiman(analysis);
				yakuOrd.push_back(name);
				suppression.insert(_T("�΁X�a"));
				suppression.insert(_T("�O�F����"));
				suppression.insert(_T("�ܖ��"));
		}
	}

	// ---------------------------------------------------------------------

	/* �J���X */
	if (RuleData::chkRuleApplied("karasu")) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(! analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan <= 0) && (totalSemiMangan == 0)) {
				LPCTSTR name = _T("�J���X");
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_1han(analysis);
				yakuOrd.push_back(name);
		}
	}
	/* �J���X���� */
	if (RuleData::chkRuleApplied("karasu_riichi")) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan == 1) && (totalSemiMangan == 0)) {
				LPCTSTR name = _T("�J���X����");
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_1han(analysis);
				yakuOrd.push_back(name);
		}
	}

	// ---------------------------------------------------------------------

	/* �k�� */
	if (RuleData::chkRuleApplied("kitamakura")) {
		if ((totalHan >= 2) && /* 2�ʈȏ゠�邩�H */
			(totalSemiMangan == 0) && /* �𖞖������H */
			(analysis->GameStat->DoraFlag.Omote[NorthWind] == 0) && /* �k�̓h���ł͂Ȃ����H */
			((!*analysis->MenzenFlag) || (!analysis->PlayerStat->RichiFlag.RichiFlag) ||
			(RuleData::chkRuleApplied("uradora")) ||
			(analysis->GameStat->DoraFlag.Ura[NorthWind] == 0)) && /* �k�͗��h���ł͂Ȃ����H */
			(analysis->MianziDat[0].tile == NorthWind)) { /* �������k���H */
				LPCTSTR name = _T("�k��");
				yakuHan[name] = yaku::yakuCalculator::Yaku::YAKU_HAN(
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(-1));
				yakuOrd.push_back(name);
		}
	}
}
