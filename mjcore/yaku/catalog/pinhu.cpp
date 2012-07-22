#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu()
{
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���a", yaku::yakuCalculator::Yaku::yval_1han /* ��O�ł��邩�ǂ����̔���͊��ɍs�Ȃ��Ă���̂ŕs�v */,
		[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) {
			return analysis->isPinfu;
		}
	));
	if (getRule(RULE_KOFUKU_GEKIJOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ە�����", yaku::yakuCalculator::Yaku::yval_2han /* ��O�ł��邩�ǂ����̔���͊��ɍs�Ȃ��Ă���̂ŕs�v */,
			"���a",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
				return (analysis->isPinfu &&
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleOne));
			}
		));
}
