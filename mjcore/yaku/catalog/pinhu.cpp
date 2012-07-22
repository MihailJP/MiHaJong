#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu()
{
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"平和", Yyaku::yakuCalculator::Yaku::yval_1han /* 門前であるかどうかの判定は既に行なっているので不要 */,
		[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) {
			return analysis->isPinfu;
		}
	));
}
