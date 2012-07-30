#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* ��F�o���� */
	if (getRule(RULE_ISSHOKU_SOURYUU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�o����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"����F", "��u��", "����u��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			"��F�O��", "��u��", "��u��", "����u��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(gameStat->Player[analysis->player].MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
			}),
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis,
		bool* const yakuFlag, int* const yakuCol) -> void {
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
		[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis,
		bool* const yakuFlag, int* const yakuCol) -> void {
			int samecol;
			ikki_tsuukan(gameStat, analysis, yakuFlag, yakuCol);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->DuiziCount[EastWind] >= 1) && // ����
					(analysis->DuiziCount[NorthWind] >= 1) ); // �k������
			}
		));
	/* ��C�ʊёS�ћ�� */
	if (getRule(RULE_ITTSUU_CHANTA) != 0) {
		auto ittsuu_chanta_counting =
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis,
			int* const yaojiu, int* const zipai) -> void {
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
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��C�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"��C�ʊ�",
			[ittsuu_chanta_counting](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai;
				ittsuu_chanta_counting(gameStat, analysis, &yaojiu, &zipai);
				return ((yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����C�ʊёS�ћ��", yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			"��C�ʊ�",
			[ittsuu_chanta_counting](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai;
				ittsuu_chanta_counting(gameStat, analysis, &yaojiu, &zipai);
				return ((yaojiu == 4) && (zipai == 0));
			}
		));
	}
	/* �ܐS�ʊ� */
	if (getRule(RULE_UUSHINTONKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܐS�ʊ�", yaku::yakuCalculator::Yaku::yval_1han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // �ƒ��҂���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // �a���v��5
					);
			}
		));
	/* ���Y�� */
	if (getRule(RULE_LONGXIZHU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Y��", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[WestWind] >= 1) && // ���̍��q������
					(analysis->MianziDat[0].tile == CircleOne) ); // �A�^�}���ꓛ
			}
		));
	/* �����Y�k�l */
	if (getRule(RULE_JINLONG_DOU_BEIDOU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����Y�k�l", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
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
				(const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag; int yakuCol;
					ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[NorthWind] >= 1) && // �k�̍��q������
					((analysis->MianziDat[0].tile / TILE_SUIT_STEP) == (yakuCol / TILE_SUIT_STEP)) && // ��F�ɂȂ��Ă���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND].tile == (yakuCol + 5)) // �a���v��5
					);
			}
		));
	/* ��z�V�����Ƃ� */
	if (getRule(RULE_JOETSU_TOKI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��z�V�����Ƃ�", yaku::yakuCalculator::Yaku::yval_2han,
			[ittsuu_monotonic](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(gameStat, analysis, &yakuFlag, &yakuCol);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->MianziDat[0].tile == NorthWind) && // �k��������
					(analysis->AnKangziCount[GreenDragon] >= 1) // ᢂ̈ÞȂ�����
					);
			}
		));
	/* �S�[���h���b�V�� */
	if (getRule(RULE_GOLDRUSH) != 0) {
		auto goldrush1 =
			[ittsuu_monotonic](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(yakuCol == TILE_SUIT_CIRCLES) && // ���q�̃C�b�c�[��
					(analysis->DuiziCount[GreenDragon] >= 1) // ᢂ̍��q������������
					);
			};
		auto goldrush2 =
			[goldrush1](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (goldrush1(gameStat, analysis) && // goldrush1�̏����𖞂���
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // �ƒ��҂���
					((gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == yakuCol + 2) || // 2��
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == yakuCol + 4) || // 4��
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == yakuCol + 6)) // 6�Řa��
					);
			}
		));
	/* �㗴�闎�n */
	if (getRule(RULE_GAULUNGSING_LOKDEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�㗴�闎�n", yaku::yakuCalculator::Yaku::yval_2han,
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(gameStat, analysis, &yakuFlag, NULL);
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
			[ikki_tsuukan](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(gameStat, analysis, &yakuFlag, &yakuCol);
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
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
	/* �l���� */
	if (getRule(RULE_OKASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l����", (getRule(RULE_OKASUUJUN) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"�O����",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					for (int k = 1; k <= (7-3); k++)
						if ((analysis->ShunziCount[i + k] >= 1) &&
							(analysis->ShunziCount[i + k + 1] >= 1) &&
							(analysis->ShunziCount[i + k + 2] >= 1) &&
							(analysis->ShunziCount[i + k + 3] >= 1)) yakuFlag = true;
				}
				return yakuFlag;
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					for (int k = 1; k <= (7-2); k++)
						if ((analysis->ShunziCount[i + k] >= 1) &&
							(analysis->ShunziCount[i + k + 1] >= 1) &&
							(analysis->ShunziCount[i + k + 2] >= 1)) yakuFlag = true;
				}
				return yakuFlag;
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					for (int k = 1; k <= (7-4); k++)
						if ((analysis->ShunziCount[i + k] >= 1) &&
							(analysis->ShunziCount[i + k + 2] >= 1) &&
							(analysis->ShunziCount[i + k + 4] >= 1)) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ��F�l���� */
	auto puukao =
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis,
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
	const std::array<char[8], 42> parsedat_bichrome4 = {
		"0001","0002","0010","0011","0020","0022","0100",
		"0101","0110","0111","0200","0202","0220","0222",
		"1000","1001","1010","1011","1100","1101","1110",
		"1112","1121","1122","1211","1212","1221","1222",
		"2000","2002","2020","2022","2111","2112","2121",
		"2122","2200","2202","2211","2212","2220","2221",
	};
	const std::array<char[4], 18> parsedat_bichrome3 = {
		"001","002","010","011","020","022",
		"100","101","110","112","121","122",
		"200","202","211","212","220","221",
	};
	if (getRule(RULE_NISHOKU_OKASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�l����", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"��F�O����",
			[puukao, parsedat_bichrome4](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* ��F�O���� */
	if (getRule(RULE_NISHOKU_OKASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�O����", yaku::yakuCalculator::Yaku::yval_1han,
			[puukao, parsedat_bichrome3](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* ��F�R�l�� */
	if (getRule(RULE_NISHOKU_YAMASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��G�A��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"�G�A��",
			[puukao, parsedat_bichrome4](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* ��F�R�O�� */
	if (getRule(RULE_NISHOKU_YAMASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�G�A��", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
			[puukao, parsedat_bichrome3](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* �O�F�l���� */
	const std::array<char[8], 36> parsedat_trichrome4 = {
		"0012","0021","0102","0112","0120","0121",
		"0122","0201","0210","0211","0212","0221",
		"1002","1012","1020","1021","1022","1102",
		"1120","1200","1201","1202","1210","1220",
		"2001","2010","2011","2012","2021","2100",
		"2101","2102","2110","2120","2201","2210",
	};
	const std::array<char[4], 6> parsedat_trichrome3 = {
		"012","021","102","120","201","210",
	};
	if (getRule(RULE_SANSHOKU_OKASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�l����", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"�O�F�O����",
			[puukao, parsedat_trichrome4](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
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
			[puukao, parsedat_trichrome3](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* �O�F�R�l�� */
	if (getRule(RULE_SANSHOKU_YAMASUUJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�卬�A��", (getRule(RULE_SANSHOKU_YAMASUUJUN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
			"���A��",
			[puukao, parsedat_trichrome4](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* �O�F�R�O�� */
	if (getRule(RULE_NISHOKU_YAMASANJUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��",  (getRule(RULE_NISHOKU_YAMASANJUN) == 1) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han :
			((getRule(RULE_NISHOKU_YAMASANJUN) == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_2han_kuisagari),
			[puukao, parsedat_trichrome3](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(gameStat, analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));
}
