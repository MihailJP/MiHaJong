#include "../catalog.h"

extern const std::array<char[4], 6> parsedat_trichrome3;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_2() {
	/* ��΁X�a */
	if (RuleData::getRule("odd_toitoi") != 0)
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
	if (RuleData::getRule("even_toitoi") != 0)
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
	if (RuleData::getRule("chuugen_chikuroku") != 0)
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
	if (RuleData::getRule("turtle_egg") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���T�z�`", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) );
			}
		));
	/* �������� */
	if (RuleData::getRule("kyokujitsu_toushou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) );
			}
		));
	/* �������R */
	if (RuleData::getRule("riirao_shiishan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������R", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* �h�]�k�l */
	if (RuleData::getRule("youbou_hokuto") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�]�k�l", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) );
			}
		));
	/* �k�l���� */
	if (RuleData::getRule("big_dipper") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�l����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�r�b�N�{�[�i�X", "�O�F����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) );
			}
		));
	/* �����Ҍ� */
	if (RuleData::getRule("seisou_taigetsu") != 0)
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
	if (RuleData::getRule("namu_hachiman") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�얳����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E��", "�씒������",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CharacterEight] >= 1) );
			}
		));
	/* ���Ώ\�i */
	if (RuleData::getRule("seiko_jikkei") != 0)
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
	if (RuleData::getRule("volcanic_erruption") != 0)
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
	if (RuleData::getRule("tanfon_chouyou") != 0)
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
	if (RuleData::getRule("haouben") == 1)
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
	else if (RuleData::getRule("haouben") == 2)
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
	if (RuleData::getRule("ryuukanchan") == 2)
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

	// ---------------------------------------------------------------------

	/* �F���� */
	if (RuleData::getRule("queqiaohui") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�F����", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->KeziCount[BambooOne] >= 1) {
					if ((analysis->KeziCount[CharacterSeven] >= 1) &&
						(analysis->KeziCount[CircleSeven] >= 1)) return true;
					else if ((analysis->KeziCount[CharacterSeven] >= 1) &&
						(analysis->KeziCount[BambooSeven] >= 1)) return true;
					else if ((analysis->KeziCount[CircleSeven] >= 1) &&
						(analysis->KeziCount[BambooSeven] >= 1)) return true;
					else return false;
				}
				else return false;
			}
		));
	/* ��ƕ��� */
	if (RuleData::getRule("yigan_fengyue") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��ƕ���", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					((analysis->KeziCount[EastWind] >= 1) || (analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) || (analysis->KeziCount[NorthWind] >= 1) ));
			}
		));
	/* ��̎鐝 */
	if (RuleData::getRule("vermilion_bird") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��̎鐝", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "����F",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[BambooOne] >= 1) &&
					((analysis->KeziCount[BambooFive] >= 1 && analysis->KeziCount[BambooSeven] >= 1) ||
					(analysis->KeziCount[BambooFive] >= 1 && analysis->KeziCount[BambooNine] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1 && analysis->KeziCount[BambooNine] >= 1)) &&
					(analysis->MianziDat[0].tile == RedDragon));
			}
		));
	/* ���̔��� */
	if (RuleData::getRule("white_tiger") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���̔���", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "�O�F����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[CharacterThree] >= 1) &&
					(analysis->KangziCount[CircleThree] >= 1) &&
					(analysis->KangziCount[BambooThree] >= 1) &&
					(analysis->MianziDat[0].tile == WhiteDragon));
			}
		));
	/* �O�t�����h�~�� */
	if (RuleData::getRule("sanbei_meijiu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�t�����h�~��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					((analysis->KeziCount[CharacterNine] >= 1) ||
					(analysis->KeziCount[CircleNine] >= 1) ||
					(analysis->KeziCount[BambooNine] >= 1) ));
			}
		));
	/* �������|���� */
	if (RuleData::getRule("dongfeng_yadao") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������|����", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->KangziCount[EastWind] >= 1) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han);
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (((analysis->KangziCount[EastWind] == 0) && // �����q�E���Ύq
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->MianziDat[0].tile == WestWind)) ||
					((analysis->KangziCount[EastWind] >= 1) && // ���Ȏq�E�����q
					(analysis->KangziCount[WestWind] == 0) &&
					(analysis->KeziCount[WestWind] >= 1)));
			}
		));
	/* �����ދ��T */
	if (RuleData::getRule("taikou_turtle") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����ދ��T", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->TsumoHai->tile == BambooThree) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki));
			}
		));
	/* ���g���l�� */
	if (RuleData::getRule("seikan_tunnel") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���g���l��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E�", "��ᢂ�����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[EastWind] == 0) &&
					(analysis->KeziCount[CircleOne] >= 1));
			}
		));
	/* ��g�J�� */
	if (RuleData::getRule("baohongkaihua") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��g�J��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "��v�E��", "��v�E��", "���V��", "�O�F������", "����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CharacterOne] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1) &&
					(analysis->DuiziCount[BambooOne] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* ��ᢂ����� */
	if (RuleData::getRule("east_green") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��ᢂ�����", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* �씒������ */
	if (RuleData::getRule("south_white") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�씒������", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* �����N */
	if (RuleData::getRule("west_red") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����N", (RuleData::getRule("west_red") == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* ���� */
	if (RuleData::getRule("hakucho") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_5han,
			"�΁X�a", "��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1));
			}
		));
	/* �ΎR����(�܍��Ǝ���) */
	if (RuleData::getRule("volcanic_toitoi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ΎR���� ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "��v�E��", "��v�E�", "����F",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[BambooThree] >= 1) &&
					(analysis->DuiziCount[BambooFive] >= 1) &&
					(analysis->DuiziCount[BambooSeven] >= 1) &&
					(analysis->DuiziCount[GreenDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* ����F�̒�`������misc.cpp�Ɉړ����܂��� */

	/* ��̋�t */
	if (RuleData::getRule("snow_ginkaku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��̋�t", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "��v�E��", "����F", "�f�g�a", "���f�g",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* �k���� */
	if (RuleData::getRule("kitashirakawa") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[BambooSix] >= 1));
			}
		));
	/* �f���𖞂̒�`������misc.cpp�Ɉړ����܂��� */

	/* �A���� */
	if (RuleData::getRule("alcoholism") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�A����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "��v�E��", "�O�F����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CharacterTwo] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[BambooTwo] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* �~�}�� */
	if (RuleData::getRule("kyukyusha") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�~�}��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "��v�E��", "�O�F����", "���V��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* ����� */
	if (RuleData::getRule("tounansha") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1));
			}
		));
	/* �t�ɉ����Ȃ� */
	if (RuleData::getRule("seishun_kuinashi") != 0) {
		auto seishunnikuinashi =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1));
			};
		if (RuleData::getRule("seishun_kuinashi") == 1) {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�t�ɉ����Ȃ�", yaku::yakuCalculator::Yaku::yval_1han_menzen,
				seishunnikuinashi
			));
		} else {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�t�ɉ����Ȃ�", yaku::yakuCalculator::Yaku::yval_2han_menzen,
				"�t�ɉ�������",
				seishunnikuinashi
			));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�t�ɉ�������", yaku::yakuCalculator::Yaku::yval_1han,
				seishunnikuinashi
			));
		}
	}
	/* �X�[�p�[�}�� */
	if (RuleData::getRule("superman") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�X�[�p�[�}��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if ((analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CharacterEight] >= 1)) return true;
				else if ((analysis->MianziDat[0].tile == CharacterEight) &&
					(analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->TsumoHai->tile == CharacterFour)) return true;
				else if ((analysis->MianziDat[0].tile == CharacterFour) &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->TsumoHai->tile == CharacterEight)) return true;
				else return false;
			}
		));
	/* ���ԉ� */
	if (RuleData::getRule("higashi_hanazono_triplets") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԉ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					((analysis->KeziCount[CharacterEight] >= 1) || (analysis->KeziCount[CircleEight] >= 1) ||
					(analysis->KeziCount[BambooEight] >= 1)) &&
					((analysis->KeziCount[CharacterSeven] >= 1) || (analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1)));
			}
		));
	/* �ԗ��� */
	if (RuleData::getRule("rinryanshii") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ԗ���", yaku::yakuCalculator::Yaku::yval_6han,
			/* �΁X�a���K������ */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				char pattern[] = "121314151617181923242526272829343536373839454647484956575859676869787989";
				tileCode tCode[TILE_SUIT_HONORS / TILE_SUIT_STEP * 2];
				bool yakuFlag = false;
				for (int i = 0; i < 36; i++) {
					int num1 = (int)(pattern[i * 2] - '0');
					int num2 = (int)(pattern[i * 2 + 1] - '0');
					for (int k = 0; k < TILE_SUIT_HONORS / TILE_SUIT_STEP * 2; k += 2) {
						tCode[k] = (tileCode)(k / 2 * TILE_SUIT_HONORS + num1);
						tCode[k + 1] = (tileCode)(k / 2 * TILE_SUIT_HONORS + num2);
					}
					if (yaku::countingFacility::countSpecMentz(
						analysis->MianziDat, tCode, TILE_SUIT_HONORS / TILE_SUIT_STEP * 2, NULL, 0, false))
						yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* �R���ԑ҂��Ă�� */
	auto anysuit3 =
		[](const MENTSU_ANALYSIS* const analysis, int val1, int val2, int val3, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (int suit1 = 0; suit1 < TILE_SUIT_HONORS; suit1+= TILE_SUIT_STEP)
				for (int suit2 = 0; suit2 < TILE_SUIT_HONORS; suit2 += TILE_SUIT_STEP)
					for (int suit3 = 0; suit3 < TILE_SUIT_HONORS; suit3 += TILE_SUIT_STEP)
						if (((*count)[suit1 + val1] >= 1) && ((*count)[suit2 + val2] >= 1) &&
							((*count)[suit3 + val3] >= 1)) yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::getRule("colonel_muska") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���X�J�卲", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 6, 4, 3, true) && (analysis->Machi == yaku::yakuCalculator::machiKanchan);
			}
		));
	/* �s�̐��k */
	if (RuleData::getRule("miyakonoseihoku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�s�̐��k", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a",
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 5, false) &&
					(analysis->DuiziCount[WestWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1);
			}
		));
	/* ������\�O�� */
	if (RuleData::getRule("tokyo") != 0) {
#include "isshoku.h" /* ��Ԃꂩ�Ԃ�B�s�V�̈���include */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������\�O��",  yaku::yakuCalculator::Yaku::HANFUNC(
			[isshoku](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(isshoku(analysis, false)) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"�΁X�a",
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 2, 3, 9, true) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	}
	/* �����~�N */
	auto anysuit2 =
		[](const MENTSU_ANALYSIS* const analysis, int val1, int val2, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (int suit1 = 0; suit1 < TILE_SUIT_HONORS; suit1+= TILE_SUIT_STEP)
				for (int suit2 = 0; suit2 < TILE_SUIT_HONORS; suit2 += TILE_SUIT_STEP)
					if (((*count)[suit1 + val1] >= 1) && ((*count)[suit2 + val2] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::getRule("hatsune_miku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����~�N", yaku::yakuCalculator::Yaku::yval_2han,
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 3, 9, true) && (analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* ���� */
	if (RuleData::getRule("baihua") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_1han,
			/* �ʓr����1�� */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));
	/* ���ۜ��� */
	if (RuleData::getRule("jigu_ecao") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ۜ���", yaku::yakuCalculator::Yaku::yval_2han,
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* �r�b�N�{�[�i�X */
	if (RuleData::getRule("777") != 0) {
		auto yaku777 =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1);
			};
		if (RuleData::getRule("777") == 2)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�r�b�N�{�[�i�X", yaku::yakuCalculator::Yaku::yval_yakuman,
				"�O�F����",
				yaku777
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�r�b�N�{�[�i�X", yaku::yakuCalculator::Yaku::yval_2han,
				yaku777
			));
	}
	/* �N���X�}�X */
	if (RuleData::getRule("christmas") != 0) {
		auto chris =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < 6; i++)
					if ((analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 0] + 1] >= 1) &&
						(analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 0] + 2] >= 1) &&
						(analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 1] + 2] >= 1) &&
						(analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 1] + 5] >= 1))
						return true;
				return false;
			};
		auto chrisday = 
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 12) && (nowTime.wDay == 25);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�N���X�}�X", yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"�΁X�a",
			chris
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����N���X�}�X", yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman);
			}),
			"�΁X�a", "�N���X�}�X",
			[chris](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chris(analysis) &&
					(analysis->MianziDat[0].tile == BambooSeven);
			}
		));
	}
	/* ��x�m���O�֎q */
	if (RuleData::getRule("newyear_dream") != 0) {
		auto isnewyeardays = 
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 1) && (nowTime.wDay <= 2);
			};
		auto newyrdrm =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->PlayerStat->MeldPointer >= 3)
					return ((analysis->PlayerStat->Meld[1].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[1].tile == BambooThree) &&
						(analysis->PlayerStat->Meld[2].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[2].tile == BambooOne) &&
						(analysis->PlayerStat->Meld[3].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[3].tile == CircleEight));
				else return false;
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��x�m���O�֎q", yaku::yakuCalculator::Yaku::yval_yakuman,
			newyrdrm
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������x�m���O�֎q", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"��x�m���O�֎q",
			[newyrdrm, isnewyeardays](const MENTSU_ANALYSIS* const analysis) -> bool {
				return newyrdrm(analysis) && isnewyeardays();
			}
		));
	}
	/* ������ */
	if (RuleData::getRule("musashimaru") != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 6, 3, 4, true) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����������", yaku::yakuCalculator::Yaku::yval_yakuman,
			"������",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSix] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->MianziDat[0].tile / TILE_SUIT_STEP == TILE_SUIT_CIRCLES / TILE_SUIT_STEP);
			}
		));
	}

	// ---------------------------------------------------------------------

	/* ��_ */
	if (RuleData::getRule("hanshin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��_", yaku::yakuCalculator::Yaku::yval_1han,
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 8, 4, true);
			}
		));
	/* �_�� */
	if (RuleData::getRule("kobe") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�_��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterFive] >= 1) ||
					(analysis->KeziCount[CircleFive] >= 1) ||
					(analysis->KeziCount[BambooFive] >= 1)) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* �T�b�|����� */
	if (RuleData::getRule("sapporo1") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�T�b�|�����", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 6, true) &&
					(analysis->GameStat->GameRound == 0) && // ����ǂł�����
					(analysis->GameStat->LoopRound == 0) && // �Ԃ蓌��ł��Ȃ���
					(analysis->GameStat->Honba == 0); // ����̎�
			}
		));
	/* �|�J���X�G�b�g */
	if (RuleData::getRule("pcrswt") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�|�J���X�G�b�g", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleTwo] >= 1) ||
					(analysis->KeziCount[CircleFour] >= 1) ||
					(analysis->KeziCount[CircleSix] >= 1) ||
					(analysis->KeziCount[CircleEight] >= 1)) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* �A�N�G���A�X */
	if (RuleData::getRule("aqrs") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�A�N�G���A�X", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) ||
					(analysis->KeziCount[CircleThree] >= 1) ||
					(analysis->KeziCount[CircleFive] >= 1) ||
					(analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[CircleNine] >= 1)) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* �r�l�`�o */
	if (RuleData::getRule("smap") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�r�l�`�o", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterFour] +
					analysis->KeziCount[CircleFour] +
					analysis->KeziCount[BambooFour] -
					analysis->AnKeziCount[CharacterFour] -
					analysis->AnKeziCount[CircleFour] -
					analysis->AnKeziCount[BambooFour] >= 1));
			}
		));
	/* �����X�^�[�n���^�[ */
	if (RuleData::getRule("monster_hunter") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����X�^�[�n���^�[", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) ||
					(analysis->KeziCount[NorthWind] >= 1)) &&
					(analysis->GameStat->TurnRound <= 6);
			}
		));
	/* ���색�[���� */
	if (RuleData::getRule("asahikawa_raamen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���색�[����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* �����d�X�� */
	if (RuleData::getRule("perfect_cherry_blossom") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����d�X��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�F����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* �����g�O�� */
	if (RuleData::getRule("touhou_kouroumu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����g�O��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CharacterSix] >= 1) &&
					(analysis->TsumoHai->tile == EastWind);
			}
		));
	/* Windows95 */
	if (RuleData::getRule("windows95") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows95", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�F����", "�΁X�a", "�ԗ���",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterFive] +
					analysis->DuiziCount[CircleFive] +
					analysis->DuiziCount[BambooFive] >= 2);
			}
		));
	/* Windows98 */
	if (RuleData::getRule("windows98") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows98", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�F����", "�΁X�a", "�ԗ���",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] +
					analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2);
			}
		));
	/* Windows3.1 */
	if (RuleData::getRule("windows31") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows3.1", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "�ԗ���",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->DuiziCount[CharacterThree] +
					analysis->DuiziCount[CircleThree] +
					analysis->DuiziCount[BambooThree] +
					analysis->DuiziCount[CharacterOne] +
					analysis->DuiziCount[CircleOne] +
					analysis->DuiziCount[BambooOne] == SIZE_OF_MELD_BUFFER);
			}
		));
	/* Windows7 */
	if (RuleData::getRule("windows7") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows7", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->TsumoHai->tile == CharacterSeven) &&
					((analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai->tile == CircleSeven) &&
					((analysis->KeziCount[BambooSeven] >= 1) ||
					(analysis->KeziCount[CharacterSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai->tile == BambooSeven) &&
					((analysis->KeziCount[CharacterSeven] >= 1) ||
					(analysis->KeziCount[CircleSeven] >= 1))) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* �������_�^ */
	if (RuleData::getRule("mountain_of_faith") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������_�^", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 6, 7, true) &&
					((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) ||
					(analysis->KeziCount[NorthWind] >= 1));
			}
		));
	/* ����얲 */
	if (RuleData::getRule("hakurei_reimu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����얲", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CharacterEight] >= 1) &&
					(analysis->DuiziCount[CharacterNine] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1));
			}
		));
	/* ���J������ */
	if (RuleData::getRule("kirisame_marisa") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���J������", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CharacterThree] >= 1));
			}
		));
	/* �\�Z���� */
	if (RuleData::getRule("izayoi_sakuya") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�\�Z����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�΁X�a", "����F",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiShanpon));
			}
		));
	/* ����푈 */
	if (RuleData::getRule("seinansensou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����푈", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1));
			}
		));
	/* ��������푈 */
	if (RuleData::getRule("junsei_seinansensou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������푈", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�F������",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[CharacterNine] >= 1) &&
					(analysis->DuiziCount[CircleNine] >= 1) &&
					(analysis->DuiziCount[BambooNine] >= 1));
			}
		));
	/* ��B�V���� */
	if (RuleData::getRule("kyushu_shinkansen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��B�V����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�F����", "�ܖ��", "���V��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] + analysis->DuiziCount[GreenDragon] +
					analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* ���f */
	if (RuleData::getRule("ara") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���f", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���O��", "�΁X�a", "���V��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* ���n���� */
	if (RuleData::getRule("kaibakaben") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���n����", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"��O��", "����F", "�΁X�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* ���]�� */
	if (RuleData::getRule("suikougetsu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���]��", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"��O��", "����F", "�΁X�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* ���ڍ� */
	if (RuleData::getRule("nakameguro") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ڍ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind]>= 1));
			}
		));
}
