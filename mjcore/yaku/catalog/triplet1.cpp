#include "../catalog.h"

#include "../../func.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_1() {
	/* �l�Í� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�l�Í�"), yaku::yakuCalculator::Yaku::yval_64,
		_T("\u78b0\u78b0�a"), _T("�O�Í�"), _T("�o�Í�"), _T("��O��"),
#else /* GUOBIAO */
		_T("�l�Í�"), yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		_T("�΁X�a"), _T("�O�Í�"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 4);
		}
	));
#ifndef GUOBIAO
	/* �l�Í��P�R */
	if (RuleData::chkRuleApplied("double_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�Í��P�R"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("�l�Í�"), _T("�΁X�a"), _T("�O�Í�"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && (
					(analysis->GameStat->TianHuFlag) ||
					((analysis->Machi == yaku::yakuCalculator::machiTanki) && (!analysis->MachiInfo.FuritenFlag))
					);
			}
		));
#endif /* GUOBIAO */
	/* �O�Í� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�O�Í�"), yaku::yakuCalculator::Yaku::yval_16,
		_T("�o�Í�"),
#else /* GUOBIAO */
		_T("�O�Í�"), yaku::yakuCalculator::Yaku::yval_2han,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 3);
		}
	));
#ifdef GUOBIAO
	/* �o�Í� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�o�Í�"), yaku::yakuCalculator::Yaku::yval_2,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
				1 : 0) == 2);
		}
	));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* �ܘA�� */
	auto wulianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * 4); k++)
					if ((analysis->DuiziCount[(int)(parsedat[i * fldsize + 0] - _T('0')) * TileSuitStep + k + step * 0] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - _T('0')) * TileSuitStep + k + step * 1] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - _T('0')) * TileSuitStep + k + step * 2] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - _T('0')) * TileSuitStep + k + step * 3] >= 1) &&
						(analysis->DuiziCount[(int)(parsedat[i * fldsize + 4] - _T('0')) * TileSuitStep + k + step * 4] >= 1) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#endif /* GUOBIAO */
	auto lianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suurnkoh) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * (suurnkoh ? 3 : 2)); k++)
					if ((analysis->KeziCount[(int)(parsedat[i * fldsize + 0] - _T('0')) * TileSuitStep + k + step * 0] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - _T('0')) * TileSuitStep + k + step * 1] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - _T('0')) * TileSuitStep + k + step * 2] >= 1) &&
						((!suurnkoh)||(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - _T('0')) * TileSuitStep + k + step * 3] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("golden_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����"), get_yaku_han("golden_ladder"),
			_T("����F"), _T("�΁X�a"), _T("���l�A��"), _T("�l�A��"), _T("�O�A��"), _T("���O�A��"),
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_monochrome5[0], 3, 8, 1);
			}
		));
#endif /* GUOBIAO */
	/* �l�A�� */
	auto suurenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
		};
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��F�l�ߍ�"), yaku::yakuCalculator::Yaku::yval_48,
		_T("\u78b0\u78b0�a"), _T("��F�O�ߍ�"),
		suurenkoh
	));
