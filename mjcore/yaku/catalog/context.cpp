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
	/* ���Ƃ� */
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
}
