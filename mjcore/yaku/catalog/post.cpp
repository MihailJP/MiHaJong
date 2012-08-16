#include "../yaku.h"

/* 判定を後回しにする役の処理(特殊な処理が必要なもの) */
void yaku::yakuCalculator::CalculatorThread::checkPostponedYaku
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result,
	std::map<std::string, Yaku::YAKU_HAN> &yakuHan, std::set<std::string> &suppression,
	std::vector<std::string> &yakuOrd)
{
	if (analysis->shanten[ShantenAnalyzer::shantenAll] >= 0) return; // 和了ってないなら戻る(一応……)

	// ---------------------------------------------------------------------

	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, NULL);

	// ---------------------------------------------------------------------

	/* アリス・マーガトロイド */
	if (RuleData::getRule("alice_margatroid") != 0) {
		/*	制限事項
			固定満貫役、固定跳満役が成立している場合、この役は成立しません */
		if ((analysis->TotalAnKezi < 3) && (analysis->DuiziCount[EastWind] >= 1) &&
			(analysis->DuiziCount[WhiteDragon] + analysis->DuiziCount[GreenDragon] + analysis->DuiziCount[RedDragon] >= 1) &&
			(analysis->KeziCount[CharacterSeven] >= 1) &&
			(analysis->KeziCount[CircleSeven] >= 1) &&
			(analysis->KeziCount[BambooSeven] >= 1) &&
			(totalHan + totalBonusHan <= 7) &&
			(totalSemiMangan + totalBonusSemiMangan == 0)) {
				const char* name = "アリス・マーガトロイド";
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_baiman(analysis);
				yakuOrd.push_back(name);
				suppression.insert("対々和");
				suppression.insert("三色同刻");
				suppression.insert("五門斎");
		}
	}

	// ---------------------------------------------------------------------

	/* カラス */
	if (RuleData::getRule("karasu") != 0) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(! analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan <= 0) && (totalSemiMangan == 0)) {
				const char* name = "カラス";
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_1han(analysis);
				yakuOrd.push_back(name);
		}
	}
	/* カラス立直 */
	if (RuleData::getRule("karasu_riichi") != 0) {
		if (((analysis->Machi == yaku::yakuCalculator::machiKanchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiPenchan) ||
			(analysis->Machi == yaku::yakuCalculator::machiTanki)) &&
			(analysis->PlayerStat->RichiFlag.RichiFlag) &&
			(totalHan == 1) && (totalSemiMangan == 0)) {
				const char* name = "カラス立直";
				yakuHan[name] = yaku::yakuCalculator::Yaku::yval_1han(analysis);
				yakuOrd.push_back(name);
		}
	}

	// ---------------------------------------------------------------------

	/* 北枕 */
	if (RuleData::getRule("kitamakura") != 0) {
		if ((totalHan >= 2) && /* 2飜以上あるか？ */
			(totalSemiMangan == 0) && /* 役満未満か？ */
			(analysis->GameStat->DoraFlag.Omote[NorthWind] == 0) && /* 北はドラではないか？ */
			((!*analysis->MenzenFlag) || (!analysis->PlayerStat->RichiFlag.RichiFlag) ||
			(RuleData::getRule("uradora") == 1) ||
			(analysis->GameStat->DoraFlag.Ura[NorthWind] == 0)) && /* 北は裏ドラではないか？ */
			(analysis->MianziDat[0].tile == NorthWind)) { /* 雀頭が北か？ */
				const char* name = "北枕";
				yakuHan[name] = yaku::yakuCalculator::Yaku::YAKU_HAN(
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(-1));
				yakuOrd.push_back(name);
		}
	}
}
