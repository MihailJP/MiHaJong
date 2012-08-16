#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_dora() {
	/* ƒhƒ‰‚Å˜a—¹‚Á‚½‚ç1ãÊ */
	if (RuleData::getRule("dorahoh") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ºèt˜a", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					((analysis->GameStat->DoraFlag.Omote[analysis->TsumoHai->tile] > 0) || // ƒcƒ‚”v‚ª•\ƒhƒ‰‚É‚È‚Á‚Ä‚¢‚é(— ƒhƒ‰‚Í‘ÎÛŠO)
					(analysis->TsumoHai->red != Normal))); // Ôƒhƒ‰‚©Âƒhƒ‰‚É‚È‚Á‚Ä‚¢‚é
			}
		));
	/* –kl–‡”²‚« */
	if (RuleData::getRule("four_northes") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–kl–‡”²‚«", yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->NorthFlag == 4)); // –k4–‡
			}
		));
	/* ”ªå‰ßŠC */
	if (RuleData::getRule("eight_flowers") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”ªå‰ßŠC", yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			"t‰ÄH“~", "l‰ØŠJ—ä", "–{‰Ô‹Gß”v", "–{‰Ô‘–Ø”v",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter) &&
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* t‰ÄH“~ */
	if (RuleData::getRule("four_seasons") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"t‰ÄH“~", (RuleData::getRule("four_seasons") == 1) ?
			yaku::yakuCalculator::Yaku::yval_2han_dependent :
			(RuleData::getRule("four_seasons") == 2) ? yaku::yakuCalculator::Yaku::yval_4han_dependent :
			yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			"–{‰Ô‹Gß”v",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter));
			}
		));
	/* l‰ØŠJ—ä */
	if (RuleData::getRule("four_flowers") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"l‰ØŠJ—ä", (RuleData::getRule("four_flowers") == 1) ?
			yaku::yakuCalculator::Yaku::yval_2han_dependent :
			(RuleData::getRule("four_flowers") == 2) ? yaku::yakuCalculator::Yaku::yval_4han_dependent :
			yaku::yakuCalculator::Yaku::yval_yakuman_dependent,
			"–{‰Ô‘–Ø”v",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* –{‰Ô */
	if (RuleData::getRule("own_flower") != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–{‰Ô‹Gß”v", yaku::yakuCalculator::Yaku::yval_1han_dependent,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenAll] != -1) return false; // ˜a—¹‚Á‚Ä‚È‚¢‚È‚ç–ß‚é
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Spring;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Summer;
					case sWest: return analysis->PlayerStat->FlowerFlag.Autumn;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Winter;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "©•—‚ª“Œ“ì¼–k‚Ì‚Ç‚ê‚Å‚à‚ ‚è‚Ü‚¹‚ñ");
						return false;
				}
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–{‰Ô‘–Ø”v", yaku::yakuCalculator::Yaku::yval_1han_dependent,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenAll] != -1) return false; // ˜a—¹‚Á‚Ä‚È‚¢‚È‚ç–ß‚é
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Plum;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Orchid;
					case sWest: return analysis->PlayerStat->FlowerFlag.Chrys;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Bamboo;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "©•—‚ª“Œ“ì¼–k‚Ì‚Ç‚ê‚Å‚à‚ ‚è‚Ü‚¹‚ñ");
						return false;
				}
			}
		));
	}
}
