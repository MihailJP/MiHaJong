#include "../catalog.h"

#include "../../func.h"

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
	/* �ԃh���𑵂���n�̖� */
	if (RuleData::chkRuleApplied("akadora_all")) {
		auto countRed = [](const MENTSU_ANALYSIS* const analysis) -> unsigned {
			unsigned red = 0;
			for (int i = 0; i < NumOfTilesInHand; i++) {
				if (analysis->PlayerStat->Hand[i].tile == NoTile) continue;
				else if (analysis->PlayerStat->Hand[i].tile >= TileNonflowerMax) continue;
				else if (analysis->PlayerStat->Hand[i].red == AkaDora) ++red;
			}
			for (int i = 1; i < analysis->PlayerStat->MeldPointer; i++) {
				auto k = &analysis->PlayerStat->Meld[i];
				for (int j = 0; j < (k->mstat >= meldQuadConcealed ? 4 : 3); j++) {
					if (analysis->PlayerStat->Meld[i].red[j] == AkaDora) ++red;
				}
			}
			return red;
		};
		/* �ԃh���O�F */
		if (RuleData::chkRule("red_one", "no") && RuleData::chkRule("red_two", "no") &&
			RuleData::chkRule("red_three", "no") && RuleData::chkRule("red_four", "no") &&
			RuleData::chkRule("red_five", "3tiles") &&
			RuleData::chkRule("red_six", "no") && RuleData::chkRule("red_seven", "no") &&
			RuleData::chkRule("red_eight", "no") && RuleData::chkRule("red_nine", "no") &&
			RuleData::chkRule("red_west", "no") && RuleData::chkRule("red_north", "no"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�ԃh���O�F"), get_yaku_han("akadora_all"),
				[countRed](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
						(countRed(analysis) == 3u); // �ԃh����3���ł���
				}
			));
		/* ��D�� */
		if (RuleData::chkRule("red_one", "no") && RuleData::chkRule("red_two", "no") &&
			RuleData::chkRule("red_three", "no") && RuleData::chkRule("red_four", "no") &&
			RuleData::chkRule("red_five", "4tiles") &&
			RuleData::chkRule("red_six", "no") && RuleData::chkRule("red_seven", "no") &&
			RuleData::chkRule("red_eight", "no") && RuleData::chkRule("red_nine", "no") &&
			RuleData::chkRule("red_west", "no") && RuleData::chkRule("red_north", "no"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("��D��"), get_yaku_han("akadora_all"),
				[countRed](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // �����̎�Řa���ɂȂ��Ă���
						(countRed(analysis) == 4u); // �ԃh����4���ł���
				}
			));
	}
}
