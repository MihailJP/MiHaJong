#include "../catalog.h"

#include "../../func.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_dora() {
#ifndef GUOBIAO
	/* ドラで和了ったら1飜 */
	if (RuleData::chkRuleApplied("dorahoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("銅鑼和"), get_yaku_han("dorahoh"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					((analysis->GameStat->DoraFlag.Omote[analysis->TsumoHai().tile] > 0) || // ツモ牌が表ドラになっている(裏ドラは対象外)
					(analysis->TsumoHai().red != DoraCol::normal))); // 赤ドラか青ドラになっている
			}
		));
	/* 北四枚抜き */
	if (RuleData::chkRuleApplied("four_northes"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北四枚抜き"), get_yaku_han("four_northes"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat()->NorthFlag == 4)); // 北4枚
			}
		));
	/* 八仙過海 */
	if (RuleData::chkRuleApplied("eight_flowers"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八仙過海"), get_yaku_han("eight_flowers"),
			_T("春夏秋冬"), _T("四華開嶺"), _T("本花季節牌"), _T("本花草木牌"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat()->FlowerFlag.Spring) && (analysis->PlayerStat()->FlowerFlag.Summer) &&
					(analysis->PlayerStat()->FlowerFlag.Autumn) && (analysis->PlayerStat()->FlowerFlag.Winter) &&
					(analysis->PlayerStat()->FlowerFlag.Plum) && (analysis->PlayerStat()->FlowerFlag.Orchid) &&
					(analysis->PlayerStat()->FlowerFlag.Chrys) && (analysis->PlayerStat()->FlowerFlag.Bamboo));
			}
		));
	/* 春夏秋冬 */
	if (RuleData::chkRuleApplied("four_seasons"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("春夏秋冬"), get_yaku_han("four_seasons"),
			_T("本花季節牌"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat()->FlowerFlag.Spring) && (analysis->PlayerStat()->FlowerFlag.Summer) &&
					(analysis->PlayerStat()->FlowerFlag.Autumn) && (analysis->PlayerStat()->FlowerFlag.Winter));
			}
		));
	/* 四華開嶺 */
	if (RuleData::chkRuleApplied("four_flowers"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四華開嶺"), get_yaku_han("four_flowers"),
			_T("本花草木牌"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat()->FlowerFlag.Plum) && (analysis->PlayerStat()->FlowerFlag.Orchid) &&
					(analysis->PlayerStat()->FlowerFlag.Chrys) && (analysis->PlayerStat()->FlowerFlag.Bamboo));
			}
		));
	/* 本花 */
	if (RuleData::chkRuleApplied("own_flower")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("本花季節牌"), get_yaku_han("own_flower"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if (analysis->shanten[ShantenType::all] != -1) return false; // 和了ってないなら戻る
				switch (analysis->GameStat->playerwind(analysis->player)) {
					case SeatAbsolute::east: return analysis->PlayerStat()->FlowerFlag.Spring;
					case SeatAbsolute::south: return analysis->PlayerStat()->FlowerFlag.Summer;
					case SeatAbsolute::west: return analysis->PlayerStat()->FlowerFlag.Autumn;
					case SeatAbsolute::north: return analysis->PlayerStat()->FlowerFlag.Winter;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("自風が東南西北のどれでもありません"));
						return false;
				}
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("本花草木牌"), get_yaku_han("own_flower"),
			[](const MentsuAnalysis* const analysis) -> bool {
				if (analysis->shanten[ShantenType::all] != -1) return false; // 和了ってないなら戻る
				switch (analysis->GameStat->playerwind(analysis->player)) {
					case SeatAbsolute::east: return analysis->PlayerStat()->FlowerFlag.Plum;
					case SeatAbsolute::south: return analysis->PlayerStat()->FlowerFlag.Orchid;
					case SeatAbsolute::west: return analysis->PlayerStat()->FlowerFlag.Chrys;
					case SeatAbsolute::north: return analysis->PlayerStat()->FlowerFlag.Bamboo;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("自風が東南西北のどれでもありません"));
						return false;
				}
			}
		));
	}
	/* 赤ドラを揃える系の役 */
	if (RuleData::chkRuleApplied("akadora_all")) {
		const auto countRed = [](const MentsuAnalysis* const analysis) -> unsigned {
			unsigned red = 0;
			for (int i = 0; i < NumOfTilesInHand; i++) {
				if (analysis->PlayerStat()->Hand[i].tile == TileCode::noTile) continue;
				else if (static_cast<int>(analysis->PlayerStat()->Hand[i].tile) >= TileNonflowerMax) continue;
				else if (analysis->PlayerStat()->Hand[i].red == DoraCol::akaDora) ++red;
			}
			for (int i = 1; i < analysis->PlayerStat()->MeldPointer; i++) {
				const auto k = &analysis->PlayerStat()->Meld[i];
				for (int j = 0; j < (k->mstat >= MeldStat::quadConcealed ? 4 : 3); j++) {
					if (analysis->PlayerStat()->Meld[i].red[j] == DoraCol::akaDora) ++red;
				}
			}
			return red;
		};
		/* 赤ドラ三色 */
		if (RuleData::chkRule("red_one", "no") && RuleData::chkRule("red_two", "no") &&
			RuleData::chkRule("red_three", "no") && RuleData::chkRule("red_four", "no") &&
			RuleData::chkRule("red_five", "3tiles") &&
			RuleData::chkRule("red_six", "no") && RuleData::chkRule("red_seven", "no") &&
			RuleData::chkRule("red_eight", "no") && RuleData::chkRule("red_nine", "no") &&
			RuleData::chkRule("red_west", "no") && RuleData::chkRule("red_north", "no"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("赤ドラ三色"), get_yaku_han("akadora_all"),
				[countRed](const MentsuAnalysis* const analysis) -> bool {
					return (analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
						(countRed(analysis) == 3u); // 赤ドラが3枚である
				}
			));
		/* 門泥公 */
		if (RuleData::chkRule("red_one", "no") && RuleData::chkRule("red_two", "no") &&
			RuleData::chkRule("red_three", "no") && RuleData::chkRule("red_four", "no") &&
			RuleData::chkRule("red_five", "4tiles") &&
			RuleData::chkRule("red_six", "no") && RuleData::chkRule("red_seven", "no") &&
			RuleData::chkRule("red_eight", "no") && RuleData::chkRule("red_nine", "no") &&
			RuleData::chkRule("red_west", "no") && RuleData::chkRule("red_north", "no"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("門泥公"), get_yaku_han("akadora_all"),
				[countRed](const MentsuAnalysis* const analysis) -> bool {
					return (analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
						(countRed(analysis) == 4u); // 赤ドラが4枚である
				}
			));
	}
#endif /* GUOBIAO */
}
