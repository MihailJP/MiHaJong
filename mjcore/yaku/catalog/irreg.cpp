#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* �`�[�g�C */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���Ύq", (getRule(RULE_SEVEN_PAIRS) == 1) ? /* 1�|50���̃��[����2�|25���̃��[��������B���͂����ł͐ݒ�ł��Ȃ��ł��c�c */
		yaku::yakuCalculator::Yaku::yval_1han_menzen : yaku::yakuCalculator::Yaku::yval_2han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		}
	));
	/* ���m */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���m���o", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"�\�O�s��",
		[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0) /* ���m13�ʑ҂��i�_�u���𖞁j */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���m���o�\�O��", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"���m���o", "�\�O�s��",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // ���m���o�ɂȂ��Ă��āA
					(/* �t���e�����ǂ������肷�� */0) && // �t���e���ł͂Ȃ��āA
					((/* 13�ʑ҂������肷�� */0)||(gameStat->TianHuFlag))); // 13�ʑ҂����V�a�ɂȂ��Ă���
			}
		));
}
