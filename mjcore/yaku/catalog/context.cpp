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
	if (RuleData::chkRuleApplied("open_riichi")) {
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
	if (RuleData::chkRuleApplied("riichi_ippatsu")&&(!RuleData::chkRule("riichi_ippatsu", "chip_only"))) { // �ꔭ�����ɂȂ�Ȃ����[�������O
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꔭ", RuleData::chkRule("riichi_ippatsu", "yes_but_unbound") ?
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
		if (RuleData::chkRuleApplied("chouippatsu"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"���ꔭ", RuleData::chkRule("riichi_ippatsu", "yes_but_unbound") ?
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
	if (RuleData::chkRuleApplied("bird_riichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����[�`", get_yaku_han("bird_riichi"),
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
		"�V�a", get_yaku_han("tenhoh"), // ���ʂɖ𖞂ɂȂ邩
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
	if (RuleData::chkRuleApplied("renhoh")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�a", get_yaku_han("renhoh"), "��O�����̘a",
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
	if (RuleData::chkRuleApplied("sanseki_tempuku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�Ǔ]��", get_yaku_han("sanseki_tempuku"),
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
	if (RuleData::chkRuleApplied("kishi_kaisei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�N����", get_yaku_han("kishi_kaisei"),
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
	if (RuleData::chkRule("minkan_pao", "no_but_2han") ||
		RuleData::chkRule("minkan_pao", "yes_2han") ||
		RuleData::chkRule("minkan_pao", "yes_2han_andalso_contiguous_kong"))
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
	if (RuleData::chkRuleApplied("renkan_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�A�ȊJ��", get_yaku_han("renkan_kaihoh"),
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.chainFlag >= 2)); // �A���ŞȂ�������
			}
		));
	/* �ܓ��J�� */
	if (RuleData::chkRuleApplied("uupin_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ܓ��J��", get_yaku_han("uupin_kaihoh"),
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����J�� */
	if (RuleData::chkRuleApplied("chunshan_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����J��", get_yaku_han("chunshan_kaihoh"),
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == RedDragon)); // �a���v����
			}
		));
	/* ���Șa */
	if (RuleData::chkRuleApplied("toukanhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Șa", get_yaku_han("toukanhoh"),
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.topFlag)); // ���Șa�t���O�������Ă���
			}
		));
	/* �T���i���z�[������ */
	if (RuleData::chkRuleApplied("sayonara_homerun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�T���i���z�[������", get_yaku_han("sayonara_homerun"),
			"���J��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0)); // ���v���������c��R�v��0
			}
		));
	/* ���ԉ� */
	if (RuleData::chkRuleApplied("higashi_hanazono_triplets"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԉ�", get_yaku_han("higashi_hanazono_triplets"),
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
	if (RuleData::chkRuleApplied("iipin_moyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ꓛ�̌�", get_yaku_han("iipin_moyue"),
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
	if (RuleData::chkRuleApplied("haitei_raotsen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ�j", get_yaku_han("haitei_raotsen"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CharacterOne)); // �a���v������
			}
		));
	/* �C�ꝝ�� */
	if (RuleData::chkRuleApplied("haitei_raohwa"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�ꝝ��", get_yaku_han("haitei_raohwa"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����̌� */
	if (RuleData::chkRuleApplied("hatsutei_moyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����̌�", get_yaku_han("hatsutei_moyue"),
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
	if (RuleData::chkRuleApplied("chuupin_raoyui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�㓛����", get_yaku_han("chuupin_raoyui"),
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
	if (RuleData::chkRuleApplied("hakutei_raoyui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ꝝ��", get_yaku_han("hakutei_raoyui"),
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
	if (RuleData::chkRuleApplied("nitei_raochun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�D�ꝝ�", get_yaku_han("nitei_raochun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // ���P�R�ł���
			}
		));
	/* ���X�g�I�[�_�[���k���� */
	if (RuleData::chkRuleApplied("maaboodoufu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���X�g�I�[�_�[���k����", get_yaku_han("maaboodoufu"),
			"���ꝝ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(tilesLeft(analysis->GameStat) == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �΂̏�ɂ��O�N */
	if (RuleData::chkRuleApplied("three_years_on_stone"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�΂̏�ɂ��O�N", get_yaku_han("three_years_on_stone"),
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
	if (RuleData::chkRuleApplied("kamburi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ȐU��", get_yaku_han("kamburi"),
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
	if (RuleData::chkRuleApplied("ryanzoh_chankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�������", get_yaku_han("ryanzoh_chankan"),
			"����",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chankanFlag) && // ���ȃt���O�������Ă���
					(analysis->TsumoHai->tile == BambooTwo)); // �a���v�����
			}
		));
	/* ���{�D�H */
	if (RuleData::chkRuleApplied("jinji_duoshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���{�D�H", get_yaku_han("jinji_duoshi"),
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
	if (RuleData::chkRuleApplied("paarenchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���A��", get_yaku_han("paarenchan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->AgariChain == 8)); // �a�������̂��A��8���
			}
		));
	/* ���A�E��(�p�[�����u���C�J�[) */
	if (RuleData::chkRuleApplied("breaking_paarenchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�j�񔪘A��", get_yaku_han("breaking_paarenchan"), // �������A�Ă߂������̋ǂŔ��A����a�������Ă����Ȃ�
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �܂��͂���
					(analysis->GameStat->AgariChain == -1)); // �ӂ������a���𓪃n�l(����Ȃ��Ă���������)�I
			}
		));

	// ---------------------------------------------------------------------

	/* ���Ԃ�(�����錾�v�Ń�������Ɩ����t��) */
	if (RuleData::chkRuleApplied("tsubame_gaeshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���Ԃ�", get_yaku_han("tsubame_gaeshi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->RichiCounter)); // �t���O�������Ă���
			}
		));
	/* ��(�_�u���[�錾�v�Ń�������Ɩ����t��) */
	if (RuleData::chkRuleApplied("hien"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��", get_yaku_han("hien"),
			"���Ԃ�",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->DoubleRichiCounter)); // �t���O�������Ă���
			}
		));

	// ---------------------------------------------------------------------

	/* �l�n�H */
	if (RuleData::chkRuleApplied("sumaro"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l�n�H", get_yaku_han("sumaro"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == CharacterFour) && // �a���v���l��
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(analysis->PlayerStat->SumaroFlag)); // ���փt���O�������Ă���
			}
		));
	/* ���� */
	if (RuleData::chkRuleApplied("meiling"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", get_yaku_han("meiling"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooTwo) && // �a���v�����
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	/* �q���a(�I�^���̏o�a��) */
	if (RuleData::chkRuleApplied("otakaze_ron"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�q���a", get_yaku_han("otakaze_ron"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag;
				switch (tileCode tc = analysis->TsumoHai->tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind: // ���v������
					yakuFlag = (!*analysis->TsumoAgariFlag); // ���Ƀt���O�ݒ�
					if (tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))) // ����������
						yakuFlag = false;
					if (tc == Wind2Tile(analysis->GameStat->GameRound / 4)) // �ꕗ������
							yakuFlag = false;
					if (RuleData::chkRuleApplied("kaimenkaze") &&
						(tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound)))) // �J�啗������
							yakuFlag = false;
					if (RuleData::chkRuleApplied("urakaze") &&
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
			if (RuleData::chkRuleApplied(tmpRuleCodeList[i])) {
				tileCode tc = tmpTileCodeList[i];
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					tmpYakuNameList[i], get_yaku_han(tmpRuleCodeList[i]),
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
	if (RuleData::chkRuleApplied("golddragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����V�a", get_yaku_han("golddragon"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= CircleOne) && // �a���v�����q
					(analysis->TsumoHai->tile <= CircleNine) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �����h�a */
	if (RuleData::chkRuleApplied("whirlwind"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����h�a", get_yaku_han("whirlwind"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= EastWind) && // �a���v�����v
					(analysis->TsumoHai->tile <= NorthWind) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �V������ */
	if (RuleData::chkRuleApplied("bolt_from_the_blue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�V������", get_yaku_han("bolt_from_the_blue"),
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
	if (RuleData::chkRuleApplied("nekomasshigura"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�L�܂�������", get_yaku_han("nekomasshigura"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) && // �ʎq��Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooThree) && // �a���v���O��
					(analysis->Machi == yaku::yakuCalculator::machiKanchan)); // �ƒ��҂�
			}
		));

	// ---------------------------------------------------------------------

	/* ���v�a(���X��chk-post.hsp�ɏ����Ă����ǌ�񂵂ɂ���K�v�Ȃ�ĂȂ�����) */
	if (RuleData::chkRuleApplied("keppaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���v�a", get_yaku_han("keppaihoh"),
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
