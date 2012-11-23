#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* ƒ`[ƒgƒC */
	auto isQiDui =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)&&(analysis->shanten[ShantenAnalyzer::shantenRegular] >= 0));
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"Žµ‘ÎŽq", get_yaku_han("seven_pairs"), /* 1–|50•„‚Ìƒ‹[ƒ‹‚Æ2–|25•„‚Ìƒ‹[ƒ‹‚ª‚ ‚éB•„‚Í‚±‚±‚Å‚ÍÝ’è‚Å‚«‚È‚¢‚Å‚·cc */
		isQiDui
	));

	auto chktiles = // ”»’èŠÖ”ƒIƒuƒWƒFƒNƒg
		[isQiDui](const MENTSU_ANALYSIS* const analysis, const tileCode* const targetDuiz, int numOfDuiz) -> bool {
			if (isQiDui(analysis))
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetDuiz, numOfDuiz)
					== NUM_OF_TILES_IN_HAND / 2);
			else return false;
		};
	// ---------------------------------------------------------------------

	/* Žµ•Ÿ¯ */
	if (RuleData::chkRuleApplied("seven_honor_pairs") && !RuleData::chkRule("seven_honor_pairs", "yes"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žµ•Ÿ¯", get_yaku_han("seven_honor_pairs"),
			"¬˜V“ª", "¬ˆêF", "ŽOŒ³Žµ‘ÎŽq", "ŽlŠìŽµ‘ÎŽq", "g”’Žµ‘Î",
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (isQiDui(analysis))
					return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NUM_OF_TILES_IN_HAND / 2);
				else return false;
			}
		));
	/* ŽOŒ³Žµ‘ÎŽq */
	if (RuleData::chkRuleApplied("dragon_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOŒ³Žµ‘ÎŽq", get_yaku_han("dragon_pairs"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[GreenDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* ŽlŠìŽµ‘ÎŽq */
	if (RuleData::chkRuleApplied("wind_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽlŠìŽµ‘ÎŽq", get_yaku_han("wind_pairs"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[EastWind] >= 2) &&
					(analysis->TileCount[SouthWind] >= 2) &&
					(analysis->TileCount[WestWind] >= 2) &&
					(analysis->TileCount[NorthWind] >= 2));
			}
		));
	/* Šï”Žµ‘ÎŽq */
	if (RuleData::chkRuleApplied("odd_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Šï”Žµ‘ÎŽq", get_yaku_han("odd_pairs"),
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode tc[15] = {
					CharacterOne, CharacterThree, CharacterFive, CharacterSeven, CharacterNine,
					CircleOne, CircleThree, CircleFive, CircleSeven, CircleNine,
					BambooOne, BambooThree, BambooFive, BambooSeven, BambooNine,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 15);
			}
		));
	/* ‹ô”Žµ‘ÎŽq */
	if (RuleData::chkRuleApplied("even_pairs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ô”Žµ‘ÎŽq", get_yaku_han("even_pairs"),
			"’f›ô‹ã",
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode tc[12] = {
					CharacterTwo, CharacterFour, CharacterSix, CharacterEight,
					CircleTwo, CircleFour, CircleSix, CircleEight,
					BambooTwo, BambooFour, BambooSix, BambooEight,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 12);
			}
		));

	// ---------------------------------------------------------------------

	/* g”’Žµ‘Î */
	if (RuleData::chkRuleApplied("kouhaku_chiitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"g”’Žµ‘Î", get_yaku_han("kouhaku_chiitoi"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* –k‚Ì‘å’n */
	if (RuleData::chkRuleApplied("kita_no_daichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–k‚Ì‘å’n", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
				switch (analysis->TsumoHai->tile) { /* ‰½‚Å˜a—¹‚Á‚½‚©‚Å•ªŠò */
				case NorthWind: /* –k‚Å˜a—¹‚é‚Æƒ_ƒuƒ‹–ð–ž */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman);
				case WhiteDragon: /* ”’‚Å˜a—¹‚é‚ÆƒgƒŠƒvƒ‹–ð–ž */
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman);
				default:
					return yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
				}
			}),
			"¬ˆêF",
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode tc[7] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					NorthWind, WhiteDragon,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
			}
		));
	/* ¢ŠEˆê */
	if (RuleData::chkRuleApplied("sekaiichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¢ŠEˆê", get_yaku_han("sekaiichi"),
			"ŽlŠìŽµ‘ÎŽq", "¬˜V“ª",
			[isQiDui, chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const tileCode tc[7] = {
					CharacterOne, CircleOne, BambooOne,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return isQiDui(analysis) && chktiles(analysis, tc, 7);
			}
		));

	// ---------------------------------------------------------------------

	/* ŽOF“¯‘Î */
	if (RuleData::chkRuleApplied("sanshoku_doutoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOF“¯‘Î", get_yaku_han("sanshoku_doutoi"),
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++)
					if ((analysis->TileCount[TILE_SUIT_CHARACTERS + i] >= 2) &&
						(analysis->TileCount[TILE_SUIT_CIRCLES + i] >= 2) &&
						(analysis->TileCount[TILE_SUIT_BAMBOOS + i] >= 2))
						yakuFlag = true;
				return isQiDui(analysis) && yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	{
		auto gyakusharin =
			[isQiDui](const MENTSU_ANALYSIS* const analysis, int suit) -> bool {
				std::array<tileCode, 7> honors = {
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon, RedDragon,
				}; int count = 0;
				for (auto k = honors.begin(); k != honors.end(); k++)
					if (analysis->TileCount[*k] >= 2) ++count;
				return isQiDui(analysis) &&
					(analysis->TileCount[suit + 1] >= 2) &&
					(analysis->TileCount[suit + 9] >= 2) &&
					(count == 5);
			};
		/* ‹tŽÔ—Ö */
		if (RuleData::chkRuleApplied("gyakusharin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‹tŽÔ—Ö", get_yaku_han("gyakusharin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_CIRCLES);
				}
			));
		/* ‹t’|—Ñ */
		if (RuleData::chkRuleApplied("gyakuchikurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‹t’|—Ñ", get_yaku_han("gyakuchikurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_BAMBOOS);
				}
			));
		/* ‹t”—× */
		if (RuleData::chkRuleApplied("gyakusuurin"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‹t”—×", get_yaku_han("gyakusuurin"),
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_CHARACTERS);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ‘Žm */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘Žm–³‘o", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"\ŽO•s“ƒ", "ŒÜ–åÖ",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[ShantenAnalyzer::shantenOrphans] == -1);
		}
	));
	if (RuleData::chkRuleApplied("double_yakuman")) /* ‘Žm13–Ê‘Ò‚¿iƒ_ƒuƒ‹–ð–žj */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘Žm–³‘o\ŽO–Ê", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"‘Žm–³‘o", "\ŽO•s“ƒ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenAnalyzer::shantenOrphans] == -1) && // ‘Žm–³‘o‚É‚È‚Á‚Ä‚¢‚ÄA
					(!analysis->MachiInfo.FuritenFlag) && // ƒtƒŠƒeƒ“‚Å‚Í‚È‚­‚ÄA
					((analysis->MachiInfo.MachiMen == 13)||(analysis->GameStat->TianHuFlag))); // 13–Ê‘Ò‚¿‚©“V˜a‚É‚È‚Á‚Ä‚¢‚é
			}
		));
	/* Žµ¯–³èÏ */
	if (RuleData::chkRuleApplied("stellar_uushii"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žµ¯–³èÏ", get_yaku_han("stellar_uushii"),
			"‘S•sèÏ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenStellar] == -1);
			}
		));
	/* “ì–kí‘ˆ */
	if (RuleData::chkRuleApplied("civil_war"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ì–kí‘ˆ", get_yaku_han("civil_war"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenCivilWar] == -1);
			}
		));
	/* ˜f¯’¼—ñ */
	if (RuleData::chkRuleApplied("syzygy"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"˜f¯’¼—ñ", get_yaku_han("syzygy"),
			"´ˆêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenSyzygy] == -1);
			}
		));
	/* ‘S•sèÏ */
	if (RuleData::chkRuleApplied("quanbukao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘S•sèÏ", get_yaku_han("quanbukao"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenQuanbukao] == -1);
			}
		));
	/* ƒZƒuƒ“ƒAƒbƒv */
	if (RuleData::chkRuleApplied("sevenup"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒZƒuƒ“ƒAƒbƒv", get_yaku_han("sevenup"),
			"¬ˆêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenSevenup] == -1);
			}
		));
	/* ‘g‡—´ */
	if (RuleData::chkRuleApplied("zuhelong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘g‡—´", get_yaku_han("zuhelong"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[ShantenAnalyzer::shantenZuhelong] == -1);
			}
		));
}
