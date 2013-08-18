#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* �`�[�g�C */
	auto isQiDui =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("���Ύq"), yaku::yakuCalculator::Yaku::yval_24,
		_T("��O��"), _T("�P����"),
#else /* GUOBIAO */
		_T("���Ύq"), get_yaku_han("seven_pairs"), /* 1�|50���̃��[����2�|25���̃��[��������B���͂����ł͐ݒ�ł��Ȃ��ł��c�c */
#endif /* GUOBIAO */
		isQiDui
	));

#ifndef GUOBIAO
	auto chktiles = // ����֐��I�u�W�F�N�g
		[isQiDui](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetDuiz, int numOfDuiz) -> bool {
			if (isQiDui(analysis))
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetDuiz, numOfDuiz)
					== (numOfDuiz > (NumOfTilesInHand / 2) ? (NumOfTilesInHand / 2) : numOfDuiz));
			else return false;
		};
	// ---------------------------------------------------------------------

	/* ������ */
	if (RuleData::chkRuleApplied("seven_honor_pairs") && !RuleData::chkRule("seven_honor_pairs", "yes"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������"), get_yaku_han("seven_honor_pairs"),
			_T("���V��"), _T("����F"), _T("�O�����Ύq"), _T("�l�쎵�Ύq"), _T("�g������"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (isQiDui(analysis))
					return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NumOfTilesInHand / 2);
				else return false;
			}
		));
	/* �O�����Ύq */
	if (RuleData::chkRuleApplied("dragon_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�����Ύq"), get_yaku_han("dragon_pairs"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[GreenDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* �l�쎵�Ύq */
	if (RuleData::chkRuleApplied("wind_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�쎵�Ύq"), get_yaku_han("wind_pairs"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[EastWind] >= 2) &&
					(analysis->TileCount[SouthWind] >= 2) &&
					(analysis->TileCount[WestWind] >= 2) &&
					(analysis->TileCount[NorthWind] >= 2));
			}
		));
	/* ����Ύq */
	if (RuleData::chkRuleApplied("odd_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����Ύq"), get_yaku_han("odd_pairs"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[15] = {
					CharacterOne, CharacterThree, CharacterFive, CharacterSeven, CharacterNine,
					CircleOne, CircleThree, CircleFive, CircleSeven, CircleNine,
					BambooOne, BambooThree, BambooFive, BambooSeven, BambooNine,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 15);
			}
		));
	/* �������Ύq */
	if (RuleData::chkRuleApplied("even_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�������Ύq"), get_yaku_han("even_pairs"),
			_T("�f���"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[12] = {
					CharacterTwo, CharacterFour, CharacterSix, CharacterEight,
					CircleTwo, CircleFour, CircleSix, CircleEight,
					BambooTwo, BambooFour, BambooSix, BambooEight,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 12);
			}
		));

	// ---------------------------------------------------------------------

	/* �g������ */
	if (RuleData::chkRuleApplied("kouhaku_chiitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�g������"), get_yaku_han("kouhaku_chiitoi"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* �k�̑�n */
	if (RuleData::chkRuleApplied("kita_no_daichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�k�̑�n"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
				switch (analysis->TsumoHai->tile) { /* ���Řa���������ŕ��� */
				case NorthWind: /* �k�Řa����ƃ_�u���� */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman);
				case WhiteDragon: /* ���Řa����ƃg���v���� */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman);
				default:
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
				}
			}),
			_T("����F"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[7] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					NorthWind, WhiteDragon,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
			}
		));
	/* ���E�� */
	if (RuleData::chkRuleApplied("sekaiichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���E��"), get_yaku_han("sekaiichi"),
			_T("�l�쎵�Ύq"), _T("���V��"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[7] = {
					CharacterOne, CircleOne, BambooOne,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
			}
		));
	/* �j�R�j�R���Ύq */
	if (RuleData::chkRuleApplied("nikoniko_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�j�R�j�R���Ύq"), get_yaku_han("nikoniko_pairs"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[6] = {
					CharacterTwo, CharacterFive,
					CircleTwo, CircleFive, BambooTwo, BambooFive,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 6);
			}
		));
	/* ��x���񎵑Ύq */
	if (RuleData::chkRuleApplied("ikkyuusan_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��x���񎵑Ύq"), get_yaku_han("ikkyuusan_pairs"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode tc[9] = {
					CharacterOne, CharacterThree, CharacterNine,
					CircleOne, CircleThree, CircleNine,
					BambooOne, BambooThree, BambooNine,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 9);
			}
		));

	// ---------------------------------------------------------------------

	/* �O�F���� */
	if (RuleData::chkRuleApplied("sanshoku_doutoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F����"), get_yaku_han("sanshoku_doutoi"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->TileCount[TileSuitCharacters + i] >= 2) &&
						(analysis->TileCount[TileSuitCircles + i] >= 2) &&
						(analysis->TileCount[TileSuitBamboos + i] >= 2))
						yakuFlag = true;
				return isQiDui(analysis) && yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	{
		auto gyakusharin =
			[isQiDui](const MENTSU_ANALYSIS* const analysis, int suit) -> bool {
				std::array<TileCode, 7> honors = {
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon, RedDragon,
				}; int count = 0;
				for (auto k = honors.begin(); k != honors.end(); k++)
					if (analysis->TileCount[*k] >= 2) ++count;
				return isQiDui(analysis) &&
					(analysis->TileCount[suit + 1] >= 2) &&
					(analysis->TileCount[suit + 9] >= 2) &&
					(count == 5);
			};
		/* �t�ԗ� */
		if (RuleData::chkRuleApplied("gyakusharin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�t�ԗ�"), get_yaku_han("gyakusharin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TileSuitCircles);
				}
			));
		/* �t�|�� */
		if (RuleData::chkRuleApplied("gyakuchikurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�t�|��"), get_yaku_han("gyakuchikurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TileSuitBamboos);
				}
			));
		/* �t���� */
		if (RuleData::chkRuleApplied("gyakusuurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�t����"), get_yaku_han("gyakusuurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TileSuitCharacters);
				}
			));
	}
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* ���m */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("���m���o"), yaku::yakuCalculator::Yaku::yval_88,
		_T("�ܖ��"), _T("��O��"), _T("�P����"),
