#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_dora() {
	/* �h���Řa��������1�� */
	if (RuleData::chkRuleApplied("dorahoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���t�a"), get_yaku_han("dorahoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					((analysis->GameStat->DoraFlag.Omote[analysis->TsumoHai->tile] > 0) || // �c���v���\�h���ɂȂ��Ă���(���h���͑ΏۊO)
					(analysis->TsumoHai->red != Normal))); // �ԃh�����h���ɂȂ��Ă���
			}
		));
	/* �k�l������ */
	if (RuleData::chkRuleApplied("four_northes"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�k�l������"), get_yaku_han("four_northes"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->NorthFlag == 4)); // �k4��
			}
		));
	/* ����ߊC */
	if (RuleData::chkRuleApplied("eight_flowers"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����ߊC"), get_yaku_han("eight_flowers"),
			_T("�t�ďH�~"), _T("�l�؊J��"), _T("�{�ԋG�ߔv"), _T("�{�ԑ��ؔv"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter) &&
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* �t�ďH�~ */
	if (RuleData::chkRuleApplied("four_seasons"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�t�ďH�~"), get_yaku_han("four_seasons"),
			_T("�{�ԋG�ߔv"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter));
			}
		));
	/* �l�؊J�� */
	if (RuleData::chkRuleApplied("four_flowers"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�؊J��"), get_yaku_han("four_flowers"),
			_T("�{�ԑ��ؔv"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* �{�� */
	if (RuleData::chkRuleApplied("own_flower")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�{�ԋG�ߔv"), get_yaku_han("own_flower"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenAll] != -1) return false; // �a�����ĂȂ��Ȃ�߂�
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Spring;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Summer;
					case sWest: return analysis->PlayerStat->FlowerFlag.Autumn;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Winter;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("���������쐼�k�̂ǂ�ł�����܂���"));
						return false;
				}
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�{�ԑ��ؔv"), get_yaku_han("own_flower"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenAll] != -1) return false; // �a�����ĂȂ��Ȃ�߂�
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Plum;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Orchid;
					case sWest: return analysis->PlayerStat->FlowerFlag.Chrys;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Bamboo;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("���������쐼�k�̂ǂ�ł�����܂���"));
						return false;
				}
			}
		));
	}
}
