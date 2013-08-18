#include "../catalog.h"

#include "../../func.h"

inline bool isFinalRound(const GameTable* const gameStat) { // �I�[���X�ł���H
	return (gameStat->LoopRound * roundLoopRate() +
		gameStat->GameRound) >= gameStat->GameLength;
}

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
#ifndef GUOBIAO
	/* ���[�` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("����"), yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->PlayerStat->RichiFlag.RichiFlag)); // �������Ă���
		}
	));
	/* �_�u���[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�_�u������"), yaku::yakuCalculator::Yaku::yval_2han_menzen,
		_T("����"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
				(analysis->PlayerStat->RichiFlag.DoubleFlag)); // �_�u�������t���O�������Ă���
		}
	));
	if (RuleData::chkRuleApplied("open_riichi")) {
		/* �v�����[ */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�I�[�v������"), yaku::yakuCalculator::Yaku::yval_2han_menzen,
			_T("����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �_�u�v�� */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�_�u���J����"), yaku::yakuCalculator::Yaku::yval_3han_menzen,
			_T("����"), _T("�_�u������"), _T("�I�[�v������"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.DoubleFlag) && // �_�u�������t���O�������Ă���
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // �I�[�v�������t���O�������Ă���
			}
		));
		/* �v�����[���e */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�J�����h�a"), yaku::yakuCalculator::Yaku::HANFUNC( [](const MENTSU_ANALYSIS* const analysis) {
				return (*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(11, yaku::yakuCalculator::Han),
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
	if (RuleData::chkRuleApplied("riichi_ippatsu")&&(!RuleData::chkRule("riichi_ippatsu", "chip_only"))) { // �ꔭ�����ɂȂ�Ȃ����[�������O
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ꔭ"), RuleData::chkRule("riichi_ippatsu", "yes_but_unbound") ?
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
		if (RuleData::chkRuleApplied("chouippatsu"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("���ꔭ"), RuleData::chkRule("riichi_ippatsu", "yes_but_unbound") ?
				yaku::yakuCalculator::Yaku::yval_2han_menzen_dependent : /* �ꔭ�͔���𖞂����Ȃ�(�������V�o�Ń��[���݂̘̂a����F�߂Ȃ�)���[�� */
				yaku::yakuCalculator::Yaku::yval_2han_menzen, // ���[���݂̂ł��a�����ėǂ����[��
				_T("�ꔭ"), /* �K�������ƕ������� */
				[](const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
						(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
						(analysis->PlayerStat->RichiFlag.IppatsuFlag) && // �ꔭ�t���O�������Ă���
						(!*analysis->TsumoAgariFlag) && // �����ł���
						(
							(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sRight)) || // ���Ƃ���a������
							((analysis->GameStat->chkGameType(SanmaT)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == 0) && (analysis->player == 2)) || // �O���̓���
							((analysis->GameStat->chkGameType(Sanma4)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sOpposite)) &&
								(analysis->GameStat->playerwind(analysis->player) == sWest)) // �l�l�O���̓���
						));
				}
			));
		/* ��P */
		if (RuleData::chkRuleApplied("minamityan"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("��P"), get_yaku_han("minamityan"),
				/* �K�������A�ꔭ�A�c���ƕ������� */
				[](const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
						(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
						(analysis->PlayerStat->RichiFlag.IppatsuFlag) && // �ꔭ�t���O�������Ă���
						(*analysis->TsumoAgariFlag) && // �c���ł���
						(analysis->TsumoHai->tile == SouthWind)); // ��Řa��
				}
			));
	}
	/* �����[�` */
	if (RuleData::chkRuleApplied("bird_riichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����[�`"), get_yaku_han("bird_riichi"),
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
	/* �����[�z���C�g */
	if (RuleData::chkRuleApplied("lily_white"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����[�z���C�g"), yaku::yakuCalculator::Yaku::yval_1han_menzen,
			/* �_�u�������ɒǉ������ */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->PlayerStat->RichiFlag.DoubleFlag) && // �_�u�������t���O�������Ă���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �k�l��(��������) */
	if (RuleData::chkRuleApplied("pei4jun_riichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�k�l��"), get_yaku_han("pei4jun_riichi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // �������Ă���
					(analysis->TsumoHai->tile == NorthWind) && // �a���v���k
					(analysis->GameStat->TurnRound == 4)); // 4���ڂł���
			}
		));

	// ---------------------------------------------------------------------

	/* �V�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�V�a"), get_yaku_han("tenhoh"), // ���ʂɖ𖞂ɂȂ邩
		_T("��O�����̘a"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->playerwind(analysis->player) == sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(analysis->PlayerStat->FirstDrawFlag)); // �V�a�E�n�a�t���O�������Ă���
		}
	));
	/* �n�a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�n�a"), yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		_T("��O�����̘a"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->playerwind(analysis->player) != sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
				(analysis->PlayerStat->FirstDrawFlag) && // �V�a�E�n�a�t���O�������Ă���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* �Q�ʂ���ʖڂȂ�ł����H */
	if (RuleData::chkRuleApplied("renhoh")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�a"), get_yaku_han("renhoh"), _T("��O�����̘a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->playerwind(analysis->player) != sEast) && // �e�ł���(���ƂƂ��Č��ߑł��c�c)
					(analysis->PlayerStat->FirstDrawFlag) && // �V�a�E�n�a�t���O�������Ă���
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	}
#endif /* GUOBIAO */
#ifdef GUOBIAO
	/* �s���l */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�s���l"), yaku::yakuCalculator::Yaku::yval_4,
		_T("����"), _T("��O��"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �a���ɂȂ��Ă���
				(*analysis->MenzenFlag) && // ��O�ł���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
	/* ��O�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��O��"), yaku::yakuCalculator::Yaku::yval_2,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �a���ɂȂ��Ă���
				(*analysis->MenzenFlag)); // ��O�ł���
		}
	));
	/* ���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("����"), yaku::yakuCalculator::Yaku::yval_1,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �a���ɂȂ��Ă���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
#else /* GUOBIAO */
	/* �c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("��O�����̘a"), yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(*analysis->MenzenFlag) && // ��O�ł���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* �O�Ǔ]�� */
	if (RuleData::chkRuleApplied("sanseki_tempuku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�Ǔ]��"), get_yaku_han("sanseki_tempuku"),
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
	if (RuleData::chkRuleApplied("kishi_kaisei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�N����"), get_yaku_han("kishi_kaisei"),
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

	/* �s���� */
	if (RuleData::chkRuleApplied("phoenix"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�s����"), get_yaku_han("phoenix"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->YakitoriFlag) && // �Ă����ł���
					(isFinalRound(analysis->GameStat))); // �I�[���X�ł���
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �����V�����c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�ȏ�J��"), yaku::yakuCalculator::Yaku::yval_8,
		_T("����"),
#else /* GUOBIAO */
		_T("���J��"), yaku::yakuCalculator::Yaku::yval_1han,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
				(*analysis->TsumoAgariFlag)); // �c���A�K��
		}
	));