#else /* GUOBIAO */
		_T("���m���o"), yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		_T("�\�O�s��"), _T("�ܖ��"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("double_yakuman")) /* ���m13�ʑ҂��i�_�u���𖞁j */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���m���o�\�O��"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("���m���o"), _T("�\�O�s��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // ���m���o�ɂȂ��Ă��āA
					(!analysis->MachiInfo.FuritenFlag) && // �t���e���ł͂Ȃ��āA
					((analysis->MachiInfo.MachiMen == 13)||(analysis->GameStat->TianHuFlag))); // 13�ʑ҂����V�a�ɂȂ��Ă���
			}
		));
	/* �������� */
	if (RuleData::chkRuleApplied("stellar_uushii"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("�����s��"), yaku::yakuCalculator::Yaku::yval_24,
			_T("�S�s��"), _T("�ܖ��"), _T("��O��"), _T("�P����"),
#else /* GUOBIAO */
			_T("��������"), get_yaku_han("stellar_uushii"),
			_T("�S�s��"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenStellar] == -1);
			}
		));
#ifndef GUOBIAO
	/* ��k�푈 */
	if (RuleData::chkRuleApplied("civil_war"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��k�푈"), get_yaku_han("civil_war"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenCivilWar] == -1);
			}
		));
	/* ���k�V�����O���[���� */
	if (RuleData::chkRuleApplied("tohoku_shinkansen_green"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���k�V�����O���[����"), get_yaku_han("tohoku_shinkansen_green"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenTohokuGreen] == -1);
			}
		));
	/* �f������ */
	if (RuleData::chkRuleApplied("syzygy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�f������"), get_yaku_han("syzygy"),
			_T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenSyzygy] == -1);
			}
		));
	/* �S�s�� */
	if (RuleData::chkRuleApplied("quanbukao"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("�S�s��"), yaku::yakuCalculator::Yaku::yval_12,
			_T("�ܖ��"), _T("��O��"), _T("�P����"), _T("�����s��"),
#else /* GUOBIAO */
			_T("�S�s��"), get_yaku_han("quanbukao"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenQuanbukao] == -1);
			}
		));
#ifndef GUOBIAO
	/* �Z�u���A�b�v */
	if (RuleData::chkRuleApplied("sevenup"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Z�u���A�b�v"), get_yaku_han("sevenup"),
			_T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenSevenup] == -1);
			}
		));
	/* �g���� */
	if (RuleData::chkRuleApplied("zuhelong"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("�g����"), yaku::yakuCalculator::Yaku::yval_12,
#else /* GUOBIAO */
			_T("�g����"), get_yaku_han("zuhelong"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenZuhelong] == -1);
			}
		));
#ifndef GUOBIAO
	/* �m�a�� */
	if (RuleData::chkRuleApplied("ninnaji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�m�a��"), get_yaku_han("ninnaji"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenNinnaji] == -1);
			}
		));
#endif /* GUOBIAO */
}
