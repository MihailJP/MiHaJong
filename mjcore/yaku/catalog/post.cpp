#include "../yaku.h"
#include "../../ruletbl.h"

/* 判定を後回しにする役の処理(特殊な処理が必要なもの) */
void yaku::yakuCalculator::CalculatorThread::checkPostponedYaku
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result,
	std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::set<CodeConv::tstring> &suppression,
	std::vector<CodeConv::tstring> &yakuOrd)
{
	if (analysis->shanten[shantenAll] >= 0) return; // 和了ってないなら戻る(一応……)

	// ---------------------------------------------------------------------

	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, nullptr);

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* アリス・マーガトロイド */
	if (RuleData::chkRuleApplied("alice_margatroid")) {
		/*	制限事項
			固定満貫役、固定跳満役が成立している場合、この役は成立しません */
		if ((analysis->TotalAnKezi < 3) && (analysis->DuiziCount[EastWind] >= 1) &&
			(analysis->DuiziCount[WhiteDragon] + analysis->DuiziCount[GreenDragon] + analysis->DuiziCount[RedDragon] >= 1) &&
			(analysis->KeziCount[CharacterSeven] >= 1) &&
			(analysis->KeziCount[CircleSeven] >= 1) &&
			(analysis->KeziCount[BambooSeven] >= 1) &&
			(totalHan + totalBonusHan <= 7) &&
			(totalSemiMangan + totalBonusSemiMangan == 0)) {
				LPCTSTR name = _T("アリス・マーガトロイド");
				yakuHan[name] = 2_manganF(analysis);
				yakuOrd.push_back(name);
				suppression.insert(_T("対々和"));
				suppression.insert(_T("三色同刻"));
				suppression.insert(_T("五門斎"));
		}
	}
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifdef GUOBIAO
	/* 无番和 */
	if (totalHan <= 0) {
		LPCTSTR name = _T("无番和");
		yakuHan[name] = yaku::yakuCalculator::Yaku::yval_8(analysis);
		yakuOrd.push_back(name);
	}
#else /* GUOBIAO */
	/* カラス */
	if (RuleData::chkRuleApplied("karasu")) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(! analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan <= 0) && (totalSemiMangan == 0)) {
				LPCTSTR name = _T("カラス");
				yakuHan[name] = 1_hanF(analysis);
				yakuOrd.push_back(name);
		}
	}
	/* カラス立直 */
	if (RuleData::chkRuleApplied("karasu_riichi")) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan == 1) && (totalSemiMangan == 0)) {
				LPCTSTR name = _T("カラス立直");
				yakuHan[name] = 1_hanF(analysis);
				yakuOrd.push_back(name);
		}
	}
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 北枕 */
	if (RuleData::chkRuleApplied("kitamakura")) {
		if ((totalHan >= 2) && /* 2飜以上あるか？ */
			(totalSemiMangan == 0) && /* 役満未満か？ */
			(analysis->GameStat->DoraFlag.Omote[NorthWind] == 0) && /* 北はドラではないか？ */
			((!*analysis->MenzenFlag) || (!analysis->PlayerStat->RichiFlag.RichiFlag) ||
			(RuleData::chkRuleApplied("uradora")) ||
			(analysis->GameStat->DoraFlag.Ura[NorthWind] == 0)) && /* 北は裏ドラではないか？ */
			(analysis->MianziDat[0].tile == NorthWind)) { /* 雀頭が北か？ */
				LPCTSTR name = _T("北枕");
				yakuHan[name] = yaku::yakuCalculator::Yaku::YAKU_HAN(
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(-1));
				yakuOrd.push_back(name);
		}
	}
#endif /* GUOBIAO */
}
