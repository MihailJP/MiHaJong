#include "../catalog.h"

extern const std::array<char[8], 3> parsedat_monochrome4;
extern const std::array<char[4], 3> parsedat_monochrome3;
extern const std::array<char[8], 42> parsedat_bichrome4;
extern const std::array<char[4], 18> parsedat_bichrome3;
extern const std::array<char[8], 36> parsedat_trichrome4;
extern const std::array<char[4], 6> parsedat_trichrome3;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* ��F�o���� */
	if (getRule(RULE_ISSHOKU_SOURYUU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�o����", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (getRule(RULE_ISSHOKU_SUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�l��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��F�O��", "��u��", "��u��", "����u��", "�d�l�A�l", "�l�A�l",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ��F�O�� */
	if (getRule(RULE_ISSHOKU_SANJUN) != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC hf = yaku::yakuCalculator::Yaku::HANFUNC();
		switch (getRule(RULE_ISSHOKU_SANJUN)) {
			case 1: hf = yaku::yakuCalculator::Yaku::yval_2han_menzen; break;
			case 2: hf = yaku::yakuCalculator::Yaku::yval_2han_kuisagari; break;
			case 3: hf = yaku::yakuCalculator::Yaku::yval_3han_menzen; break;
			case 4: hf = yaku::yakuCalculator::Yaku::yval_3han_kuisagari; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�O��", hf,
			"��u��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 3) yakuFlag = true;
				return yakuFlag;
			}
		));
	}
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
		"��u��", (getRule(RULE_RYANPEIKOU) != 0) ?
		yaku::yakuCalculator::Yaku::yval_2han_menzen : yaku::yakuCalculator::Yaku::yval_3han_menzen,
		"��u��",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TILE_SUIT_HONORS; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 2);
		}
	));
	/* �勾 */
	if (getRule(RULE_OHKAGAMI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�勾", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return ((yakuFlagCount == 2)&&(analysis->MianziDat[0].tile == CircleOne));
			}
		));
	/* ���u�� */
	if (getRule(RULE_CHINPEIKOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���u��", (getRule(RULE_CHINPEIKOU) == 1) ? yaku::yakuCalculator::Yaku::yval_6han_menzen :
			((getRule(RULE_CHINPEIKOU) == 2) ? yaku::yakuCalculator::Yaku::yval_5han_menzen :
			yaku::yakuCalculator::Yaku::yval_4han_menzen),
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
	if (getRule(RULE_EXPOSED_RYANPEIKOU) != 0)
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
		"�O�F����",yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
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
						if (yakuCol != NULL) *yakuCol = i;
				}
			}
		};
	auto ittsuu_monotonic =
		[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			int samecol;
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
		"��C�ʊ�",yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->DuiziCount[EastWind] >= 1) && // ����
					(analysis->DuiziCount[NorthWind] >= 1) ); // �k������
			}
	));
	/* 20101204�j�����`�V�X�S�� */
	if (getRule(RULE_TOHOKU_SHINKANSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���k�V����", (getRule(RULE_TOHOKU_SHINKANSEN) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
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
	if (getRule(RULE_ITTSUU_CHANTA) != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��C�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"��C�ʊ�",
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, NULL);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����C�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"��C�ʊ�",
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, NULL);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}
	/* �ܐS�ʊ� */
	if (getRule(RULE_UUSHINTONKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܐS�ʊ�", yaku::yakuCalculator::Yaku::yval_1han,
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
	if (getRule(RULE_LONGXIZHU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Y��", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[WestWind] >= 1) && // ���̍��q������
					(analysis->MianziDat[0].tile == CircleOne) ); // �A�^�}���ꓛ
			}
		));
	/* �����Y�k�l */
	if (getRule(RULE_JINLONG_DOU_BEIDOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����Y�k�l", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[CircleSeven] >= 1) && // �����̍��q������
					(analysis->MianziDat[0].tile == NorthWind) ); // �A�^�}���k
			}
		));
	/* ���Ώ\�i�n��̖� */
	{
		const int Rules = 10;
		const RuleCode tmpRuleCodeList[Rules] = {
			RULE_SHUANGFENG_ZHAYUN, RULE_NANBING_WANZHONG, RULE_LEIFENG_XIZHAO, RULE_LIULANG_WENYING, RULE_DUANQIAO_CANXUE,
			RULE_SUDI_CHUNXIAO, RULE_HUAGANG_GUANYU, RULE_SANTAN_YINYUE, RULE_QUYUAN_FENGHE, RULE_PINGHU_QIUYUE,
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
			if (getRule(tmpRuleCodeList[i]) == 0) continue;
			tileCode tmpTileCode1 = tmpTileCodeList[i][0];
			tileCode tmpTileCode2 = tmpTileCodeList[i][1];
			int tmpStraightColor = tmpStraightColorList[i];
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				tmpYakuNameList[i], yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
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
	if (getRule(RULE_AZURE_DRAGON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���̐�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
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
	if (getRule(RULE_BLACK_TORTOISE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�̌���", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
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
	if (getRule(RULE_AKITA_SHINKANSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�H�c�V����", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (getRule(RULE_JOETSU_TOKI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��z�V�����Ƃ�", yaku::yakuCalculator::Yaku::yval_2han,
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(analysis->DuiziCount[NorthWind] >= 1) // �k�̍��q�܂��͐���������
					);
			}
		));
	/* ��z�V���������� */
	if (getRule(RULE_JOETSU_ASAHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��z�V����������", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��z�V�����Ƃ�", "��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->DuiziCount[NorthWind] >= 1) && // �k��
					(analysis->DuiziCount[WhiteDragon] >= 1) // �������q�Ɛ����Ƃ��đ���
					);
			}
		));
	/* ���A���D */
	if (getRule(RULE_SEIKAN_SHIP) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A���D", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->MianziDat[0].tile == NorthWind) && // �k��������
					(analysis->AnKangziCount[GreenDragon] >= 1) // ᢂ̈ÞȂ�����
					);
			}
		));
	/* �S�[���h���b�V�� */
	if (getRule(RULE_GOLDRUSH) != 0) {
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
		switch (getRule(RULE_GOLDRUSH)) {
		case 1:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush1
			)); break;
		case 2:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_2han,
				goldrush2
			)); break;
		case 3:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_yakuman,
				"��C�ʊ�", "����F",
				goldrush1
			)); break;
		case 4:
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"�S�[���h���b�V��", yaku::yakuCalculator::Yaku::yval_yakuman,
				"��C�ʊ�", "����F",
				goldrush2
			)); break;
		}
	}
	/* ���[�g�Q�S�U */
	if (getRule(RULE_ROUTE_246) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���[�g�Q�S�U", yaku::yakuCalculator::Yaku::yval_1han,
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
	if (getRule(RULE_GAULUNGSING_LOKDEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�㗴�闎�n", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (getRule(RULE_HOKKAIDO_SHINKANSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�C���V����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��C�ʊ�", "����F",
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->AnKangziCount[WhiteDragon] >= 1) && // ���̈ÞȂ�������
					(analysis->MianziDat[0].tile == NorthWind) ); // �������k
			}
		));

	// ---------------------------------------------------------------------

	/* �S�[���f���Q�[�g�u���b�W */
	if (getRule(RULE_GOLDEN_GATE_BRIDGE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���勴", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (getRule(RULE_GAISENMON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�M����", getRule(RULE_GAISENMON) == 1 ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
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
	if (getRule(RULE_CHOUJOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (getRule(RULE_QINGLONG_ZAITIAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ݓV", yaku::yakuCalculator::Yaku::yval_yakuman,
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
	if (getRule(RULE_KYANTONHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����a", (getRule(RULE_KYANTONHOH) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < (TILE_SUIT_HONORS * 2); i += TILE_SUIT_STEP * 2) {
					int yakuTmpFlag = 0;
					for (int k = 1; k <= 7; k++)
						if ((analysis->ShunziCount[i % TILE_SUIT_HONORS + k] >= 1) &&
							(analysis->ShunziCount[(i + TILE_SUIT_HONORS) % TILE_SUIT_HONORS + k] >= 1))
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
					if ((analysis->ShunziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(suupuukao&&(analysis->ShunziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	if (getRule(RULE_OKASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l����", (getRule(RULE_OKASUUJUN) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"�O����",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
			}
		));
	/* �O���� */
	if (getRule(RULE_OKASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O����", (getRule(RULE_OKASANJUN) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((getRule(RULE_OKASANJUN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
			}
		));
	/* �R�O�� */
	if (getRule(RULE_YAMASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��", (getRule(RULE_YAMASANJUN) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((getRule(RULE_YAMASANJUN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* ��F�l���� */
	if (getRule(RULE_NISHOKU_OKASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�l����", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"��F�O����",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* ��F�O���� */
	if (getRule(RULE_NISHOKU_OKASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�O����", yaku::yakuCalculator::Yaku::yval_1han,
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* ��F�R�l�� */
	if (getRule(RULE_NISHOKU_YAMASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��G�A��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"�G�A��",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* ��F�R�O�� */
	if (getRule(RULE_NISHOKU_YAMASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�G�A��", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* �O�F�l���� */
	if (getRule(RULE_SANSHOKU_OKASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�l����", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"�O�F�O����",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* �O�F�O���� */
	if (getRule(RULE_SANSHOKU_OKASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�O����", (getRule(RULE_SANSHOKU_OKASANJUN) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((getRule(RULE_SANSHOKU_OKASANJUN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* �O�F�R�l�� */
	if (getRule(RULE_SANSHOKU_YAMASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�卬�A��", (getRule(RULE_SANSHOKU_YAMASUUJUN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
			"���A��",
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* �O�F�R�O�� */
	if (getRule(RULE_NISHOKU_YAMASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��", (getRule(RULE_NISHOKU_YAMASANJUN) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((getRule(RULE_NISHOKU_YAMASANJUN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));

	// ---------------------------------------------------------------------

	/* �o������ */
	if (getRule(RULE_SHANRON_CHONCHUU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o������", yaku::yakuCalculator::Yaku::yval_yakuman,
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
				if ((analysis->ShunziCount[(int)((&*k)[0] - '0') * TILE_SUIT_STEP + 1] >= 1) &&
					(analysis->ShunziCount[(int)((&*k)[1] - '0') * TILE_SUIT_STEP + 4] >= 1) &&
					(analysis->ShunziCount[(int)((&*k)[2] - '0') * TILE_SUIT_STEP + 7] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};

	if (getRule(RULE_HUALONG) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�ʊ�", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			sanshoku_tsuukan
		));
	/* �P�� */
	if (getRule(RULE_TANRON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�P��", yaku::yakuCalculator::Yaku::yval_6han_menzen,
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
	if (getRule(RULE_HUALONG_CHANTA) != 0) {
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

	if (getRule(RULE_SHANRON_SHANKUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���o�T", yaku::yakuCalculator::Yaku::yval_4han,
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooThree);
			}
		));
	/* �o������(�V�����^�C�v) */
	if (getRule(RULE_SHANRON_CHONCHU_LSF) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o������ ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�����a", "�O�F�o����",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_BAMBOOS, CircleFive);
			}
		));
	/* �O�F�o���� */
	if (getRule(RULE_SANSHOKU_SOURYUU) != 0) {
		yaku::yakuCalculator::Yaku::HANFUNC han;
		switch (getRule(RULE_SANSHOKU_SOURYUU)) {
			case 1: han = yaku::yakuCalculator::Yaku::yval_2han_kuisagari; break;
			case 2: han = yaku::yakuCalculator::Yaku::yval_6han_kuisagari; break;
			case 3: han = yaku::yakuCalculator::Yaku::yval_6han; break;
			case 4: han = yaku::yakuCalculator::Yaku::yval_yakuman; break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�o����", han,
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
					if (ShuangLong(analysis, (int)((&*k)[0] - '0') * TILE_SUIT_STEP,
						(int)((&*k)[1] - '0') * TILE_SUIT_STEP,
						(tileCode)((int)((&*k)[1] - '0') * TILE_SUIT_STEP + 5))) return true;
				return false;
			}
		));
	}
	/* �o���Y�P */
	if (getRule(RULE_SHANRON_SHIIFON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���Y�P", yaku::yakuCalculator::Yaku::yval_4han,
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooOne);
			}
		));
	/* �o��� */
	if (getRule(RULE_SHANRON_PAOCHUU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���", yaku::yakuCalculator::Yaku::yval_4han,
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooTwo) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooFour) ||
					ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSix));
			}
		));
	/* �o���Y�T */
	if (getRule(RULE_SHANRON_SHIIKUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���Y�T", yaku::yakuCalculator::Yaku::yval_4han,
			"�����a",
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES, BambooSeven);
			}
		));
	/* �o������ */
	if (getRule(RULE_SHANRON_TOUJA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o���Y�T", yaku::yakuCalculator::Yaku::yval_4han,
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
	if (getRule(RULE_CHII_SANKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�O��", yaku::yakuCalculator::Yaku::yval_1han,
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 3);
			}
		));
	/* �h�l�� */
	if (getRule(RULE_CHII_SUUKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�l��", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (getRule(RULE_CHII_SANPEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�O��", yaku::yakuCalculator::Yaku::yval_1han,
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 3);
			}
		));
	/* �h�l�� */
	if (getRule(RULE_CHII_SUUPEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�l��", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (getRule(RULE_CHII_SHAOSHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h����", yaku::yakuCalculator::Yaku::yval_1han,
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 3);
			}
		));
	/* �h���l�� */
	if (getRule(RULE_CHII_SHAOSUUSHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h���l��", yaku::yakuCalculator::Yaku::yval_2han,
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
	if (getRule(RULE_CHII_TAASHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h�叇", yaku::yakuCalculator::Yaku::yval_1han,
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 3);
			}
		));
	/* �h��l�� */
	if (getRule(RULE_CHII_TAASUUSHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�h��l��", yaku::yakuCalculator::Yaku::yval_2han,
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 4);
			}
		));
}