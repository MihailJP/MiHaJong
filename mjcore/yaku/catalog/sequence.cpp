#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_sequence() {
	/* ��F�o���� */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�o����"), yaku::yakuCalculator::Yaku::yval_64,
			_T("����F"), _T("���a"), _T("��ʍ�"), _T("��ʍ�x2"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("isshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�o����"), get_yaku_han("isshoku_souryuu"),
			_T("����F"), _T("��u��"), _T("����u��"),
#endif /* GUOBIAO */
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
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�l����"), yaku::yakuCalculator::Yaku::yval_48,
			_T("��ʍ�"), _T("��ʍ�x2"), _T("��F�O����"), _T("�l�A��"), _T("�l�A��x2"), _T("�l�A��x3"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("isshoku_suujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�l��"), get_yaku_han("isshoku_suujun"),
			_T("��F�O��"), _T("��u��"), _T("��u��"), _T("����u��"), _T("�d�l�A�l"), _T("�l�A�l"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ��F�O�� */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�O����"), yaku::yakuCalculator::Yaku::yval_24,
			_T("��ʍ�"), _T("��ʍ�x2"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("isshoku_sanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�O��"), get_yaku_han("isshoku_sanjun"),
			_T("��u��"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 3) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* ��u�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("��ʍ�"), yaku::yakuCalculator::Yaku::yval_1,
#else /* GUOBIAO */
		_T("��u��"), yaku::yakuCalculator::Yaku::yval_1han_menzen,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TileSuitHonors; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 1);
		}
	));
	/* ��u�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("��ʍ�x2"), yaku::yakuCalculator::Yaku::yval_2,
		_T("��ʍ�"),
#else /* GUOBIAO */
		_T("��u��"), get_yaku_han("ryanpeikou"),
		_T("��u��"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			int yakuFlagCount = 0;
			for (int i = 1; i < TileSuitHonors; i++)
				if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
			return (yakuFlagCount == 2);
		}
	));
#ifndef GUOBIAO
	/* �勾 */
	if (RuleData::chkRuleApplied("ohkagami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�勾"), get_yaku_han("ohkagami"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return ((yakuFlagCount == 2)&&(analysis->MianziDat[0].tile == CircleOne));
			}
		));
	/* ���u�� */
	if (RuleData::chkRuleApplied("chinpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���u��"), get_yaku_han("chinpeikou"),
			_T("��u��"), _T("���i�΂̗d��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlagCount = false;
				for (int i = 1; i <= 7; i++) {
					if ((analysis->ShunziCount[i + TileSuitCharacters] == 2) &&
						(analysis->ShunziCount[i + TileSuitCircles] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TileSuitCharacters] == 2) &&
						(analysis->ShunziCount[i + TileSuitBamboos] == 2))
						yakuFlagCount = true;
					if ((analysis->ShunziCount[i + TileSuitCircles] == 2) &&
						(analysis->ShunziCount[i + TileSuitBamboos] == 2))
						yakuFlagCount = true;
				}
				return yakuFlagCount;
			}
		));
	/* ���i�΂̗d�� */
	if (RuleData::chkRuleApplied("biwakonoyousei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���i�΂̗d��"), get_yaku_han("biwakonoyousei"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlagCount = false;
				if ((analysis->ShunziCount[CharacterOne] == 2) &&
					(analysis->ShunziCount[CircleOne] == 2))
					yakuFlagCount = true;
				if ((analysis->ShunziCount[CharacterOne] == 2) &&
					(analysis->ShunziCount[BambooOne] == 2))
					yakuFlagCount = true;
				if ((analysis->ShunziCount[CircleOne] == 2) &&
					(analysis->ShunziCount[BambooOne] == 2))
					yakuFlagCount = true;
				return yakuFlagCount;
			}
		));
	/* ����u�� */
	if (RuleData::chkRuleApplied("exposed_ryanpeikou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����u��"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yakuFlagCount = 0;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 2) ++yakuFlagCount;
				return (yakuFlagCount == 2);
			}
		));
	/* �k�l��(��F�l��) */
	if (RuleData::chkRuleApplied("pei4jun_isshoku_suujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�k�l�� "), get_yaku_han("pei4jun_isshoku_suujun"),
			_T("��F�l��"), _T("��F�O��"), _T("��u��"), _T("��u��"), _T("����u��"), _T("�d�l�A�l"), _T("�l�A�l"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if (analysis->ShunziCount[i] == 4) yakuFlag = true;
				return yakuFlag && (analysis->MianziDat[0].tile == NorthWind);
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �T���V���N�Ƃ��T���V�L�Ƃ��Ă΂�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�O�F�O����"), yaku::yakuCalculator::Yaku::yval_8,
#else /* GUOBIAO */
		_T("�O�F����"), yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 7; i++)
				if ((analysis->ShunziCount[i + TileSuitCharacters] >= 1) &&
					(analysis->ShunziCount[i + TileSuitCircles] >= 1) &&
					(analysis->ShunziCount[i + TileSuitBamboos] >= 1)) yakuFlag = true;
			return yakuFlag;
		}
	));

	// ---------------------------------------------------------------------

	/* �C�[�`�[�g���J�� */
	auto ikki_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis, bool* const yakuFlag, int* const yakuCol) -> void {
			*yakuFlag = false;
			for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
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
			for (int i = 0; i < SizeOfMeldBuffer; i++) {
				if ((analysis->MianziDat[i].tile / TileSuitStep) ==
					(*yakuCol / TileSuitStep)) ++samecol;
				else if ((analysis->MianziDat[i].tile / TileSuitStep) ==
					(TileSuitHonors / TileSuitStep)) ++samecol;
			}
			*yakuFlag = (samecol == SizeOfMeldBuffer); // ��F�ɂȂ��Ă��邩�ǂ���
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("����"), yaku::yakuCalculator::Yaku::yval_24,
#else /* GUOBIAO */
		_T("��C�ʊ�"), yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
#endif /* GUOBIAO */
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return yakuFlag;
			}
	));
