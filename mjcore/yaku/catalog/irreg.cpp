#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* ƒ`[ƒgƒC */
	auto isQiDui =
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"Žµ‘ÎŽq", (getRule(RULE_SEVEN_PAIRS) == 1) ? /* 1–|50•„‚Ìƒ‹[ƒ‹‚Æ2–|25•„‚Ìƒ‹[ƒ‹‚ª‚ ‚éB•„‚Í‚±‚±‚Å‚ÍÝ’è‚Å‚«‚È‚¢‚Å‚·cc */
		yaku::yakuCalculator::Yaku::yval_1han_menzen : yaku::yakuCalculator::Yaku::yval_2han_menzen,
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

	/* ƒc[ƒC[ƒ\[ */
	if (getRule(RULE_SEVEN_HONOR_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žµ•Ÿ¯", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"¬˜V“ª", "¬ˆêF", "ŽOŒ³Žµ‘ÎŽq", "ŽlŠìŽµ‘ÎŽq", "g”’Žµ‘Î",
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (isQiDui(analysis))
					return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NUM_OF_TILES_IN_HAND / 2);
				else return false;
			}
		));
	/* ŽOŒ³Žµ‘ÎŽq */
	if (getRule(RULE_DRAGON_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOŒ³Žµ‘ÎŽq", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[GreenDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* ŽlŠìŽµ‘ÎŽq */
	if (getRule(RULE_WIND_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽlŠìŽµ‘ÎŽq", yaku::yakuCalculator::Yaku::yval_1han_menzen,
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[EastWind] >= 2) &&
					(analysis->TileCount[SouthWind] >= 2) &&
					(analysis->TileCount[WestWind] >= 2) &&
					(analysis->TileCount[NorthWind] >= 2));
			}
		));
	/* Šï”Žµ‘ÎŽq */
	if (getRule(RULE_ODD_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Šï”Žµ‘ÎŽq", yaku::yakuCalculator::Yaku::yval_1han_menzen,
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
	if (getRule(RULE_EVEN_PAIRS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ô”Žµ‘ÎŽq", yaku::yakuCalculator::Yaku::yval_2han_menzen,
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
	if (getRule(RULE_KOUHAKU_CHIITOI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"g”’Žµ‘Î", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			[isQiDui](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (isQiDui(analysis) &&
					(analysis->TileCount[WhiteDragon] >= 2) &&
					(analysis->TileCount[RedDragon] >= 2));
			}
		));
	/* –k‚Ì‘å’n */
	if (getRule(RULE_KITA_NO_DAICHI) != 0)
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
	if (getRule(RULE_SEKAIICHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¢ŠEˆê", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
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
	if (getRule(RULE_SANSHOKU_DOUTOI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽOF“¯‘Î", yaku::yakuCalculator::Yaku::yval_1han_menzen,
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
		if (getRule(RULE_GYAKUSHARIN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‹tŽÔ—Ö", yaku::yakuCalculator::Yaku::yval_4han_menzen,
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_CIRCLES);
				}
			));
		/* ‹t’|—Ñ */
		if (getRule(RULE_GYAKUCHIKURIN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‹t’|—Ñ", yaku::yakuCalculator::Yaku::yval_4han_menzen,
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_BAMBOOS);
				}
			));
		/* ‹t”—× */
		if (getRule(RULE_GYAKUSUURIN) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"‹t”—×", yaku::yakuCalculator::Yaku::yval_4han_menzen,
				[gyakusharin](const MENTSU_ANALYSIS* const analysis) -> bool {
					return gyakusharin(analysis, TILE_SUIT_CHARACTERS);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ‘Žm */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘Žm–³‘o", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"\ŽO•s“ƒ",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0) /* ‘Žm13–Ê‘Ò‚¿iƒ_ƒuƒ‹–ð–žj */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘Žm–³‘o\ŽO–Ê", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"‘Žm–³‘o", "\ŽO•s“ƒ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // ‘Žm–³‘o‚É‚È‚Á‚Ä‚¢‚ÄA
					(/* ƒtƒŠƒeƒ“‚©‚Ç‚¤‚©”»’è‚·‚é */0) && // ƒtƒŠƒeƒ“‚Å‚Í‚È‚­‚ÄA
					((/* 13–Ê‘Ò‚¿‚©”»’è‚·‚é */0)||(analysis->GameStat->TianHuFlag))); // 13–Ê‘Ò‚¿‚©“V˜a‚É‚È‚Á‚Ä‚¢‚é
			}
		));
	/* Žµ¯–³èÏ */
	if (getRule(RULE_STELLAR_UUSHII) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Žµ¯–³èÏ", (getRule(RULE_STELLAR_UUSHII) == 2) ?
			yaku::yakuCalculator::Yaku::yval_baiman_menzen : yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"‘S•sèÏ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenStellar] == -1);
			}
		));
	/* “ì–kí‘ˆ */
	if (getRule(RULE_CIVIL_WAR) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ì–kí‘ˆ", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenCivilWar] == -1);
			}
		));
	/* ˜f¯’¼—ñ */
	if (getRule(RULE_SYZYGY) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"˜f¯’¼—ñ", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenSyzygy] == -1);
			}
		));
	/* ‘S•sèÏ */
	if (getRule(RULE_QUANBUKAO) != 0) {
		const yaku::yakuCalculator::Yaku::MenzenHan* qbk_han = NULL;
		switch (getRule(RULE_QUANBUKAO)) {
		case 1: case 2:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_3han_menzen;
			break;
		case 3:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_4han_menzen;
			break;
		case 4:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_mangan_menzen;
			break;
		case 5:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_yakuman_menzen;
			break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘S•sèÏ", *qbk_han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenQuanbukao] == -1);
			}
		));
	}
}
