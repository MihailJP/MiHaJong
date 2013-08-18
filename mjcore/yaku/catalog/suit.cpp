#include "../catalog.h"

#include "../../func.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_suit() {
	/* �`���C�c */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("����F"), yaku::yakuCalculator::Yaku::yval_24,
		_T("����F"), _T("����"),
#else /* GUOBIAO */
		_T("����F"), get_yaku_han("chiniisoh"),
		_T("����F"), _T("����C�ʊёS�ћ��"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, true);
		}
	));

	// ---------------------------------------------------------------------

	/* �a�������玀�� */
	auto chkHaishiki =
		[](const MENTSU_ANALYSIS* const analysis, const char* const haishiki) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
				if ((analysis->TileCount[i + 1] >= (int)(haishiki[0] - _T('0'))) &&
					(analysis->TileCount[i + 2] >= (int)(haishiki[1] - _T('0'))) &&
					(analysis->TileCount[i + 3] >= (int)(haishiki[2] - _T('0'))) &&
					(analysis->TileCount[i + 4] >= (int)(haishiki[3] - _T('0'))) &&
					(analysis->TileCount[i + 5] >= (int)(haishiki[4] - _T('0'))) &&
					(analysis->TileCount[i + 6] >= (int)(haishiki[5] - _T('0'))) &&
					(analysis->TileCount[i + 7] >= (int)(haishiki[6] - _T('0'))) &&
					(analysis->TileCount[i + 8] >= (int)(haishiki[7] - _T('0'))) &&
					(analysis->TileCount[i + 9] >= (int)(haishiki[8] - _T('0'))) ) yakuFlag = true;
			return yakuFlag;
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("��@��"), yaku::yakuCalculator::Yaku::yval_88,
		_T("����F"), _T("��㍏"), _T("����"),
#else /* GUOBIAO */
		_T("��@��"), yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		_T("����F"), _T("��@�ԓ�"),
#endif /* GUOBIAO */
		[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkHaishiki(analysis, "311111113");
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("double_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������@��"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("��@��"), _T("����F"), _T("��@�ԓ�"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const TileCode* tsumoTile = &(analysis->TsumoHai->tile);
				if ((*tsumoTile % 10 == 1) || (*tsumoTile % 10 == 9)) {
					if (analysis->TileCount[*tsumoTile] >= 4) yakuFlag = true;
				} else {
					if (analysis->TileCount[*tsumoTile] >= 2) yakuFlag = true;
				}
				return (chkHaishiki(analysis, "311111113")) && (!analysis->MachiInfo.FuritenFlag);
			}
		));
