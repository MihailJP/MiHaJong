#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
	/* ���[�` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->PlayerStat->RichiFlag.RichiFlag)); // �������Ă���
		}
	));
	/* �_�u���[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�_�u������", yaku::yakuCalculator::Yaku::yval_2han_menzen,
		"����",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
				(analysis->PlayerStat->RichiFlag.DoubleFlag)); // �_�u�������t���O�������Ă���
		}
	));
	if (getRule(RULE_OPEN_RIICHI) != 0) {
		/* �v�����[ */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�I�[�v������", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			"����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �_�u�v�� */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�_�u���J����", yaku::yakuCalculator::Yaku::yval_3han_menzen,
			"����", "�_�u������", "�I�[�v������",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.DoubleFlag) && // �_�u�������t���O�������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �v�����[���e */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�J�����h�a", yaku::yakuCalculator::Yaku::HANFUNC( [](const MENTSU_ANALYSIS* const analysis) {
				return (*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(11, yaku::yakuCalculator::hanUnit::Han),
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN();
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag) && // �I�[�v�������t���O�������Ă���
					(!*analysis->TsumoAgariFlag) && // ����
					(analysis->GameStat->CurrentPlayer.Furikomi >= 0) &&
					(!analysis->GameStat->Player[analysis->GameStat->CurrentPlayer.Furikomi].RichiFlag.RichiFlag)); // �񃊁[�`�҂���̘a��
			}
		));
	}
	/* �ꔭ(�����ł͑��Ƃ�����) */
	if ((getRule(RULE_RIICHI_IPPATSU) != 1)&&(getRule(RULE_RIICHI_IPPATSU) != 3)) { // �ꔭ�����ɂȂ�Ȃ����[�������O
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꔭ", (getRule(RULE_RIICHI_IPPATSU) == 2) ?
			yaku::yakuCalculator::Yaku::yval_1han_menzen_dependent : /* �ꔭ�͔���𖞂����Ȃ�(�������V�o�Ń��[���݂̘̂a����F�߂Ȃ�)���[�� */
			yaku::yakuCalculator::Yaku::yval_1han_menzen, // ���[���݂̂ł��a�����ėǂ����[��
			/* �K�������ƕ������� */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.IppatsuFlag)); // �ꔭ�t���O�������Ă���
			}
		));
		/* ���ꔭ(���[�`�ꔭ�����Ƃ���a����) */
		if (getRule(RULE_CHOUIPPATSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"���ꔭ", (getRule(RULE_RIICHI_IPPATSU) == 2) ?
				yaku::yakuCalculator::Yaku::yval_2han_menzen_dependent : /* �ꔭ�͔���𖞂����Ȃ�(�������V�o�Ń��[���݂̘̂a����F�߂Ȃ�)���[�� */
				yaku::yakuCalculator::Yaku::yval_2han_menzen, // ���[���݂̂ł��a�����ėǂ����[��
				"�ꔭ", /* �K�������ƕ������� */
				[](const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
						(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
						(analysis->PlayerStat->RichiFlag.IppatsuFlag) && // �ꔭ�t���O�������Ă���
						(!*analysis->TsumoAgariFlag) && // �����ł���
						(
							(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sRight)) || // ���Ƃ���a������
							((chkGameType(analysis->GameStat, SanmaT)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == 0) && (analysis->player == 2)) || // �O���̓���
							((chkGameType(analysis->GameStat, Sanma4)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sOpposite)) &&
								(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sWest)) // �l�l�O���̓���
						));
				}
			));
	}
	/* �����[�` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�����[�`", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		/* �����͕�������炵�� */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->PlayerStat->RichiFlag.RichiFlag)) // �������Ă���
			{
				for (int i = 1; i < analysis->PlayerStat->DiscardPointer; i++)
					if ((analysis->PlayerStat->Discard[i].tcode.tile == BambooOne) &&
						((analysis->PlayerStat->Discard[i].dstat == discardRiichi) ||
						(analysis->PlayerStat->Discard[i].dstat == discardRiichiTaken)))
						return true;
				return false;
			}
			else return false;
		}
	));

	// ---------------------------------------------------------------------

	/* �V�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�V�a", (getRule(RULE_TENHOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : // �V�a���_�u���𖞂̃��[����
		yaku::yakuCalculator::Yaku::yval_yakuman_menzen, // ���ʂɖ𖞂ɂȂ邩
		"��O�����̘a",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(analysis->PlayerStat->FirstDrawFlag)); // �V�a�E�n�a�t���O�������Ă���
		}
	));
	/* �n�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�n�a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"��O�����̘a",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) != sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(analysis->PlayerStat->FirstDrawFlag) && // �V�a�E�n�a�t���O�������Ă���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
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
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) != sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
					(analysis->PlayerStat->FirstDrawFlag) && // �V�a�E�n�a�t���O�������Ă���
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	}
	/* �c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��O�����̘a", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(*analysis->MenzenFlag) && // ��O�ł���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));

	// ---------------------------------------------------------------------

	/* �O�Ǔ]�� */
	if (getRule(RULE_SANSEKI_TEMPUKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�Ǔ]��", yaku::yakuCalculator::Yaku::yval_3han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(!analysis->PlayerStat->RichiFlag.RichiFlag) && // �����̓��[�`���Ă��Ȃ�
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.RichiFlag) // ���ƑS������
					);
			}
		));
	/* �N���� */
	if (getRule(RULE_KISHI_KAISEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�N����", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(!analysis->PlayerStat->RichiFlag.RichiFlag) && // �����̓��[�`���Ă��Ȃ�
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.DoubleFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.DoubleFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.DoubleFlag) // ���ƑS���_�u���[(!)
					);
			}
		));

	// ---------------------------------------------------------------------

	/* �����V�����c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���J��", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* ���ȊJ�� */
	if ((getRule(RULE_MINKAN_PAO) >= 3) && (getRule(RULE_MINKAN_PAO) <= 5))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ȊJ��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->PaoFlag[pyMinkan].paoPlayer >= 0)); // ���Ȃ̒���
			}
		));
	/* �A�ȊJ�� */
	if (getRule(RULE_RENKAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�A�ȊJ��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.chainFlag >= 2)); // �A���ŞȂ�������
			}
		));
	/* �ܓ��J�� */
	if (getRule(RULE_UUPIN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܓ��J��", (getRule(RULE_UUPIN_KAIHOH) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����J�� */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����J��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == RedDragon)); // �a���v����
			}
		));
	/* ���Șa */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Șa", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.topFlag)); // ���Șa�t���O�������Ă���
			}
		));
	/* �T���i���z�[������ */
	if (getRule(RULE_SAYONARA_HOMERUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�T���i���z�[������", yaku::yakuCalculator::Yaku::yval_3han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0)); // ���v���������c��R�v��0
			}
		));
	/* ���ԉ� */
	if (getRule(RULE_HIGASHI_HANAZONO_TRIPLETS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԉ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->GameRound / 4 == 0) && // ����
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sEast) && // ����
					(analysis->KangziCount[EastWind] >= 1)); // ���̞Ȏq������
			}
		));

	// ---------------------------------------------------------------------

	/* �C��c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�C��̌�", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(*analysis->TsumoAgariFlag) && // �c���A�K��
				(tilesLeft(analysis->GameStat) == 0)); // �n�C�e�C�ł���
		}
	));
	/* �ꓛ�̌� */
	if (getRule(RULE_IIPIN_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꓛ�̌�", (getRule(RULE_IIPIN_MOYUE) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"�C��̌�", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleOne)); // �a���v���ꓛ
			}
		));
	/* �C�ꝝ�j */
	if (getRule(RULE_HAITEI_RAOTSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ�j", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CharacterOne)); // �a���v������
			}
		));
	/* �C�ꝝ�� */
	if (getRule(RULE_HAITEI_RAOHWA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����̌� */
	if (getRule(RULE_HATSUTEI_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����̌�", yaku::yakuCalculator::Yaku::yval_2han,
			"�C��̌�", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == GreenDragon)); // �a���v���
			}
		));
	/* �͒ꃍ�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�͒ꝝ��", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(!*analysis->TsumoAgariFlag) && // �����A�K��
				(tilesLeft(analysis->GameStat) == 0)); // �n�C�e�C�ł���
		}
	));
	/* �㓛���� */
	if (getRule(RULE_CHUUPIN_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�㓛����", (getRule(RULE_CHUUPIN_RAOYUI) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"�͒ꝝ��", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleNine)); // �a���v���㓛
			}
		));
	/* ���ꝝ�� */
	if (getRule(RULE_HAKUTEI_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ꝝ��", yaku::yakuCalculator::Yaku::yval_2han,
			"�͒ꝝ��", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �D�ꝝ� */
	if (getRule(RULE_NITEI_RAOCHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�D�ꝝ�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // ���P�R�ł���
			}
		));
	/* ���X�g�I�[�_�[���k���� */
	if (getRule(RULE_MAABOODOUFU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���X�g�I�[�_�[���k����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���ꝝ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �΂̏�ɂ��O�N */
	if (getRule(RULE_THREE_YEARS_ON_STONE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�΂̏�ɂ��O�N", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�_�u������", "�_�u���J����", "�C��̌�", "�͒ꝝ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->PlayerStat->RichiFlag.DoubleFlag)); // �_�u���������Ă���
			}
		));

	// ---------------------------------------------------------------------

	/* �ȐU�� */
	if (getRule(RULE_KAMBURI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ȐU��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chainFlag >= 1) && // �Ȃ���������ł���
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	/* ���Ȃ͐��m�ɂ͖ؕ΂ł͂Ȃ���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->KangFlag.chankanFlag)); // ���ȃt���O�������Ă���
		}
	));
	/* ������� */
	if (getRule(RULE_RYANZOH_CHANKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������",  (getRule(RULE_RYANZOH_CHANKAN) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chankanFlag) && // ���ȃt���O�������Ă���
					(analysis->TsumoHai->tile == BambooTwo)); // �a���v�����
			}
		));
	/* ���{�D�H */
	if (getRule(RULE_JINJI_DUOSHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���{�D�H", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chankanFlag) && // ���ȃt���O�������Ă���
					((analysis->SeenTiles[analysis->TsumoHai->tile] +
					analysis->TileCount[analysis->TsumoHai->tile]) >=
					(*analysis->TsumoAgariFlag ? 4 : 5)) && // ���X�v���ǂ�������
					(analysis->MachiInfo.MachiMen == 1) // 1�ʑ҂��ł���
					);
			}
		));

	// ---------------------------------------------------------------------

	/* ���A�� */
	if (getRule(RULE_PAARENCHAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��", yaku::yakuCalculator::Yaku::yval_yakuman_dependent, // �ڐA���d�l�ύX�F���A�����̂�����𖞂����Ȃ��悤�ɂ���
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->AgariChain == 8)); // �a�������̂��A��8���
			}
		));
	/* ���A�E��(�p�[�����u���C�J�[) */
	if (getRule(RULE_BREAKING_PAARENCHAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�j�񔪘A��", yaku::yakuCalculator::Yaku::yval_yakuman_dependent, // �������A�Ă߂������̋ǂŔ��A����a�������Ă����Ȃ�
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �܂��͂���
					(analysis->GameStat->AgariChain == -1)); // �ӂ������a���𓪃n�l(����Ȃ��Ă���������)�I
			}
		));

	// ---------------------------------------------------------------------

	/* ���Ԃ�(�����錾�v�Ń�������Ɩ����t��) */
	if (getRule(RULE_TSUBAME_GAESHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Ԃ�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->RichiCounter)); // �t���O�������Ă���
			}
		));
	/* ��(�_�u���[�錾�v�Ń�������Ɩ����t��) */
	if (getRule(RULE_HIEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"���Ԃ�",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->DoubleRichiCounter)); // �t���O�������Ă���
			}
		));

	// ---------------------------------------------------------------------

	/* �l�n�H */
	if (getRule(RULE_SUMARO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�n�H", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == CharacterFour) && // �a���v���l��
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(analysis->PlayerStat->SumaroFlag)); // ���փt���O�������Ă���
			}
		));
	/* ���� */
	if (getRule(RULE_MEILING) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooTwo) && // �a���v�����
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	/* �q���a(�I�^���̏o�a��) */
	if (getRule(RULE_OTAKAZE_RON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�q���a", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag;
				switch (tileCode tc = analysis->TsumoHai->tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind: // ���v������
					yakuFlag = (!*analysis->TsumoAgariFlag); // ���Ƀt���O�ݒ�
					if (tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))) // ����������
						yakuFlag = false;
					if (tc == Wind2Tile(analysis->GameStat->GameRound / 4)) // �ꕗ������
							yakuFlag = false;
					if ((getRule(RULE_KAIMENKAZE) != 0) &&
						(tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound)))) // �J�啗������
							yakuFlag = false;
					if ((getRule(RULE_URAKAZE) != 0) &&
						(tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, RelativePositionOf(analysis->player, sOpposite), analysis->GameStat->GameRound)))) // ����������
							yakuFlag = false;
					break;
				default: // ���v����Ȃ�����
					yakuFlag = false; break;
				}
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* �P�ґ��n��̖� */
	{
		const int Rules = 10;
		const RuleCode tmpRuleCodeList[Rules] = {
			RULE_HOUKANSOU, RULE_GEKKANJUN, RULE_SHINRIISHOU, RULE_KINKI_KIKAN, RULE_HOKUTO_SHOUKAN,
			RULE_DAIJA_KANKETSU, RULE_DONGFENGCHUI, RULE_NANFENGCHUI, RULE_XIFENGCHUI, RULE_BEIFENGCHUI,
		};
		const tileCode tmpTileCodeList[Rules] = {
			BambooOne, CircleOne, CharacterOne, BambooSeven, CircleSeven,
			CharacterSeven, EastWind, SouthWind, WestWind, NorthWind,
		};
		const char tmpYakuNameList[Rules][16] = {
			"�P�ґ�", "���ҏ�", "�j�ߎ�", "���T�A��", "�k�l����",
			"��֊Ҍ�", "������", "�앗��", "������", "�k����",
		};
		for (int i = 0; i < Rules; i++) {
			if (getRule(tmpRuleCodeList[i]) != 0) {
				tileCode tc = tmpTileCodeList[i];
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					tmpYakuNameList[i], (getRule(tmpRuleCodeList[i]) == 2) ?
					yaku::yakuCalculator::Yaku::yval_2han : yaku::yakuCalculator::Yaku::yval_1han,
					[tc](const MENTSU_ANALYSIS* const analysis) -> bool {
						bool yakuFlag = false;
						for (int i = 0; i < analysis->PlayerStat->DiscardPointer; i++) {
							if (analysis->PlayerStat->Discard[i].tcode.tile == tc) {
								yakuFlag = true; break; // �����t���e���ł���
							}
						}
						return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
							(analysis->TsumoHai->tile == tc) && // �a���v
							(*analysis->TsumoAgariFlag) && (yakuFlag) && // �t���e���c��
							(analysis->Machi == yaku::yakuCalculator::machiTanki)); // �P�R�҂��ł���
					}
				));
			}
		}
	}

	// ---------------------------------------------------------------------

	/* �����V�a */
	if (getRule(RULE_GOLDDRAGON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����V�a", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= CircleOne) && // �a���v�����q
					(analysis->TsumoHai->tile <= CircleNine) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �����h�a */
	if (getRule(RULE_WHIRLWIND) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����h�a", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= EastWind) && // �a���v�����v
					(analysis->TsumoHai->tile <= NorthWind) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �V������ */
	if (getRule(RULE_BOLT_FROM_THE_BLUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�V������", yaku::yakuCalculator::Yaku::yval_4han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->Player[0].RichiFlag.RichiFlag) && // �N�����[�`���Ă��Ȃ�
					(!analysis->GameStat->Player[1].RichiFlag.RichiFlag) &&
					(!analysis->GameStat->Player[2].RichiFlag.RichiFlag) &&
					(!analysis->GameStat->Player[3].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[0].DiscardPointer == 0) && // �N�����Ă��Ȃ�
					(analysis->GameStat->Player[1].DiscardPointer == 0) &&
					(analysis->GameStat->Player[2].DiscardPointer == 0) &&
					(analysis->GameStat->Player[3].DiscardPointer == 0) &&
					(analysis->GameStat->TurnRound <= 4)); // 4���ڈȓ�
			}
		));
	/* �L�܂������� */
	if (getRule(RULE_GOLDDRAGON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�L�܂�������", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenRegular] == -1) && // �ʎq��Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooThree) && // �a���v���O��
					(analysis->Machi == yaku::yakuCalculator::machiKanchan)); // �ƒ��҂�
			}
		));

}
