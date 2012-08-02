#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_dora() {
	/* �h���Řa��������1�� */
	if (getRule(RULE_DORAHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���t�a", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					((analysis->GameStat->DoraFlag.Omote[analysis->TsumoHai->tile] > 0) || // �c���v���\�h���ɂȂ��Ă���(���h���͑ΏۊO)
					(analysis->TsumoHai->red != Normal))); // �ԃh�����h���ɂȂ��Ă���
			}
		));
}
