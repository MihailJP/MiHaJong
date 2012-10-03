#include "../catalog.h"

extern const std::array<char[8], 3> parsedat_monochrome5, parsedat_monochrome4;
extern const std::array<char[4], 3> parsedat_monochrome3;
extern const std::array<char[8], 90> parsedat_bichrome5;
extern const std::array<char[8], 42> parsedat_bichrome4;
extern const std::array<char[4], 18> parsedat_bichrome3;
extern const std::array<char[8], 150> parsedat_trichrome5;
extern const std::array<char[8], 36> parsedat_trichrome4;
extern const std::array<char[4], 6> parsedat_trichrome3;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_1() {
	/* �l�Í� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�l�Í�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"�΁X�a", "�O�Í�",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
				1 : 0) == 4);
		}
	));
	/* �l�Í��P�R */
	if (RuleData::chkRuleApplied("double_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�Í��P�R", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"�l�Í�", "�΁X�a", "�O�Í�",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && (
					(analysis->GameStat->TianHuFlag) ||
					((analysis->Machi == yaku::yakuCalculator::machiTanki) && (!analysis->MachiInfo.FuritenFlag))
					);
			}
		));
	/* �O�Í� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�O�Í�", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalAnKezi - (
				((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (*analysis->TsumoAgariFlag)) ?
				1 : 0) == 3);
		}
	));

	// ---------------------------------------------------------------------

	/* �ܘA�� */
	auto wulianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * 4); k++)
					if ((analysis->DuiziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->DuiziCount[(int)(parsedat[i * fldsize + 4] - '0') * TILE_SUIT_STEP + step * k] >= 1) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	auto lianke =
		[](const MENTSU_ANALYSIS* const analysis,
		const char* const parsedat, int pdsize, int fldsize, int step, bool suurnkoh) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < pdsize; i++) {
				for (int k = 1; k <= (9 - step * (suurnkoh ? 3 : 2)); k++)
					if ((analysis->KeziCount[(int)(parsedat[i * fldsize + 0] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 1] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(analysis->KeziCount[(int)(parsedat[i * fldsize + 2] - '0') * TILE_SUIT_STEP + step * k] >= 1) &&
						(suurnkoh&&(analysis->KeziCount[(int)(parsedat[i * fldsize + 3] - '0') * TILE_SUIT_STEP + step * k] >= 1)) )
						yakuFlag = true;
			}
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("golden_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", get_yaku_han("golden_ladder"),
			"����F", "�΁X�a",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_monochrome5[0], 3, 8, 1);
			}
		));
	/* �l�A�� */
	auto suurenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 1, true);
		};
	if (RuleData::chkRule("suurenkoh", "8han_incl_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�A��", yaku::yakuCalculator::Yaku::yval_6han /* �΁X�a����8�� */,
			"����",
			suurenkoh
		));
	else if (RuleData::chkRuleApplied("suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�A��", get_yaku_han("suurenkoh"),
			"����", "�΁X�a",
			suurenkoh
		));
	/* �O�A�� */
	auto sanrenkoh =
		[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
			return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 1, false);
		};
	if (RuleData::chkRuleApplied("sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�A��", get_yaku_han("sanrenkoh"),
			"����", "�l�A��",
			sanrenkoh
		));
	/* �������� */
	if (RuleData::chkRuleApplied("kagamine_len"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", get_yaku_han("kagamine_len"),
			/* �O�A���ɒǉ������ */
			[sanrenkoh](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) && sanrenkoh(analysis));
			}
		));

	// ---------------------------------------------------------------------

	/* ���A�ȕ��̎O�F */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�O�F����", get_yaku_han("sanshoku_doukoh"),
		"�O�F������",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			bool yakuFlag = false;
			for (int i = 1; i <= 9; i++)
				if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
					(analysis->KeziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
					(analysis->KeziCount[i + TILE_SUIT_BAMBOOS] >= 1))
					yakuFlag = true;
			return yakuFlag;
		}
	));
	/* �l�F�����i���� */
	if (RuleData::chkRuleApplied("yonshoku_doukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�F����", get_yaku_han("yonshoku_doukoh"),
			"�O�F����", "�O�F������", "�΁X�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_BAMBOOS] >= 1) &&
						(analysis->KeziCount[WhiteDragon] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));
	/* �O�F������ */
	if (RuleData::chkRuleApplied("sanshoku_shoudoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F������", get_yaku_han("sanshoku_shoudoukoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->DuiziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
						(analysis->DuiziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
						(analysis->DuiziCount[i + TILE_SUIT_BAMBOOS] >= 1))
						yakuFlag = true;
				return yakuFlag;
			}
		));

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
		"���O��", yaku::yakuCalculator::Yaku::yval_2han,
		"�t�ɉ����Ȃ�", "�t�ɉ�������", /* ��v2�͕K���������� */
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 5);
		}
	));
	/* ��O�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��O��", yaku::yakuCalculator::Yaku::yval_yakuman,
		"��v�E��", "��v�E�", "��v�E��", "�t�ɉ����Ȃ�", "�t�ɉ�������",
		[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (DragonCnt(analysis) == 6);
		}
	));
	/* ��O��O�� */
	if (RuleData::chkRuleApplied("menzen_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��O��O��", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"��O��",
			[DragonCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (DragonCnt(analysis) == 6);
			}
		));
	/* ���l�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���l��", yaku::yakuCalculator::Yaku::yval_yakuman,
		"����F", "�����",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 7);
		}
	));
	/* �V�� */
	if (RuleData::chkRuleApplied("heaven_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�V��", get_yaku_han("heaven_tiger"),
			"����F", "���l��", "����F", "��v�E�", "�����",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* ����� */
	if (RuleData::chkRuleApplied("daikaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����", get_yaku_han("daikaiko"),
			"����F", "���l��", "����F", "��v�E��", "�����",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* ��N�� */
	if (RuleData::chkRuleApplied("y2kbug"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��N��", get_yaku_han("y2kbug"),
			"����F", "���l��", "���V��", "�����",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 7)&&
					(analysis->KeziCount[CharacterNine] >= 1)&&(analysis->KeziCount[SouthWind] == 1));
			}
		));
	/* ��l�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��l��", (RuleData::chkRuleApplied("double_yakuman")) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"����F", "�΁X�a", "��v�E�ꕗ", "��v�E����", "��v�E�J�啗", "��v�E����", "�����",
		[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (WindCnt(analysis) == 8);
		}
	));
	/* �e�� */
	if (RuleData::chkRuleApplied("dan_yaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�e��", get_yaku_han("dan_yaku"),
			"����F", "�΁X�a", "��v�E�ꕗ", "��v�E����", "��v�E�J�啗", "��v�E����", "��l��", "���V��", "�����",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[CharacterNine] >= 1));
			}
		));
	/* ��l�� */
	if (RuleData::chkRuleApplied("cactus"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��l��", get_yaku_han("cactus"),
			"����F", "�΁X�a", "��v�E�ꕗ", "��v�E����", "��v�E�J�啗", "��v�E����", "��l��", "����F", "�����",
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((WindCnt(analysis) == 8)&&(analysis->DuiziCount[RedDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* �΁X�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�΁X�a", yaku::yakuCalculator::Yaku::yval_2han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1);
		}
	));

	// ---------------------------------------------------------------------

	/* ��v */
	auto chkYakuhai = RuleData::chkRuleApplied("exposed_yakuhai") ?
		(std::function<const Int8ByTile& (const MENTSU_ANALYSIS* const)>)
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->KeziCount;} :
		[](const MENTSU_ANALYSIS* const analysis) -> const Int8ByTile& {return analysis->AnKeziCount;};
	auto bakaze =
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��v�E�ꕗ", yaku::yakuCalculator::Yaku::yval_1han,
		bakaze
	));
	auto jikaze =
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
				analysis->player, analysis->GameStat->GameRound))] >= 1);
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��v�E����", yaku::yakuCalculator::Yaku::yval_1han,
		jikaze
	));
	YAKUFUNC kaimenkaze, urakaze;
	if (RuleData::chkRuleApplied("kaimenkaze")) {
		kaimenkaze =
			[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��v�E�J�啗", get_yaku_han("kaimenkaze"),
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
			[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkYakuhai(analysis)[Wind2Tile((uint8_t)playerwind(analysis->GameStat,
					RelativePositionOf(analysis->player, sOpposite), analysis->GameStat->GameRound))] >= 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��v�E����", get_yaku_han("urakaze"),
			urakaze
		));
	} else {
		urakaze = 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return false;
			};
	}
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��v�E��", yaku::yakuCalculator::Yaku::yval_1han,
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[WhiteDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��v�E�", yaku::yakuCalculator::Yaku::yval_1han,
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[GreenDragon] >= 1);
		}
	));
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��v�E��", yaku::yakuCalculator::Yaku::yval_1han,
		[&chkYakuhai](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (chkYakuhai(analysis)[RedDragon] >= 1);
		}
	));

	// ---------------------------------------------------------------------

	/* �V������ */
	if (RuleData::chkRuleApplied("niinuma_kenji"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�V������", get_yaku_han("niinuma_kenji"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* �w���� */
	if (RuleData::chkRuleApplied("jindaiko"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�w����", get_yaku_han("jindaiko"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ���O�� */
	if (RuleData::chkRuleApplied("xiaosanfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���O��", get_yaku_han("xiaosanfeng"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 5);
			}
		));
	/* �O���� */
	if (RuleData::chkRuleApplied("three_winds"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O����", get_yaku_han("three_winds"),
			[WindCnt](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (WindCnt(analysis) == 6);
			}
		));
	/* �q���O�� */
	if (RuleData::chkRuleApplied("three_otakaze"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�q���O��", get_yaku_han("three_otakaze"),
			"�O����",
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
			"�q�����O��", get_yaku_han("otakaze_xiaosanfeng"),
			"���O��",
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
			"���������", get_yaku_han("nishinakajima"),
			"��v�E�ꕗ", "��v�E����", "��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->GameStat->GameRound / 4 == sWest) &&
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sSouth))
					yakuFlag = true;
				if ((analysis->GameStat->GameRound / 4 == sSouth) &&
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sWest))
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
			"�ጎ��", get_yaku_han("setsugetsuka"),
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1));
			}
		));
	/* ���Ԑጎ */
	if (RuleData::chkRuleApplied("fuuka_setsugetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Ԑጎ", get_yaku_han("fuuka_setsugetsu"),
			"�ጎ��", "�΁X�a", "��v�E�ꕗ", "��v�E����",
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
			"�Ԓ�����", get_yaku_han("kachou_fuugetsu"),
			"�ጎ��", "�΁X�a", "��v�E�ꕗ", "��v�E����",
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
			"���k�����ԓ�", get_yaku_han("tohoku_expwy"),
			"�΁X�a", "����F",
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
			"���֓������ԓ�", get_yaku_han("higashikanto_expwy"),
			"�΁X�a", "����F",
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
			"�g�{���", get_yaku_han("koukei_hougyou"),
			"�΁X�a", "����F", "����", "��v�E��", "��v�E��", "���{���", "��{��t", "����",
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
			"���{���", get_yaku_han("kinkei_hougyou"),
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));
	if (RuleData::chkRuleApplied("ginkei_houshun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��{��t", get_yaku_han("ginkei_houshun"),
			"��v�E��", "����",
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
			"�l���v��", get_yaku_han("suuchaopaikoh"),
			"�΁X�a", "���v��",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome4[0], 3, 8, 2, true);
			}
		));
	/* ���v�� */
	if (RuleData::chkRuleApplied("chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���v��", get_yaku_han("chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 2, false);
			}
		));
	/* �ؔv�� */
	if (RuleData::chkRuleApplied("chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ؔv��", get_yaku_han("chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 3, false);
			}
		));
	/* ���O�� */
	if (RuleData::chkRuleApplied("tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���O��", get_yaku_han("tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_monochrome3[0], 3, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* ��F�ܘA�� */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���", get_yaku_han("silver_ladder"),
			"��F�l�A��", "��F�O�A��", "�΁X�a",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_bichrome5[0], 90, 8, 1);
			}
		));
	/* ��F�l�A�� */
	if (RuleData::chkRuleApplied("nishoku_sanrenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�l�A��", get_yaku_han("nishoku_sanrenkoh"),
			"��F�O�A��", "�΁X�a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 1, true);
			}
		));
	/* ��F�O�A�� */
	if (RuleData::chkRuleApplied("silver_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�O�A��", get_yaku_han("silver_ladder"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 1, false);
			}
		));
	/* ��F�l���v�� */
	if (RuleData::chkRuleApplied("nishoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�l���v��", get_yaku_han("nishoku_suuchaopaikoh"),
			"��F���v��", "�΁X�a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome4[0], 42, 8, 2, true);
			}
		));
	/* ��F���v�� */
	if (RuleData::chkRuleApplied("nishoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F���v��", get_yaku_han("nishoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 2, false);
			}
		));
	/* ��F�ؔv�� */
	if (RuleData::chkRuleApplied("nishoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F�ؔv��", get_yaku_han("nishoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 3, false);
			}
		));
	/* ��F���O�� */
	if (RuleData::chkRuleApplied("nishoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��F���O��", get_yaku_han("nishoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_bichrome3[0], 18, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* �O�F�ܘA�� */
	if (RuleData::chkRuleApplied("sky_ladder"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�_��", get_yaku_han("sky_ladder"),
			"�O�F�l�A��", "�O�F�A��", "�΁X�a",
			[wulianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return wulianke(analysis, (const char*)&parsedat_trichrome5[0], 150, 8, 1);
			}
		));
	/* �O�F�l�A�� */
	if (RuleData::chkRuleApplied("sanshoku_suurenkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�l�A��", get_yaku_han("sanshoku_suurenkoh"),
			"�O�F�A��", "�΁X�a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 1, true);
			}
		));
	/* �O�F�A�� */
	if (RuleData::chkRuleApplied("sanshoku_renkoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�A��", get_yaku_han("sanshoku_renkoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 1, false);
			}
		));
	/* �O�F�l���v�� */
	if (RuleData::chkRuleApplied("sanshoku_suuchaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�l���v��", get_yaku_han("sanshoku_suuchaopaikoh"),
			"�O�F���v��", "�΁X�a",
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome4[0], 36, 8, 2, true);
			}
		));
	/* �O�F���v�� */
	if (RuleData::chkRuleApplied("sanshoku_chaopaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F���v��", get_yaku_han("sanshoku_chaopaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 2, false);
			}
		));
	/* �O�F�ؔv�� */
	if (RuleData::chkRuleApplied("sanshoku_chinpaikoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F�ؔv��", get_yaku_han("sanshoku_chinpaikoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 3, false);
			}
		));
	/* �O�F���O�� */
	if (RuleData::chkRuleApplied("sanshoku_tinsankoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F���O��", get_yaku_han("sanshoku_tinsankoh"),
			[lianke](const MENTSU_ANALYSIS* const analysis) -> bool {
				return lianke(analysis, (const char*)&parsedat_trichrome3[0], 6, 4, 4, false);
			}
		));

	// ---------------------------------------------------------------------

	/* �񓯍� */
	if (RuleData::chkRuleApplied("ryandoukoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�񓯍�", get_yaku_han("ryandoukoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const int suit[3][2] = {
					{TILE_SUIT_CHARACTERS, TILE_SUIT_CIRCLES},
					{TILE_SUIT_CHARACTERS, TILE_SUIT_BAMBOOS},
					{TILE_SUIT_CIRCLES, TILE_SUIT_BAMBOOS},
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
	/* 䠍��|�� */
	if (RuleData::chkRuleApplied("uikyou_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"䠍��|��", get_yaku_han("uikyou_toitoi"),
			"�O�F����", "�΁X�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_CIRCLES] >= 1) &&
						(analysis->KeziCount[i + TILE_SUIT_BAMBOOS] >= 1))
						if ((analysis->KeziCount[i + TILE_SUIT_CHARACTERS + 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_CIRCLES + 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_BAMBOOS + 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_CHARACTERS - 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_CIRCLES - 1] >= 1) ||
							(analysis->KeziCount[i + TILE_SUIT_BAMBOOS - 1] >= 1))
							yakuFlag = true;
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* ����敗�ߊC */
	if (RuleData::chkRuleApplied("baxian_chengfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����敗�ߊC", get_yaku_han("baxian_chengfeng"),
			"�΁X�a", "�O�F����",
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
			"����Y�P", get_yaku_han("baxian_xifeng"),
			"�΁X�a", "�O�F����",
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
			"����Y�l�P", get_yaku_han("baxian_xisifeng"),
			"�΁X�a", "�O�F����", "����Y�P",
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
			"����Y���T", get_yaku_han("baxian_xijingui"),
			"�΁X�a", "�O�F����", "䠍��|��",
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
			"���哬�k�l", get_yaku_han("baxian_dou_beidou"),
			"�΁X�a", "�O�F����", "䠍��|��",
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
			"�����ڎ�", get_yaku_han("baxian_jushou"),
			"�΁X�a", "�O�F����", "����ߊC",
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
}
