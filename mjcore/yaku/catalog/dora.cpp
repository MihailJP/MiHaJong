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
	/* �k�l������ */
	if (getRule(RULE_FOUR_NORTHES) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�l������", yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->NorthFlag == 4)); // �k4��
			}
		));
	/* ����ߊC */
	if (getRule(RULE_EIGHT_FLOWERS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����ߊC", yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			"�t�ďH�~", "�l�؊J��", "�{�ԋG�ߔv", "�{�ԑ��ؔv",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter) &&
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* �t�ďH�~ */
	if (getRule(RULE_FOUR_SEASONS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�t�ďH�~", (getRule(RULE_FOUR_SEASONS) == 1) ?
			yaku::yakuCalculator::Yaku::yval_2han_dependent :
			(getRule(RULE_FOUR_SEASONS) == 2) ? yaku::yakuCalculator::Yaku::yval_4han_dependent :
			yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			"�{�ԋG�ߔv",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter));
			}
		));
	/* �l�؊J�� */
	if (getRule(RULE_FOUR_FLOWERS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�؊J��", (getRule(RULE_FOUR_FLOWERS) == 1) ?
			yaku::yakuCalculator::Yaku::yval_2han_dependent :
			(getRule(RULE_FOUR_FLOWERS) == 2) ? yaku::yakuCalculator::Yaku::yval_4han_dependent :
			yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			"�{�ԑ��ؔv",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* �{�� */
	if (getRule(RULE_OWN_FLOWER) != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�{�ԋG�ߔv", yaku::yakuCalculator::Yaku::yval_1han_dependent,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenAll] != -1) return false; // �a�����ĂȂ��Ȃ�߂�
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Spring;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Summer;
					case sWest: return analysis->PlayerStat->FlowerFlag.Autumn;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Winter;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "���������쐼�k�̂ǂ�ł�����܂���");
						return false;
				}
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�{�ԑ��ؔv", yaku::yakuCalculator::Yaku::yval_1han_dependent,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenAll] != -1) return false; // �a�����ĂȂ��Ȃ�߂�
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Plum;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Orchid;
					case sWest: return analysis->PlayerStat->FlowerFlag.Chrys;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Bamboo;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "���������쐼�k�̂ǂ�ł�����܂���");
						return false;
				}
			}
		));
	}
}
