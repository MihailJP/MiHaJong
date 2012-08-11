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
		auto withdragon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode kezi[] = {GreenDragon};
				if (analysis->shanten[shantenRegular] == -1)
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 1, NULL, 0, false) > 0);
				else if (analysis->shanten[shantenPairs] == -1)
					return (yaku::countingFacility::countPairs(analysis->TileCount, kezi, 1) > 0);
				else return false;
			};
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) == 0) // ᢂȂ��ł��ǂ����[��
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�Έ�F", yaku::yakuCalculator::Yaku::yval_yakuman,
				"����F", // ����F�͕�����
				allgrean
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�Έ�F", yaku::yakuCalculator::Yaku::yval_yakuman,
				"����F",
				[allgrean, withdragon](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (withdragon(analysis));
				}
			));
		if (getRule(RULE_DRAGONLESS_ALL_GREEN) == 2) // ᢂȂ����_�u���𖞂ɂȂ郋�[��
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				chkGameType(&GameStat, SanmaS) ? "���Έ�F" : "�Έ�F", yaku::yakuCalculator::Yaku::yval_double_yakuman,
				"����F",
				[allgrean, withdragon](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (!withdragon(analysis));
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
}
