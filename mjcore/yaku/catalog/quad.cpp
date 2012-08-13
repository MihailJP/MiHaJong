#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_quad() {
	/* ���l�� */
	if (getRule(RULE_CHOUSIXI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���l��", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"��l��", "�l�Ȏq", "�΁X�a", "��������", "����菖k",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* �l�Ȏq */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�l�Ȏq", ((getRule(RULE_SUUKANTSU) == 1) || (getRule(RULE_SUUKANTSU) == 3)) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"�΁X�a",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 4;
		}
	));
	/* �O�Þ� */
	if (getRule(RULE_SANANKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�Þ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 3;
			}
		));
	/* �勬�J */
	if (getRule(RULE_GRAND_CANYON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�勬�J", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CharacterNine] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] + analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2));
			}
		));
	/* �ቻ�� */
	if (getRule(RULE_YUKIGESHOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ቻ��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E��", "�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[WhiteDragon] >= 1));
			}
		));
	/* �O�F���� */
	if (getRule(RULE_SANSHOKU_DOUKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F����", (getRule(RULE_SANSHOKU_DOUKAN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_5han_kuisagari,
			"�O�F����", "�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if ((analysis->KangziCount[TILE_SUIT_CHARACTERS + i] >= 1) &&
						(analysis->KangziCount[TILE_SUIT_CIRCLES + i] >= 1) &&
						(analysis->KangziCount[TILE_SUIT_BAMBOOS + i] >= 1))
						return true;
				return false;
			}
		));
	/* ���O�� */
	if (getRule(RULE_CHOUSANGEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���O��", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"��O��", "�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WhiteDragon] >= 1) &&
					(analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[RedDragon] >= 1));
			}
		));
	/* �O�Ȏq */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�O�Ȏq", (getRule(RULE_SANKANTSU) == 2) ? yaku::yakuCalculator::Yaku::yval_yakuman :
		((getRule(RULE_SANKANTSU) == 1) ? yaku::yakuCalculator::Yaku::yval_3han :
		yaku::yakuCalculator::Yaku::yval_2han),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 3;
		}
	));
	/* ��Þ� */
	if (getRule(RULE_RYANANKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��Þ�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 2;
			}
		));
	/* �������� */
	if (getRule(RULE_SHENGDONG_JIXI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", yaku::yakuCalculator::Yaku::yval_6han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1));
			}
		));
	/* ����菖k */
	if (getRule(RULE_ZAONAN_CHUANGBEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����菖k", yaku::yakuCalculator::Yaku::yval_6han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* �Ò��͍� */
	if (getRule(RULE_ANCHUMOSAKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�Ò��͍�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKangziCount[RedDragon] >= 1) &&
					(analysis->TsumoHai->tile / TILE_SUIT_STEP == TILE_SUIT_BAMBOOS / TILE_SUIT_STEP) &&
					(*analysis->TsumoAgariFlag));
			}
		));
	/* ��� */
	if (getRule(RULE_TANK) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooSeven] >= 1);
			}
		));
	/* �^�c�Z���K */
	if (getRule(RULE_SANADACOIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�^�c�Z���K", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* �O��̐��� */
	if (getRule(RULE_THREE_ARROWS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O��̐���", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooThree] >= 1);
			}
		));
	/* �S�͊O */
	if (getRule(RULE_SETSUBUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�S�͊O", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				/* TODO: ���������t�����肷�� */
				return (analysis->KangziCount[CircleNine] >= 1);
			}
		));
	/* ���� */
	if (getRule(RULE_SOUKA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if (analysis->KaKangziCount[TILE_SUIT_BAMBOOS + i] >= 1)
						return true;
				return false;
			}
		));
}