#else /* GUOBIAO */
	if (RuleData::chkRule("suurenkoh", "8han_incl_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�A��"), yaku::yakuCalculator::Yaku::yval_6han /* �΁X�a����8�� */,
			_T("���l�A��"), _T("�O�A��"), _T("���O�A��"),
			suurenkoh
		));
	else if (RuleData::chkRuleApplied("suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�A��"), get_yaku_han("suurenkoh"),
			_T("�΁X�a"), _T("���l�A��"), _T("�O�A��"), _T("���O�A��"),
			suurenkoh
		));
	/* �l�A�����ʑ҂� */
	if (RuleData::chkRuleApplied("suurenkoh_double"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�A�����ʑ҂�"), get_yaku_han("suurenkoh_double"),
			_T("����F"), _T("�΁X�a"), _T("����"), _T("�l�A��"), _T("���l�A��"), _T("�O�A��"), _T("���O�A��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (auto i = parsedat_monochrome5.begin(); i != parsedat_monochrome5.end(); i++) {
					for (int k = 1; k <= 5; k++)
						if ((analysis->DuiziCount[(int)((*i)[0] - _T('0')) * TileSuitStep + k + 0] >= 1) &&
							(analysis->KeziCount[(int)((*i)[1] - _T('0')) * TileSuitStep + k + 1] >= 1) &&
							(analysis->KeziCount[(int)((*i)[2] - _T('0')) * TileSuitStep + k + 2] >= 1) &&
							(analysis->KeziCount[(int)((*i)[3] - _T('0')) * TileSuitStep + k + 3] >= 1) &&
							(analysis->DuiziCount[(int)((*i)[4] - _T('0')) * TileSuitStep + k + 4] >= 1) &&
							((analysis->TsumoHai->tile == (TileCode)((int)((*i)[0] - _T('0')) * TileSuitStep + k + 0)) ||
							(analysis->TsumoHai->tile == (TileCode)((int)((*i)[4] - _T('0')) * TileSuitStep + k + 4)))
							)
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */
	/* �O�A�� */
	auto sanrenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
		};
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("sanrenkoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("��F�O�ߍ�"), yaku::yakuCalculator::Yaku::yval_24,
#else /* GUOBIAO */
			_T("�O�A��"), get_yaku_han("sanrenkoh"),
			_T("���O�A��"),
#endif /* GUOBIAO */
			sanrenkoh
		));
#ifndef GUOBIAO
	/* ���l�A�� */
	if (RuleData::chkRuleApplied("shou_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���l�A��"), get_yaku_han("shou_suurenkoh"),
			_T("�O�A��"), _T("���O�A��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
					for (int k = 1; k <= 6; k++)
						if ((analysis->DuiziCount[i + k + 0] >= 1) && (analysis->KeziCount[i + k + 1] >= 1) &&
							(analysis->KeziCount[i + k + 2] >= 1) && (analysis->DuiziCount[i + k + 3] >= 1))
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ���O�A�� */
	if (RuleData::chkRuleApplied("shou_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���O�A��"), get_yaku_han("shou_sanrenkoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep) {
					for (int k = 1; k <= 7; k++)
						if ((analysis->DuiziCount[i + k + 0] >= 1) && (analysis->DuiziCount[i + k + 1] >= 1) &&
							(analysis->DuiziCount[i + k + 2] >= 1))
							yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* �������� */
	if (RuleData::chkRuleApplied("kagamine_len"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��������"), get_yaku_han("kagamine_len"),
			/* �O�A���ɒǉ������ */
			[sanrenkoh](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* ���A�ȕ��̎O�F */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�O����"), yaku::yakuCalculator::Yaku::yval_16,
		_T("�o����"),
#else /* GUOBIAO */
		_T("�O�F����"), get_yaku_han("sanshoku_doukoh"),
		_T("�O�F������"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 9; i++)
				if ((analysis->KeziCount[i + TileSuitCharacters] >= 1) &&
					(analysis->KeziCount[i + TileSuitCircles] >= 1) &&
					(analysis->KeziCount[i + TileSuitBamboos] >= 1))
					yakuFlag = true;
			return yakuFlag;
		}
	));
#ifndef GUOBIAO
	/* �l�F�����i���� */
	if (RuleData::chkRuleApplied("yonshoku_doukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�F����"), get_yaku_han("yonshoku_doukoh"),
			_T("�O�F����"), _T("�O�F������"), _T("�΁X�a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TileSuitCharacters] >= 1) &&
						(analysis->KeziCount[i + TileSuitCircles] >= 1) &&
						(analysis->KeziCount[i + TileSuitBamboos] >= 1) &&
						(analysis->KeziCount[WhiteDragon] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
	/* �O�F������ */
	if (RuleData::chkRuleApplied("sanshoku_shoudoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F������"), get_yaku_han("sanshoku_shoudoukoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->DuiziCount[i + TileSuitCharacters] >= 1) &&
						(analysis->DuiziCount[i + TileSuitCircles] >= 1) &&
						(analysis->DuiziCount[i + TileSuitBamboos] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �O���a�E�l��a����p */
	auto WindCnt =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			return analysis->DuiziCount[EastWind] + analysis->KeziCount[EastWind] +
				 analysis->DuiziCount[SouthWind] + analysis->KeziCount[SouthWind] +
				  analysis->DuiziCount[WestWind] + analysis->KeziCount[WestWind] +
				  analysis->DuiziCount[NorthWind] + analysis->KeziCount[NorthWind];
		};
	auto DragonCnt =
		[](const MENTSU_ANALYSIS* const analysis) -> int {
			return analysis->DuiziCount[WhiteDragon] + analysis->KeziCount[WhiteDragon] +
				 analysis->DuiziCount[GreenDragon] + analysis->KeziCount[GreenDragon] +
				  analysis->DuiziCount[RedDragon] + analysis->KeziCount[RedDragon];
		};
	/* ���O�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("���O��"), yaku::yakuCalculator::Yaku::yval_64,
		_T("����"), _T("�o����"),
#else /* GUOBIAO */
		_T("���O��"), yaku::yakuCalculator::Yaku::yval_2han,
		_T("�t�ɉ����Ȃ�"), _T("�t�ɉ�������"), /* ��v2�͕K���������� */
#endif /* GUOBIAO */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* ��O�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("��O��"), yaku::yakuCalculator::Yaku::yval_88,
		_T("����"), _T("�o����"),
#else /* GUOBIAO */
		_T("��O��"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("��v�E��"), _T("��v�E�"), _T("��v�E��"), _T("�t�ɉ����Ȃ�"), _T("�t�ɉ�������"),
#endif /* GUOBIAO */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
#ifndef GUOBIAO
	/* ��O��O�� */
	if (RuleData::chkRuleApplied("menzen_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��O��O��"), yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			_T("��O��"),
			[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (DragonCnt(analysis) == 6);
			}
		));
#endif /* GUOBIAO */
	/* ���l�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("���l����"), yaku::yakuCalculator::Yaku::yval_64,
		_T("�O����"), _T("��㍏"), _T("��㍏x2"), _T("��㍏x3"),
#else /* GUOBIAO */
		_T("���l��"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("����F"), _T("�����"),
#endif /* GUOBIAO */
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
#ifndef GUOBIAO
	/* �V�� */
	if (RuleData::chkRuleApplied("heaven_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�V��"), get_yaku_han("heaven_tiger"),
			_T("����F"), _T("���l��"), _T("����F"), _T("��v�E�"), _T("�����"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* ����� */
	if (RuleData::chkRuleApplied("daikaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����"), get_yaku_han("daikaiko"),
			_T("����F"), _T("���l��"), _T("����F"), _T("��v�E��"), _T("�����"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* ��N�� */
	if (RuleData::chkRuleApplied("y2kbug"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��N��"), get_yaku_han("y2kbug"),
			_T("����F"), _T("���l��"), _T("���V��"), _T("�����"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[CharacterNine] >= 1)&&(analysis->KeziCount[SouthWind] == 1));
			}
		));
#endif /* GUOBIAO */
	/* ��l�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("��l����"), yaku::yakuCalculator::Yaku::yval_88,
		_T("������"), _T("�啗��"), _T("�O����"), _T("\u78b0\u78b0�a"), _T("��㍏"), _T("��㍏x2"), _T("��㍏x3"), _T("��㍏x4"),
#else /* GUOBIAO */
		_T("��l��"), (RuleData::chkRuleApplied("double_yakuman")) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("����F"), _T("�΁X�a"), _T("��v�E�ꕗ"), _T("��v�E����"), _T("��v�E�J�啗"), _T("��v�E����"), _T("�����"),
#endif /* GUOBIAO */
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
#ifndef GUOBIAO
	/* �e�� */
	if (RuleData::chkRuleApplied("dan_yaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�e��"), get_yaku_han("dan_yaku"),
			_T("����F"), _T("�΁X�a"), _T("��v�E�ꕗ"), _T("��v�E����"), _T("��v�E�J�啗"), _T("��v�E����"), _T("��l��"), _T("���V��"), _T("�����"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[CharacterNine] >= 1));
			}
		));
	/* ��l�� */
	if (RuleData::chkRuleApplied("cactus"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��l��"), get_yaku_han("cactus"),
			_T("����F"), _T("�΁X�a"), _T("��v�E�ꕗ"), _T("��v�E����"), _T("��v�E�J�啗"), _T("��v�E����"), _T("��l��"), _T("����F"), _T("�����"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �΁X�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("\u78b0\u78b0�a"), yaku::yakuCalculator::Yaku::yval_6,
#else /* GUOBIAO */
		_T("�΁X�a"), yaku::yakuCalculator::Yaku::yval_2han,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKezi == SizeOfMeldBuffer - 1);
		}
	));

	// ---------------------------------------------------------------------

	/* ��v */
#ifdef GUOBIAO
	auto chkYakuhai = [](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->KeziCount;};
#else /* GUOBIAO */
	auto chkYakuhai = RuleData::chkRuleApplied("exposed_yakuhai") ?
#ifdef _MSC_VER
		(std::function<const Int8ByTile& (const MENTSU_ANALYSIS* const)>)
#endif
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->KeziCount;} :
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->AnKeziCount;};
#endif /* GUOBIAO */
	auto bakaze =
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("������"), yaku::yakuCalculator::Yaku::yval_2,
#else /* GUOBIAO */
		_T("��v�E�ꕗ"), yaku::yakuCalculator::Yaku::yval_1han,
#endif /* GUOBIAO */
		bakaze
	));
	auto jikaze =
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile((uint8_t)analysis->GameStat->playerwind(analysis->player))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�啗��"), yaku::yakuCalculator::Yaku::yval_2,
#else /* GUOBIAO */
		_T("��v�E����"), yaku::yakuCalculator::Yaku::yval_1han,
#endif /* GUOBIAO */
		jikaze
	));
#ifdef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�o����"), yaku::yakuCalculator::Yaku::yval_6,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkYakuhai(analysis)[WhiteDragon] +
				chkYakuhai(analysis)[GreenDragon] +
				chkYakuhai(analysis)[RedDragon] == 2;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("����"), yaku::yakuCalculator::Yaku::yval_2,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkYakuhai(analysis)[WhiteDragon] +
				chkYakuhai(analysis)[GreenDragon] +
				chkYakuhai(analysis)[RedDragon] == 1;
		}
	));