#ifndef GUOBIAO
	/* ���ȊJ�� */
	if (RuleData::chkRule("minkan_pao", "no_but_2han") ||
		RuleData::chkRule("minkan_pao", "yes_2han") ||
		RuleData::chkRule("minkan_pao", "yes_2han_andalso_contiguous_kong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ȊJ��"), yaku::yakuCalculator::Yaku::yval_2han,
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->PaoFlag[pyMinkan].paoPlayer >= 0)); // ���Ȃ̒���
			}
		));
	/* �A�ȊJ�� */
	if (RuleData::chkRuleApplied("renkan_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�A�ȊJ��"), get_yaku_han("renkan_kaihoh"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.chainFlag >= 2)); // �A���ŞȂ�������
			}
		));
	/* �ܓ��J�� */
	if (RuleData::chkRuleApplied("uupin_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ܓ��J��"), get_yaku_han("uupin_kaihoh"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����J�� */
	if (RuleData::chkRuleApplied("chunshan_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����J��"), get_yaku_han("chunshan_kaihoh"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->TsumoHai->tile == RedDragon)); // �a���v����
			}
		));
	/* ���Șa */
	if (RuleData::chkRuleApplied("toukanhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���Șa"), get_yaku_han("toukanhoh"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->KangFlag.topFlag)); // ���Șa�t���O�������Ă���
			}
		));
	/* �T���i���z�[������ */
	if (RuleData::chkRuleApplied("sayonara_homerun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�T���i���z�[������"), get_yaku_han("sayonara_homerun"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->tilesLeft() == 0)); // ���v���������c��R�v��0
			}
		));
	/* �[��J�� */
	if (RuleData::chkRuleApplied("shenshang_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�[��J��"), get_yaku_han("shenshang_kaihoh"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(isFinalRound(analysis->GameStat))); // �I�[���X�ł���
			}
		));
	/* ���ԉ� */
	if (RuleData::chkRuleApplied("higashi_hanazono_triplets"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ԉ�"), get_yaku_han("higashi_hanazono_triplets"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->GameRound / 4 == 0) && // ����
					(analysis->GameStat->playerwind(analysis->player) == sEast) && // ����
					(analysis->KangziCount[EastWind] >= 1)); // ���̞Ȏq������
			}
		));
	/* �Ԍ� */
	if (RuleData::chkRuleApplied("hanami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Ԍ�"), get_yaku_han("hanami"),
			_T("���J��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł���
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					((analysis->TsumoHai->tile == CharacterThree) || // �a���v���O�݂�
					(analysis->TsumoHai->tile == CircleThree) || // �O����
					(analysis->TsumoHai->tile == BambooThree)) && // �O��
					(analysis->KangziCount[CircleFive] >= 1)); // �ܓ���Ȃ��Ă���
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* �C��c�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�����t"), yaku::yakuCalculator::Yaku::yval_8,
		_T("����"),
#else /* GUOBIAO */
		_T("�C��̌�"), yaku::yakuCalculator::Yaku::yval_1han,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(*analysis->TsumoAgariFlag) && // �c���A�K��
				(analysis->GameStat->tilesLeft() == 0)); // �n�C�e�C�ł���
		}
	));
