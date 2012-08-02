#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_dora() {
	/* ドラで和了ったら1飜 */
	if (getRule(RULE_DORAHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"銅鑼和", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					((analysis->GameStat->DoraFlag.Omote[analysis->TsumoHai->tile] > 0) || // ツモ牌が表ドラになっている(裏ドラは対象外)
					(analysis->TsumoHai->red != Normal))); // 赤ドラか青ドラになっている
			}
		));
}
