#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* ƒ`[ƒgƒC */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"µ‘Îq", (getRule(RULE_SEVEN_PAIRS) == 1) ? /* 1–|50•„‚Ìƒ‹[ƒ‹‚Æ2–|25•„‚Ìƒ‹[ƒ‹‚ª‚ ‚éB•„‚Í‚±‚±‚Å‚Íİ’è‚Å‚«‚È‚¢‚Å‚·cc */
		yaku::yakuCalculator::Yaku::yval_1han_menzen : yaku::yakuCalculator::Yaku::yval_2han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		}
	));
	/* ‘m */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‘m–³‘o", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"\O•s“ƒ",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0) /* ‘m13–Ê‘Ò‚¿iƒ_ƒuƒ‹–ğ–j */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘m–³‘o\O–Ê", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"‘m–³‘o", "\O•s“ƒ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // ‘m–³‘o‚É‚È‚Á‚Ä‚¢‚ÄA
					(/* ƒtƒŠƒeƒ“‚©‚Ç‚¤‚©”»’è‚·‚é */0) && // ƒtƒŠƒeƒ“‚Å‚Í‚È‚­‚ÄA
					((/* 13–Ê‘Ò‚¿‚©”»’è‚·‚é */0)||(analysis->GameStat->TianHuFlag))); // 13–Ê‘Ò‚¿‚©“V˜a‚É‚È‚Á‚Ä‚¢‚é
			}
		));
	/* µ¯–³èÏ */
	if (getRule(RULE_STELLAR_UUSHII) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"µ¯–³èÏ", (getRule(RULE_STELLAR_UUSHII) == 2) ?
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
