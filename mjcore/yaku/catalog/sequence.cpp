#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* ��F�o���� */
	if (RuleData::chkRuleApplied("isshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�o����", get_yaku_han("isshoku_souryuu"),
			"����F", "��u��", "����u��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[CharacterOne] >= 2) && (analysis->ShunziCount[CharacterSeven] >= 2) &&
					(analysis->MianziDat[0].tile == CharacterFive)) yakuFlag = true;
				if ((analysis->ShunziCount[CircleOne] >= 2) && (analysis->ShunziCount[CircleSeven] >= 2) &&
					(analysis->MianziDat[0].tile == CircleFive)) yakuFlag = true;
				if ((analysis->ShunziCount[BambooOne] >= 2) && (analysis->ShunziCount[BambooSeven] >= 2) &&
					(analysis->MianziDat[0].tile == BambooFive)) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ��F�l�� */
	if (RuleData::chkRuleApplied("isshoku_suujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�l��", get_yaku_han("isshoku_suujun"),
			"��F�O��", "��u��", "��u��", "����u��", "�d�l�A�l", "�l�A�l",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ��F�O�� */
	if (RuleData::chkRuleApplied("isshoku_sanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�O��", get_yaku_han("isshoku_sanjun"),
			"��u��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 3) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ��u�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��u��", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 1);
		}
	));
	/* ��u�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��u��", get_yaku_han("ryanpeikou"),
		"��u��",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 2);
		}
	));
	/* �勾 */
	if (RuleData::chkRuleApplied("ohkagami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�勾", get_yaku_han("ohkagami"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return ((yakuFlagCount == 2)&&(analysis->MianziDat[0].tile == CircleOne));
			}
		));
	/* ���u�� */
	if (RuleData::chkRuleApplied("chinpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���u��", get_yaku_han("chinpeikou"),
			"��u��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlagCount = false;
				for (int i = 1; i <= 7; i++) {
					if ((analysis->ShunziCount[i + TILE_SUIT_CHARACTERS] == 2) &&
						(analysis->ShunziCount[i + TILE_SUIT_CIRCLES] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TILE_SUIT_CHARACTERS] == 2) &&
						(analysis->ShunziCount[i + TILE_SUIT_BAMBOOS] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TILE_SUIT_CIRCLES] == 2) &&
						(analysis->ShunziCount[i + TILE_SUIT_BAMBOOS] == 2))
						yakuFlagCount = true;
				}
				return yakuFlagCount;
			}
		));
	/* ����u�� */
	if (RuleData::chkRuleApplied("exposed_ryanpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����u��", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return (yakuFlagCount == 2);
			}
		));

	// ---------------------------------------------------------------------

	/* �T���V���N�Ƃ��T���V�L�Ƃ��Ă΂�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�O�F����", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 7; i++)
				if ((analysis->ShunziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
					(analysis->ShunziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
					(analysis->ShunziCount[i + TILE_SUIT_BAMBOOS] >= 1)) yakuFlag = true;
			return yakuFlag;
		}
	));

	// ---------------------------------------------------------------------

	/* �C�[�`�[�g���J�� */
	auto ikki_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			*yakuFlag = false;
			for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
				if ((analysis->ShunziCount[i + 1] >= 1) &&
					(analysis->ShunziCount[i + 4] >= 1) &&
					(analysis->ShunziCount[i + 7] >= 1)) {
						*yakuFlag = true;
						if (yakuCol != nullptr) *yakuCol = i;
				}
			}
		};
	auto ittsuu_monotonic =
		[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			int samecol = 0;
			ikki_tsuukan(analysis, yakuFlag, yakuCol);
			if (!(*yakuFlag)) return;
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
				if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) ==
					(*yakuCol / TILE_SUIT_STEP)) ++samecol;
				else if ((analysis->MianziDat[i].tile / TILE_SUIT_STEP) ==
					(TILE_SUIT_HONORS / TILE_SUIT_STEP)) ++samecol;
			}
			*yakuFlag = (samecol == SIZE_OF_MELD_BUFFER); // ��F�ɂȂ��Ă��邩�ǂ���
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��C�ʊ�", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return yakuFlag;
			}
	));
	/* 20101204�j�����`�V�X�S�� */
	if (RuleData::chkRuleApplied("tohoku_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���k�V����", get_yaku_han("tohoku_shinkansen"),
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->DuiziCount[EastWind] >= 1) && // ����
					(analysis->DuiziCount[NorthWind] >= 1) ); // �k������
			}
		));
	/* ��C�ʊёS�ћ�� */
	auto ittsuu_chanta_counting =
		[](const MENTSU_ANALYSIS* const analysis, int* const yaojiu, int* const zipai) -> void {
			*yaojiu = *zipai = 0;
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
				switch (analysis->MianziDat[i].tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind:
				case WhiteDragon: case GreenDragon: case RedDragon:
					++*zipai;
					/* FALLTHRU */
				case CharacterOne: case CharacterNine: case CircleOne: case CircleNine:
				case BambooOne: case BambooNine:
					++*yaojiu; break;
				case CharacterSeven: case CircleSeven: case BambooSeven:
					if ((i > 0) && (analysis->MianziDat[i].mstat < meldTripletConcealed))
						++*yaojiu;
					break;
				}
			}
		};
	if (RuleData::chkRuleApplied("ittsuu_chanta")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��C�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"��C�ʊ�",
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, nullptr);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����C�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"��C�ʊ�",
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, nullptr);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}
	/* �ܐS�ʊ� */
	if (RuleData::chkRuleApplied("uushintonkan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܐS�ʊ�", get_yaku_han("uushintonkan"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // �ƒ��҂���
					(analysis->PlayerStat->Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // �a���v��5
					);
			}
		));
	/* ���Y�� */
	if (RuleData::chkRuleApplied("longxizhu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Y��", get_yaku_han("longxizhu"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[WestWind] >= 1) && // ���̍��q������
					(analysis->MianziDat[0].tile == CircleOne) ); // �A�^�}���ꓛ
			}
		));
	/* �����Y�k�l */
	if (RuleData::chkRuleApplied("jinlong_dou_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����Y�k�l", get_yaku_han("jinlong_dou_beidou"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[CircleSeven] >= 1) && // �����̍��q������
					(analysis->MianziDat[0].tile == NorthWind) ); // �A�^�}���k
			}
		));
	/* ���Ώ\�i�n��̖� */
	{
		const int Rules = 10;
		const char tmpRuleCodeList[Rules][32] = {
			"shuangfeng_zhayun", "nanbing_wanzhong", "leifeng_xizhao", "liulang_wenying", "duanqiao_canxue",
			"sudi_chunxiao", "huagang_guanyu", "santan_yinyue", "quyuan_fenghe", "pinghu_qiuyue",
		};
		const tileCode tmpTileCodeList[Rules][2] = {
			{EastWind, BambooEight}, {SouthWind, RedDragon}, {WestWind, CircleEight}, {NorthWind, CharacterEight},
			{WhiteDragon, BambooTwo}, {GreenDragon, CharacterTwo}, {RedDragon, CircleTwo},
			{WhiteDragon, CharacterOne}, {GreenDragon, CircleOne}, {RedDragon, BambooOne},
		};
		const int tmpStraightColorList[Rules] = {
			TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS, TILE_SUIT_CIRCLES, TILE_SUIT_CHARACTERS,
			TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS,
		};
		const char tmpYakuNameList[Rules][16] = {
			"�o���}�_", "�웠�ӏ�", "�����[��", "���Q����", "�f���c��",
			"�h��t��", "�ԍ`�ϋ�", "�O杈�", "�ȉ@����", "���ΏH��",
		};
		for (int i = 0; i < Rules; i++) {
			if (!RuleData::chkRuleApplied(tmpRuleCodeList[i])) continue;
			tileCode tmpTileCode1 = tmpTileCodeList[i][0];
			tileCode tmpTileCode2 = tmpTileCodeList[i][1];
			int tmpStraightColor = tmpStraightColorList[i];
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				tmpYakuNameList[i], get_yaku_han(tmpRuleCodeList[i]),
				[ikki_tsuukan, tmpTileCode1, tmpTileCode2, tmpStraightColor]
				(const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag; int yakuCol;
					ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
					return (yakuFlag && // ��C�ʊт��������Ă���
						(analysis->DuiziCount[tmpTileCode1] >= 1) && // �w��̔v�����q�Ɛ����Ƃ��đ��݂�
						(analysis->DuiziCount[tmpTileCode2] >= 1) &&
						(yakuCol == tmpStraightColor) ); // �C�b�c�[�������w�肳�ꂽ�F
				}
			));
		}
	}
	/* ���̐� */
	if (RuleData::chkRuleApplied("azure_dragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���̐�", get_yaku_han("azure_dragon"),
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[EastWind] >= 1) && // ���̍��q������
					(analysis->MianziDat[0].tile == WhiteDragon) && // �A�^�}������
					(yakuCol == TILE_SUIT_CIRCLES) ); // �C�b�c�[���������q
			}
		));
	/* �k�̌��� */
	if (RuleData::chkRuleApplied("black_tortoise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�̌���", get_yaku_han("black_tortoise"),
			"��C�ʊ�",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[NorthWind] >= 1) && // �k�̍��q������
					(analysis->MianziDat[0].tile == BambooSeven) && // �A�^�}��������
					(yakuCol == TILE_SUIT_CIRCLES) ); // �C�b�c�[���������q
			}
		));
	/* �H�c�V���� */
	if (RuleData::chkRuleApplied("akita_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�H�c�V����", get_yaku_han("akita_shinkansen"),
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[NorthWind] >= 1) && // �k�̍��q������
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (yakuCol / TILE_SUIT_STEP)) && // ��F�ɂȂ��Ă���
					(analysis->PlayerStat->Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // �a���v��5
					);
			}
		));
	/* ��z�V�����Ƃ� */
	if (RuleData::chkRuleApplied("joetsu_toki"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��z�V�����Ƃ�", get_yaku_han("joetsu_toki"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(analysis->DuiziCount[NorthWind] >= 1) // �k�̍��q�܂��͐���������
					);
			}
		));
	/* ��z�V���������� */
	if (RuleData::chkRuleApplied("joetsu_asahi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��z�V����������", get_yaku_han("joetsu_asahi"),
			"��z�V�����Ƃ�", "��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->DuiziCount[NorthWind] >= 1) && // �k��
					(analysis->DuiziCount[WhiteDragon] >= 1) // �������q�Ɛ����Ƃ��đ���
					);
			}
		));
	/* ���A���D */
	if (RuleData::chkRuleApplied("seikan_ship"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A���D", get_yaku_han("seikan_ship"),
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->MianziDat[0].tile == NorthWind) && // �k��������
					(analysis->AnKangziCount[GreenDragon] >= 1) // ᢂ̈ÞȂ�����
					);
			}
		));
	/* �S�[���h���b�V�� */
	if (RuleData::chkRuleApplied("goldrush")) {
		auto goldrush1 =
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(yakuCol == TILE_SUIT_CIRCLES) && // ���q�̃C�b�c�[��
					(analysis->DuiziCount[GreenDragon] >= 1) // ᢂ̍��q������������
					);
			};
		auto goldrush2 =
			[goldrush1](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (goldrush1(analysis) && // goldrush1�̏����𖞂���
					(analysis->DuiziCount[WestWind] >= 1) // ���̍��q������������
					);
			};
		if (RuleData::chkRule("goldrush", "2han"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "2han_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush2
			));
		else if (RuleData::chkRule("goldrush", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_yakuman,
				"��C�ʊ�", "����F",
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "yakuman_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_yakuman,
				"��C�ʊ�", "����F",
				goldrush2
			));
	}
	/* ���[�g�Q�S�U */
	if (RuleData::chkRuleApplied("route_246"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���[�g�Q�S�U", get_yaku_han("route_246"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // �ƒ��҂���
					((analysis->TsumoHai->tile == yakuCol + 2) || // 2��
					(analysis->TsumoHai->tile == yakuCol + 4) || // 4��
					(analysis->TsumoHai->tile == yakuCol + 6)) // 6�Řa��
					);
			}
		));
	/* �㗴�闎�n */
	if (RuleData::chkRuleApplied("gaulungsing_lokdei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�㗴�闎�n", get_yaku_han("gaulungsing_lokdei"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->ShunziCount[yakuCol + 1] - analysis->AnShunziCount[yakuCol + 1] >= 1) &&
					(analysis->ShunziCount[yakuCol + 4] - analysis->AnShunziCount[yakuCol + 4] >= 1) &&
					(analysis->ShunziCount[yakuCol + 7] - analysis->AnShunziCount[yakuCol + 7] >= 1) // ��C�ʊт̔v�͑S���`�[
					);
			}
		));
	/* �k�C���V���� */
	if (RuleData::chkRuleApplied("hokkaido_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�C���V����", get_yaku_han("hokkaido_shinkansen"),
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->AnKangziCount[WhiteDragon] >= 1) && // ���̈ÞȂ�������
					(analysis->MianziDat[0].tile == NorthWind) ); // �������k
			}
		));

	// ---------------------------------------------------------------------

	/* �S�[���f���Q�[�g�u���b�W */
	if (RuleData::chkRuleApplied("golden_gate_bridge"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���勴", get_yaku_han("golden_gate_bridge"),
			"���A��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 3] >= 1) &&
						(analysis->ShunziCount[i + 5] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 1)) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* Arc de Triomphe */
	if (RuleData::chkRuleApplied("gaisenmon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�M����", get_yaku_han("gaisenmon"),
			"����F",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					int yakuFlagCount = 0;
					if ((analysis->ShunziCount[i + 1] >= 1) && (analysis->ShunziCount[i + 4] >= 1)) ++yakuFlag;
					if ((analysis->ShunziCount[i + 2] >= 1) && (analysis->ShunziCount[i + 5] >= 1)) ++yakuFlag;
					if ((analysis->ShunziCount[i + 3] >= 1) && (analysis->ShunziCount[i + 6] >= 1)) ++yakuFlag;
					if ((analysis->ShunziCount[i + 4] >= 1) && (analysis->ShunziCount[i + 7] >= 1)) ++yakuFlag;
					if (yakuFlagCount == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ���� */
	if (RuleData::chkRuleApplied("choujou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", get_yaku_han("choujou"),
			"��C�ʊ�", "����F",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					int yakuFlagCount = 0;
					if ((analysis->ShunziCount[i + 1] >= 2) &&
						(analysis->ShunziCount[i + 4] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 1)) yakuFlagCount += 2;
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 4] >= 2) &&
						(analysis->ShunziCount[i + 7] >= 1)) yakuFlagCount += 2;
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 4] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 2)) yakuFlagCount += 2;
					if ((analysis->ShunziCount[i + 1] == 1) &&
						(analysis->ShunziCount[i + 4] == 1) &&
						(analysis->ShunziCount[i + 7] == 1)) yakuFlagCount += 1;
					if ((analysis->ShunziCount[i + 2] >= 1) ||
						(analysis->ShunziCount[i + 3] >= 1) ||
						(analysis->ShunziCount[i + 5] >= 1) ||
						(analysis->ShunziCount[i + 6] >= 1)) yakuFlagCount += 1;
					if ((yakuFlagCount == 2) &&
						((analysis->MianziDat[0].tile == i + 2) ||
						(analysis->MianziDat[0].tile == i + 5) ||
						(analysis->MianziDat[0].tile == i + 8))) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ���ݓV */
	if (RuleData::chkRuleApplied("qinglong_zaitian"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ݓV", get_yaku_han("qinglong_zaitian"),
			"��C�ʊ�", "����F", "��u��",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт�
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (yakuCol / TILE_SUIT_STEP)) && // ��F��
					((analysis->ShunziCount[yakuCol + 1] >= 2) ||
					(analysis->ShunziCount[yakuCol + 4] >= 2) ||
					(analysis->ShunziCount[yakuCol + 7] >= 2)) // ��u���ɂȂ��Ă���
					);
			}
		));
	/* �����a */
	if (RuleData::chkRuleApplied("kyantonhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����a", get_yaku_han("kyantonhoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < (TILE_SUIT_HONORS * 2); i += TILE_SUIT_STEP * 2) {
					int yakuTmpFlag = 0;
					for (int k = 1; k <= 7; k++)
						if ((analysis->ShunziCount[i % TILE_SUIT_HONORS + k] >= 1) &&
							(analysis->ShunziCount[(i + TILE_SUIT_STEP) % TILE_SUIT_HONORS + k] >= 1))
								++yakuTmpFlag;
					if (yakuTmpFlag == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* �l���� */
	auto puukao =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suupuukao) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (7 - step * (suupuukao ? 3 : 2)); k++)
					if ((analysis->ShunziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * 0 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * 1 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * 2 + k] >= 1) &&
						(suupuukao&&(analysis->ShunziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * 3 + k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l����", get_yaku_han("okasuujun"),
			"�O����",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
			}
		));
	/* �O���� */
	if (RuleData::chkRuleApplied("okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O����", get_yaku_han("okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
			}
		));
	/* �R�O�� */
	if (RuleData::chkRuleApplied("yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��", get_yaku_han("yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* ��F�l���� */
	if (RuleData::chkRuleApplied("nishoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�l����", get_yaku_han("nishoku_okasuujun"),
			"��F�O����",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* ��F�O���� */
	if (RuleData::chkRuleApplied("nishoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�O����", get_yaku_han("nishoku_okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* ��F�R�l�� */
	if (RuleData::chkRuleApplied("nishoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��G�A��", get_yaku_han("nishoku_yamasuujun"),
			"�G�A��",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* ��F�R�O�� */
	if (RuleData::chkRuleApplied("nishoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�G�A��", get_yaku_han("nishoku_yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* �O�F�l���� */
	if (RuleData::chkRuleApplied("sanshoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�l����", get_yaku_han("sanshoku_okasuujun"),
			"�O�F�O����",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* �O�F�O���� */
	if (RuleData::chkRuleApplied("sanshoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�O����", get_yaku_han("sanshoku_okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* �O�F�R�l�� */
	if (RuleData::chkRuleApplied("sanshoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�卬�A��", get_yaku_han("sanshoku_yamasuujun"),
			"���A��",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* �O�F�R�O�� */
	if (RuleData::chkRuleApplied("nishoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��", get_yaku_han("nishoku_yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));

	// ---------------------------------------------------------------------

	/* �o������ */
	if (RuleData::chkRuleApplied("shanron_chonchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o������", get_yaku_han("shanron_chonchuu"),
			"�����a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 4; i++)
					if ((analysis->ShunziCount[TILE_SUIT_CHARACTERS + i] >= 1) &&
						(analysis->ShunziCount[TILE_SUIT_CHARACTERS + i + 3] >= 1) &&
						(analysis->ShunziCount[TILE_SUIT_BAMBOOS + i] >= 1) &&
						(analysis->ShunziCount[TILE_SUIT_BAMBOOS + i + 3] >= 1) &&
						((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (TILE_SUIT_CIRCLES / TILE_SUIT_STEP)))
							yakuFlag = true;
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* �ԗ� */
	auto sanshoku_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
				if ((analysis->ShunziCount[(int)((*k)[0] - '0') * TILE_SUIT_STEP + 1] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[1] - '0') * TILE_SUIT_STEP + 4] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[2] - '0') * TILE_SUIT_STEP + 7] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};

	if (RuleData::chkRuleApplied("hualong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�ʊ�", get_yaku_han("hualong"),
			sanshoku_tsuukan
		));
	/* �P�� */
	if (RuleData::chkRuleApplied("tanron"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�P��", get_yaku_han("tanron"),
			"�O�F�ʊ�",
			[sanshoku_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (sanshoku_tsuukan(analysis) && // �O�F�ʊт�
					(*analysis->MenzenFlag) && // ��O��
					(analysis->Machi == yaku::yakuCalculator::machiTanki) && // �P�R�҂���
					(analysis->TsumoHai->tile != WhiteDragon) && // ���҂��łȂ�
					(analysis->TsumoHai->tile != GreenDragon) ); // ᢑ҂��łȂ�
			}
		));
	/* �O�F�ʊёS�ћ�� */
	if (RuleData::chkRuleApplied("hualong_chanta")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"�O�F�ʊ�",
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting( analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���O�F�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"�O�F�ʊ�",
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}

	// ---------------------------------------------------------------------

	/* �o���o�T */
	auto ShuangLong =
		[](const MENTSU_ANALYSIS* const analysis, int suit1, int suit2, tileCode head) -> bool {
			return ((analysis->ShunziCount[suit1 + 1] >= 1) &&
				(analysis->ShunziCount[suit1 + 7] >= 1) &&
				(analysis->ShunziCount[suit2 + 1] >= 1) &&
				(analysis->ShunziCount[suit2 + 7] >= 1) &&
				(analysis->MianziDat[0].tile == head));
		};

	if (RuleData::chkRuleApplied("shanron_shankui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���o�T", get_yaku_han("shanron_shankui"),
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooThree);
			}
		));
	/* �o������(�V�����^�C�v) */
	if (RuleData::chkRuleApplied("shanron_chonchu_lsf"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o������ ", get_yaku_han("shanron_chonchu_lsf"),
			"�����a", "�O�F�o����",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_BAMBOOS, CircleFive);
			}
		));
	/* �O�F�o���� */
	if (RuleData::chkRuleApplied("sanshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�o����", get_yaku_han("sanshoku_souryuu"),
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
					if (ShuangLong(analysis, (int)((*k)[0] - '0') * TILE_SUIT_STEP,
						(int)((*k)[1] - '0') * TILE_SUIT_STEP,
						(tileCode)((int)((*k)[1] - '0') * TILE_SUIT_STEP + 5))) return true;
				return false;
			}
		));
	/* �o���Y�P */
	if (RuleData::chkRuleApplied("shanron_shiifon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���Y�P", get_yaku_han("shanron_shiifon"),
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooOne);
			}
		));
	/* �o��� */
	if (RuleData::chkRuleApplied("shanron_paochuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���", get_yaku_han("shanron_paochuu"),
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooTwo) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooFour) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSix));
			}
		));
	/* �o���Y�T */
	if (RuleData::chkRuleApplied("shanron_shiikui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���Y�T", get_yaku_han("shanron_shiikui"),
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSeven);
			}
		));
	/* �o������ */
	if (RuleData::chkRuleApplied("shanron_touja"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���Y�T", get_yaku_han("shanron_touja"),
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS, CharacterOne);
			}
		));

	// ---------------------------------------------------------------------

	/* �h�O�� */
	auto kanchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++)
				if (analysis->MianziDat[i].mstat == meldSequenceExposedMiddle) ++count;
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�O��", get_yaku_han("chii_sankan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 3);
			}
		));
	/* �h�l�� */
	if (RuleData::chkRuleApplied("chii_suukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�l��", get_yaku_han("chii_suukan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 4);
			}
		));
	/* �h�O�� */
	auto penchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++) {
				for (int j = 0; j < TILE_SUIT_HONORS; j += TILE_SUIT_STEP) {
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedLower) &&
						(analysis->MianziDat[i].tile == TILE_SUIT_STEP + 7)) ++count;
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedUpper) &&
						(analysis->MianziDat[i].tile == TILE_SUIT_STEP + 1)) ++count;
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sanpen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�O��", get_yaku_han("chii_sanpen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 3);
			}
		));
	/* �h�l�� */
	if (RuleData::chkRuleApplied("chii_suupen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�l��", get_yaku_han("chii_suupen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 4);
			}
		));
	/* �h���� */
	auto xiaoShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedLower) {
					++count;
					for (int j = 0; j < TILE_SUIT_HONORS; j += TILE_SUIT_STEP) {
						if (analysis->MianziDat[i].tile == TILE_SUIT_STEP + 7) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_shaoshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h����", get_yaku_han("chii_shaoshun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 3);
			}
		));
	/* �h���l�� */
	if (RuleData::chkRuleApplied("chii_shaosuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h���l��", get_yaku_han("chii_shaosuushun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 4);
			}
		));
	/* �h�叇 */
	auto daShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SIZE_OF_MELD_BUFFER; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedUpper) {
					++count;
					for (int j = 0; j < TILE_SUIT_HONORS; j += TILE_SUIT_STEP) {
						if (analysis->MianziDat[i].tile == TILE_SUIT_STEP + 1) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_taashun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�叇", get_yaku_han("chii_taashun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 3);
			}
		));
	/* �h��l�� */
	if (RuleData::chkRuleApplied("chii_taasuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h��l��", get_yaku_han("chii_taasuushun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 4);
			}
		));
}
