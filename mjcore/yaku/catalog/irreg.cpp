#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* �`�[�g�C */
	auto isQiDui =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���Ύq", (getRule(RULE_SEVEN_PAIRS) == 1) ? /* 1�|50���̃��[����2�|25���̃��[��������B���͂����ł͐ݒ�ł��Ȃ��ł��c�c */
		yaku::yakuCalculator::Yaku::yval_1han_menzen : yaku::yakuCalculator::Yaku::yval_2han_menzen,
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

	/* �c�[�C�[�\�[ */
	if (getRule(RULE_SEVEN_HONOR_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"���V��", "����F", "�O�����Ύq", "�l�쎵�Ύq", "�g������",
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (isQiDui(analysis))
					return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NUM_OF_TILES_IN_HAND / 2);
				else return false;
			}
		));
	/* �O�����Ύq */
	if (getRule(RULE_DRAGON_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�����Ύq", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[GreenDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* �l�쎵�Ύq */
	if (getRule(RULE_WIND_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�쎵�Ύq", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[EastWind] >= 2) &&
					(analysis->TileCount[SouthWind] >= 2) &&
					(analysis->TileCount[WestWind] >= 2) &&
					(analysis->TileCount[NorthWind] >= 2));
			}
		));
	/* ����Ύq */
	if (getRule(RULE_ODD_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����Ύq", yaku::yakuCalculator::Yaku::yval_1han_menzen,
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
	if (getRule(RULE_EVEN_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������Ύq", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			"�f���",
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
	if (getRule(RULE_KOUHAKU_CHIITOI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�g������", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* �k�̑�n */
	if (getRule(RULE_KITA_NO_DAICHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�̑�n", yaku::yakuCalculator::Yaku::HANFUNC(
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
			"����F",
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode tc[7] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					NorthWind, WhiteDragon,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
			}
		));
	/* ���E�� */
	if (getRule(RULE_SEKAIICHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���E��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"�l�쎵�Ύq", "���V��",
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
	if (getRule(RULE_SANSHOKU_DOUTOI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F����", yaku::yakuCalculator::Yaku::yval_1han_menzen,
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
		if (getRule(RULE_GYAKUSHARIN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�t�ԗ�", yaku::yakuCalculator::Yaku::yval_4han_menzen,
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_CIRCLES);
				}
			));
		/* �t�|�� */
		if (getRule(RULE_GYAKUCHIKURIN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�t�|��", yaku::yakuCalculator::Yaku::yval_4han_menzen,
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_BAMBOOS);
				}
			));
		/* �t���� */
		if (getRule(RULE_GYAKUSUURIN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�t����", yaku::yakuCalculator::Yaku::yval_4han_menzen,
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_CHARACTERS);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ���m */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���m���o", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"�\�O�s��",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0) /* ���m13�ʑ҂��i�_�u���𖞁j */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���m���o�\�O��", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"���m���o", "�\�O�s��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // ���m���o�ɂȂ��Ă��āA
					(/* �t���e�����ǂ������肷�� */0) && // �t���e���ł͂Ȃ��āA
					((/* 13�ʑ҂������肷�� */0)||(analysis->GameStat->TianHuFlag))); // 13�ʑ҂����V�a�ɂȂ��Ă���
			}
		));
	/* �������� */
	if (getRule(RULE_STELLAR_UUSHII) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", (getRule(RULE_STELLAR_UUSHII) == 2) ?
			yaku::yakuCalculator::Yaku::yval_baiman_menzen : yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"�S�s��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenStellar] == -1);
			}
		));
	/* ��k�푈 */
	if (getRule(RULE_CIVIL_WAR) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��k�푈", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenCivilWar] == -1);
			}
		));
	/* �f������ */
	if (getRule(RULE_SYZYGY) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�f������", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenSyzygy] == -1);
			}
		));
	/* �S�s�� */
	if (getRule(RULE_QUANBUKAO) != 0) {
		const yaku::yakuCalculator::Yaku::MenzenHan* qbk_han = NULL;
		switch (getRule(RULE_QUANBUKAO)) {
		case 1: case 2:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_3han_menzen;
			break;
		case 3:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_4han_menzen;
			break;
		case 4:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_mangan_menzen;
			break;
		case 5:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_yakuman_menzen;
			break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�S�s��", *qbk_han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenQuanbukao] == -1);
			}
		));
	}
}
