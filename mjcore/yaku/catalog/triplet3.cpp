#include "../catalog.h"

#include "../../func.h"

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
			_T("�C�i�o�E�A�["), get_yaku_han("inabauer"),
			_T("����F"), _T("�΁X�a"), _T("�ؔv��"),
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
			_T("�l���n��"), get_yaku_han("april_fool"),
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
			_T("�����̓J"), get_yaku_han("chuutounofue"),
			_T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1));
			}
		));
	/* �O���u */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O���u"), get_yaku_han("sangokushi"),
			_T("�΁X�a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (bool)
					(countKeziOf(analysis, 3) * countKeziOf(analysis, 5) *
					countKeziOf(analysis, 9) * countKeziOf(analysis, 4));
			}
		));
	/* ��x���� */
	if (RuleData::chkRuleApplied("ikkyuusan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��x����"), get_yaku_han("ikkyuusan"),
			_T("�΁X�a"),
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
			_T("�喂�_"), get_yaku_han("daimajin"),
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
			_T("���|�~"), get_yaku_han("shouchikubai"),
			_T("��v�E�"),
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
			_T("�p���_"), get_yaku_han("panda"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* �����R */
	if (RuleData::chkRuleApplied("kokkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����R"), get_yaku_han("kokkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1);
			}
		));
	/* �����R */
	if (RuleData::chkRuleApplied("hakkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����R"), get_yaku_han("hakkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* �g���R */
	if (RuleData::chkRuleApplied("kohkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�g���R"), get_yaku_han("kohkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* �����R */
	if (RuleData::chkRuleApplied("rankigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����R"), get_yaku_han("rankigun"),
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
			_T("����������"), get_yaku_han("dongfeng_yan_jian_liu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* ���{�^�� */
	if (RuleData::chkRuleApplied("hashimoto_shin_ya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���{�^��"), get_yaku_han("hashimoto_shin_ya"),
			_T("�΁X�a"), _T("����"),
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
			_T("��쐼����"), get_yaku_han("dai_nansei_shotou"),
			_T("�΁X�a"), _T("�O�F����"), _T("���V��"),
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
			_T("���߂̓V�g"), get_yaku_han("hakuinotenshi"),
			_T("��v�E��"),
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
			_T("������"), get_yaku_han("dragon_ball"),
			_T("�΁X�a"), _T("��O��"), _T("����F"),
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
			_T("���܎O"), get_yaku_han("shichigosan"),
			_T("��΁X�a"), _T("����Ύq"), _T("�f���"),
			[countTilesOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countTilesOf(analysis, 7) + countTilesOf(analysis, 5) + countTilesOf(analysis, 3) == NUM_OF_TILES_IN_HAND) &&
					(countTilesOf(analysis, 7) * countTilesOf(analysis, 5) * countTilesOf(analysis, 3));
			}
		));
	/* ���J���� */
	if (RuleData::chkRuleApplied("gokurousan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���J����"), get_yaku_han("gokurousan"),
			_T("�΁X�a"), _T("�ؔv��"),
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
			_T("�j�R�j�R��"), get_yaku_han("nikoniko_yakuman"),
			_T("�΁X�a"), _T("�f���"),
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 2) + countDuiziOf(analysis, 5) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* �����o�� */
	if (RuleData::chkRuleApplied("feng_xue_bao_shangfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����o��"), get_yaku_han("feng_xue_bao_shangfeng"),
			_T("�΁X�a"), _T("���V��"),
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
			_T("�y���ޕ�"), get_yaku_han("haruka_kanata"),
			_T("��v�E��"), _T("��v�E�"), _T("�O�Í�"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->AnKeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[RedDragon] >= 1) &&
					(analysis->AnKeziCount[WhiteDragon] >= 1);
			}
		));
	/* ���l�Í� */
	if (RuleData::chkRuleApplied("geki_sianke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���l�Í�"), get_yaku_han("geki_sianke"),
			_T("�l�Í�"),
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
			_T("����O��"), get_yaku_han("geki_daisangen"),
			_T("��O��"),
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
			_T("���k"), get_yaku_han("geki_north"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* ���a */
	if (RuleData::chkRuleApplied("daikyuuhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���a"), get_yaku_han("daikyuuhoh"),
			_T("�O�F����"), _T("�΁X�a"), _T("���V��"), _T("�ܖ��"),
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
			_T("��ъ�"), get_yaku_han("dairinkan"),
			_T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ���q�X�y�V���� */
	if (RuleData::chkRuleApplied("fujikospecial"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���q�X�y�V����"), get_yaku_han("fujikospecial"),
			_T("��v�E��"),
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
			_T("�t�W�e���r"), get_yaku_han("fujitv"),
			_T("�O�F����"),
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
			_T("���A�W�A"), get_yaku_han("eastasia"),
			_T("��v�E��"), _T("�΁X�a"),
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
			_T("�o�b�n"), get_yaku_han("bach"),
			_T("��F����"),
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
			_T("�k���O�Y"), get_yaku_han("kitajima_saburoh"),
			_T("����F"), _T("�l�A��"),
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
			_T("�劙�̎��_"), get_yaku_han("reaper"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* �z�J */
	if (RuleData::chkRuleApplied("koshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�z�J"), get_yaku_han("koshigaya"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* �k�z�J */
	if (RuleData::chkRuleApplied("kitakoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�k�z�J"), get_yaku_han("kitakoshigaya"),
			_T("�z�J"),
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
			_T("��z�J"), get_yaku_han("minamikoshigaya"),
			_T("�z�J"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* �a�m���� */
	if (RuleData::chkRuleApplied("wayouchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�a�m����"), get_yaku_han("wayouchuu"),
			_T("�΁X�a"), _T("��v�E��"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* �[�N�� */
	if (RuleData::chkRuleApplied("leap_year"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�[�N��"),
			yaku::yakuCalculator::Yaku::HANFUNC(
				[](const MENTSU_ANALYSIS* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
					SYSTEMTIME nowTime; GetLocalTime(&nowTime);
					if ((nowTime.wMonth == 2) && (nowTime.wDay == 29))
						return yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman;
					else return yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman;
				}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				bool isLeapYear = (nowTime.wYear % 400 == 0) || ((nowTime.wYear % 4 == 0) && (nowTime.wYear % 100 != 0));
				if (!isLeapYear) return false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - _T('0')) * TILE_SUIT_STEP + 2] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 2] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 9] >= 1))
						return true;
				return false;
			}
		));
	/* Windows8 */
	if (RuleData::chkRuleApplied("windows8"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Windows8"), get_yaku_han("windows8"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) &&
					(!*analysis->TsumoAgariFlag)) ? 1 : 0) >= 3);
			}
		));
	/* �N���C�J�� */
	if (RuleData::chkRuleApplied("ikamusume"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�N���C�J��"), get_yaku_han("ikamusume"),
			_T("��v�E��"), _T("��v�E�"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKeziCount[EastWind] - analysis->AnKangziCount[EastWind] +
					analysis->AnKeziCount[SouthWind] - analysis->AnKangziCount[SouthWind] +
					analysis->AnKeziCount[WestWind] - analysis->AnKangziCount[WestWind] +
					analysis->AnKeziCount[NorthWind] - analysis->AnKangziCount[NorthWind]) >= 1) &&
					((analysis->AnKeziCount[WhiteDragon] - analysis->AnKangziCount[WhiteDragon]) >= 1) &&
					((analysis->AnKeziCount[GreenDragon] - analysis->AnKangziCount[GreenDragon]) >= 1);
			}
		));
	/* �����܎��� */
	if (RuleData::chkRuleApplied("azumazushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����܎���"), get_yaku_han("azumazushi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == CharacterFour) ||
					(analysis->MianziDat[0].tile == CircleFour) ||
					(analysis->MianziDat[0].tile == BambooFour)) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* ���V�� */
	if (RuleData::chkRuleApplied("nishiarai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���V��"), get_yaku_han("nishiarai"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					((analysis->TsumoHai->tile == CharacterOne) ||
					(analysis->TsumoHai->tile == CircleOne) ||
					(analysis->TsumoHai->tile == BambooOne) ||
					(analysis->TsumoHai->tile == CharacterTwo) ||
					(analysis->TsumoHai->tile == CircleTwo) ||
					(analysis->TsumoHai->tile == BambooTwo));
			}
		));
	/* �����g */
	if (RuleData::chkRuleApplied("kongougumi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����g"), get_yaku_han("kongougumi"),
			_T("�΁X�a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 2) &&
					(countKeziOf(analysis, 9) >= 1) &&
					(countKeziOf(analysis, 3) >= 1);
			}
		));
	/* �����{���� */
	if (RuleData::chkRuleApplied("eastjapan_fukkou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����{����"), get_yaku_han("eastjapan_fukkou"),
			_T("�΁X�a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(countKeziOf(analysis, 2) >= 1) &&
					(countKeziOf(analysis, 5) >= 1);
			}
		));
	/* ���I�� */
	if (RuleData::chkRuleApplied("seirogan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���I��"), get_yaku_han("seirogan"),
			_T("��v�E�"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* �鐝�� */
	if (RuleData::chkRuleApplied("suzakumon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�鐝��"), get_yaku_han("suzakumon"),
			_T("�΁X�a"), _T("����F"), _T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooFour] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->MianziDat[0].tile == SouthWind);
			}
		));
	/* �S�Z�� */
	if (RuleData::chkRuleApplied("quanliuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S�Z��"), get_yaku_han("quanliuke"),
			_T("�΁X�a"), _T("��v�E�"), _T("�O�F����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSix] >= 1) &&
					(analysis->KeziCount[CircleSix] >= 1) &&
					(analysis->KeziCount[BambooSix] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* �S�荏 */
	if (RuleData::chkRuleApplied("quanjiuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S�荏"), get_yaku_han("quanjiuke"),
			_T("�΁X�a"), _T("��v�E��"), _T("�O�F����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ������ */
	if (RuleData::chkRuleApplied("swanlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������"), get_yaku_han("swanlake"),
			_T("�΁X�a"), _T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));
	/* ������ */
	if (RuleData::chkRuleApplied("blackbirdlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������"), get_yaku_han("blackbirdlake"),
			_T("�΁X�a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* ����{�j */
	if (RuleData::chkRuleApplied("dainihonshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����{�j"), get_yaku_han("dainihonshi"),
			_T("�΁X�a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1) &&
					(analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[BambooFour] >= 1);
			}
		));
	/* �剻�̉��V */
	if (RuleData::chkRuleApplied("taikanokaishin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�剻�̉��V"), get_yaku_han("taikanokaishin"),
			_T("�΁X�a"), _T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - _T('0')) * TILE_SUIT_STEP + 6] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 4] >= 1) &&
						(analysis->KeziCount[((*k)[2] - _T('0')) * TILE_SUIT_STEP + 5] >= 1))
						flag = true;
				return flag &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* �R���k�s����竓��� */
	if (RuleData::chkRuleApplied("song_youren"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�R���k�s����竓���"), get_yaku_han("song_youren"),
			_T("�΁X�a"), _T("��v�E��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[BambooThree] - analysis->AnKeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[NorthWind] - analysis->AnKeziCount[NorthWind] >= 1);
			}
		));
	/* ����F */
	if (RuleData::chkRuleApplied("lanyise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����F"), get_yaku_han("lanyise"),
			_T("�΁X�a"), _T("����F"), _T("���O��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1) &&
					((analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind]) >= (SIZE_OF_MELD_BUFFER - 2));
			}
		));
	/* ���Ôg */
	if (RuleData::chkRuleApplied("minami_ootsunami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���Ôg"), get_yaku_han("minami_ootsunami"),
			_T("�΁X�a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1);
			}
		));
	/* ������ */
	if (RuleData::chkRuleApplied("xijiujiu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������"), get_yaku_han("xijiujiu"),
			_T("�΁X�a"), _T("�O�F����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(analysis->MianziDat[0].tile == BambooThree);
			}
		));
	/* ��g�~�i�~ */
	if (RuleData::chkRuleApplied("namba_minami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��g�~�i�~"), get_yaku_han("namba_minami"),
			_T("�΁X�a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag1 = false; bool flag2 = false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - _T('0')) * TILE_SUIT_STEP + 3] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 7] >= 1) &&
						(analysis->KeziCount[((*k)[2] - _T('0')) * TILE_SUIT_STEP + 3] >= 1))
						flag1 = true;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 8] >= 1) &&
						(analysis->TsumoHai->tile == (tileCode)(i + 8)))
						flag2 = true;
				return flag1 && flag2 &&
					(analysis->MianziDat[0].tile == SouthWind);
			}
		));
	/* ���_�Y�k�l */
	if (RuleData::chkRuleApplied("qishen_xi_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���_�Y�k�l"), get_yaku_han("qishen_xi_beidou"),
			_T("�΁X�a"), _T("�O�F����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
}
