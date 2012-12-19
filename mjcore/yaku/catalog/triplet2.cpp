#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_2() {
	/* Šï”‘ÎX˜a */
	if (RuleData::chkRuleApplied("odd_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Šï”‘ÎX˜a", get_yaku_han("odd_toitoi"),
			"‘ÎX˜a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SIZE_OF_MELD_BUFFER);
			}
		));
	/* ‹ô”‘ÎX˜a */
	if (RuleData::chkRuleApplied("even_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ô”‘ÎX˜a", get_yaku_han("even_toitoi"),
			"‘ÎX˜a", "’f›ô‹ã",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 2; i < TILE_SUIT_HONORS; i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SIZE_OF_MELD_BUFFER);
			}
		));

	// ---------------------------------------------------------------------

	/* ’†Œ´’€Ž­ */
	if (RuleData::chkRuleApplied("chuugen_chikuroku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’†Œ´’€Ž­", get_yaku_han("chuugen_chikuroku"),
			"‘ÎX˜a", "–ð”vEá¢", "–ð”vE’†", "âˆê–å",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[CircleSix] >= 1) );
			}
		));
	/* ‹à‹T›z’` */
	if (RuleData::chkRuleApplied("turtle_egg"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹à‹T›z’`", get_yaku_han("turtle_egg"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) );
			}
		));
	/* ˆ®“ú“Œ¸ */
	if (RuleData::chkRuleApplied("kyokujitsu_toushou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ˆ®“ú“Œ¸", get_yaku_han("kyokujitsu_toushou"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) );
			}
		));
	/* “ú—Ž¼ŽR */
	if (RuleData::chkRuleApplied("riirao_shiishan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ú—Ž¼ŽR", get_yaku_han("riirao_shiishan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* —h–]–k“l */
	if (RuleData::chkRuleApplied("youbou_hokuto"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"—h–]–k“l", get_yaku_han("youbou_hokuto"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) );
			}
		));
	/* –k“lŽµ¯ */
	if (RuleData::chkRuleApplied("big_dipper"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–k“lŽµ¯", get_yaku_han("big_dipper"),
			"ƒrƒbƒNƒ{[ƒiƒX", "ŽOF“¯",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) );
			}
		));
	/* ¼›ù‘ÒŒŽ */
	if (RuleData::chkRuleApplied("seisou_taigetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¼›ù‘ÒŒŽ", get_yaku_han("seisou_taigetsu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1) &&
					(analysis->TsumoHai->tile == CircleOne) );
			}
		));
	/* “ì–³”ª”¦ */
	if (RuleData::chkRuleApplied("namu_hachiman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ì–³”ª”¦", get_yaku_han("namu_hachiman"),
			"–ð”vE”’", "“ì”’‚Â‚«‚à‚Ì",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CharacterEight] >= 1) );
			}
		));
	/* ¼ŒÎ\Œi */
	if (RuleData::chkRuleApplied("seiko_jikkei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¼ŒÎ\Œi", get_yaku_han("seiko_jikkei"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1 && analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1 && analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1 && analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1 && analysis->KeziCount[CircleSix] >= 1);
			}
		));
	/* ‰ÎŽR”š”­ */
	if (RuleData::chkRuleApplied("volcanic_erruption"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰ÎŽR”š”­", get_yaku_han("volcanic_erruption"),
			"‘ÎX˜a", "–ð”vEá¢", "–ð”vE’†",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CharacterNine] >= 1 ||
					analysis->KeziCount[CircleNine] >= 1 || analysis->KeziCount[BambooNine] >= 1);
			}
		));
	/* ’O–P’©—z */
	if (RuleData::chkRuleApplied("tanfon_chouyou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’O–P’©—z", get_yaku_han("tanfon_chouyou"),
			"–ð”vE’†",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) );
			}
		));

	// ---------------------------------------------------------------------

	auto haouben =
		[](const MENTSU_ANALYSIS* const analysis, tileCode tc, int step) -> bool {
			if ((analysis->KangziCount[tc] >= 1) &&
				(analysis->KeziCount[tc + step] >= 1) &&
				(analysis->KangziCount[tc + step] == 0) &&
				(analysis->MianziDat[0].tile == (tileCode)(tc + step * 2)) )
				return true;
			if ((analysis->KangziCount[tc + step * 2] >= 1) &&
				(analysis->KeziCount[tc + step] >= 1) &&
				(analysis->KangziCount[tc + step] == 0) &&
				(analysis->MianziDat[0].tile == tc) )
				return true;
			return false;
		};

	/* ”e‰¤•Ú */
	if (RuleData::chkRule("haouben") == "6han_bamboo_only")
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”e‰¤•Ú", yaku::yakuCalculator::Yaku::yval_6han,
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int k = 1; k <= 3; k++)
					yakuFlag = yakuFlag || haouben(analysis, (tileCode)(TILE_SUIT_BAMBOOS + k), 3);
				return yakuFlag;
			}
		));
	else if (RuleData::chkRuleApplied("haouben"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”e‰¤•Ú", get_yaku_han("haouben"),
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 3; k++)
						yakuFlag = yakuFlag || haouben(analysis, (tileCode)(i + k), 3);
				return yakuFlag;
			}
		));
	/* —«ŠÖ’£ */
	if (RuleData::chkRuleApplied("ryuukanchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"—«ŠÖ’£", get_yaku_han("ryuukanchan"),
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 7; k++)
						yakuFlag = yakuFlag || haouben(analysis, (tileCode)(i + k), 1);
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* êF‹´‰ï */
	if (RuleData::chkRuleApplied("queqiaohui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"êF‹´‰ï", get_yaku_han("queqiaohui"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->KeziCount[BambooOne] >= 1) {
					if ((analysis->KeziCount[CharacterSeven] >= 1) &&
						(analysis->KeziCount[CircleSeven] >= 1)) return true;
					else if ((analysis->KeziCount[CharacterSeven] >= 1) &&
						(analysis->KeziCount[BambooSeven] >= 1)) return true;
					else if ((analysis->KeziCount[CircleSeven] >= 1) &&
						(analysis->KeziCount[BambooSeven] >= 1)) return true;
					else return false;
				}
				else return false;
			}
		));
	/* ˆêŠÆ•—ŒŽ */
	if (RuleData::chkRuleApplied("yigan_fengyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ˆêŠÆ•—ŒŽ", get_yaku_han("yigan_fengyue"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					((analysis->KeziCount[EastWind] >= 1) || (analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) || (analysis->KeziCount[NorthWind] >= 1) ));
			}
		));
	/* “ì‚ÌŽé */
	if (RuleData::chkRuleApplied("vermilion_bird"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ì‚ÌŽé", get_yaku_han("vermilion_bird"),
			"‘ÎX˜a", "¬ˆêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[BambooOne] >= 1) &&
					((analysis->KeziCount[BambooFive] >= 1 && analysis->KeziCount[BambooSeven] >= 1) ||
					(analysis->KeziCount[BambooFive] >= 1 && analysis->KeziCount[BambooNine] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1 && analysis->KeziCount[BambooNine] >= 1)) &&
					(analysis->MianziDat[0].tile == RedDragon));
			}
		));
	/* ¼‚Ì”’ŒÕ */
	if (RuleData::chkRuleApplied("white_tiger"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¼‚Ì”’ŒÕ", get_yaku_han("white_tiger"),
			"‘ÎX˜a", "ŽOF“¯",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[CharacterThree] >= 1) &&
					(analysis->KangziCount[CircleThree] >= 1) &&
					(analysis->KangziCount[BambooThree] >= 1) &&
					(analysis->MianziDat[0].tile == WhiteDragon));
			}
		));
	/* ŽO”t”üŽðŒh”~‰Ô */
	if (RuleData::chkRuleApplied("sanbei_meijiu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŽO”t”üŽðŒh”~‰Ô", get_yaku_han("sanbei_meijiu"),
			"–ð”vE’†",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					((analysis->KeziCount[CharacterNine] >= 1) ||
					(analysis->KeziCount[CircleNine] >= 1) ||
					(analysis->KeziCount[BambooNine] >= 1) ));
			}
		));
	/* “Œ•—ˆ³“|¼•— */
	if (RuleData::chkRuleApplied("dongfeng_yadao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ•—ˆ³“|¼•—", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->KangziCount[EastWind] >= 1) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han);
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (((analysis->KangziCount[EastWind] == 0) && // “ŒŽqE¼‘ÎŽq
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->MianziDat[0].tile == WestWind)) ||
					((analysis->KangziCount[EastWind] >= 1) && // “ŒžÈŽqE¼Žq
					(analysis->KangziCount[WestWind] == 0) &&
					(analysis->KeziCount[WestWind] >= 1)));
			}
		));
	/* ‘¾Œö’Þ‹à‹T */
	if (RuleData::chkRuleApplied("taikou_turtle"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‘¾Œö’Þ‹à‹T", get_yaku_han("taikou_turtle"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->TsumoHai->tile == BambooThree) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki));
			}
		));
	/* Â”Ÿƒgƒ“ƒlƒ‹ */
	if (RuleData::chkRuleApplied("seikan_tunnel"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Â”Ÿƒgƒ“ƒlƒ‹", get_yaku_han("seikan_tunnel"),
			"–ð”vEá¢", "“Œá¢‚Â‚«‚à‚Ì",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[EastWind] == 0) &&
					(analysis->KeziCount[CircleOne] >= 1));
			}
		));
	/* •ógŠJ‰Ô */
	if (RuleData::chkRuleApplied("baohongkaihua"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"•ógŠJ‰Ô", get_yaku_han("baohongkaihua"),
			"‘ÎX˜a", "–ð”vE”’", "–ð”vE’†", "¬˜V“ª", "ŽOF¬“¯", "”’’¹",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CharacterOne] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1) &&
					(analysis->DuiziCount[BambooOne] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* “Œá¢‚Â‚«‚à‚Ì */
	if (RuleData::chkRuleApplied("east_green"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œá¢‚Â‚«‚à‚Ì", get_yaku_han("east_green"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* “ì”’‚Â‚«‚à‚Ì */
	if (RuleData::chkRuleApplied("south_white"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ì”’‚Â‚«‚à‚Ì", get_yaku_han("south_white"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* ’†¼ŒN */
	if (RuleData::chkRuleApplied("west_red"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’†¼ŒN", get_yaku_han("west_red"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* ”’’¹ */
	if (RuleData::chkRuleApplied("hakucho"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”’’¹", get_yaku_han("hakucho"),
			"‘ÎX˜a", "–ð”vE”’",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1));
			}
		));
	/* ‰ÎŽR”š”­(ŒÜõ‚ÆŽµõ) */
	if (RuleData::chkRuleApplied("volcanic_toitoi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰ÎŽR”š”­ ", get_yaku_han("volcanic_toitoi"),
			"‘ÎX˜a", "–ð”vE’†", "–ð”vEá¢", "¬ˆêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[BambooThree] >= 1) &&
					(analysis->DuiziCount[BambooFive] >= 1) &&
					(analysis->DuiziCount[BambooSeven] >= 1) &&
					(analysis->DuiziCount[GreenDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* ”’ˆêF‚Ì’è‹`ˆ—‚Ímisc.cpp‚ÉˆÚ“®‚µ‚Ü‚µ‚½ */

	/* á‚Ì‹âŠt */
	if (RuleData::chkRuleApplied("snow_ginkaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"á‚Ì‹âŠt", get_yaku_han("snow_ginkaku"),
			"‘ÎX˜a", "–ð”vE”’", "¬ˆêF", "’fg˜a", "¬’fg",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* –k”’ì */
	if (RuleData::chkRuleApplied("kitashirakawa"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–k”’ì", get_yaku_han("kitashirakawa"),
			"–ð”vE”’",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[BambooSix] >= 1));
			}
		));
	/* ‘f”–ð–ž‚Ì’è‹`ˆ—‚Ímisc.cpp‚ÉˆÚ“®‚µ‚Ü‚µ‚½ */

	/* ƒAƒ‹’† */
	if (RuleData::chkRuleApplied("alcoholism"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒAƒ‹’†", get_yaku_han("alcoholism"),
			"‘ÎX˜a", "–ð”vE’†", "ŽOF“¯",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CharacterTwo] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[BambooTwo] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* ‹~‹}ŽÔ */
	if (RuleData::chkRuleApplied("kyukyusha"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹~‹}ŽÔ", get_yaku_han("kyukyusha"),
			"‘ÎX˜a", "–ð”vE”’", "ŽOF“¯", "¬˜V“ª",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* ““ïŽÔ */
	if (RuleData::chkRuleApplied("tounansha"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"““ïŽÔ", get_yaku_han("tounansha"),
			"ŽO•—",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1));
			}
		));
	/* Ât‚É‰÷‚¢‚È‚µ */
	if (RuleData::chkRuleApplied("seishun_kuinashi")) {
		auto seishunnikuinashi =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1));
			};
		if (RuleData::chkRule("seishun_kuinashi") == "1han_menzen") {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"Ât‚É‰÷‚¢‚È‚µ", yaku::yakuCalculator::Yaku::yval_1han_menzen,
				seishunnikuinashi
			));
		} else if (RuleData::chkRule("seishun_kuinashi") == "2han_kuisagari"){
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"Ât‚É‰÷‚¢‚È‚µ", yaku::yakuCalculator::Yaku::yval_2han_menzen,
				"Ât‚É‰÷‚¢‚ ‚è",
				seishunnikuinashi
			));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"Ât‚É‰÷‚¢‚ ‚è", yaku::yakuCalculator::Yaku::yval_1han,
				seishunnikuinashi
			));
		}
	}
	/* ƒX[ƒp[ƒ}ƒ“ */
	if (RuleData::chkRuleApplied("superman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒX[ƒp[ƒ}ƒ“", get_yaku_han("superman"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if ((analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CharacterEight] >= 1)) return true;
				else if ((analysis->MianziDat[0].tile == CharacterEight) &&
					(analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->TsumoHai->tile == CharacterFour)) return true;
				else if ((analysis->MianziDat[0].tile == CharacterFour) &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->TsumoHai->tile == CharacterEight)) return true;
				else return false;
			}
		));
	/* “Œ‰Ô‰€ */
	if (RuleData::chkRuleApplied("higashi_hanazono_triplets"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ‰Ô‰€", get_yaku_han("higashi_hanazono_triplets"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					((analysis->KeziCount[CharacterEight] >= 1) || (analysis->KeziCount[CircleEight] >= 1) ||
					(analysis->KeziCount[BambooEight] >= 1)) &&
					((analysis->KeziCount[CharacterSeven] >= 1) || (analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1)));
			}
		));
	/* —Ô—¼” */
	if (RuleData::chkRuleApplied("rinryanshii"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"—Ô—¼”", get_yaku_han("rinryanshii"),
			/* ‘ÎX˜a‚ª•K‚¸•¡‡ */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				char pattern[] = "121314151617181923242526272829343536373839454647484956575859676869787989";
				tileCode tCode[TILE_SUIT_HONORS / TILE_SUIT_STEP * 2];
				bool yakuFlag = false;
				for (int i = 0; i < 36; i++) {
					int num1 = (int)(pattern[i * 2] - '0');
					int num2 = (int)(pattern[i * 2 + 1] - '0');
					for (int k = 0; k < TILE_SUIT_HONORS / TILE_SUIT_STEP * 2; k += 2) {
						tCode[k] = (tileCode)(k / 2 * TILE_SUIT_STEP + num1);
						tCode[k + 1] = (tileCode)(k / 2 * TILE_SUIT_STEP + num2);
					}
					if (yaku::countingFacility::countSpecMentz(
						analysis->MianziDat, tCode, TILE_SUIT_HONORS / TILE_SUIT_STEP * 2, nullptr, 0, false) == SIZE_OF_MELD_BUFFER)
						yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ‚R•ªŠÔ‘Ò‚Á‚Ä‚â‚é */
	auto anysuit3 =
		[](const MENTSU_ANALYSIS* const analysis, int val1, int val2, int val3, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (int suit1 = 0; suit1 < TILE_SUIT_HONORS; suit1+= TILE_SUIT_STEP)
				for (int suit2 = 0; suit2 < TILE_SUIT_HONORS; suit2 += TILE_SUIT_STEP)
					for (int suit3 = 0; suit3 < TILE_SUIT_HONORS; suit3 += TILE_SUIT_STEP)
						if (((*count)[suit1 + val1] >= 1) && ((*count)[suit2 + val2] >= 1) &&
							((*count)[suit3 + val3] >= 1)) yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("colonel_muska"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒ€ƒXƒJ‘å²", get_yaku_han("colonel_muska"),
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 6, 4, 3, true) && (analysis->Machi == yaku::yakuCalculator::machiKanchan);
			}
		));
	/* “s‚Ì¼–k */
	if (RuleData::chkRuleApplied("miyakonoseihoku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“s‚Ì¼–k", get_yaku_han("miyakonoseihoku"),
			"‘ÎX˜a",
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 5, false) &&
					(analysis->DuiziCount[WestWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1);
			}
		));
	/* “Œ‹ž“ñ\ŽO‹æ */
	if (RuleData::chkRuleApplied("tokyo")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ‹ž“ñ\ŽO‹æ",  yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(isshoku(analysis, false)) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"‘ÎX˜a",
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 2, 3, 9, true) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	}
	/* ‰‰¹ƒ~ƒN */
	auto anysuit2 =
		[](const MENTSU_ANALYSIS* const analysis, int val1, int val2, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (int suit1 = 0; suit1 < TILE_SUIT_HONORS; suit1+= TILE_SUIT_STEP)
				for (int suit2 = 0; suit2 < TILE_SUIT_HONORS; suit2 += TILE_SUIT_STEP)
					if (((*count)[suit1 + val1] >= 1) && ((*count)[suit2 + val2] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::chkRuleApplied("hatsune_miku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰‰¹ƒ~ƒN", get_yaku_han("hatsune_miku"),
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 3, 9, true) && (analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* ”’‰Ô */
	if (RuleData::chkRuleApplied("baihua"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”’‰Ô", get_yaku_han("baihua"),
			/* •Ê“r”’‚Å1ãÊ */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));
	/* Œ‚ŒÛœ±‘‚ */
	if (RuleData::chkRuleApplied("jigu_ecao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Œ‚ŒÛœ±‘‚", get_yaku_han("jigu_ecao"),
			"–ð”vE”’",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ƒrƒbƒNƒ{[ƒiƒX */
	if (RuleData::chkRuleApplied("777")) {
		auto yaku777 =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1);
			};
		if (RuleData::chkRule("777") == "yakuman")
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ƒrƒbƒNƒ{[ƒiƒX", yaku::yakuCalculator::Yaku::yval_yakuman,
				"ŽOF“¯",
				yaku777
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ƒrƒbƒNƒ{[ƒiƒX", get_yaku_han("777"),
				yaku777
			));
	}
	/* ƒNƒŠƒXƒ}ƒX */
	if (RuleData::chkRuleApplied("christmas")) {
		auto chris =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < 6; i++)
					if ((analysis->KeziCount[(parsedat_trichrome3[i][0] - '0') + 1] >= 1) &&
						(analysis->KeziCount[(parsedat_trichrome3[i][0] - '0') + 2] >= 1) &&
						(analysis->KeziCount[(parsedat_trichrome3[i][1] - '0') + 2] >= 1) &&
						(analysis->KeziCount[(parsedat_trichrome3[i][1] - '0') + 5] >= 1))
						return true;
				return false;
			};
		auto chrisday = 
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 12) && (nowTime.wDay == 25);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒNƒŠƒXƒ}ƒX", yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"‘ÎX˜a",
			chris
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒ³ƒNƒŠƒXƒ}ƒX", yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman);
			}),
			"‘ÎX˜a", "ƒNƒŠƒXƒ}ƒX",
			[chris](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chris(analysis) &&
					(analysis->MianziDat[0].tile == BambooSeven);
			}
		));
	}
	/* ˆê•xŽm“ñ‘éŽO‰ÖŽq */
	if (RuleData::chkRuleApplied("newyear_dream")) {
		auto isnewyeardays = 
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 1) && (nowTime.wDay <= 2);
			};
		auto newyrdrm =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->PlayerStat->MeldPointer >= 3)
					return ((analysis->PlayerStat->Meld[1].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[1].tile == BambooThree) &&
						(analysis->PlayerStat->Meld[2].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[2].tile == BambooOne) &&
						(analysis->PlayerStat->Meld[3].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[3].tile == CircleEight));
				else return false;
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ˆê•xŽm“ñ‘éŽO‰ÖŽq", yaku::yakuCalculator::Yaku::yval_yakuman,
			newyrdrm
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒ³ˆê•xŽm“ñ‘éŽO‰ÖŽq", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"ˆê•xŽm“ñ‘éŽO‰ÖŽq",
			[newyrdrm, isnewyeardays](const MENTSU_ANALYSIS* const analysis) -> bool {
				return newyrdrm(analysis) && isnewyeardays();
			}
		));
	}
	/* •‘ ŠÛ */
	if (RuleData::chkRuleApplied("musashimaru")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"•‘ ŠÛ", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 6, 3, 4, true) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒ³•‘ ŠÛ", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"•‘ ŠÛ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSix] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->MianziDat[0].tile / TILE_SUIT_STEP == TILE_SUIT_CIRCLES / TILE_SUIT_STEP);
			}
		));
	}

	// ---------------------------------------------------------------------

	/* ã_ */
	if (RuleData::chkRuleApplied("hanshin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ã_", get_yaku_han("hanshin"),
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 8, 4, true);
			}
		));
	/* _ŒË */
	if (RuleData::chkRuleApplied("kobe"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"_ŒË", get_yaku_han("kobe"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterFive] >= 1) ||
					(analysis->KeziCount[CircleFive] >= 1) ||
					(analysis->KeziCount[BambooFive] >= 1)) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* ƒTƒbƒ|ƒˆê”Ô */
	if (RuleData::chkRuleApplied("sapporo1"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒTƒbƒ|ƒˆê”Ô", get_yaku_han("sapporo1"),
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 6, true) &&
					(analysis->GameStat->GameRound == 0) && // “Œˆê‹Ç‚Å‚ ‚Á‚Ä
					(analysis->GameStat->LoopRound == 0) && // •Ô‚è“Œê‚Å‚à‚È‚­‚Ä
					(analysis->GameStat->Honba == 0); // •½ê‚ÌŽž
			}
		));
	/* ƒ|ƒJƒŠƒXƒGƒbƒg */
	if (RuleData::chkRuleApplied("pcrswt"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒ|ƒJƒŠƒXƒGƒbƒg", get_yaku_han("pcrswt"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleTwo] >= 1) ||
					(analysis->KeziCount[CircleFour] >= 1) ||
					(analysis->KeziCount[CircleEight] >= 1)) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* ƒAƒNƒGƒŠƒAƒX */
	if (RuleData::chkRuleApplied("aqrs"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒAƒNƒGƒŠƒAƒX", get_yaku_han("aqrs"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) ||
					(analysis->KeziCount[CircleThree] >= 1) ||
					(analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[CircleNine] >= 1)) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* ‚r‚l‚`‚o */
	if (RuleData::chkRuleApplied("smap"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‚r‚l‚`‚o", get_yaku_han("smap"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterFour] -
					analysis->AnKeziCount[CharacterFour] >= 1));
			}
		));
	/* ƒ‚ƒ“ƒXƒ^[ƒnƒ“ƒ^[ */
	if (RuleData::chkRuleApplied("monster_hunter"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒ‚ƒ“ƒXƒ^[ƒnƒ“ƒ^[", get_yaku_han("monster_hunter"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) ||
					(analysis->KeziCount[NorthWind] >= 1)) &&
					(analysis->GameStat->TurnRound <= 6);
			}
		));
	/* ˆ®ìƒ‰[ƒƒ“ */
	if (RuleData::chkRuleApplied("asahikawa_raamen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ˆ®ìƒ‰[ƒƒ“", get_yaku_han("asahikawa_raamen"),
			"‘ÎX˜a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* “Œ•û—dX–² */
	if (RuleData::chkRuleApplied("perfect_cherry_blossom"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ•û—dX–²", get_yaku_han("perfect_cherry_blossom"),
			"ŽOF“¯",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* “Œ•ûg˜O–² */
	if (RuleData::chkRuleApplied("touhou_kouroumu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ•ûg˜O–²", get_yaku_han("touhou_kouroumu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CharacterSix] >= 1) &&
					(analysis->TsumoHai->tile == EastWind);
			}
		));
	/* Windows95 */
	if (RuleData::chkRuleApplied("windows95"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows95", get_yaku_han("windows95"),
			"ŽOF“¯", "‘ÎX˜a", "—Ô—¼”",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterFive] +
					analysis->DuiziCount[CircleFive] +
					analysis->DuiziCount[BambooFive] >= 2);
			}
		));
	/* Windows98 */
	if (RuleData::chkRuleApplied("windows98"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows98", get_yaku_han("windows98"),
			"ŽOF“¯", "‘ÎX˜a", "—Ô—¼”",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] +
					analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2);
			}
		));
	/* Windows3.1 */
	if (RuleData::chkRuleApplied("windows31"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows3.1", get_yaku_han("windows31"),
			"‘ÎX˜a", "—Ô—¼”",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->DuiziCount[CharacterThree] +
					analysis->DuiziCount[CircleThree] +
					analysis->DuiziCount[BambooThree] +
					analysis->DuiziCount[CharacterOne] +
					analysis->DuiziCount[CircleOne] +
					analysis->DuiziCount[BambooOne] == SIZE_OF_MELD_BUFFER);
			}
		));
	/* Windows7 */
	if (RuleData::chkRuleApplied("windows7"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows7", get_yaku_han("windows7"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->TsumoHai->tile == CharacterSeven) &&
					((analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai->tile == CircleSeven) &&
					((analysis->KeziCount[BambooSeven] >= 1) ||
					(analysis->KeziCount[CharacterSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai->tile == BambooSeven) &&
					((analysis->KeziCount[CharacterSeven] >= 1) ||
					(analysis->KeziCount[CircleSeven] >= 1))) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* “Œ•û•—_˜^ */
	if (RuleData::chkRuleApplied("mountain_of_faith"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ•û•—_˜^", get_yaku_han("mountain_of_faith"),
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 6, 7, true) &&
					((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) ||
					(analysis->KeziCount[NorthWind] >= 1)) &&
					(analysis->PlayerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile == EastWind);
			}
		));
	/* ”Ž—í—ì–² */
	if (RuleData::chkRuleApplied("hakurei_reimu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”Ž—í—ì–²", get_yaku_han("hakurei_reimu"),
			"‘ÎX˜a", "âˆê–å",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CharacterEight] >= 1) &&
					(analysis->DuiziCount[CharacterNine] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1));
			}
		));
	/* –¶‰J–‚—¹ */
	if (RuleData::chkRuleApplied("kirisame_marisa"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–¶‰J–‚—¹", get_yaku_han("kirisame_marisa"),
			"‘ÎX˜a", "âˆê–å",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CharacterThree] >= 1));
			}
		));
	/* \˜Z–éç–é */
	if (RuleData::chkRuleApplied("izayoi_sakuya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"\˜Z–éç–é", get_yaku_han("izayoi_sakuya"),
			"‘ÎX˜a", "¬ˆêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiShanpon));
			}
		));
	/* ¼“ìí‘ˆ */
	if (RuleData::chkRuleApplied("seinansensou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"¼“ìí‘ˆ", get_yaku_han("seinansensou"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1));
			}
		));
	/* ƒ³¼“ìí‘ˆ */
	if (RuleData::chkRuleApplied("junsei_seinansensou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒ³¼“ìí‘ˆ", get_yaku_han("junsei_seinansensou"),
			"ŽOF¬“¯",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[CharacterNine] >= 1) &&
					(analysis->DuiziCount[CircleNine] >= 1) &&
					(analysis->DuiziCount[BambooNine] >= 1));
			}
		));
	/* ‹ãBVŠ²ü */
	if (RuleData::chkRuleApplied("kyushu_shinkansen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ãBVŠ²ü", get_yaku_han("kyushu_shinkansen"),
			"ŽOF“¯", "ŒÜ–åÖ", "¬˜V“ª",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] + analysis->DuiziCount[GreenDragon] +
					analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* ˆ¢f */
	if (RuleData::chkRuleApplied("ara"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ˆ¢f", get_yaku_han("ara"),
			"¬ŽOŒ³", "‘ÎX˜a", "¬˜V“ª",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* ‰õ”n‰Á•Ú */
	if (RuleData::chkRuleApplied("kaibakaben"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‰õ”n‰Á•Ú", get_yaku_han("kaibakaben"),
			"‘åŽOŒ³", "ŽšˆêF", "‘ÎX˜a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* Œ]ŒŽ */
	if (RuleData::chkRuleApplied("suikougetsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Œ]ŒŽ", get_yaku_han("suikougetsu"),
			"‘åŽOŒ³", "ŽšˆêF", "‘ÎX˜a",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* ’†–Ú• */
	if (RuleData::chkRuleApplied("nakameguro"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’†–Ú•", get_yaku_han("nakameguro"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind]>= 1));
			}
		));
}
