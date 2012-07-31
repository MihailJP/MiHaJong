#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_suit() {
	/* チンイツ */
	auto isshoku =
		[](const MENTSU_ANALYSIS* const analysis, bool chin_itsu) -> bool {
			bool flag[TILE_SUIT_HONORS / TILE_SUIT_STEP]; bool yakuFlag = true;
			for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++) flag[i] = true;
			for (int k = 1; k < (chin_itsu ? TILE_NONFLOWER_MAX : TILE_SUIT_HONORS); k++)
				if (analysis->TileCount[k] > 0)
					for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
						if ((k / TILE_SUIT_STEP) != i) flag[i] = false;
			for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
				yakuFlag &= flag[i];
			return yakuFlag;
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"清一色", (getRule(RULE_CHINIISOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_4han_kuisagari : yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
		"混一色",
		[isshoku](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, true);
		}
	));
	/* ここにいろいろチンイツの上位役 */

	/* ホンイツ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"混一色", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		[isshoku](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, false);
		}
	));
}
