#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu()
{
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"平和", yaku::yakuCalculator::Yaku::yval_1han /* 門前であるかどうかの判定は既に行なっているので不要 */,
		[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) {
			return analysis->isPinfu;
		}
	));
	if (getRule(RULE_KOFUKU_GEKIJOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鼓腹撃壌", yaku::yakuCalculator::Yaku::yval_2han /* 門前であるかどうかの判定は既に行なっているので不要 */,
			"平和",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
				return (analysis->isPinfu &&
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleOne));
			}
		));
}
