#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* �`�[�g�C */
	auto isQiDui =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)&&(analysis->shanten[ShantenAnalyzer::shantenRegular] >= 0));
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("���Ύq"), get_yaku_han("seven_pairs"), /* 1�|50���̃��[����2�|25���̃��[��������B���͂����ł͐ݒ�ł��Ȃ��ł��c�c */
		isQiDui
	));

	auto chktiles = // ����֐��I�u�W�F�N�g
		[isQiDui](const MENTSU_ANALYSIS* const analysis, const tileCode* const targetDuiz, int numOfDuiz) -> bool {
			if (isQiDui(analysis))
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetDuiz, numOfDuiz)
					== NUM_OF_TILES_IN_HAND / 2);
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
					return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NUM_OF_TILES_IN_HAND / 2);
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
				const tileCode tc[15] = {
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
				const tileCode tc[12] = {
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
				const tileCode tc[7] = {
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
				const tileCode tc[7] = {
					CharacterOne, CircleOne, BambooOne,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
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
					if ((analysis->TileCount[TILE_SUIT_CHARACTERS + i] >= 2) &&
						(analysis->TileCount[TILE_SUIT_CIRCLES + i] >= 2) &&
						(analysis->TileCount[TILE_SUIT_BAMBOOS + i] >= 2))
						yakuFlag = true;
				return isQiDui(analysis) && yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	{
		auto gyakusharin =
			[isQiDui](const MENTSU_ANALYSIS* const analysis, int suit) -> bool {
				std::array<tileCode, 7> honors = {
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
					return gyakusharin(analysis, TILE_SUIT_CIRCLES);
				}
			));
		/* �t�|�� */
		if (RuleData::chkRuleApplied("gyakuchikurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�t�|��"), get_yaku_han("gyakuchikurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_BAMBOOS);
				}
			));
		/* �t���� */
		if (RuleData::chkRuleApplied("gyakusuurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�t����"), get_yaku_han("gyakusuurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_CHARACTERS);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ���m */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("���m���o"), yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		_T("�\�O�s��"), _T("�ܖ��"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[ShantenAnalyzer::shantenOrphans] == -1);
		}
	));
	if (RuleData::chkRuleApplied("double_yakuman")) /* ���m13�ʑ҂��i�_�u���𖞁j */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���m���o�\�O��"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("���m���o"), _T("�\�O�s��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenOrphans] == -1) && // ���m���o�ɂȂ��Ă��āA
					(!analysis->MachiInfo.FuritenFlag) && // �t���e���ł͂Ȃ��āA
					((analysis->MachiInfo.MachiMen == 13)||(analysis->GameStat->TianHuFlag))); // 13�ʑ҂����V�a�ɂȂ��Ă���
			}
		));
	/* �������� */
	if (RuleData::chkRuleApplied("stellar_uushii"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��������"), get_yaku_han("stellar_uushii"),
			_T("�S�s��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenStellar] == -1);
			}
		));
	/* ��k�푈 */
	if (RuleData::chkRuleApplied("civil_war"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��k�푈"), get_yaku_han("civil_war"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenCivilWar] == -1);
			}
		));
	/* �f������ */
	if (RuleData::chkRuleApplied("syzygy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�f������"), get_yaku_han("syzygy"),
			_T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenSyzygy] == -1);
			}
		));
	/* �S�s�� */
	if (RuleData::chkRuleApplied("quanbukao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S�s��"), get_yaku_han("quanbukao"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenQuanbukao] == -1);
			}
		));
	/* �Z�u���A�b�v */
	if (RuleData::chkRuleApplied("sevenup"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Z�u���A�b�v"), get_yaku_han("sevenup"),
			_T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenSevenup] == -1);
			}
		));
	/* �g���� */
	if (RuleData::chkRuleApplied("zuhelong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�g����", get_yaku_han("zuhelong"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenZuhelong] == -1);
			}
		));
}
