#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu()
{
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���a", Yyaku::yakuCalculator::Yaku::yval_1han /* ��O�ł��邩�ǂ����̔���͊��ɍs�Ȃ��Ă���̂ŕs�v */,
		[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) {
			return analysis->isPinfu;
		}
	));
}
