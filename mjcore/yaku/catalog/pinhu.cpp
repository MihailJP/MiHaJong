#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu()
{
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���a", yaku::yakuCalculator::Yaku::yval_1han /* ��O�ł��邩�ǂ����̔���͊��ɍs�Ȃ��Ă���̂ŕs�v */,
		[](const MENTSU_ANALYSIS* const analysis) {
			return analysis->isPinfu;
		}
	));
	if (RuleData::chkRuleApplied("kofuku_gekijoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ە�����", get_yaku_han("kofuku_gekijoh") /* ��O�ł��邩�ǂ����̔���͊��ɍs�Ȃ��Ă���̂ŕs�v */,
			"���a",
			[](const MENTSU_ANALYSIS* const analysis) {
				return (analysis->isPinfu &&
					(analysis->TsumoHai->tile == CircleOne));
			}
		));
}
