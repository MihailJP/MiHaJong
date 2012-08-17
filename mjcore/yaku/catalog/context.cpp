#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
	/* ���[�` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->PlayerStat->RichiFlag.RichiFlag)); // �������Ă���
		}
	));
	/* �_�u���[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�_�u������", yaku::yakuCalculator::Yaku::yval_2han_menzen,
		"����",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
				(analysis->PlayerStat->RichiFlag.DoubleFlag)); // �_�u�������t���O�������Ă���
		}
	));
	if (RuleData::getRule("open_riichi") != 0) {
		/* �v�����[ */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�I�[�v������", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			"����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �_�u�v�� */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�_�u���J����", yaku::yakuCalculator::Yaku::yval_3han_menzen,
			"����", "�_�u������", "�I�[�v������",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.DoubleFlag) && // �_�u�������t���O�������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �v�����[���e */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�J�����h�a", yaku::yakuCalculator::Yaku::HANFUNC( [](const MENTSU_ANALYSIS* const analysis) {
				return (*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(11, yaku::yakuCalculator::Han),
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN();
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag) && // �I�[�v�������t���O�������Ă���
					(!*analysis->TsumoAgariFlag) && // ����
					(analysis->GameStat->CurrentPlayer.Furikomi >= 0) &&
					(!analysis->GameStat->Player[analysis->GameStat->CurrentPlayer.Furikomi].RichiFlag.RichiFlag)); // �񃊁[�`�҂���̘a��
			}
		));
	}
	/* �ꔭ(�����ł͑��Ƃ�����) */
	if ((RuleData::getRule("riichi_ippatsu") != 1)&&(RuleData::getRule("riichi_ippatsu") != 3)) { // �ꔭ�����ɂȂ�Ȃ����[�������O
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꔭ", (RuleData::getRule("riichi_ippatsu") == 2) ?
			yaku::yakuCalculator::Yaku::yval_1han_menzen_dependent : /* �ꔭ�͔���𖞂����Ȃ�(�������V�o�Ń��[���݂̘̂a����F�߂Ȃ�)���[�� */
			yaku::yakuCalculator::Yaku::yval_1han_menzen, // ���[���݂̂ł��a�����ėǂ����[��
			/* �K�������ƕ������� */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.IppatsuFlag)); // �ꔭ�t���O�������Ă���
			}
		));
		/* ���ꔭ(���[�`�ꔭ�����Ƃ���a����) */
		if (RuleData::getRule("chouippatsu") != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"���ꔭ", (RuleData::getRule("riichi_ippatsu") == 2) ?
				yaku::yakuCalculator::Yaku::yval_2han_menzen_dependent : /* �ꔭ�͔���𖞂����Ȃ�(�������V�o�Ń��[���݂̘̂a����F�߂Ȃ�)���[�� */
				yaku::yakuCalculator::Yaku::yval_2han_menzen, // ���[���݂̂ł��a�����ėǂ����[��
				"�ꔭ", /* �K�������ƕ������� */
				[](const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
			if ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
		"�V�a", (RuleData::getRule("tenhoh") != 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : // �V�a���_�u���𖞂̃��[����
		yaku::yakuCalculator::Yaku::yval_yakuman_menzen, // ���ʂɖ𖞂ɂȂ邩
		"��O�����̘a",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(analysis->PlayerStat->FirstDrawFlag)); // �V�a�E�n�a�t���O�������Ă���
		}
	));
	/* �n�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�n�a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"��O�����̘a",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) != sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(analysis->PlayerStat->FirstDrawFlag) && // �V�a�E�n�a�t���O�������Ă���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* �Q�ʂ���ʖڂȂ�ł����H */
	if (RuleData::getRule("renhoh") != 0) {
		yaku::yakuCalculator::Yaku::MenzenHan renhoh_han;
		switch (RuleData::getRule("renhoh")) {
			case 1: renhoh_han = yaku::yakuCalculator::Yaku::yval_yakuman_menzen; break; // ��
			case 2: renhoh_han = yaku::yakuCalculator::Yaku::yval_baiman_menzen; break; // �{��
			case 3: renhoh_han = yaku::yakuCalculator::Yaku::yval_mangan_menzen; break; // ����
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�a", renhoh_han, "��O�����̘a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(*analysis->MenzenFlag) && // ��O�ł���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));

	// ---------------------------------------------------------------------

	/* �O�Ǔ]�� */
	if (RuleData::getRule("sanseki_tempuku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�Ǔ]��", yaku::yakuCalculator::Yaku::yval_3han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(!analysis->PlayerStat->RichiFlag.RichiFlag) && // �����̓��[�`���Ă��Ȃ�
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.RichiFlag) // ���ƑS������
					);
			}
		));
	/* �N���� */
	if (RuleData::getRule("kishi_kaisei") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�N����", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* ���ȊJ�� */
	if ((RuleData::getRule("minkan_pao") >= 3) && (RuleData::getRule("minkan_pao") <= 5))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ȊJ��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->PaoFlag[pyMinkan].paoPlayer >= 0)); // ���Ȃ̒���
			}
		));
	/* �A�ȊJ�� */
	if (RuleData::getRule("renkan_kaihoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�A�ȊJ��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.chainFlag >= 2)); // �A���ŞȂ�������
			}
		));
	/* �ܓ��J�� */
	if (RuleData::getRule("uupin_kaihoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܓ��J��", (RuleData::getRule("uupin_kaihoh") == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����J�� */
	if (RuleData::getRule("chunshan_kaihoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����J��", yaku::yakuCalculator::Yaku::yval_2han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == RedDragon)); // �a���v����
			}
		));
	/* ���Șa */
	if (RuleData::getRule("chunshan_kaihoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Șa", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.topFlag)); // ���Șa�t���O�������Ă���
			}
		));
	/* �T���i���z�[������ */
	if (RuleData::getRule("sayonara_homerun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�T���i���z�[������", yaku::yakuCalculator::Yaku::yval_3han,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0)); // ���v���������c��R�v��0
			}
		));
	/* ���ԉ� */
	if (RuleData::getRule("higashi_hanazono_triplets") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԉ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(*analysis->TsumoAgariFlag) && // �c���A�K��
				(tilesLeft(analysis->GameStat) == 0)); // �n�C�e�C�ł���
		}
	));
	/* �ꓛ�̌� */
	if (RuleData::getRule("iipin_moyue") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꓛ�̌�", (RuleData::getRule("iipin_moyue") == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"�C��̌�", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleOne)); // �a���v���ꓛ
			}
		));
	/* �C�ꝝ�j */
	if (RuleData::getRule("haitei_raotsen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ�j", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CharacterOne)); // �a���v������
			}
		));
	/* �C�ꝝ�� */
	if (RuleData::getRule("haitei_raohwa") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����̌� */
	if (RuleData::getRule("hatsutei_moyue") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����̌�", yaku::yakuCalculator::Yaku::yval_2han,
			"�C��̌�", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(!*analysis->TsumoAgariFlag) && // �����A�K��
				(tilesLeft(analysis->GameStat) == 0)); // �n�C�e�C�ł���
		}
	));
	/* �㓛���� */
	if (RuleData::getRule("chuupin_raoyui") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�㓛����", (RuleData::getRule("chuupin_raoyui") == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"�͒ꝝ��", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleNine)); // �a���v���㓛
			}
		));
	/* ���ꝝ�� */
	if (RuleData::getRule("hakutei_raoyui") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ꝝ��", yaku::yakuCalculator::Yaku::yval_2han,
			"�͒ꝝ��", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �D�ꝝ� */
	if (RuleData::getRule("nitei_raochun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�D�ꝝ�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // ���P�R�ł���
			}
		));
	/* ���X�g�I�[�_�[���k���� */
	if (RuleData::getRule("maaboodoufu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���X�g�I�[�_�[���k����", yaku::yakuCalculator::Yaku::yval_yakuman,
			"���ꝝ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �΂̏�ɂ��O�N */
	if (RuleData::getRule("three_years_on_stone") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�΂̏�ɂ��O�N", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�_�u������", "�_�u���J����", "�C��̌�", "�͒ꝝ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->PlayerStat->RichiFlag.DoubleFlag)); // �_�u���������Ă���
			}
		));

	// ---------------------------------------------------------------------

	/* �ȐU�� */
	if (RuleData::getRule("kamburi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ȐU��", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chainFlag >= 1) && // �Ȃ���������ł���
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	/* ���Ȃ͐��m�ɂ͖ؕ΂ł͂Ȃ���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����", yaku::yakuCalculator::Yaku::yval_1han,
		"���v�a", "�ȐU��" /* �ȐU��͖{�����ʖ��ł͂Ȃ������ʖ𔻒�̃V�X�e�����g���ĕ������Ȃ��悤�ɂ��� */,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->KangFlag.chankanFlag)); // ���ȃt���O�������Ă���
		}
	));
	/* ������� */
	if (RuleData::getRule("ryanzoh_chankan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������",  (RuleData::getRule("ryanzoh_chankan") == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chankanFlag) && // ���ȃt���O�������Ă���
					(analysis->TsumoHai->tile == BambooTwo)); // �a���v�����
			}
		));
	/* ���{�D�H */
	if (RuleData::getRule("jinji_duoshi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���{�D�H", yaku::yakuCalculator::Yaku::yval_1han,
			"���v�a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
	if (RuleData::getRule("paarenchan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��", yaku::yakuCalculator::Yaku::yval_yakuman_dependent, // �ڐA���d�l�ύX�F���A�����̂�����𖞂����Ȃ��悤�ɂ���
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->AgariChain == 8)); // �a�������̂��A��8���
			}
		));
	/* ���A�E��(�p�[�����u���C�J�[) */
	if (RuleData::getRule("breaking_paarenchan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�j�񔪘A��", yaku::yakuCalculator::Yaku::yval_yakuman_dependent, // �������A�Ă߂������̋ǂŔ��A����a�������Ă����Ȃ�
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �܂��͂���
					(analysis->GameStat->AgariChain == -1)); // �ӂ������a���𓪃n�l(����Ȃ��Ă���������)�I
			}
		));

	// ---------------------------------------------------------------------

	/* ���Ԃ�(�����錾�v�Ń�������Ɩ����t��) */
	if (RuleData::getRule("tsubame_gaeshi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Ԃ�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->RichiCounter)); // �t���O�������Ă���
			}
		));
	/* ��(�_�u���[�錾�v�Ń�������Ɩ����t��) */
	if (RuleData::getRule("hien") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"���Ԃ�",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->DoubleRichiCounter)); // �t���O�������Ă���
			}
		));

	// ---------------------------------------------------------------------

	/* �l�n�H */
	if (RuleData::getRule("sumaro") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�n�H", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == CharacterFour) && // �a���v���l��
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(analysis->PlayerStat->SumaroFlag)); // ���փt���O�������Ă���
			}
		));
	/* ���� */
	if (RuleData::getRule("meiling") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooTwo) && // �a���v�����
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	/* �q���a(�I�^���̏o�a��) */
	if (RuleData::getRule("otakaze_ron") != 0)
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
					if ((RuleData::getRule("kaimenkaze") != 0) &&
						(tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound)))) // �J�啗������
							yakuFlag = false;
					if ((RuleData::getRule("urakaze") != 0) &&
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
		const char tmpRuleCodeList[Rules][16] = {
			"houkansou", "gekkanjun", "shinriishou", "kinki_kikan", "hokuto_shoukan",
			"daija_kanketsu", "dongfengchui", "nanfengchui", "xifengchui", "beifengchui",
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
			if (RuleData::getRule(tmpRuleCodeList[i]) != 0) {
				tileCode tc = tmpTileCodeList[i];
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					tmpYakuNameList[i], (RuleData::getRule(tmpRuleCodeList[i]) == 2) ?
					yaku::yakuCalculator::Yaku::yval_2han : yaku::yakuCalculator::Yaku::yval_1han,
					[tc](const MENTSU_ANALYSIS* const analysis) -> bool {
						bool yakuFlag = false;
						for (int i = 0; i < analysis->PlayerStat->DiscardPointer; i++) {
							if (analysis->PlayerStat->Discard[i].tcode.tile == tc) {
								yakuFlag = true; break; // �����t���e���ł���
							}
						}
						return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
	if (RuleData::getRule("golddragon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����V�a", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= CircleOne) && // �a���v�����q
					(analysis->TsumoHai->tile <= CircleNine) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �����h�a */
	if (RuleData::getRule("whirlwind") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����h�a", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= EastWind) && // �a���v�����v
					(analysis->TsumoHai->tile <= NorthWind) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �V������ */
	if (RuleData::getRule("bolt_from_the_blue") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�V������", yaku::yakuCalculator::Yaku::yval_4han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
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
	if (RuleData::getRule("golddragon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�L�܂�������", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) && // �ʎq��Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooThree) && // �a���v���O��
					(analysis->Machi == yaku::yakuCalculator::machiKanchan)); // �ƒ��҂�
			}
		));

	// ---------------------------------------------------------------------

	/* ���v�a(���X��chk-post.hsp�ɏ����Ă����ǌ�񂵂ɂ���K�v�Ȃ�ĂȂ�����) */
	if (RuleData::getRule("keppaihoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���v�a", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag =
					*analysis->TsumoAgariFlag ? // �c���A�K����������A
					(analysis->SeenTiles[analysis->TsumoHai->tile] + // �����Ă�v��
					analysis->TileCount[analysis->TsumoHai->tile] >= 4) // ��̓��̔v�𑫂���4��
					: // ������������A
					(analysis->SeenTiles[analysis->TsumoHai->tile] + // �����Ă�v��
					analysis->TileCount[analysis->TsumoHai->tile] > 4); // ��̓��̔v�𑫂���4��
				return (yakuFlag && // �����𖞂����Ă��āA
					(analysis->MachiInfo.MachiMen == 1)); // 1�ʑ҂�(�ڐA���ύX�F�m�x�P�Ƃ��ł͐������Ȃ��悤�ɂ���)
			}
		));
}
