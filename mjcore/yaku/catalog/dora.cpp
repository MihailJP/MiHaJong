#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_dora() {
	/* ƒhƒ‰‚Å˜a—¹‚Á‚½‚ç1ãÊ */
	if (RuleData::chkRuleApplied("dorahoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“ºèt˜a"), get_yaku_han("dorahoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					((analysis->GameStat->DoraFlag.Omote[analysis->TsumoHai->tile] > 0) || // ƒcƒ‚”v‚ª•\ƒhƒ‰‚É‚È‚Á‚Ä‚¢‚é(— ƒhƒ‰‚Í‘ÎÛŠO)
					(analysis->TsumoHai->red != Normal))); // Ôƒhƒ‰‚©Âƒhƒ‰‚É‚È‚Á‚Ä‚¢‚é
			}
		));
	/* –kl–‡”²‚« */
	if (RuleData::chkRuleApplied("four_northes"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("–kl–‡”²‚«"), get_yaku_han("four_northes"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->NorthFlag == 4)); // –k4–‡
			}
		));
	/* ”ªå‰ßŠC */
	if (RuleData::chkRuleApplied("eight_flowers"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("”ªå‰ßŠC"), get_yaku_han("eight_flowers"),
			_T("t‰ÄH“~"), _T("l‰ØŠJ—ä"), _T("–{‰Ô‹Gß”v"), _T("–{‰Ô‘–Ø”v"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter) &&
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* t‰ÄH“~ */
	if (RuleData::chkRuleApplied("four_seasons"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("t‰ÄH“~"), get_yaku_han("four_seasons"),
			_T("–{‰Ô‹Gß”v"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->FlowerFlag.Spring) && (analysis->PlayerStat->FlowerFlag.Summer) &&
					(analysis->PlayerStat->FlowerFlag.Autumn) && (analysis->PlayerStat->FlowerFlag.Winter));
			}
		));
	/* l‰ØŠJ—ä */
	if (RuleData::chkRuleApplied("four_flowers"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("l‰ØŠJ—ä"), get_yaku_han("four_flowers"),
			_T("–{‰Ô‘–Ø”v"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenAll] == -1) && // ‰½‚©‚Ìè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(analysis->PlayerStat->FlowerFlag.Plum) && (analysis->PlayerStat->FlowerFlag.Orchid) &&
					(analysis->PlayerStat->FlowerFlag.Chrys) && (analysis->PlayerStat->FlowerFlag.Bamboo));
			}
		));
	/* –{‰Ô */
	if (RuleData::chkRuleApplied("own_flower")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("–{‰Ô‹Gß”v"), get_yaku_han("own_flower"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenAll] != -1) return false; // ˜a—¹‚Á‚Ä‚È‚¢‚È‚ç–ß‚é
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Spring;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Summer;
					case sWest: return analysis->PlayerStat->FlowerFlag.Autumn;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Winter;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("©•—‚ª“Œ“ì¼–k‚Ì‚Ç‚ê‚Å‚à‚ ‚è‚Ü‚¹‚ñ"));
						return false;
				}
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("–{‰Ô‘–Ø”v"), get_yaku_han("own_flower"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenAll] != -1) return false; // ˜a—¹‚Á‚Ä‚È‚¢‚È‚ç–ß‚é
				switch (playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound)) {
					case sEast: return analysis->PlayerStat->FlowerFlag.Plum;
					case sSouth: return analysis->PlayerStat->FlowerFlag.Orchid;
					case sWest: return analysis->PlayerStat->FlowerFlag.Chrys;
					case sNorth: return analysis->PlayerStat->FlowerFlag.Bamboo;
					default:
						RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("©•—‚ª“Œ“ì¼–k‚Ì‚Ç‚ê‚Å‚à‚ ‚è‚Ü‚¹‚ñ"));
						return false;
				}
			}
		));
	}
}