#ifndef GUOBIAO
	/* 20101204�j�����`�V�X�S�� */
	if (RuleData::chkRuleApplied("tohoku_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���k�V����"), get_yaku_han("tohoku_shinkansen"),
			_T("��C�ʊ�"), _T("����F"),
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
			for (int i = 0; i < SizeOfMeldBuffer; i++) {
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
			_T("��C�ʊёS�ћ��"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("��C�ʊ�"),
			[ikki_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = false;
				ikki_tsuukan(analysis, &yakuFlag, nullptr);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����C�ʊёS�ћ��"), yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			_T("��C�ʊ�"),
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
			_T("�ܐS�ʊ�"), get_yaku_han("uushintonkan"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol = 0;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // �ƒ��҂���
					(analysis->PlayerStat->Hand[NumOfTilesInHand].tile == (yakuCol + 5)) // �a���v��5
					);
			}
		));
	/* ���Y�� */
	if (RuleData::chkRuleApplied("longxizhu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���Y��"), get_yaku_han("longxizhu"),
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
			_T("�����Y�k�l"), get_yaku_han("jinlong_dou_beidou"),
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
		const TileCode tmpTileCodeList[Rules][2] = {
			{EastWind, BambooEight}, {SouthWind, RedDragon}, {WestWind, CircleEight}, {NorthWind, CharacterEight},
			{WhiteDragon, BambooTwo}, {GreenDragon, CharacterTwo}, {RedDragon, CircleTwo},
			{WhiteDragon, CharacterOne}, {GreenDragon, CircleOne}, {RedDragon, BambooOne},
		};
		const int tmpStraightColorList[Rules] = {
			TileSuitCharacters, TileSuitCircles, TileSuitBamboos, TileSuitCircles, TileSuitCharacters,
			TileSuitCircles, TileSuitBamboos, TileSuitCharacters, TileSuitCircles, TileSuitBamboos,
		};
		const TCHAR tmpYakuNameList[Rules][16] = {
			_T("�o���}�_"), _T("�웠�ӏ�"), _T("�����[��"), _T("���Q����"), _T("�f���c��"),
			_T("�h��t��"), _T("�ԍ`�ϋ�"), _T("�O杈�"), _T("�ȉ@����"), _T("���ΏH��"),
		};
		for (int i = 0; i < Rules; i++) {
			if (!RuleData::chkRuleApplied(tmpRuleCodeList[i])) continue;
			TileCode tmpTileCode1 = tmpTileCodeList[i][0];
			TileCode tmpTileCode2 = tmpTileCodeList[i][1];
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
			_T("���̐�"), get_yaku_han("azure_dragon"),
			_T("��C�ʊ�"), _T("����F"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[EastWind] >= 1) && // ���̍��q������
					(analysis->MianziDat[0].tile == WhiteDragon) && // �A�^�}������
					(yakuCol == TileSuitCircles) ); // �C�b�c�[���������q
			}
		));
	/* �k�̌��� */
	if (RuleData::chkRuleApplied("black_tortoise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�k�̌���"), get_yaku_han("black_tortoise"),
			_T("��C�ʊ�"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[NorthWind] >= 1) && // �k�̍��q������
					(analysis->MianziDat[0].tile == BambooSeven) && // �A�^�}��������
					(yakuCol == TileSuitCircles) ); // �C�b�c�[���������q
			}
		));
	/* �H�c�V���� */
	if (RuleData::chkRuleApplied("akita_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�H�c�V����"), get_yaku_han("akita_shinkansen"),
			_T("��C�ʊ�"), _T("����F"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->KeziCount[NorthWind] >= 1) && // �k�̍��q������
					((analysis->MianziDat[0].tile / TileSuitStep) == (yakuCol / TileSuitStep)) && // ��F�ɂȂ��Ă���
					(analysis->PlayerStat->Hand[NumOfTilesInHand].tile == (yakuCol + 5)) // �a���v��5
					);
			}
		));
	/* ��z�V�����Ƃ� */
	if (RuleData::chkRuleApplied("joetsu_toki"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��z�V�����Ƃ�"), get_yaku_han("joetsu_toki"),
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
			_T("��z�V����������"), get_yaku_han("joetsu_asahi"),
			_T("��z�V�����Ƃ�"), _T("��C�ʊ�"), _T("����F"),
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
			_T("���A���D"), get_yaku_han("seikan_ship"),
			_T("��C�ʊ�"), _T("����F"),
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
					(yakuCol == TileSuitCircles) && // ���q�̃C�b�c�[��
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
				_T("�S�[���h���b�V��"), yaku::yakuCalculator::Yaku::yval_2han,
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "2han_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�S�[���h���b�V��"), yaku::yakuCalculator::Yaku::yval_2han,
				goldrush2
			));
		else if (RuleData::chkRule("goldrush", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�S�[���h���b�V��"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("��C�ʊ�"), _T("����F"),
				goldrush1
			));
		else if (RuleData::chkRule("goldrush", "yakuman_require_west"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�S�[���h���b�V��"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("��C�ʊ�"), _T("����F"),
				goldrush2
			));
	}
	/* ���[�g�Q�S�U */
	if (RuleData::chkRuleApplied("route_246"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���[�g�Q�S�U"), get_yaku_han("route_246"),
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
			_T("�㗴�闎�n"), get_yaku_han("gaulungsing_lokdei"),
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
			_T("�k�C���V����"), get_yaku_han("hokkaido_shinkansen"),
			_T("��C�ʊ�"), _T("����F"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; ikki_tsuukan(analysis, &yakuFlag, nullptr);
				return (yakuFlag && // ��C�ʊт��������Ă���
					(analysis->AnKangziCount[WhiteDragon] >= 1) && // ���̈ÞȂ�������
					(analysis->MianziDat[0].tile == NorthWind) ); // �������k
			}
		));
	/* �ܒ܂̗� */
	if (RuleData::chkRuleApplied("five_claw_dragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ܒ܂̗�"), get_yaku_han("five_claw_dragon"),
			_T("��C�ʊ�"), _T("����F"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(analysis->DuiziCount[RedDragon] >= 1) && // ���̐���
					(analysis->KeziCount[BambooFive] >= 1) && // �܍��̍��q
					(analysis->Machi == yaku::yakuCalculator::machiShanpon) // �V�����|���҂�
					);
			}
		));
	/* �����l�ܗ� */
	if (RuleData::chkRuleApplied("ryukyu_four_claw_dragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����l�ܗ�"), get_yaku_han("ryukyu_four_claw_dragon"),
			_T("��C�ʊ�"), _T("����F"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(analysis->DuiziCount[BambooNine] >= 1) && // ����̐���
					(analysis->KeziCount[BambooFour] >= 1) && // �l���̍��q
					(analysis->Machi == yaku::yakuCalculator::machiShanpon) // �V�����|���҂�
					);
			}
		));
	/* ��V�S�� */
	if (RuleData::chkRuleApplied("southern_stars"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��V�S��"), get_yaku_han("southern_stars"),
			_T("��C�ʊ�"), _T("����F"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(analysis->DuiziCount[CircleThree] >= 1) && // �O���̐���
					(analysis->KeziCount[SouthWind] >= 1) // ��̍��q
					);
			}
		));
	/* ���C���V������K�� */
	if (RuleData::chkRuleApplied("tokaido_shinkansen_double"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���C���V������K��"), get_yaku_han("tokaido_shinkansen_double"),
			_T("��C�ʊ�"), _T("����F"),
			[ittsuu_monotonic](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ittsuu_monotonic(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт���F��
					(analysis->DuiziCount[EastWind] >= 1) && // ���̐���
					((analysis->ShunziCount[CharacterTwo] >= 1) || // �Q�R�S�̏��q
					(analysis->ShunziCount[CircleTwo] >= 1) || // �Q�R�S�̏��q
					(analysis->ShunziCount[BambooTwo] >= 1)) // �Q�R�S�̏��q
					);
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �S�[���f���Q�[�g�u���b�W */
	auto chkGoldenGateBridge =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
				if ((analysis->ShunziCount[i + 1] >= 1) &&
					(analysis->ShunziCount[i + 3] >= 1) &&
					(analysis->ShunziCount[i + 5] >= 1) &&
					(analysis->ShunziCount[i + 7] >= 1)) yakuFlag = true;
			return yakuFlag;
		};
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�l����"), yaku::yakuCalculator::Yaku::yval_32,
			_T("��F�O����"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("golden_gate_bridge"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���勴"), get_yaku_han("golden_gate_bridge"),
			_T("���A��"),
#endif /* GUOBIAO */
			chkGoldenGateBridge
		));
#ifndef GUOBIAO
	/* �S�[���f���Q�[�g�u���b�W */
	if (RuleData::chkRuleApplied("naruto_bridge"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��勴"), get_yaku_han("naruto_bridge"),
			_T("���A��"), _T("���勴"),
			[chkGoldenGateBridge](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkGoldenGateBridge(analysis) && (analysis->MianziDat[0].tile == CircleOne);
			}
		));
	/* Arc de Triomphe */
	if (RuleData::chkRuleApplied("gaisenmon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�M����"), get_yaku_han("gaisenmon"),
			_T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
					int yakuFlagCount = 0;
					if ((analysis->ShunziCount[i + 0] >= 1) && (analysis->ShunziCount[i + 3] >= 2) && (analysis->ShunziCount[i + 6] >= 1)) {
						yakuFlag = true;
					} else if ((analysis->ShunziCount[i + 0] >= 1) && (analysis->ShunziCount[i + 3] == 1) && (analysis->ShunziCount[i + 6] >= 1)) {
						continue;
					} else {
						for (int j = 1; j <= 4; j++)
							for (int k = 1; k <= 2; k++)
								if ((analysis->ShunziCount[i + j] >= k) && (analysis->ShunziCount[i + j + 3] >= k))
									++yakuFlagCount;
					}
					if ((yakuFlagCount == 2) && (analysis->MianziDat[0].tile == i + 7))
						yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ���� */
	if (RuleData::chkRuleApplied("choujou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����"), get_yaku_han("choujou"),
			_T("��C�ʊ�"), _T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
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
			_T("���ݓV"), get_yaku_han("qinglong_zaitian"),
			_T("��C�ʊ�"), _T("����F"), _T("��u��"),
			[ikki_tsuukan](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag; int yakuCol;
				ikki_tsuukan(analysis, &yakuFlag, &yakuCol);
				return (yakuFlag && // ��C�ʊт�
					((analysis->MianziDat[0].tile / TileSuitStep) == (yakuCol / TileSuitStep)) && // ��F��
					((analysis->ShunziCount[yakuCol + 1] >= 2) ||
					(analysis->ShunziCount[yakuCol + 4] >= 2) ||
					(analysis->ShunziCount[yakuCol + 7] >= 2)) // ��u���ɂȂ��Ă���
					);
			}
		));
	/* �����a */
	if (RuleData::chkRuleApplied("kyantonhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����a"), get_yaku_han("kyantonhoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < (TileSuitHonors * 2); i += TileSuitStep * 2) {
					int yakuTmpFlag = 0;
					for (int k = 1; k <= 7; k++)
						if ((analysis->ShunziCount[i % TileSuitHonors + k] >= 1) &&
							(analysis->ShunziCount[(i + TileSuitStep) % TileSuitHonors + k] >= 1))
								++yakuTmpFlag;
					if (yakuTmpFlag == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �l���� */
	auto puukao =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suupuukao) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (7 - step * (suupuukao ? 3 : 2)); k++)
					if ((analysis->ShunziCount[(int)(parsedat[i * fldsize + 0] - _T('0')) * TileSuitStep + step * 0 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 1] - _T('0')) * TileSuitStep + step * 1 + k] >= 1) &&
						(analysis->ShunziCount[(int)(parsedat[i * fldsize + 2] - _T('0')) * TileSuitStep + step * 2 + k] >= 1) &&
						((!suupuukao)||(analysis->ShunziCount[(int)(parsedat[i * fldsize + 3] - _T('0')) * TileSuitStep + step * 3 + k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�l����"), yaku::yakuCalculator::Yaku::yval_32,
			_T("��F�O����"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l����"), get_yaku_han("okasuujun"),
			_T("�O����"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
			}
		));
	/* �O���� */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�O����"), yaku::yakuCalculator::Yaku::yval_16,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�O����"), get_yaku_han("okasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
			}
		));
	/* �R�O�� */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�O����"), yaku::yakuCalculator::Yaku::yval_16,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���A��"), get_yaku_han("yamasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, false);
			}
		));
