#include "../catalog.h"

extern GameTable GameStat;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_misc() {
	auto chktiles = // �ʎq��E���Ύq���p�̔���֐��I�u�W�F�N�g
		[](const MENTSU_ANALYSIS* const analysis, const tileCode* const targetKez, int numOfKez,
		const tileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NUM_OF_TILES_IN_HAND / 2);
			else return false;
		};

	// ---------------------------------------------------------------------

	/* �Έ�F */
	{
		auto allgrean =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, GreenDragon};
				const tileCode shunzi[] = {BambooTwo};
				return chktiles(analysis, kezi, 6, shunzi, 1, false);
			};
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�Έ�F", yaku::yakuCalculator::Yaku::yval_yakuman,
				"����F",
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] > 0);
				}
			));
		else if (chkGameType(&GameStat, SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�Έ�F", yaku::yakuCalculator::Yaku::yval_yakuman,
				"����F", "����F",
				allgrean
			));
		else // ᢂȂ��ł��ǂ����[��
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�Έ�F", yaku::yakuCalculator::Yaku::yval_yakuman,
				"����F", // ����F�͕�����
				allgrean
			));
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) == 2) // ᢂȂ����_�u���𖞂ɂȂ郋�[��
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				chkGameType(&GameStat, SanmaS) ? "���Έ�F" : "�Έ�F", yaku::yakuCalculator::Yaku::yval_double_yakuman,
				"����F",
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] == 0);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ���P�R�P�ʂƂ������[�J���� */
	{
		auto chkHadakaTanki =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++)
					if ((analysis->MianziDat[i].mstat != meldSequenceConcealed) &&
						(analysis->MianziDat[i].mstat != meldTripletConcealed) &&
						(analysis->MianziDat[i].mstat != meldQuadConcealed)) ++count;
				return (count == SIZE_OF_MELD_BUFFER - 1);
			};
		if (getRule(RULE_SHIIARU_RAOTAI) != 0) {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�|��", yaku::yakuCalculator::Yaku::yval_1han,
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && analysis->GameStat->TsumoAgariFlag;
				}
			));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S���l", yaku::yakuCalculator::Yaku::yval_1han,
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (!analysis->GameStat->TsumoAgariFlag);
				}
			));
		}
		// ���{�Ɨ��c����҂�
		if (getRule(RULE_KINKEI_DOKURITSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"���{�Ɨ�", yaku::yakuCalculator::Yaku::yval_yakuman,
				"�S�|��", "�S���l",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == BambooOne);
				}
			));
		// �ƒފ��]��c���҂�
		if (getRule(RULE_KANKOU_NO_YUKI) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�ƒފ��]��", yaku::yakuCalculator::Yaku::yval_yakuman,
				"�S�|��", "�S���l",
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == WhiteDragon);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ���s�| */
	if (getRule(RULE_TOIPUUTAO) != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC han;
		switch (getRule(RULE_TOIPUUTAO)) {
			case 1: han = yaku::yakuCalculator::Yaku::yval_1han; break;
			case 2: han = yaku::yakuCalculator::Yaku::yval_4han; break;
			case 3: han = yaku::yakuCalculator::Yaku::yval_yakuman; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���s�|", han,
			"����",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					CircleOne, CircleTwo, CircleThree, CircleFour, CircleFive, CircleEight, CircleNine,
					BambooTwo, BambooFour, BambooFive, BambooSix, BambooEight, BambooNine, WhiteDragon,
				};
				const tileCode shunzi[] = {
					CircleOne, CircleTwo, CircleThree, BambooFour,
				};
				return chktiles(analysis, kezi, 14, shunzi, 4, false);
			}
		));
	}
	/* �g�E�� */
	if (getRule(RULE_BENIKUJAKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�g�E��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F", "�΁X�a",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooOne, BambooFive, BambooSeven, BambooNine, RedDragon,
				};
				return chktiles(analysis, kezi, 5, NULL, 0, false);
			}
		));
	/* �ݗΑp����_�g */
	if (getRule(RULE_KOUITTEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�g��_", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, RedDragon,
				};
				const tileCode shunzi[] = {
					BambooTwo,
				};
				return chktiles(analysis, kezi, 6, shunzi, 1, false) &&
					(analysis->TileCount[RedDragon] > 0);
			}
		));
	/* ���L��Έ�F */
	if (getRule(RULE_ALL_GREEN_WITH_WHITE) != 0) {
		auto allgreen_white =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, WhiteDragon,
				};
				const tileCode shunzi[] = {
					BambooTwo,
				};
				return chktiles(analysis, kezi, 6, shunzi, 1, false) &&
					(analysis->TileCount[WhiteDragon] > 0);
			};
		if (getRule(RULE_ALL_GREEN_WITH_WHITE) == 1)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"����F", yaku::yakuCalculator::Yaku::yval_2han,
				allgreen_white
			));
		else if (getRule(RULE_ALL_GREEN_WITH_WHITE) == 2)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"���L��Έ�F", yaku::yakuCalculator::Yaku::yval_yakuman,
				"����F",
				allgreen_white
			));
	}
	/* �Έ�F�� */
	if (getRule(RULE_RYUUIISOHRIN) != 0) {
		auto allgreen_five =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive, BambooSix, BambooEight,
				};
				const tileCode shunzi[] = {
					BambooTwo,
				};
				return chktiles(analysis, kezi, 6, shunzi, 1, false) &&
					(analysis->MianziDat[0].tile == BambooFive);
			};
		if (chkGameType(&GameStat, SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�Έ�F��", yaku::yakuCalculator::Yaku::yval_yakuman,
				"����F",
				allgreen_five
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�Έ�F��", yaku::yakuCalculator::Yaku::yval_yakuman,
				allgreen_five
			));
	}
	/* �呐�� */
	if (getRule(RULE_DAISOUGEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�呐��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine,
				};
				const tileCode shunzi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
				};
				return chktiles(analysis, kezi, 7, shunzi, 4, false);
			}
		));
	/* ��呐�� */
	if (getRule(RULE_SOUTH_DAISOUGEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��呐��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine, SouthWind,
				};
				const tileCode shunzi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
				};
				return chktiles(analysis, kezi, 7, shunzi, 4, false) &&
					(analysis->TileCount[SouthWind] > 0);
			}
		));
}
