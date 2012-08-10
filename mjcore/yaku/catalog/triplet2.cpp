#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_2() {
	/* ��΁X�a */
	if (getRule(RULE_ODD_TOITOI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��΁X�a", yaku::yakuCalculator::Yaku::yval_4han,
			"�΁X�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SIZE_OF_MELD_BUFFER);
			}
		));
	/* �����΁X�a */
	if (getRule(RULE_EVEN_TOITOI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����΁X�a", yaku::yakuCalculator::Yaku::yval_5han,
			"�΁X�a", "�f���",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 2; i < TILE_SUIT_HONORS; i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SIZE_OF_MELD_BUFFER);
			}
		));

	// ---------------------------------------------------------------------

	/* �������� */
	if (getRule(RULE_CHUUGEN_CHIKUROKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "��v�E�", "��v�E��", "����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[CircleSix] >= 1) );
			}
		));
	/* ���T�z�` */
	if (getRule(RULE_TURTLE_EGG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���T�z�`", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) );
			}
		));
	/* �������� */
	if (getRule(RULE_KYOKUJITSU_TOUSHOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) );
			}
		));
	/* �������R */
	if (getRule(RULE_RIIRAO_SHIISHAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������R", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* �h�]�k�l */
	if (getRule(RULE_YOUBOU_HOKUTO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�]�k�l", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) );
			}
		));
	/* �k�l���� */
	if (getRule(RULE_BIG_DIPPER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�l����", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) );
			}
		));
	/* �����Ҍ� */
	if (getRule(RULE_SEISOU_TAIGETSU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����Ҍ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1) &&
					(analysis->TsumoHai->tile == CircleOne) );
			}
		));
	/* �얳���� */
	if (getRule(RULE_NAMU_HACHIMAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�얳����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CharacterEight] >= 1) );
			}
		));
	/* ���Ώ\�i */
	if (getRule(RULE_SEIKO_JIKKEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Ώ\�i", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1 && analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1 && analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1 && analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1 && analysis->KeziCount[CircleSix] >= 1);
			}
		));
	/* �ΎR���� */
	if (getRule(RULE_VOLCANIC_ERRUPTION) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ΎR����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "��v�E�", "��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CharacterNine] >= 1 ||
					analysis->KeziCount[CircleNine] >= 1 || analysis->KeziCount[BambooNine] >= 1);
			}
		));
	/* �O�P���z */
	if (getRule(RULE_TANFON_CHOUYOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�P���z", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) );
			}
		));

	// ---------------------------------------------------------------------

	auto haouben =
		[](const MENTSU_ANALYSIS* const analysis, tileCode tc, int step) -> bool {
			if ((analysis->KangziCount[tc] >= 1) &&
				(analysis->KeziCount[tc + step] >= 1) &&
				(analysis->KangziCount[tc + step] == 0) &&
				(analysis->MianziDat[0].tile == (tileCode)(tc + step * 2)) )
				return true;
			if ((analysis->KangziCount[tc + step * 2] >= 1) &&
				(analysis->KeziCount[tc + step] >= 1) &&
				(analysis->KangziCount[tc + step] == 0) &&
				(analysis->MianziDat[0].tile == tc) )
				return true;
			return false;
		};

	/* �e���� */
	if (getRule(RULE_HAOUBEN) == 1)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�e����", yaku::yakuCalculator::Yaku::yval_6han,
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 3; k++)
						yakuFlag = yakuFlag || haouben(analysis, (tileCode)(i + k), 3);
				return yakuFlag;
			}
		));
	else if (getRule(RULE_HAOUBEN) == 2)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�e����", yaku::yakuCalculator::Yaku::yval_6han,
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int k = 1; k <= 3; k++)
					yakuFlag = yakuFlag || haouben(analysis, (tileCode)(TILE_SUIT_BAMBOOS + k), 3);
				return yakuFlag;
			}
		));
	/* ���֒� */
	if (getRule(RULE_RYUUKANCHAN) == 2)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���֒�", yaku::yakuCalculator::Yaku::yval_6han,
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 7; k++)
						yakuFlag = yakuFlag || haouben(analysis, (tileCode)(i + k), 1);
				return yakuFlag;
			}
		));
}