#else /* GUOBIAO */
	YAKUFUNC kaimenkaze, urakaze;
	if (RuleData::chkRuleApplied("kaimenkaze")) {
		kaimenkaze =
			[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)analysis->GameStat->playerwind(analysis->GameStat->WaremePlayer))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��v�E�J�啗"), get_yaku_han("kaimenkaze"),
			kaimenkaze
		));
	} else {
		kaimenkaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	if (RuleData::chkRuleApplied("urakaze")) {
		urakaze =
			[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)analysis->GameStat->playerwind(
					RelativePositionOf(analysis->player, sOpposite)))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��v�E����"), get_yaku_han("urakaze"),
			urakaze
		));
	} else {
		urakaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��v�E��"), yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[WhiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��v�E�"), yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[GreenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��v�E��"), yaku::yakuCalculator::Yaku::yval_1han,
		[chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[RedDragon] >= 1);
		}
	));
#endif /* GUOBIAO */

#ifdef GUOBIAO
	/* ��㍏ */
	auto YaoJiuKe =
		[chkYakuhai, bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> int {
			return chkYakuhai(analysis)[CharacterOne] + chkYakuhai(analysis)[CharacterNine] +
				chkYakuhai(analysis)[CircleOne] + chkYakuhai(analysis)[CircleNine] +
				chkYakuhai(analysis)[BambooOne] + chkYakuhai(analysis)[BambooNine] +
				chkYakuhai(analysis)[EastWind] + chkYakuhai(analysis)[SouthWind] +
				chkYakuhai(analysis)[WestWind] + chkYakuhai(analysis)[NorthWind] -
				(bakaze(analysis) ? 1 : 0) - (jikaze(analysis) ? 1 : 0);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��㍏"), yaku::yakuCalculator::Yaku::yval_1,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 1;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��㍏x2"), yaku::yakuCalculator::Yaku::yval_2,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 2;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��㍏x3"), yaku::yakuCalculator::Yaku::yval_3,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 3;
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��㍏x4"), yaku::yakuCalculator::Yaku::yval_4,
		[YaoJiuKe](const MENTSU_ANALYSIS* const analysis) -> bool {
			return YaoJiuKe(analysis) == 4;
		}
	));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* �V������ */
	if (RuleData::chkRuleApplied("niinuma_kenji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�V������"), get_yaku_han("niinuma_kenji"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* �w���� */
	if (RuleData::chkRuleApplied("jindaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�w����"), get_yaku_han("jindaiko"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ���O�� */
	if (RuleData::chkRuleApplied("xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���O��"), get_yaku_han("xiaosanfeng"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* �O���� */
	if (RuleData::chkRuleApplied("three_winds"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("�O����"), yaku::yakuCalculator::Yaku::yval_12,
#else /* GUOBIAO */
			_T("�O����"), get_yaku_han("three_winds"),
#endif /* GUOBIAO */
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 6);
			}
		));
#ifndef GUOBIAO
	/* �q���O�� */
	if (RuleData::chkRuleApplied("three_otakaze"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�q���O��"), get_yaku_han("three_otakaze"),
			_T("�O����"),
			[WindCnt, bakaze, jikaze, kaimenkaze, urakaze]
			(const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 6) &&
					(!bakaze(analysis)) &&
					(!jikaze(analysis)) &&
					(!kaimenkaze(analysis)) &&
					(!urakaze(analysis)));
			}
		));
	/* �q�����O�� */
	if (RuleData::chkRuleApplied("otakaze_xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�q�����O��"), get_yaku_han("otakaze_xiaosanfeng"),
			_T("���O��"),
			[WindCnt, bakaze, jikaze, kaimenkaze, urakaze]
			(const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 5) &&
					(!bakaze(analysis)) &&
					(!jikaze(analysis)) &&
					(!kaimenkaze(analysis)) &&
					(!urakaze(analysis)));
			}
		));

	// ---------------------------------------------------------------------

	/* ��������� */
	if (RuleData::chkRuleApplied("nishinakajima"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���������"), get_yaku_han("nishinakajima"),
			_T("��v�E�ꕗ"), _T("��v�E����"), _T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->GameStat->GameRound / 4 == sWest) &&
					(analysis->GameStat->playerwind(analysis->player) == sSouth))
					yakuFlag = true;
				if ((analysis->GameStat->GameRound / 4 == sSouth) &&
					(analysis->GameStat->playerwind(analysis->player) == sWest))
					yakuFlag = true;
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(yakuFlag));
			}
		));
	/* �ጎ�� */
	if (RuleData::chkRuleApplied("setsugetsuka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ጎ��"), get_yaku_han("setsugetsuka"),
			_T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1));
			}
		));
	/* ���Ԑጎ */
	if (RuleData::chkRuleApplied("fuuka_setsugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���Ԑጎ"), get_yaku_han("fuuka_setsugetsu"),
			_T("�ጎ��"), _T("�΁X�a"), _T("��v�E�ꕗ"), _T("��v�E����"),
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* �Ԓ����� */
	if (RuleData::chkRuleApplied("kachou_fuugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Ԓ�����"), get_yaku_han("kachou_fuugetsu"),
			_T("�ጎ��"), _T("�΁X�a"), _T("��v�E�ꕗ"), _T("��v�E����"),
			[bakaze, jikaze](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(bakaze(analysis) || jikaze(analysis)));
			}
		));
	/* ���k�����ԓ� */
	if (RuleData::chkRuleApplied("tohoku_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���k�����ԓ�"), get_yaku_han("tohoku_expwy"),
			_T("�΁X�a"), _T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* ���֓������ԓ� */
	if (RuleData::chkRuleApplied("higashikanto_expwy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���֓������ԓ�"), get_yaku_han("higashikanto_expwy"),
			_T("�΁X�a"), _T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleSix] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1));
			}
		));
	/* �����n�̂��߂ł����� */
	if (RuleData::chkRuleApplied("koukei_hougyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�g�{���"), get_yaku_han("koukei_hougyou"),
			_T("�΁X�a"), _T("����F"), _T("����"), _T("��v�E��"), _T("��v�E��"), _T("���{���"), _T("��{��t"), _T("����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == CircleOne) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("kinkei_hougyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���{���"), get_yaku_han("kinkei_hougyou"),
			_T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("ginkei_houshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��{��t"), get_yaku_han("ginkei_houshun"),
			_T("��v�E��"), _T("����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->PlayerStat->FlowerFlag.Spring));
			}
		));

	// ---------------------------------------------------------------------

	/* �l���v�� */
	if (RuleData::chkRuleApplied("suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l���v��"), get_yaku_han("suuchaopaikoh"),
			_T("�΁X�a"), _T("���v��"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* ���v�� */
	if (RuleData::chkRuleApplied("chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���v��"), get_yaku_han("chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 2, false);
			}
		));
	/* �ؔv�� */
	if (RuleData::chkRuleApplied("chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ؔv��"), get_yaku_han("chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 3, false);
			}
		));
	/* ���O�� */
	if (RuleData::chkRuleApplied("tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���O��"), get_yaku_han("tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* ��F�ܘA�� */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���"), get_yaku_han("silver_ladder"),
			_T("��F�l�A��"), _T("��F�O�A��"), _T("�΁X�a"),
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_bichrome5[0], 90, 8, 1);
			}
		));
	/* ��F�l�A�� */
	if (RuleData::chkRuleApplied("nishoku_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�l�A��"), get_yaku_han("nishoku_sanrenkoh"),
			_T("��F�O�A��"), _T("�΁X�a"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* ��F�O�A�� */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�O�A��"), get_yaku_han("silver_ladder"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* ��F�l���v�� */
	if (RuleData::chkRuleApplied("nishoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�l���v��"), get_yaku_han("nishoku_suuchaopaikoh"),
			_T("��F���v��"), _T("�΁X�a"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* ��F���v�� */
	if (RuleData::chkRuleApplied("nishoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F���v��"), get_yaku_han("nishoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* ��F�ؔv�� */
	if (RuleData::chkRuleApplied("nishoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F�ؔv��"), get_yaku_han("nishoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 3, false);
			}
		));
	/* ��F���O�� */
	if (RuleData::chkRuleApplied("nishoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��F���O��"), get_yaku_han("nishoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* �O�F�ܘA�� */
	if (RuleData::chkRuleApplied("sky_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�_��"), get_yaku_han("sky_ladder"),
			_T("�O�F�l�A��"), _T("�O�F�A��"), _T("�΁X�a"),
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_trichrome5[0], 150, 8, 1);
			}
		));
	/* �O�F�l�A�� */
	if (RuleData::chkRuleApplied("sanshoku_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�l�A��"), get_yaku_han("sanshoku_suurenkoh"),
			_T("�O�F�A��"), _T("�΁X�a"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* �O�F�A�� */
	if (RuleData::chkRuleApplied("sanshoku_renkoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("�O�F�O�ߍ�"), yaku::yakuCalculator::Yaku::yval_8,
#else /* GUOBIAO */
			_T("�O�F�A��"), get_yaku_han("sanshoku_renkoh"),
#endif /* GUOBIAO */
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
#ifndef GUOBIAO
	/* �O�F�l���v�� */
	if (RuleData::chkRuleApplied("sanshoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�l���v��"), get_yaku_han("sanshoku_suuchaopaikoh"),
			_T("�O�F���v��"), _T("�΁X�a"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* �O�F���v�� */
	if (RuleData::chkRuleApplied("sanshoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F���v��"), get_yaku_han("sanshoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));
	/* �O�F�ؔv�� */
	if (RuleData::chkRuleApplied("sanshoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F�ؔv��"), get_yaku_han("sanshoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 3, false);
			}
		));
	/* �O�F���O�� */
	if (RuleData::chkRuleApplied("sanshoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F���O��"), get_yaku_han("sanshoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* �񓯍� */
	if (RuleData::chkRuleApplied("ryandoukoh"))
#endif /* GUOBIAO */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
			_T("�o����"), yaku::yakuCalculator::Yaku::yval_2,
#else /* GUOBIAO */
			_T("�񓯍�"), get_yaku_han("ryandoukoh"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const int suit[3][2] = {
					{TileSuitCharacters, TileSuitCircles},
					{TileSuitCharacters, TileSuitBamboos},
					{TileSuitCircles, TileSuitBamboos},
				};
				for (int k = 0; k < 3; k++) {
					int j = 0;
					for (int i = 1; i <= 9; i++)
						if ((analysis->KeziCount[i + suit[k][0]] >= 1) &&
							(analysis->KeziCount[i + suit[k][1]] >= 1)) ++j;
					if (j == 2) yakuFlag = true;
				}
				return yakuFlag;
			}
		));
#ifndef GUOBIAO
	/* 䠍��|�� */
	if (RuleData::chkRuleApplied("uikyou_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("䠍��|��"), get_yaku_han("uikyou_toitoi"),
			_T("�O�F����"), _T("�΁X�a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TileSuitCharacters] >= 1) &&
						(analysis->KeziCount[i + TileSuitCircles] >= 1) &&
						(analysis->KeziCount[i + TileSuitBamboos] >= 1))
						if ((analysis->KeziCount[i + TileSuitCharacters + 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitCircles + 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitBamboos + 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitCharacters - 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitCircles - 1] >= 1) ||
							(analysis->KeziCount[i + TileSuitBamboos - 1] >= 1))
							yakuFlag = true;
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* ����敗�ߊC */
	if (RuleData::chkRuleApplied("baxian_chengfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����敗�ߊC"), get_yaku_han("baxian_chengfeng"),
			_T("�΁X�a"), _T("�O�F����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1)) );
			}
		));
	/* ����Y�P */
	if (RuleData::chkRuleApplied("baxian_xifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����Y�P"), get_yaku_han("baxian_xifeng"),
			_T("�΁X�a"), _T("�O�F����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* ����Y�l�P */
	if (RuleData::chkRuleApplied("baxian_xisifeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����Y�l�P"), get_yaku_han("baxian_xisifeng"),
			_T("�΁X�a"), _T("�O�F����"), _T("����Y�P"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KangziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* ����Y���T */
	if (RuleData::chkRuleApplied("baxian_xijingui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����Y���T"), get_yaku_han("baxian_xijingui"),
			_T("�΁X�a"), _T("�O�F����"), _T("䠍��|��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* ���哬�k�l */
	if (RuleData::chkRuleApplied("baxian_dou_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���哬�k�l"), get_yaku_han("baxian_dou_beidou"),
			_T("�΁X�a"), _T("�O�F����"), _T("䠍��|��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) );
			}
		));
	/* �����ڎ� */
	if (RuleData::chkRuleApplied("baxian_jushou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����ڎ�"), get_yaku_han("baxian_jushou"),
			_T("�΁X�a"), _T("�O�F����"), _T("����ߊC"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					analysis->PlayerStat->FlowerFlag.Spring && analysis->PlayerStat->FlowerFlag.Summer &&
					analysis->PlayerStat->FlowerFlag.Autumn && analysis->PlayerStat->FlowerFlag.Winter &&
					analysis->PlayerStat->FlowerFlag.Plum && analysis->PlayerStat->FlowerFlag.Orchid &&
					analysis->PlayerStat->FlowerFlag.Chrys && analysis->PlayerStat->FlowerFlag.Bamboo );
			}
		));
#endif /* GUOBIAO */
}
