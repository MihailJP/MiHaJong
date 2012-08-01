#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu()
{
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"平和", yaku::yakuCalculator::Yaku::yval_1han /* 門前であるかどうかの判定は既に行なっているので不要 */,
		[](const MENTSU_ANALYSIS* const analysis) {
			return analysis->isPinfu;
		}
	));
	if (getRule(RULE_KOFUKU_GEKIJOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鼓腹撃壌", yaku::yakuCalculator::Yaku::yval_2han /* 門前であるかどうかの判定は既に行なっているので不要 */,
			"平和",
			[](const MENTSU_ANALYSIS* const analysis) {
				return (analysis->isPinfu &&
					(analysis->TsumoHai->tile == CircleOne));
			}
		));
}