#ifndef GUOBIAO
	/* �ꓛ�̌� */
	if (RuleData::chkRuleApplied("iipin_moyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ꓛ�̌�"), get_yaku_han("iipin_moyue"),
			_T("�C��̌�"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleOne)); // �a���v���ꓛ
			}
		));
	/* �C�ꝝ�j */
	if (RuleData::chkRuleApplied("haitei_raotsen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�C�ꝝ�j"), get_yaku_han("haitei_raotsen"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CharacterOne)); // �a���v������
			}
		));
	/* �C�ꝝ�� */
	if (RuleData::chkRuleApplied("haitei_raohwa"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�C�ꝝ��"), get_yaku_han("haitei_raohwa"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleFive)); // �a���v���ܓ�
			}
		));
	/* ����̌� */
	if (RuleData::chkRuleApplied("hatsutei_moyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����̌�"), get_yaku_han("hatsutei_moyue"),
			_T("�C��̌�"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == GreenDragon)); // �a���v���
			}
		));
	/* �[�C�� */
	if (RuleData::chkRuleApplied("shen_haitei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�[�C��"), get_yaku_han("shen_haitei"),
			_T("�C��̌�"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(*analysis->TsumoAgariFlag) && // �c���A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(isFinalRound(analysis->GameStat))); // �I�[���X�ł���
			}
		));
#endif /* GUOBIAO */
	/* �͒ꃍ�� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�C�ꝝ��"), yaku::yakuCalculator::Yaku::yval_8,
#else /* GUOBIAO */
		_T("�͒ꝝ��"), yaku::yakuCalculator::Yaku::yval_1han,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
				(!*analysis->TsumoAgariFlag) && // �����A�K��
				(analysis->GameStat->tilesLeft() == 0)); // �n�C�e�C�ł���
		}
	));
