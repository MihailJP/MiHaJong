#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_3() {
	auto countTilesOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->TileCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->TileCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->TileCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countKangziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->KangziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->KangziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->KangziCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countKeziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->KeziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->KeziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->KeziCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countDuiziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->DuiziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->DuiziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->DuiziCount[TILE_SUIT_BAMBOOS + numeral];
		};

	// ---------------------------------------------------------------------

	/* �C�i�o�E�A�[ */
	if (RuleData::chkRuleApplied("inabauer"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�i�o�E�A�[", get_yaku_han("inabauer"),
			"����F", "�΁X�a", "�ؔv��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->DuiziCount[i + 1] >= 1) &&
						(analysis->DuiziCount[i + 7] >= 1) &&
						(analysis->DuiziCount[i + 8] >= 1) &&
						(analysis->DuiziCount[i + 5] >= 1) &&
						(analysis->DuiziCount[i + 2] >= 1))
						flag = true;
				return flag;
			}
		));
	/* �l���n�� */
	if (RuleData::chkRuleApplied("april_fool")) {
		auto isApril1st =
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 4) && (nowTime.wDay == 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l���n��", get_yaku_han("april_fool"),
			[isApril1st](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (!isApril1st()) return false;
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 4] >= 1) &&
						(analysis->KeziCount[i + 1] >= 1))
						flag = true;
				return flag && (analysis->TileCount[CircleNine]);
			}
		));
	}
	/* �����̓J */
	if (RuleData::chkRuleApplied("chuutounofue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����̓J", get_yaku_han("chuutounofue"),
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1));
			}
		));
	/* �O���u */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O���u", get_yaku_han("sangokushi"),
			"�΁X�a",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (bool)
					(countKeziOf(analysis, 3) * countKeziOf(analysis, 5) *
					countKeziOf(analysis, 9) * countKeziOf(analysis, 4));
			}
		));
	/* ��x���� */
	if (RuleData::chkRuleApplied("ikkyuusan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��x����", get_yaku_han("ikkyuusan"),
			"�΁X�a",
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 1) > 0) &&
					(countDuiziOf(analysis, 9) > 0) &&
					(countDuiziOf(analysis, 3) > 0) &&
					(countDuiziOf(analysis, 1) + countDuiziOf(analysis, 9) +
					countDuiziOf(analysis, 3) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* �喂�_ */
	if (RuleData::chkRuleApplied("daimajin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�喂�_", get_yaku_han("daimajin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKeziCount[WhiteDragon] +
					analysis->AnKeziCount[GreenDragon] +
					analysis->AnKeziCount[RedDragon]) >= 1) &&
					(analysis->TsumoHai->tile == BambooFive) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki);
			}
		));
	/* ���|�~ */
	if (RuleData::chkRuleApplied("shouchikubai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���|�~", get_yaku_han("shouchikubai"),
			"��v�E�",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 2; i <= 9; ++i)
					if (analysis->KeziCount[TILE_SUIT_BAMBOOS + i] >= 1)
						flag = true;
				return flag &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* �p���_ */
	if (RuleData::chkRuleApplied("panda"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�p���_", get_yaku_han("panda"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* �����R */
	if (RuleData::chkRuleApplied("kokkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����R", get_yaku_han("kokkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1);
			}
		));
	/* �����R */
	if (RuleData::chkRuleApplied("hakkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����R", get_yaku_han("hakkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* �g���R */
	if (RuleData::chkRuleApplied("kohkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�g���R", get_yaku_han("kohkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* �����R */
	if (RuleData::chkRuleApplied("rankigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����R", get_yaku_han("rankigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ���������� */
	if (RuleData::chkRuleApplied("dongfeng_yan_jian_liu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����������", get_yaku_han("dongfeng_yan_jian_liu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* ���{�^�� */
	if (RuleData::chkRuleApplied("hashimoto_shin_ya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���{�^��", get_yaku_han("hashimoto_shin_ya"),
			"�΁X�a", "����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[CircleSeven] >= 1) &&
					(analysis->DuiziCount[BambooTwo] >= 1) &&
					(analysis->DuiziCount[BambooNine] >= 1);
			}
		));
	/* ��쐼���� */
	if (RuleData::chkRuleApplied("dai_nansei_shotou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��쐼����", get_yaku_han("dai_nansei_shotou"),
			"�΁X�a", "�O�F����", "���V��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1);
			}
		));
	/* ���߂̓V�g */
	if (RuleData::chkRuleApplied("hakuinotenshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���߂̓V�g", get_yaku_han("hakuinotenshi"),
			"��v�E��",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 7) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(countKangziOf(analysis, 5) >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind] >= 1);
			}
		));
	/* ������ */
	if (RuleData::chkRuleApplied("dragon_ball"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������", get_yaku_han("dragon_ball"),
			"�΁X�a", "��O��", "����F",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] - analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1);
			}
		));
	/* ���܎O */
	if (RuleData::chkRuleApplied("shichigosan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���܎O", get_yaku_han("shichigosan"),
			"��΁X�a", "����Ύq", "�f���",
			[countTilesOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countTilesOf(analysis, 7) + countTilesOf(analysis, 5) + countTilesOf(analysis, 3) == NUM_OF_TILES_IN_HAND) &&
					(countTilesOf(analysis, 7) * countTilesOf(analysis, 5) * countTilesOf(analysis, 3));
			}
		));
	/* ���J���� */
	if (RuleData::chkRuleApplied("gokurousan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���J����", get_yaku_han("gokurousan"),
			"�΁X�a", "�ؔv��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 5] >= 1) && (analysis->KeziCount[i + 9] >= 1) &&
						(analysis->KeziCount[i + 6] >= 1) && (analysis->KeziCount[i + 3] >= 1))
						flag = true;
				return flag;
			}
		));
	/* �j�R�j�R�� */
	if (RuleData::chkRuleApplied("nikoniko_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�j�R�j�R��", get_yaku_han("nikoniko_yakuman"),
			"�΁X�a", "�f���",
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 2) + countDuiziOf(analysis, 5) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* �����o�� */
	if (RuleData::chkRuleApplied("feng_xue_bao_shangfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����o��", get_yaku_han("feng_xue_bao_shangfeng"),
			"�΁X�a", "���V��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (Wind2Tile(analysis->GameStat->GameRound / 4) != // �_�u���łȂ���
					Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))) &&
					(analysis->DuiziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))] >= 1) && // ����������
					(analysis->DuiziCount[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1) && // �ꕗ������
					(analysis->DuiziCount[CircleOne] >= 1) && (analysis->DuiziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1);
			}
		));
	/* �y���ޕ� */
	if (RuleData::chkRuleApplied("haruka_kanata"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�y���ޕ�", get_yaku_han("haruka_kanata"),
			"��v�E��", "��v�E�", "�O�Í�",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->AnKeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[RedDragon] >= 1) &&
					(analysis->AnKeziCount[WhiteDragon] >= 1);
			}
		));
	/* ���l�Í� */
	if (RuleData::chkRuleApplied("geki_sianke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���l�Í�", get_yaku_han("geki_sianke"),
			"�l�Í�",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && // �l�Í���
					(analysis->TsumoHai->tile == CharacterOne); // ���݂Řa����
			}
		));
	/* ����O�� */
	if (RuleData::chkRuleApplied("geki_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����O��", get_yaku_han("geki_daisangen"),
			"��O��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiPenchan);
			}
		));
	/* ���k */
	if (RuleData::chkRuleApplied("geki_north"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���k", get_yaku_han("geki_north"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* ���a */
	if (RuleData::chkRuleApplied("daikyuuhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���a", get_yaku_han("daikyuuhoh"),
			"�O�F����", "�΁X�a", "���V��", "�ܖ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1);
			}
		));
	/* ��ъ� */
	if (RuleData::chkRuleApplied("dairinkan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��ъ�", get_yaku_han("dairinkan"),
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ���q�X�y�V���� */
	if (RuleData::chkRuleApplied("fujikospecial"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���q�X�y�V����", get_yaku_han("fujikospecial"),
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 1; i < TILE_NONFLOWER_MAX; i++)
					if (analysis->GameStat->DoraFlag.Omote[i] && analysis->KeziCount[i])
						flag = true;
				return flag &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* �t�W�e���r */
	if (RuleData::chkRuleApplied("fujitv"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�t�W�e���r", get_yaku_han("fujitv"),
			"�O�F����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->MianziDat[0].tile == CircleOne) &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1);
			}
		));
	/* ���A�W�A */
	if (RuleData::chkRuleApplied("eastasia"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A�W�A", get_yaku_han("eastasia"),
			"��v�E��", "�΁X�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* �o�b�n */
	if (RuleData::chkRuleApplied("bach"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�o�b�n", get_yaku_han("bach"),
			"��F����",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				unsigned count = 0;
				for (int i = CircleOne; i <= CircleSeven; i++) {
					count += analysis->DuiziCount[i];
					count += analysis->ShunziCount[i];
				}
				count += analysis->DuiziCount[CircleNine];
				return (countKeziOf(analysis, 8) >= 2) &&
					(count >= 3);
			}
		));
	/* �k���O�Y */
	if (RuleData::chkRuleApplied("kitajima_saburoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k���O�Y", get_yaku_han("kitajima_saburoh"),
			"����F", "�l�A��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool pairIsHonor = false; bool flag = false;
				for (int i = TILE_SUIT_HONORS; i < TILE_NONFLOWER_MAX; i++)
					if (analysis->MianziDat[0].tile == (tileCode)i)
						pairIsHonor = true;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					if ((analysis->KeziCount[i + 3] >= 1) &&
						(analysis->KeziCount[i + 6] >= 1) &&
						(analysis->KeziCount[NorthWind] >= 1) &&
						(pairIsHonor)) {
							if ((analysis->ShunziCount[i + 4] >= 1) &&
								(analysis->TsumoHai->tile == (tileCode)(i + 6)))
								flag = true;
							if ((analysis->ShunziCount[i + 3] >= 1) &&
								(analysis->TsumoHai->tile == (tileCode)(i + 3)))
								flag = true;
					}
				}
				return flag;
			}
		));
	/* �劙�̎��_ */
	if (RuleData::chkRuleApplied("reaper"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�劙�̎��_", get_yaku_han("reaper"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* �z�J */
	if (RuleData::chkRuleApplied("koshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�z�J", get_yaku_han("koshigaya"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* �k�z�J */
	if (RuleData::chkRuleApplied("kitakoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�k�z�J", get_yaku_han("kitakoshigaya"),
			"�z�J",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[NorthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* ��z�J */
	if (RuleData::chkRuleApplied("minamikoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��z�J", get_yaku_han("minamikoshigaya"),
			"�z�J",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* TODO: �a�m���� */
	/* TODO: �[�N�� */
	/* TODO: Windows8 */
	/* TODO: �N���C�J�� */
	/* �����܎��� */
	if (RuleData::chkRuleApplied("azumazushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����܎���", get_yaku_han("azumazushi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == CharacterFour) ||
					(analysis->MianziDat[0].tile == CircleFour) ||
					(analysis->MianziDat[0].tile == BambooFour)) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* TODO: ���V�� */
}