#endif /* GUOBIAO */
#ifdef GUOBIAO
	/* �A���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�A����"), yaku::yakuCalculator::Yaku::yval_88,
		_T("����"), _T("����F"), _T("�s���l"), _T("�P����"),
		[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkHaishiki(analysis, "022222220") ||
				chkHaishiki(analysis, "222222200") ||
				chkHaishiki(analysis, "002222222");
		}
	));
#else /* GUOBIAO */
	/* �^���q */
	if (RuleData::chkRuleApplied("daisharin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��ԗ�"), get_yaku_han("daisharin"),
			_T("����F"), _T("��u��"), _T("���Ύq"), _T("���a"), _T("�f���"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CircleFive] >= 2));
			}
		));
	/* ��|�� */
	if (RuleData::chkRuleApplied("daichikurin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��|��"), get_yaku_han("daichikurin"),
			_T("����F"), _T("��u��"), _T("���Ύq"), _T("���a"), _T("�f���"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[BambooFive] >= 2));
			}
		));
	/* �吔�� */
	if (RuleData::chkRuleApplied("daisuurin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�吔��"), get_yaku_han("daisuurin"),
			_T("����F"), _T("��u��"), _T("���Ύq"), _T("���a"), _T("�f���"), _T("���F"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CharacterFive] >= 2));
			}
		));
	/* ���ԗ� */
	if (RuleData::chkRuleApplied("shosharin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ԗ�"), get_yaku_han("shosharin"),
			_T("����F"), _T("��u��"), _T("���Ύq"), _T("���a"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CircleFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* ���|�� */
	if (RuleData::chkRuleApplied("shochikurin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���|��"), get_yaku_han("shochikurin"),
			_T("����F"), _T("��u��"), _T("���Ύq"), _T("���a"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[BambooFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* ������ */
	if (RuleData::chkRuleApplied("shosuurin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������"), get_yaku_han("shosuurin"),
			_T("����F"), _T("��u��"), _T("���Ύq"), _T("���a"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CharacterFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* �~�����a (3.)14159265358979 */
	if (RuleData::chkRuleApplied("pi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�~�����a"), get_yaku_han("pi"),
			_T("����F"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "211131113");
			}
		));

	// ---------------------------------------------------------------------

	/* ��Q�a 1.4142135623730 */
	if (RuleData::chkRuleApplied("square_root_two"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��Q�a"), get_yaku_han("square_root_two"),
			_T("����F"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "323211100");
			}
		));
	/* ���R�ΐ��̒� 2.7182818284590 */
	if (RuleData::chkRuleApplied("natural_logarithm"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���R�ΐ��̒�"), get_yaku_han("natural_logarithm"),
			_T("����F"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "230110141");
			}
		));
	/* �����a (0.)207879576350761 */
	if (RuleData::chkRuleApplied("imaginary_number"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����a"), get_yaku_han("imaginary_number"),
			_T("����F"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "111022411");
			}
		));
	/* ��P�O�a 3.16227766016837 */
	if (RuleData::chkRuleApplied("square_root_ten"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��P�O�a"), get_yaku_han("square_root_ten"),
			_T("����F"),
			[chkHaishiki](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "222004310");
			}
		));

	// ---------------------------------------------------------------------

	/* ���C�� */
	if (RuleData::chkRuleApplied("qinghaihu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���C��"), get_yaku_han("qinghaihu"),
			_T("����F"), _T("���S�ћ��"), _T("��u��"), _T("���a"),
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
	if (RuleData::chkRuleApplied("chuusharin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ԗ�"), get_yaku_han("chuusharin"),
			_T("����F"), _T("��u��"),
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TileSuitCircles);
			}
		));
	/* ���|�� */
	if (RuleData::chkRuleApplied("chuuchikurin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���|��"), get_yaku_han("chuuchikurin"),
			_T("����F"), _T("��u��"),
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TileSuitBamboos);
			}
		));
	/* ������ */
	if (RuleData::chkRuleApplied("chuusuurin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������"), get_yaku_han("chuusuurin"),
			_T("����F"), _T("��u��"),
			[chuusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TileSuitCharacters);
			}
		));

	// ---------------------------------------------------------------------

	/* �S���� */
	auto chiffre =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			return (analysis->shanten[shantenRegular] == -1) ?
				yaku::countingFacility::countMentzNumerals(analysis->MianziDat) :
				yaku::countingFacility::countTileNumerals(analysis->TileCount);
		};
	if (RuleData::chkRuleApplied("kaga_million")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S����"), get_yaku_han("kaga_million"),
			_T("����F"),
			[chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) > 100) &&
					((analysis->TsumoHai->tile / TileSuitStep) ==
					(TileSuitCharacters / TileSuitStep)));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����S����"), RuleData::chkRule("kaga_million", "double_yakuman_if_just_100") ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
			_T("����F"),
			[chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 100) &&
					((analysis->TsumoHai->tile / TileSuitStep) ==
					(TileSuitCharacters / TileSuitStep)));
			}
		));
	}
	/* ���q�S���� */
	if (RuleData::chkRuleApplied("pinzu_million"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���q�S����"), get_yaku_han("pinzu_million"),
			_T("����F"),
			[chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) >= 100) &&
					((analysis->TsumoHai->tile / TileSuitStep) ==
					(TileSuitCircles / TileSuitStep)));
			}
		));
	/* �I�B�܏\�ܖ��� */
	if (RuleData::chkRuleApplied("kishu550k"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�I�B�܏\�ܖ���"), get_yaku_han("kishu550k"),
			_T("����F"),
			[chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 55) &&
					((analysis->TsumoHai->tile / TileSuitStep) ==
					(TileSuitCharacters / TileSuitStep)));
			}
		));
	/* ���ˎO�\�ܖ��� */
	if (RuleData::chkRuleApplied("mito350k"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ˎO�\�ܖ���"), get_yaku_han("mito350k"),
			_T("����F"),
			[chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) <= 35) &&
					((analysis->TsumoHai->tile / TileSuitStep) ==
					(TileSuitCharacters / TileSuitStep)));
			}
		));
	/* �t�����[�� */
	if (RuleData::chkRuleApplied("fullmoon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�t�����[��"), get_yaku_han("fullmoon"),
			_T("����F"),
			[chiffre](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (chiffre(analysis) == 88));
			}
		));

	// ---------------------------------------------------------------------

	/* �����W� */
	if (RuleData::chkRuleApplied("starlight"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����W�"), get_yaku_han("starlight"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, true) && (analysis->KeziCount[CircleSeven]));
			}
		));
	/* ���F */
	if (RuleData::chkRuleApplied("cheniisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���F"), get_yaku_han("cheniisoh"),
			_T("����F"), _T("�f���"),
			/*
				���Ύq�̞��F�͂S���g����F�߂Ȃ�����͂Q�Q�R�R�S�S�T�T�U�U�V�V�W�W�̌`�̂�
				����͑吔�ׂƓ����`�ł���A���F�̂ݍ̗p�̏ꍇ�ł��ʎq��Ƃ��Ĉ�����
			*/
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if (analysis->shanten[shantenRegular] == -1) {
					int yakuFlagCount = 0;
					const TileCode targetKezi[] = {CharacterOne, CharacterNine,};
					const TileCode targetShunzi[] = {CharacterOne, CharacterSeven,};
					if (yaku::countingFacility::countSpecMentz(analysis->MianziDat, targetKezi, 2, targetShunzi, 2, false) == 0)
						yakuFlag = true;
				} else if (analysis->shanten[shantenPairs] == -1) {
					int yakuFlagCount = 0;
					const TileCode targetDuizi[] = {CharacterOne, CharacterNine,};
					if (yaku::countingFacility::countPairs(analysis->TileCount, targetDuizi, 2) == 0)
						yakuFlag = true;
				}
				return (isshoku(analysis, true) && (yakuFlag) &&
					((analysis->TsumoHai->tile / TileSuitStep) ==
					(TileSuitCharacters / TileSuitStep)));
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �z���C�c */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("����F"), yaku::yakuCalculator::Yaku::yval_6,
#else /* GUOBIAO */
		_T("����F"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		_T("��C�ʊёS�ћ��"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, false);
		}
	));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* �O�����ݎq�z���C�c */
	if ((RuleData::chkRuleApplied("characters_mahjong")) && GameStat.chkGameType(SanmaT))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ݘa"), get_yaku_han("characters_mahjong"),
			_T("����F"), _T("���V��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					((analysis->TileCount[CharacterOne] >= 1)||(analysis->TileCount[CharacterNine] >= 1)));
			}
		));
	/* �����I�����s�b�N(�z���C�c) */
	if (RuleData::chkRuleApplied("tokyo_olympic_monochrome"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����I�����s�b�N"), get_yaku_han("tokyo_olympic_monochrome"),
			_T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					(analysis->TileCount[CircleFive] >= 1) && (analysis->TileCount[EastWind] >= 1));
			}
		));
	/* ������B */
	if (RuleData::chkRuleApplied("saigou_nanshuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������B"), get_yaku_han("saigou_nanshuu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isshoku(analysis, false) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(analysis->TotalKezi == SizeOfMeldBuffer - 1));
			}
		));
	/* ������������ */
	if (RuleData::chkRuleApplied("kagamine_rinlen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������������"), get_yaku_han("kagamine_rinlen"),
			_T("����F"), _T("��������"), _T("��������"), _T("�O�A��"), _T("�΁X�a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[i + 1] >= 1) && (analysis->KeziCount[i + 2] >= 1))
						yakuFlag = true;
				return (isshoku(analysis, false) && yakuFlag &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					((analysis->MianziDat[0].tile / TileSuitStep) == (TileSuitCircles / TileSuitStep)));
			}
		));
	/* �������� */
	if (RuleData::chkRuleApplied("kagamine_rin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��������"), get_yaku_han("kagamine_rin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int PinMian = 0;
				for (int i = 0; i < SizeOfMeldBuffer; i++)
					if ((analysis->MianziDat[i].tile / TileSuitStep) == (TileSuitCircles / TileSuitStep))
						++PinMian;
				return (isshoku(analysis, false) && (PinMian > 0) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* �|���T����n */
	if (RuleData::chkRuleApplied("honmanopotechi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�|���T����n"), get_yaku_han("honmanopotechi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int PinMian = 0;
				if (analysis->shanten[shantenRegular] == -1) {
					for (int i = 0; i < SizeOfMeldBuffer; i++)
						if ((analysis->MianziDat[i].tile / TileSuitStep) == (TileSuitCircles / TileSuitStep))
							++PinMian;
					return (isshoku(analysis, false) &&
						(PinMian == SizeOfMeldBuffer - 1) &&
						(analysis->DuiziCount[GreenDragon] >= 1));
				} else if (analysis->shanten[shantenPairs] == -1) {
					for (int i = (int)CircleOne; i <= (int)CircleNine; i++)
						PinMian += analysis->TileCount[i];
					return (isshoku(analysis, false) &&
						(PinMian == NumOfTilesInHand - 2) &&
						(analysis->TileCount[GreenDragon] >= 2));
				} else return false;
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* ���� */
	auto chueiimen =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool flag[TileSuitHonors / TileSuitStep] = {false};
			if (analysis->shanten[shantenRegular] == -1) {
				for (int k = 0; k < SizeOfMeldBuffer; k++)
					if (analysis->MianziDat[k].tile < TileSuitHonors)
						flag[analysis->MianziDat[k].tile / TileSuitStep] = true;
			} else if (analysis->shanten[shantenPairs] == -1) {
				for (int k = 1; k < TileSuitHonors; k++)
					if (analysis->TileCount[k] > 0) flag[k / TileSuitStep] = true;
			}
			return (
				(flag[TileSuitCharacters / TileSuitStep] && flag[TileSuitCircles / TileSuitStep] && !flag[TileSuitBamboos / TileSuitStep]) ||
				(flag[TileSuitCharacters / TileSuitStep] && !flag[TileSuitCircles / TileSuitStep] && flag[TileSuitBamboos / TileSuitStep]) ||
				(!flag[TileSuitCharacters / TileSuitStep] && flag[TileSuitCircles / TileSuitStep] && flag[TileSuitBamboos / TileSuitStep])
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
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�����"), yaku::yakuCalculator::Yaku::yval_1,
		chueiimen1
	));
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("chueiimen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����"), yaku::yakuCalculator::Yaku::yval_1han_menzen,
			RuleData::chkRule("chueiimen", "1han_nohonors") ? chueiimen1 : chueiimen2
		));
	/* �l�ʑ^�� */
	if (RuleData::chkRuleApplied("simian_chuge"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�ʑ^��"), get_yaku_han("simian_chuge"),
			_T("����"),
			[chueiimen2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chueiimen2(analysis) &&
					(analysis->shanten[shantenRegular] == -1) &&
					((analysis->MianziDat[1].tile / TileSuitStep) ==
					(analysis->MianziDat[2].tile / TileSuitStep)) &&
					((analysis->MianziDat[2].tile / TileSuitStep) ==
					(analysis->MianziDat[3].tile / TileSuitStep)) &&
					((analysis->MianziDat[3].tile / TileSuitStep) ==
					(analysis->MianziDat[4].tile / TileSuitStep)) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki)
					);
			}
		));