#ifndef GUOBIAO
	/* �㓛���� */
	if (RuleData::chkRuleApplied("chuupin_raoyui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�㓛����"), get_yaku_han("chuupin_raoyui"),
			_T("�͒ꝝ��"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == CircleNine)); // �a���v���㓛
			}
		));
	/* ���ꝝ�� */
	if (RuleData::chkRuleApplied("hakutei_raoyui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ꝝ��"), get_yaku_han("hakutei_raoyui"),
			_T("�͒ꝝ��"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �[���a */
	if (RuleData::chkRuleApplied("shenyuanhu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�[���a"), get_yaku_han("shenyuanhu"),
			_T("�͒ꝝ��"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(isFinalRound(analysis->GameStat))); // �I�[���X�ł���
			}
		));
	/* �D�ꝝ� */
	if (RuleData::chkRuleApplied("nitei_raochun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�D�ꝝ�"), get_yaku_han("nitei_raochun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // ���P�R�ł���
			}
		));
	/* ���X�g�I�[�_�[���k���� */
	if (RuleData::chkRuleApplied("maaboodoufu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���X�g�I�[�_�[���k����"), get_yaku_han("maaboodoufu"),
			_T("���ꝝ��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->TsumoHai->tile == WhiteDragon)); // �a���v����
			}
		));
	/* �΂̏�ɂ��O�N */
	if (RuleData::chkRuleApplied("three_years_on_stone"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�΂̏�ɂ��O�N"), get_yaku_han("three_years_on_stone"),
			_T("�_�u������"), _T("�_�u���J����"), _T("�C��̌�"), _T("�͒ꝝ��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(!analysis->GameStat->KangFlag.kangFlag) && // �Ȃ���������ł͂Ȃ�
					(analysis->GameStat->tilesLeft() == 0) && // �n�C�e�C�ł���
					(analysis->PlayerStat->RichiFlag.DoubleFlag)); // �_�u���������Ă���
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* �ȐU�� */
	if (RuleData::chkRuleApplied("kamburi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ȐU��"), get_yaku_han("kamburi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chainFlag >= 1) && // �Ȃ���������ł���
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
#endif /* GUOBIAO */
	/* ���Ȃ͐��m�ɂ͖ؕ΂ł͂Ȃ���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("���Șa"), yaku::yakuCalculator::Yaku::yval_8,
		_T("�a�⒣"),
#else /* GUOBIAO */
		_T("����"), yaku::yakuCalculator::Yaku::yval_1han,
		_T("���v�a"), _T("�ȐU��") /* �ȐU��͖{�����ʖ��ł͂Ȃ������ʖ𔻒�̃V�X�e�����g���ĕ������Ȃ��悤�ɂ��� */,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
				(analysis->GameStat->KangFlag.chankanFlag)); // ���ȃt���O�������Ă���
		}
	));
