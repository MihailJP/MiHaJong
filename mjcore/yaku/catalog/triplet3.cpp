#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_3() {
	auto countKeziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->KeziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->KeziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->KeziCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countDuiziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->DuiziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->DuiziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->DuiziCount[TILE_SUIT_BAMBOOS + numeral];
		};

	// ---------------------------------------------------------------------

	/* �C�i�o�E�A�[ */
	if (RuleData::chkRuleApplied("inabauer"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�C�i�o�E�A�[", get_yaku_han("inabauer"),
			"����F", "�΁X�a", "�ؔv��",
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
	/* �l���n�� */
	if (RuleData::chkRuleApplied("april_fool")) {
		auto isApril1st =
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 4) && (nowTime.wDay == 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l���n��", get_yaku_han("april_fool"),
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
	/* �����̓J */
	if (RuleData::chkRuleApplied("chuutounofue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����̓J", get_yaku_han("chuutounofue"),
			"��v�E��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1));
			}
		));
	/* �O���u */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O���u", get_yaku_han("sangokushi"),
			"�΁X�a",
			[&countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (bool)
					(countKeziOf(analysis, 3) * countKeziOf(analysis, 5) *
					countKeziOf(analysis, 9) * countKeziOf(analysis, 4));
			}
		));
	/* ��x���� */
	if (RuleData::chkRuleApplied("ikkyuusan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��x����", get_yaku_han("ikkyuusan"),
			"�΁X�a",
			[&countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((countDuiziOf(analysis, 1) > 0) &&
					(countDuiziOf(analysis, 9) > 0) &&
					(countDuiziOf(analysis, 3) > 0) &&
					(countDuiziOf(analysis, 1) + countDuiziOf(analysis, 9) +
					countDuiziOf(analysis, 3) == SIZE_OF_MELD_BUFFER));
			}
		));
}
