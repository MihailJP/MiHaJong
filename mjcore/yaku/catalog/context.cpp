#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
	/* ���[�` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag)); // �������Ă���
		}
	));
	/* �_�u���[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�_�u������", yaku::yakuCalculator::Yaku::yval_2han_menzen,
		"����",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �������Ă���
				(gameStat->Player[analysis->player].RichiFlag.DoubleFlag)); // �_�u�������t���O�������Ă���
		}
	));
	if (getRule(RULE_OPEN_RIICHI) != 0) {
		/* �v�����[ */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�I�[�v������", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			"����",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �������Ă���
					(gameStat->Player[analysis->player].RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �_�u�v�� */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�_�u���J����", yaku::yakuCalculator::Yaku::yval_3han_menzen,
			"����", "�_�u������", "�I�[�v������",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �������Ă���
					(gameStat->Player[analysis->player].RichiFlag.DoubleFlag) && // �_�u�������t���O�������Ă���
					(gameStat->Player[analysis->player].RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �v�����[���e */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�J�����h�a", yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
				return (gameStat->Player[analysis->player].MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(11, yaku::yakuCalculator::hanUnit::Han),
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN();
			}),
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �������Ă���
					(gameStat->Player[analysis->player].RichiFlag.OpenFlag) && // �I�[�v�������t���O�������Ă���
					(!gameStat->TsumoAgariFlag) && // ����
					(gameStat->CurrentPlayer.Furikomi >= 0) &&
					(!gameStat->Player[gameStat->CurrentPlayer.Furikomi].RichiFlag.RichiFlag)); // �񃊁[�`�҂���̘a��
			}
		));
	}
	/* �ꔭ(�����ł͑��Ƃ�����) */
	if ((getRule(RULE_RIICHI_IPPATSU) != 1)&&(getRule(RULE_RIICHI_IPPATSU) != 3)) { // �ꔭ�����ɂȂ�Ȃ����[�������O
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꔭ",
			(getRule(RULE_RIICHI_IPPATSU) == 2) ? /* �ꔭ�͔���𖞂����Ȃ�(�������V�o�Ń��[���݂̘̂a����F�߂Ȃ�)���[�� */
				yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
					return (gameStat->Player[analysis->player].MenzenFlag) ?
						yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
						yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han) :
						yaku::yakuCalculator::Yaku::YAKU_HAN();
			}) : yaku::yakuCalculator::Yaku::yval_1han_menzen, // ���[���݂̂ł��a�����ėǂ����[��
			/* �K�������ƕ������� */
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �������Ă���
					(gameStat->Player[analysis->player].RichiFlag.IppatsuFlag)); // �ꔭ�t���O�������Ă���
			}
		));
		/* ���ꔭ(���[�`�ꔭ�����Ƃ���a����) */
		if (getRule(RULE_CHOUIPPATSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"���ꔭ",
				(getRule(RULE_RIICHI_IPPATSU) == 2) ? /* �ꔭ�͔���𖞂����Ȃ�(�������V�o�Ń��[���݂̘̂a����F�߂Ȃ�)���[�� */
					yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
						return (gameStat->Player[analysis->player].MenzenFlag) ?
							yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
							yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han) :
							yaku::yakuCalculator::Yaku::YAKU_HAN();
					}) : yaku::yakuCalculator::Yaku::yval_2han_menzen, // ���[���݂̂ł��a�����ėǂ����[��
				"�ꔭ", /* �K�������ƕ������� */
				[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
						(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �������Ă���
						(gameStat->Player[analysis->player].RichiFlag.IppatsuFlag) && // �ꔭ�t���O�������Ă���
						(!gameStat->TsumoAgariFlag) && // �����ł���
						(
							(gameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sRight)) || // ���Ƃ���a������
							((chkGameType(gameStat, SanmaT)) &&
								(gameStat->CurrentPlayer.Furikomi == 0) && (analysis->player == 2)) || // �O���̓���
							((chkGameType(gameStat, Sanma4)) &&
								(gameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sOpposite)) &&
								(playerwind(gameStat, analysis->player, gameStat->GameRound) == sWest)) // �l�l�O���̓���
						));
				}
			));
	}
	/* �����[�` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�����[�`", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		/* �����͕�������炵�� */
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag)) // �������Ă���
			{
				for (int i = 1; i < gameStat->Player[analysis->player].DiscardPointer; i++)
					if ((gameStat->Player[analysis->player].Discard[i].tcode.tile == BambooOne) &&
						((gameStat->Player[analysis->player].Discard[i].dstat == discardRiichi) ||
						(gameStat->Player[analysis->player].Discard[i].dstat == discardRiichiTaken)))
						return true;
				return false;
			}
			else return false;
		}
	));
	/* �V�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�V�a", (getRule(RULE_TENHOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : // �V�a���_�u���𖞂̃��[����
		yaku::yakuCalculator::Yaku::yval_yakuman_menzen, // ���ʂɖ𖞂ɂȂ邩
		"��O�����̘a",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(playerwind(gameStat, analysis->player, gameStat->GameRound) == sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(gameStat->Player[analysis->player].FirstDrawFlag)); // �V�a�E�n�a�t���O�������Ă���
		}
	));
	/* �n�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�n�a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"��O�����̘a",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(playerwind(gameStat, analysis->player, gameStat->GameRound) != sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(gameStat->Player[analysis->player].FirstDrawFlag) && // �V�a�E�n�a�t���O�������Ă���
				(gameStat->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* �Q�ʂ���ʖڂȂ�ł����H */
	if (getRule(RULE_RENHOH) != 0) {
		yaku::yakuCalculator::Yaku::MenzenHan renhoh_han;
		switch (getRule(RULE_RENHOH)) {
			case 1: renhoh_han = yaku::yakuCalculator::Yaku::yval_yakuman_menzen; break; // ��
			case 2: renhoh_han = yaku::yakuCalculator::Yaku::yval_baiman_menzen; break; // �{��
			case 3: renhoh_han = yaku::yakuCalculator::Yaku::yval_mangan_menzen; break; // ����
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�a", renhoh_han, "��O�����̘a",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(playerwind(gameStat, analysis->player, gameStat->GameRound) != sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
					(gameStat->Player[analysis->player].FirstDrawFlag) && // �V�a�E�n�a�t���O�������Ă���
					(!gameStat->TsumoAgariFlag)); // �����A�K��
			}
		));
	}
	/* �c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��O�����̘a", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(gameStat->Player[analysis->player].MenzenFlag) && // ��O�ł���
				(gameStat->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* �O�Ǔ]�� */
	if (getRule(RULE_SANSEKI_TEMPUKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�Ǔ]��", yaku::yakuCalculator::Yaku::yval_3han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(!gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �����̓��[�`���Ă��Ȃ�
					(gameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.RichiFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.RichiFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.RichiFlag) // ���ƑS������
					);
			}
		));
	/* �N���� */
	if (getRule(RULE_KISHI_KAISEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�N����", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(!gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // �����̓��[�`���Ă��Ȃ�
					(gameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.DoubleFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.DoubleFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.DoubleFlag) // ���ƑS���_�u���[(!)
					);
			}
		));
	/* �����V�����c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���J��", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
				(gameStat->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* ���ȊJ�� */
	if ((getRule(RULE_MINKAN_PAO) >= 3) && (getRule(RULE_MINKAN_PAO) <= 5))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ȊJ��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(gameStat->PaoFlag[pyMinkan].paoPlayer >= 0)); // ���Ȃ̒���
			}
		));
	/* �A�ȊJ�� */
	if (getRule(RULE_RENKAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�A�ȊJ��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(gameStat->KangFlag.chainFlag >= 2)); // �A���ŞȂ�������
			}
		));
	/* �ܓ��J�� */
	if (getRule(RULE_UUPIN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܓ��J��", (getRule(RULE_UUPIN_KAIHOH) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"���J��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����J�� */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����J��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == RedDragon)); // �a���v����
			}
		));
	/* ���Șa */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Șa", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���J��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(gameStat->KangFlag.topFlag)); // ���Șa�t���O�������Ă���
			}
		));
	/* �T���i���z�[������ */
	if (getRule(RULE_SAYONARA_HOMERUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�T���i���z�[������", yaku::yakuCalculator::Yaku::yval_3han,
			"���J��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(gameStat) == 0)); // ���v���������c��R�v��0
			}
		));
	/* ���ԉ� */
	if (getRule(RULE_HIGASHI_HANAZONO_TRIPLETS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԉ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���J��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(gameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(gameStat->GameRound / 4 == 0) && // ����
					(playerwind(gameStat, analysis->player, gameStat->GameRound) == sEast) && // ����
					(analysis->KangziCount[EastWind] >= 1)); // ���̞Ȏq������
			}
		));
	/* �C��c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�C��̌�", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(gameStat->TsumoAgariFlag) && // �c���A�K��
				(tilesLeft(gameStat) == 0)); // �n�C�e�C�ł���
		}
	));
	/* �ꓛ�̌� */
	if (getRule(RULE_IIPIN_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꓛ�̌�", (getRule(RULE_IIPIN_MOYUE) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"�C��̌�", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleOne)); // �a���v���ꓛ
			}
		));
	/* �C�ꝝ�j */
	if (getRule(RULE_HAITEI_RAOTSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ�j", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CharacterOne)); // �a���v������
			}
		));
	/* �C�ꝝ�� */
	if (getRule(RULE_HAITEI_RAOHWA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����̌� */
	if (getRule(RULE_HATSUTEI_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����̌�", yaku::yakuCalculator::Yaku::yval_2han,
			"�C��̌�", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(gameStat->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == GreenDragon)); // �a���v���
			}
		));
	/* �͒ꃍ�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�͒ꝝ��", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(!gameStat->TsumoAgariFlag) && // �����A�K��
				(tilesLeft(gameStat) == 0)); // �n�C�e�C�ł���
		}
	));
	/* �㓛���� */
	if (getRule(RULE_CHUUPIN_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�㓛����", (getRule(RULE_CHUUPIN_RAOYUI) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"�͒ꝝ��", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!gameStat->TsumoAgariFlag) && // �����A�K��
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleNine)); // �a���v���㓛
			}
		));
	/* ���ꝝ�� */
	if (getRule(RULE_HAKUTEI_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ꝝ��", yaku::yakuCalculator::Yaku::yval_2han,
			"�͒ꝝ��", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!gameStat->TsumoAgariFlag) && // �����A�K��
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == WhiteDragon)); // �a���v����
			}
		));
	/* �D�ꝝ� */
	if (getRule(RULE_NITEI_RAOCHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�D�ꝝ�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // ���P�R�ł���
			}
		));
	/* ���X�g�I�[�_�[���k���� */
	if (getRule(RULE_MAABOODOUFU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���X�g�I�[�_�[���k����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���ꝝ��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == WhiteDragon)); // �a���v����
			}
		));
	/* �΂̏�ɂ��O�N */
	if (getRule(RULE_THREE_YEARS_ON_STONE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�΂̏�ɂ��O�N", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�_�u������", "�_�u���J����", "�C��̌�", "�͒ꝝ��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!gameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(gameStat) == 0) && // �n�C�e�C�ł���
					(gameStat->Player[analysis->player].RichiFlag.DoubleFlag)); // �_�u���������Ă���
			}
		));
}