#ifndef GUOBIAO
	/* ��F�l���� */
	if (RuleData::chkRuleApplied("nishoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�l����"), get_yaku_han("nishoku_okasuujun"),
			_T("��F�O����"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* ��F�O���� */
	if (RuleData::chkRuleApplied("nishoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�O����"), get_yaku_han("nishoku_okasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* ��F�R�l�� */
	if (RuleData::chkRuleApplied("nishoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��G�A��"), get_yaku_han("nishoku_yamasuujun"),
			_T("�G�A��"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* ��F�R�O�� */
	if (RuleData::chkRuleApplied("nishoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�G�A��"), get_yaku_han("nishoku_yamasanjun"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* �O�F�l���� */
	if (RuleData::chkRuleApplied("sanshoku_okasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�l����"), get_yaku_han("sanshoku_okasuujun"),
			_T("�O�F�O����"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
#endif /* GUOBIAO */
	/* �O�F�O���� */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�O����"), yaku::yakuCalculator::Yaku::yval_6,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("sanshoku_okasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�O����"), get_yaku_han("sanshoku_okasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
#ifndef GUOBIAO
	/* �O�F�R�l�� */
	if (RuleData::chkRuleApplied("sanshoku_yamasuujun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�卬�A��"), get_yaku_han("sanshoku_yamasuujun"),
			_T("���A��"),
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
#endif /* GUOBIAO */
	/* �O�F�R�O�� */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�O����"), yaku::yakuCalculator::Yaku::yval_6,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("sanshoku_yamasanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���A��"), get_yaku_han("sanshoku_yamasanjun"),
#endif /* GUOBIAO */
			[puukao](const MENTSU_ANALYSIS* const analysis) -> bool {
				return puukao(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* �o������ */
	if (RuleData::chkRuleApplied("shanron_chonchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�o������"), get_yaku_han("shanron_chonchuu"),
			_T("�����a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 4; i++)
					if ((analysis->ShunziCount[TileSuitCharacters + i] >= 1) &&
						(analysis->ShunziCount[TileSuitCharacters + i + 3] >= 1) &&
						(analysis->ShunziCount[TileSuitBamboos + i] >= 1) &&
						(analysis->ShunziCount[TileSuitBamboos + i + 3] >= 1) &&
						((analysis->MianziDat[0].tile / TileSuitStep) == (TileSuitCircles / TileSuitStep)))
							yakuFlag = true;
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �ԗ� */
	auto sanshoku_tsuukan =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
				if ((analysis->ShunziCount[(int)((*k)[0] - _T('0')) * TileSuitStep + 1] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[1] - _T('0')) * TileSuitStep + 4] >= 1) &&
					(analysis->ShunziCount[(int)((*k)[2] - _T('0')) * TileSuitStep + 7] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};

#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ԗ�"), yaku::yakuCalculator::Yaku::yval_8,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("hualong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�ʊ�"), get_yaku_han("hualong"),
#endif /* GUOBIAO */
			sanshoku_tsuukan
		));
#ifndef GUOBIAO
	/* �P�� */
	if (RuleData::chkRuleApplied("tanron"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�P��"), get_yaku_han("tanron"),
			_T("�O�F�ʊ�"),
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
			_T("�O�F�ʊёS�ћ��"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("�O�F�ʊ�"),
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting( analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai > 0));
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���O�F�ʊёS�ћ��"), yaku::yakuCalculator::Yaku::yval_4han_kuisagari,
			_T("�O�F�ʊ�"),
			[sanshoku_tsuukan, ittsuu_chanta_counting](const MENTSU_ANALYSIS* const analysis) -> bool {
				int yaojiu, zipai; bool yakuFlag = sanshoku_tsuukan(analysis);
				ittsuu_chanta_counting(analysis, &yaojiu, &zipai);
				return ((yakuFlag) && (yaojiu == 4) && (zipai == 0));
			}
		));
	}

	// ---------------------------------------------------------------------

	/* �o���o�T */
#endif /* GUOBIAO */
	auto ShuangLong =
		[](const MENTSU_ANALYSIS* const analysis, int suit1, int suit2, TileCode head) -> bool {
			return ((analysis->ShunziCount[suit1 + 1] >= 1) &&
				(analysis->ShunziCount[suit1 + 7] >= 1) &&
				(analysis->ShunziCount[suit2 + 1] >= 1) &&
				(analysis->ShunziCount[suit2 + 7] >= 1) &&
				(analysis->MianziDat[0].tile == head));
		};

#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("shanron_shankui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�o���o�T"), get_yaku_han("shanron_shankui"),
			_T("�����a"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooThree);
			}
		));
	/* �o������(�V�����^�C�v) */
	if (RuleData::chkRuleApplied("shanron_chonchu_lsf"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�o������ "), get_yaku_han("shanron_chonchu_lsf"),
			_T("�����a"), _T("�O�F�o����"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitBamboos, CircleFive);
			}
		));
#endif /* GUOBIAO */
	/* �O�F�o���� */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�o����"), yaku::yakuCalculator::Yaku::yval_16,
			_T("���a"),
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("sanshoku_souryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�o����"), get_yaku_han("sanshoku_souryuu"),
			_T("�����a"),
#endif /* GUOBIAO */
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); k++)
					if (ShuangLong(analysis, (int)((*k)[0] - _T('0')) * TileSuitStep,
						(int)((*k)[1] - _T('0')) * TileSuitStep,
						(TileCode)((int)((*k)[1] - _T('0')) * TileSuitStep + 5))) return true;
				return false;
			}
		));
#ifndef GUOBIAO
	/* �o���Y�P */
	if (RuleData::chkRuleApplied("shanron_shiifon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�o���Y�P"), get_yaku_han("shanron_shiifon"),
			_T("�����a"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooOne);
			}
		));
	/* �o��� */
	if (RuleData::chkRuleApplied("shanron_paochuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�o���"), get_yaku_han("shanron_paochuu"),
			_T("�����a"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooTwo) ||
					ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooFour) ||
					ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooSix));
			}
		));
	/* �o���Y�T */
	if (RuleData::chkRuleApplied("shanron_shiikui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�o���Y�T"), get_yaku_han("shanron_shiikui"),
			_T("�����a"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCharacters, TileSuitCircles, BambooSeven);
			}
		));
	/* �o������ */
	if (RuleData::chkRuleApplied("shanron_touja"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�o���Y�T"), get_yaku_han("shanron_touja"),
			_T("�����a"),
			[ShuangLong](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ShuangLong(analysis, TileSuitCircles, TileSuitBamboos, CharacterOne);
			}
		));

	// ---------------------------------------------------------------------

	/* �h�O�� */
	auto kanchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++)
				if (analysis->MianziDat[i].mstat == meldSequenceExposedMiddle) ++count;
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h�O��"), get_yaku_han("chii_sankan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 3);
			}
		));
	/* �h�l�� */
	if (RuleData::chkRuleApplied("chii_suukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h�l��"), get_yaku_han("chii_suukan"),
			[kanchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (kanchanCount(analysis) == 4);
			}
		));
	/* �h�O�� */
	auto penchanCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++) {
				for (int j = 0; j < TileSuitHonors; j += TileSuitStep) {
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedLower) &&
						(analysis->MianziDat[i].tile == TileSuitStep + 7)) ++count;
					if ((analysis->MianziDat[i].mstat == meldSequenceExposedUpper) &&
						(analysis->MianziDat[i].tile == TileSuitStep + 1)) ++count;
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_sanpen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h�O��"), get_yaku_han("chii_sanpen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 3);
			}
		));
	/* �h�l�� */
	if (RuleData::chkRuleApplied("chii_suupen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h�l��"), get_yaku_han("chii_suupen"),
			[penchanCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (penchanCount(analysis) == 4);
			}
		));
	/* �h���� */
	auto xiaoShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedLower) {
					++count;
					for (int j = 0; j < TileSuitHonors; j += TileSuitStep) {
						if (analysis->MianziDat[i].tile == TileSuitStep + 7) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_shaoshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h����"), get_yaku_han("chii_shaoshun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 3);
			}
		));
	/* �h���l�� */
	if (RuleData::chkRuleApplied("chii_shaosuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h���l��"), get_yaku_han("chii_shaosuushun"),
			[xiaoShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (xiaoShunCount(analysis) == 4);
			}
		));
	/* �h�叇 */
	auto daShunCount =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			int count = 0;
			for (int i = 1; i <= SizeOfMeldBuffer; i++) {
				if (analysis->MianziDat[i].mstat == meldSequenceExposedUpper) {
					++count;
					for (int j = 0; j < TileSuitHonors; j += TileSuitStep) {
						if (analysis->MianziDat[i].tile == TileSuitStep + 1) --count;
					}
				}
			}
			return count;
		};
	if (RuleData::chkRuleApplied("chii_taashun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h�叇"), get_yaku_han("chii_taashun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 3);
			}
		));
	/* �h��l�� */
	if (RuleData::chkRuleApplied("chii_taasuushun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�h��l��"), get_yaku_han("chii_taasuushun"),
			[daShunCount](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (daShunCount(analysis) == 4);
			}
		));
#endif /* GUOBIAO */
}
