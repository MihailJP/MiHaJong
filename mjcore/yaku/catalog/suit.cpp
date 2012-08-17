#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_suit() {
	/* �`���C�c */
#include "isshoku.h" /* ��Ԃꂩ�Ԃ�B�s�V�̈���include */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����F", (RuleData::getRule("chiniisoh") != 0) ?
		yaku::yakuCalculator::Yaku::yval_4han_kuisagari : yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
		"����F", "����C�ʊёS�ћ��",
		[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, true);
		}
	));

	// ---------------------------------------------------------------------

	/* �a�������玀�� */
	auto chkHaishiki =
		[](const MENTSU_ANALYSIS* const analysis, const char* const haishiki) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
				if ((analysis->TileCount[i + 1] >= (int)(haishiki[0] - '0')) &&
					(analysis->TileCount[i + 2] >= (int)(haishiki[1] - '0')) &&
					(analysis->TileCount[i + 3] >= (int)(haishiki[2] - '0')) &&
					(analysis->TileCount[i + 4] >= (int)(haishiki[3] - '0')) &&
					(analysis->TileCount[i + 5] >= (int)(haishiki[4] - '0')) &&
					(analysis->TileCount[i + 6] >= (int)(haishiki[5] - '0')) &&
					(analysis->TileCount[i + 7] >= (int)(haishiki[6] - '0')) &&
					(analysis->TileCount[i + 8] >= (int)(haishiki[7] - '0')) &&
					(analysis->TileCount[i + 9] >= (int)(haishiki[8] - '0')) ) yakuFlag = true;
			return yakuFlag;
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��@��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"����F", "��@�ԓ�",
		[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkHaishiki(analysis, "311111113");
		}
	));
	if (RuleData::getRule("double_yakuman") == 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������@��", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"��@��", "����F", "��@�ԓ�",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const tileCode* tsumoTile = &(analysis->TsumoHai->tile);
				if ((*tsumoTile % 10 == 1) || (*tsumoTile % 10 == 9)) {
					if (analysis->TileCount[*tsumoTile] >= 4) yakuFlag = true;
				} else {
					if (analysis->TileCount[*tsumoTile] >= 2) yakuFlag = true;
				}
				return (chkHaishiki(analysis, "311111113")) && (!analysis->MachiInfo.FuritenFlag);
			}
		));
	/* �^���q */
	if (RuleData::getRule("daisharin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��ԗ�", (RuleData::getRule("daisharin") == 2) ?
			yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a", "�f���",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CircleFive] >= 2));
			}
		));
	/* ��|�� */
	if (RuleData::getRule("daichikurin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��|��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a", "�f���",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[BambooFive] >= 2));
			}
		));
	/* �吔�� */
	if (RuleData::getRule("daisuurin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�吔��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a", "�f���", "���F",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CharacterFive] >= 2));
			}
		));
	/* ���ԗ� */
	if (RuleData::getRule("shosharin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԗ�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CircleFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* ���|�� */
	if (RuleData::getRule("shochikurin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���|��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[BambooFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* ������ */
	if (RuleData::getRule("shosuurin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CharacterFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* �~�����a (3.)14159265358979 */
	if (RuleData::getRule("pi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�~�����a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "211131113");
			}
		));

	// ---------------------------------------------------------------------

	/* ��Q�a 1.4142135623730 */
	if (RuleData::getRule("square_root_two") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��Q�a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "323211100");
			}
		));
	/* ���R�ΐ��̒� 2.7182818284590 */
	if (RuleData::getRule("natural_logarithm") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���R�ΐ��̒�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "230110141");
			}
		));
	/* �����a (0.)207879576350761 */
	if (RuleData::getRule("imaginary_number") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "111022411");
			}
		));
	/* ��P�O�a 3.16227766016837 */
	if (RuleData::getRule("square_root_ten") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��P�O�a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "222004310");
			}
		));

	// ---------------------------------------------------------------------

	/* ���C�� */
	if (RuleData::getRule("qinghaihu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���C��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "���S�ћ��", "��u��", "���a",
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((chkHaishiki(analysis, "422000222") || chkHaishiki(analysis, "222000224")));
			}
		));
	/* ���ԗ� */
	auto chuusharin =
		[chkHaishiki](const MENTSU_ANALYSIS* const analysis, int suit) -> bool {
			return ((chkHaishiki(analysis, "202222220") || chkHaishiki(analysis, "222022220") ||
				chkHaishiki(analysis, "222202220") || chkHaishiki(analysis, "222222020") ||
				chkHaishiki(analysis, "022222202") || chkHaishiki(analysis, "022220222") ||
				chkHaishiki(analysis, "022202222") || chkHaishiki(analysis, "020222222") ||
				chkHaishiki(analysis, "220222220") || chkHaishiki(analysis, "222220220") ||
				chkHaishiki(analysis, "022022222") || chkHaishiki(analysis, "022222022")) &&
				((analysis->TileCount[suit + 4] >= 2) || (analysis->TileCount[suit + 5] >= 2))
				);
		};
	if (RuleData::getRule("chuusharin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԗ�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��",
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_CIRCLES);
			}
		));
	/* ���|�� */
	if (RuleData::getRule("chuuchikurin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���|��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��",
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_BAMBOOS);
			}
		));
	/* ������ */
	if (RuleData::getRule("chuusuurin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��",
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_CHARACTERS);
			}
		));

	// ---------------------------------------------------------------------

	/* �S���� */
	auto chiffre =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			return (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) ?
				yaku::countingFacility::countMentzNumerals(analysis->MianziDat) :
				yaku::countingFacility::countTileNumerals(analysis->TileCount);
		};
	if (RuleData::getRule("kaga_million") != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�S����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) > 100) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����S����", (RuleData::getRule("kaga_million") == 2) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"����F",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 100) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
	}
	/* ���q�S���� */
	if (RuleData::getRule("pinzu_million") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���q�S����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) >= 100) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CIRCLES / TILE_SUIT_STEP)));
			}
		));
	/* �I�B�܏\�ܖ��� */
	if (RuleData::getRule("kishu550k") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�I�B�܏\�ܖ���", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 55) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
	/* ���ˎO�\�ܖ��� */
	if (RuleData::getRule("mito350k") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ˎO�\�ܖ���", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) <= 35) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));
	/* �t�����[�� */
	if (RuleData::getRule("fullmoon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�t�����[��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[isshoku, chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 88));
			}
		));

	// ---------------------------------------------------------------------

	/* �����W� */
	if (RuleData::getRule("starlight") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����W�", yaku::yakuCalculator::Yaku::yval_1han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (analysis->KeziCount[CircleSeven]));
			}
		));
	/* ���F */
	if (RuleData::getRule("cheniisoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���F", yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_8han),
			"����F", "�f���",
			/*
				���Ύq�̞��F�͂S���g����F�߂Ȃ�����͂Q�Q�R�R�S�S�T�T�U�U�V�V�W�W�̌`�̂�
				����͑吔�ׂƓ����`�ł���A���F�̂ݍ̗p�̏ꍇ�ł��ʎq��Ƃ��Ĉ�����
			*/
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) {
					int yakuFlagCount = 0;
					const tileCode targetKezi[] = {CharacterOne, CharacterNine,};
					const tileCode targetShunzi[] = {CharacterOne, CharacterSeven,};
					if (yaku::countingFacility::countSpecMentz(analysis->MianziDat, targetKezi, 2, targetShunzi, 2, false) == 0)
						yakuFlag = true;
				} else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1) {
					int yakuFlagCount = 0;
					const tileCode targetDuizi[] = {CharacterOne, CharacterNine,};
					if (yaku::countingFacility::countPairs(analysis->TileCount, targetDuizi, 2) == 0)
						yakuFlag = true;
				}
				return (isshoku(analysis, true) && (yakuFlag) &&
					((analysis->TsumoHai->tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_CHARACTERS / TILE_SUIT_STEP)));
			}
		));

	// ---------------------------------------------------------------------

	/* �z���C�c */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����F", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		"��C�ʊёS�ћ��",
		[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, false);
		}
	));

	// ---------------------------------------------------------------------

	/* �O�����ݎq�z���C�c */
	if ((RuleData::getRule("characters_mahjong") != 0) && chkGameType(&GameStat, SanmaT))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ݘa", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F", "���V��",
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					((analysis->TileCount[CharacterOne] >= 1)||(analysis->TileCount[CharacterNine] >= 1)));
			}
		));
	/* �����I�����s�b�N(�z���C�c) */
	if (RuleData::getRule("tokyo_olympic_monochrome") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����I�����s�b�N", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F",
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					((analysis->TileCount[CircleFive] >= 1)||(analysis->TileCount[EastWind] >= 1)));
			}
		));
	/* ������B */
	if (RuleData::getRule("saigou_nanshuu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������B", yaku::yakuCalculator::Yaku::yval_2han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1));
			}
		));
	/* ������������ */
	if (RuleData::getRule("kagamine_rinlen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������������", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F", "��������", "��������", "�O�A��", "�΁X�a",
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[i + 1] >= 1) && (analysis->KeziCount[i + 2] >= 1))
						yakuFlag = true;
				return (isshoku(analysis, false) && yakuFlag &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP)));
			}
		));
	/* �������� */
	if (RuleData::getRule("kagamine_rin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", yaku::yakuCalculator::Yaku::yval_2han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				int PinMian = 0;
				for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
					if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP))
						++PinMian;
				return (isshoku(analysis, false) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* �|���T����n */
	if (RuleData::getRule("honmanopotechi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�|���T����n", yaku::yakuCalculator::Yaku::yval_1han,
			[isshoku](const MENTSU_ANALYSIS* const analysis) -> bool {
				int PinMian = 0;
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) {
					for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
						if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP))
							++PinMian;
					return (isshoku(analysis, false) &&
						(PinMian == SIZE_OF_MELD_BUFFER - 1) &&
						(analysis->DuiziCount[GreenDragon] >= 1));
				} else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1) {
					for (int i = (int)CircleOne; i <= (int)CircleNine; i++)
						PinMian += analysis->TileCount[i];
					return (isshoku(analysis, false) &&
						(PinMian == NUM_OF_TILES_IN_HAND - 2) &&
						(analysis->TileCount[GreenDragon] >= 2));
				} else return false;
			}
		));

	// ---------------------------------------------------------------------

	/* ���� */
	auto chueiimen =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool flag[TILE_SUIT_HONORS / TILE_SUIT_STEP] = {false};
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) {
				for (int k = 0; k < SIZE_OF_MELD_BUFFER; k++)
					if (analysis->MianziDat[k].tile < TILE_SUIT_HONORS)
						flag[analysis->MianziDat[k].tile / TILE_SUIT_STEP] = true;
			} else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1) {
				for (int k = 1; k < TILE_SUIT_HONORS; k++)
					if (analysis->TileCount[k] > 0) flag[k] = true;
			}
			return (
				(flag[TILE_SUIT_CHARACTERS / TILE_SUIT_STEP] && flag[TILE_SUIT_CIRCLES / TILE_SUIT_STEP] && !flag[TILE_SUIT_BAMBOOS / TILE_SUIT_STEP]) ||
				(flag[TILE_SUIT_CHARACTERS / TILE_SUIT_STEP] && !flag[TILE_SUIT_CIRCLES / TILE_SUIT_STEP] && flag[TILE_SUIT_BAMBOOS / TILE_SUIT_STEP]) ||
				(!flag[TILE_SUIT_CHARACTERS / TILE_SUIT_STEP] && flag[TILE_SUIT_CIRCLES / TILE_SUIT_STEP] && flag[TILE_SUIT_BAMBOOS / TILE_SUIT_STEP])
				);
		};
	std::function<bool(const MENTSU_ANALYSIS* const analysis)> chueiimen1 =
		[chueiimen](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chueiimen(analysis);
		};
	std::function<bool(const MENTSU_ANALYSIS* const analysis)> chueiimen2 =
		[chueiimen](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chueiimen(analysis) &&
				(analysis->TileCount[EastWind] + analysis->TileCount[SouthWind] + analysis->TileCount[WestWind] +
				analysis->TileCount[NorthWind] + analysis->TileCount[WhiteDragon] + analysis->TileCount[GreenDragon] +
				analysis->TileCount[RedDragon] == 0));
		};
	if (RuleData::getRule("chueiimen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			(RuleData::getRule("chueiimen") != 2) ? chueiimen1 : chueiimen2
		));
	/* �l�ʑ^�� */
	if (RuleData::getRule("simian_chuge") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�ʑ^��", yaku::yakuCalculator::Yaku::yval_4han,
			"����",
			[chueiimen2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chueiimen2(analysis) &&
					(analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) &&
					((analysis->MianziDat[1].tile / TILE_SUIT_STEP) ==
					(analysis->MianziDat[2].tile / TILE_SUIT_STEP) ==
					(analysis->MianziDat[3].tile / TILE_SUIT_STEP) ==
					(analysis->MianziDat[4].tile / TILE_SUIT_STEP)) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki)
					);
			}
		));
	/* �ܖ�� */
	auto uumenchii =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int mnzCount[TILE_SUIT_HONORS / TILE_SUIT_STEP + 2] = {0};
			bool yakuFlag = true;
			{
				tileCode tc;
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				for (int i = 0; i < ((analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) ?
					SIZE_OF_MELD_BUFFER : NUM_OF_TILES_IN_HAND); i++) {
						if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) tc = analysis->MianziDat[i].tile;
						else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1) tc = analysis->PlayerStat->Hand[i].tile;
						if (tc < TILE_SUIT_HONORS) ++mnzCount[tc / TILE_SUIT_STEP];
						else switch (tc) {
						case EastWind: case SouthWind: case WestWind: case NorthWind:
							++mnzCount[TILE_SUIT_HONORS / TILE_SUIT_STEP]; break;
						case WhiteDragon: case GreenDragon: case RedDragon:
							++mnzCount[TILE_SUIT_HONORS / TILE_SUIT_STEP + 1]; break;
						}
				}
			}
			for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP + 2); i++)
				if (mnzCount[i] == 0) yakuFlag = false;
			return yakuFlag;
		};
	if (RuleData::getRule("uumenchii") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܖ��", yaku::yakuCalculator::Yaku::yval_2han,
			uumenchii
		));
	/* �����I�����s�b�N */
	if (RuleData::getRule("tokyo_olympic_polychrome") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����I�����s�b�N ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�ܖ��",
			[uumenchii](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (uumenchii(analysis) &&
					(analysis->GameStat->GameRound / 4 == sEast) &&
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))
					);
			}
		));
	/* �����N */
	if (RuleData::getRule("minorage") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����N", (RuleData::getRule("minorage") == 2) ? 
			yaku::yakuCalculator::Yaku::yval_6han : yaku::yakuCalculator::Yaku::yval_3han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
					if (analysis->MianziDat[i].tile >= TILE_SUIT_HONORS)
						return false;
				return ((analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) &&
					(yaku::countingFacility::countMentzNumerals(analysis->MianziDat) < 20));
			}
		));
	/* ���l�� */
	if (RuleData::getRule("comingofage") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���l��", (RuleData::getRule("comingofage") == 2) ? 
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_3han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
					if (analysis->MianziDat[i].tile >= TILE_SUIT_HONORS)
						return false;
				return ((analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) &&
					(yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 20));
			}
		));
}