#endif /* GUOBIAO */
	/* �ܖ�� */
	auto uumenchii =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int mnzCount[TileSuitHonors / TileSuitStep + 2] = {0};
			bool yakuFlag = true;
			{
				TileCode tc;
				if (analysis->shanten[shantenRegular] == -1)
				for (int i = 0; i < ((analysis->shanten[shantenRegular] == -1) ?
					SizeOfMeldBuffer : NumOfTilesInHand); i++) {
						if (analysis->shanten[shantenRegular] == -1) tc = analysis->MianziDat[i].tile;
						else if (analysis->shanten[shantenPairs] == -1) tc = analysis->PlayerStat->Hand[i].tile;
						if (tc < TileSuitHonors) ++mnzCount[tc / TileSuitStep];
						else switch (tc) {
						case EastWind: case SouthWind: case WestWind: case NorthWind:
							++mnzCount[TileSuitHonors / TileSuitStep]; break;
						case WhiteDragon: case GreenDragon: case RedDragon:
							++mnzCount[TileSuitHonors / TileSuitStep + 1]; break;
						}
				}
			}
			for (int i = 0; i < (TileSuitHonors / TileSuitStep + 2); i++)
				if (mnzCount[i] == 0) yakuFlag = false;
			return yakuFlag;
		};
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�ܖ��"), yaku::yakuCalculator::Yaku::yval_6,
		uumenchii
	));
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("uumenchii"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ܖ��"), get_yaku_han("uumenchii"),
			uumenchii
		));
	/* �����I�����s�b�N */
	if (RuleData::chkRuleApplied("tokyo_olympic_polychrome"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����I�����s�b�N "), get_yaku_han("tokyo_olympic_polychrome"),
			_T("�ܖ��"),
			[uumenchii](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (uumenchii(analysis) &&
					(analysis->GameStat->GameRound / 4 == sEast) &&
					(analysis->GameStat->playerwind(analysis->player) == sEast) &&
					(analysis->KeziCount[EastWind] >= 1)
					);
			}
		));
	/* �����N */
	if (RuleData::chkRuleApplied("minorage"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����N"), get_yaku_han("minorage"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < SizeOfMeldBuffer; i++)
					if (analysis->MianziDat[i].tile >= TileSuitHonors)
						return false;
				return ((analysis->shanten[shantenRegular] == -1) &&
					(yaku::countingFacility::countMentzNumerals(analysis->MianziDat) < 20));
			}
		));
	/* ���l�� */
	if (RuleData::chkRuleApplied("comingofage"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���l��"), get_yaku_han("comingofage"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < SizeOfMeldBuffer; i++)
					if (analysis->MianziDat[i].tile >= TileSuitHonors)
						return false;
				return ((analysis->shanten[shantenRegular] == -1) &&
					(yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 20));
			}
		));
#endif /* GUOBIAO */
}