#ifndef GUOBIAO
	/* ������� */
	if (RuleData::chkRuleApplied("ryanzoh_chankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�������"), get_yaku_han("ryanzoh_chankan"),
			_T("����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chankanFlag) && // ���ȃt���O�������Ă���
					(analysis->TsumoHai->tile == BambooTwo)); // �a���v�����
			}
		));
	/* �[���� */
	if (RuleData::chkRuleApplied("shen_chankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�[����"), get_yaku_han("shen_chankan"),
			_T("����"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->KangFlag.chankanFlag) && // ���ȃt���O�������Ă���
					(isFinalRound(analysis->GameStat))); // �I�[���X�ł���
			}
		));
	/* ���{�D�H */
	if (RuleData::chkRuleApplied("jinji_duoshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���{�D�H"), get_yaku_han("jinji_duoshi"),
			_T("���v�a"),
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
	if (RuleData::chkRuleApplied("paarenchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���A��"), get_yaku_han("paarenchan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->AgariChain == 8)); // �a�������̂��A��8���
			}
		));
	/* ���A�E��(�p�[�����u���C�J�[) */
	if (RuleData::chkRuleApplied("breaking_paarenchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�j�񔪘A��"), get_yaku_han("breaking_paarenchan"), // �������A�Ă߂������̋ǂŔ��A����a�������Ă����Ȃ�
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �܂��͂���
					(analysis->GameStat->AgariChain == -1)); // �ӂ������a���𓪃n�l(����Ȃ��Ă���������)�I
			}
		));

	// ---------------------------------------------------------------------

	/* ���Ԃ�(�����錾�v�Ń�������Ɩ����t��) */
	if (RuleData::chkRuleApplied("tsubame_gaeshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���Ԃ�"), get_yaku_han("tsubame_gaeshi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->RichiCounter)); // �t���O�������Ă���
			}
		));
	/* ��(�_�u���[�錾�v�Ń�������Ɩ����t��) */
	if (RuleData::chkRuleApplied("hien"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��"), get_yaku_han("hien"),
			_T("���Ԃ�"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->GameStat->DoubleRichiCounter)); // �t���O�������Ă���
			}
		));

	// ---------------------------------------------------------------------

	/* �l�n�H */
	if (RuleData::chkRuleApplied("sumaro"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�n�H"), get_yaku_han("sumaro"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == CharacterFour) && // �a���v���l��
					(!*analysis->TsumoAgariFlag) && // �����A�K��
					(analysis->PlayerStat->SumaroFlag)); // ���փt���O�������Ă���
			}
		));
	/* ���� */
	if (RuleData::chkRuleApplied("meiling"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����"), get_yaku_han("meiling"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooTwo) && // �a���v�����
					(!*analysis->TsumoAgariFlag)); // �����A�K��
			}
		));
	/* �q���a(�I�^���̏o�a��) */
	if (RuleData::chkRuleApplied("otakaze_ron"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�q���a"), get_yaku_han("otakaze_ron"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag;
				switch (TileCode tc = analysis->TsumoHai->tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind: // ���v������
					yakuFlag = (!*analysis->TsumoAgariFlag); // ���Ƀt���O�ݒ�
					if (tc == Wind2Tile((uint8_t)analysis->GameStat->playerwind(analysis->player))) // ����������
						yakuFlag = false;
					if (tc == Wind2Tile(analysis->GameStat->GameRound / 4)) // �ꕗ������
							yakuFlag = false;
					if (RuleData::chkRuleApplied("kaimenkaze") &&
						(tc == Wind2Tile((uint8_t)analysis->GameStat->playerwind(analysis->GameStat->WaremePlayer)))) // �J�啗������
							yakuFlag = false;
					if (RuleData::chkRuleApplied("urakaze") &&
						(tc == Wind2Tile((uint8_t)analysis->GameStat->playerwind(RelativePositionOf(analysis->player, sOpposite))))) // ����������
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
		const TileCode tmpTileCodeList[Rules] = {
			BambooOne, CircleOne, CharacterOne, BambooSeven, CircleSeven,
			CharacterSeven, EastWind, SouthWind, WestWind, NorthWind,
		};
		const TCHAR tmpYakuNameList[Rules][16] = {
			_T("�P�ґ�"), _T("���ҏ�"), _T("�j�ߎ�"), _T("���T�A��"), _T("�k�l����"),
			_T("��֊Ҍ�"), _T("������"), _T("�앗��"), _T("������"), _T("�k����"),
		};
		for (int i = 0; i < Rules; i++) {
			if (RuleData::chkRuleApplied(tmpRuleCodeList[i])) {
				TileCode tc = tmpTileCodeList[i];
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					tmpYakuNameList[i], get_yaku_han(tmpRuleCodeList[i]),
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
	if (RuleData::chkRuleApplied("golddragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����V�a"), get_yaku_han("golddragon"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= CircleOne) && // �a���v�����q
					(analysis->TsumoHai->tile <= CircleNine) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �����h�a */
	if (RuleData::chkRuleApplied("whirlwind"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����h�a"), get_yaku_han("whirlwind"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile >= EastWind) && // �a���v�����v
					(analysis->TsumoHai->tile <= NorthWind) &&
					(analysis->GameStat->TurnRound <= 3)); // 3���ڈȓ�
			}
		));
	/* �V������ */
	if (RuleData::chkRuleApplied("bolt_from_the_blue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�V������"), get_yaku_han("bolt_from_the_blue"),
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
	if (RuleData::chkRuleApplied("nekomasshigura"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�L�܂�������"), get_yaku_han("nekomasshigura"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenRegular] == -1) && // �ʎq��Řa���ɂȂ��Ă���
					(analysis->TsumoHai->tile == BambooThree) && // �a���v���O��
					(analysis->Machi == yaku::yakuCalculator::machiKanchan)); // �ƒ��҂�
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* ���v�a(���X��chk-post.hsp�ɏ����Ă����ǌ�񂵂ɂ���K�v�Ȃ�ĂȂ�����) */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�a�⒣"), yaku::yakuCalculator::Yaku::yval_4,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("keppaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���v�a"), get_yaku_han("keppaihoh"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag =
					*analysis->TsumoAgariFlag ? // �c���A�K����������A
					(analysis->SeenTiles[analysis->TsumoHai->tile] + // �����Ă�v��
					analysis->TileCount[analysis->TsumoHai->tile] >= 4) // ��̓��̔v�𑫂���4��
					: // ������������A
					(analysis->SeenTiles[analysis->TsumoHai->tile] + // �����Ă�v��
					analysis->TileCount[analysis->TsumoHai->tile] > 4); // ��̓��̔v�𑫂���4��
#ifdef GUOBIAO
				return yakuFlag;
#else /* GUOBIAO */
				return (yakuFlag && // �����𖞂����Ă��āA
					(analysis->MachiInfo.MachiMen == 1)); // 1�ʑ҂�(�ڐA���ύX�F�m�x�P�Ƃ��ł͐������Ȃ��悤�ɂ���)
#endif /* GUOBIAO */
			}
		));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* ���� */
	if (RuleData::chkRuleApplied("shokan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����"), get_yaku_han("shokan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->shokanFlag)); // �t���O�������Ă���
			}
		));
	/* �ȎO�� */
	if (RuleData::chkRuleApplied("kansanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ȎO��"), get_yaku_han("kansanjun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->kansanjunFlag)); // �t���O�������Ă���
			}
		));
	/* �ߔv�V�a */
	if (RuleData::chkRuleApplied("renpai_tenhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ߔv�V�a"), get_yaku_han("renpai_tenhoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->renpaiTenhohStat > 0)); // �t���O�������Ă���
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifdef GUOBIAO
	/* ���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("����"), yaku::yakuCalculator::Yaku::yval_1,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenRegular] == -1) && // �ʎq��Řa���ɂȂ��Ă���
				(analysis->Machi == yaku::yakuCalculator::machiKanchan) && // �ƒ��҂���
				(analysis->MachiInfo.MachiMen == 1)); // 1�ʑ҂�
		}
	));
	/* �Ӓ� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�Ӓ�"), yaku::yakuCalculator::Yaku::yval_1,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenRegular] == -1) && // �ʎq��Řa���ɂȂ��Ă���
				(analysis->Machi == yaku::yakuCalculator::machiPenchan) && // �Ӓ��҂���
				(analysis->MachiInfo.MachiMen == 1)); // 1�ʑ҂�
		}
	));
	/* �P���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�P����"), yaku::yakuCalculator::Yaku::yval_1,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenRegular] == -1) && // �ʎq��Řa���ɂȂ��Ă���
				(analysis->Machi == yaku::yakuCalculator::machiTanki) && // �P�R�҂���
				(analysis->MachiInfo.MachiMen == 1)); // 1�ʑ҂�
		}
	));
#endif /* GUOBIAO */
}
