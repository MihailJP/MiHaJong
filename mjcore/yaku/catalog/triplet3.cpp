#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_3() {
	/* イナバウアー */
	if (RuleData::chkRuleApplied("inabauer"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"イナバウアー", get_yaku_han("inabauer"),
			"清一色", "対々和", "筋牌刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->DuiziCount[i + 1] >= 1) &&
						(analysis->DuiziCount[i + 7] >= 1) &&
						(analysis->DuiziCount[i + 8] >= 1) &&
						(analysis->DuiziCount[i + 5] >= 1) &&
						(analysis->DuiziCount[i + 2] >= 1))
						flag = true;
				return flag;
			}
		));
	/* 四月馬鹿 */
	if (RuleData::chkRuleApplied("april_fool")) {
		auto isApril1st =
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 4) && (nowTime.wDay == 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四月馬鹿", get_yaku_han("april_fool"),
			[isApril1st](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (!isApril1st()) return false;
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 4] >= 1) &&
						(analysis->KeziCount[i + 1] >= 1))
						flag = true;
				return flag && (analysis->TileCount[CircleNine]);
			}
		));
	}
	/* 中東の笛 */
	if (RuleData::chkRuleApplied("chuutounofue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中東の笛", get_yaku_han("chuutounofue"),
			"役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1));
			}
		));
	/* 三国志 */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三国志", get_yaku_han("sangokushi"),
			"対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				const unsigned numerals = 4;
				bool flag[numerals] = {false};
				const int numeral[numerals] = {3, 5, 9, 4,};
				for (int k = 0; k < numerals; ++k)
					for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
						if (analysis->KangziCount[i + numeral[k]] >= 1)
							flag[k] = true;
				for (int k = 0; k < numerals; ++k)
					if (!flag[k]) return false;
				return true;
			}
		));
}
