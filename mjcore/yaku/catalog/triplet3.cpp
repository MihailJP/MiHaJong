#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_3() {
	auto countTilesOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->TileCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->TileCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->TileCount[TILE_SUIT_BAMBOOS + numeral];
		};
	auto countKangziOf =
		[](const MENTSU_ANALYSIS* const analysis, unsigned numeral) -> unsigned {
			return analysis->KangziCount[TILE_SUIT_CHARACTERS + numeral] +
				analysis->KangziCount[TILE_SUIT_CIRCLES + numeral] +
				analysis->KangziCount[TILE_SUIT_BAMBOOS + numeral];
		};
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

	/* ÉCÉiÉoÉEÉAÅ[ */
	if (RuleData::chkRuleApplied("inabauer"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ÉCÉiÉoÉEÉAÅ[", get_yaku_han("inabauer"),
			"ê¥àÍêF", "ëŒÅXòa", "ãÿîvçè",
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
	/* élåéîné≠ */
	if (RuleData::chkRuleApplied("april_fool")) {
		auto isApril1st =
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 4) && (nowTime.wDay == 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"élåéîné≠", get_yaku_han("april_fool"),
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
	/* íÜìåÇÃìJ */
	if (RuleData::chkRuleApplied("chuutounofue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"íÜìåÇÃìJ", get_yaku_han("chuutounofue"),
			"ñîvÅEíÜ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1));
			}
		));
	/* éOçëéu */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"éOçëéu", get_yaku_han("sangokushi"),
			"ëŒÅXòa",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (bool)
					(countKeziOf(analysis, 3) * countKeziOf(analysis, 5) *
					countKeziOf(analysis, 9) * countKeziOf(analysis, 4));
			}
		));
	/* àÍãxÇ≥ÇÒ */
	if (RuleData::chkRuleApplied("ikkyuusan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"àÍãxÇ≥ÇÒ", get_yaku_han("ikkyuusan"),
			"ëŒÅXòa",
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 1) > 0) &&
					(countDuiziOf(analysis, 9) > 0) &&
					(countDuiziOf(analysis, 3) > 0) &&
					(countDuiziOf(analysis, 1) + countDuiziOf(analysis, 9) +
					countDuiziOf(analysis, 3) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* ëÂñÇê_ */
	if (RuleData::chkRuleApplied("daimajin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ëÂñÇê_", get_yaku_han("daimajin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKeziCount[WhiteDragon] +
					analysis->AnKeziCount[GreenDragon] +
					analysis->AnKeziCount[RedDragon]) >= 1) &&
					(analysis->TsumoHai->tile == BambooFive) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki);
			}
		));
	/* èºí|î~ */
	if (RuleData::chkRuleApplied("shouchikubai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"èºí|î~", get_yaku_han("shouchikubai"),
			"ñîvÅE·¢",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 2; i <= 9; ++i)
					if (analysis->KeziCount[TILE_SUIT_BAMBOOS + i] >= 1)
						flag = true;
				return flag &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* ÉpÉìÉ_ */
	if (RuleData::chkRuleApplied("panda"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ÉpÉìÉ_", get_yaku_han("panda"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* çïä¯åR */
	if (RuleData::chkRuleApplied("kokkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"çïä¯åR", get_yaku_han("kokkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1);
			}
		));
	/* îíä¯åR */
	if (RuleData::chkRuleApplied("hakkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"îíä¯åR", get_yaku_han("hakkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* çgä¯åR */
	if (RuleData::chkRuleApplied("kohkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"çgä¯åR", get_yaku_han("kohkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* óïä¯åR */
	if (RuleData::chkRuleApplied("rankigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"óïä¯åR", get_yaku_han("rankigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ìåïóâçôíñˆ */
	if (RuleData::chkRuleApplied("dongfeng_yan_jian_liu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ìåïóâçôíñˆ", get_yaku_han("dongfeng_yan_jian_liu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* ã¥ñ{ê^ñÁ */
	if (RuleData::chkRuleApplied("hashimoto_shin_ya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ã¥ñ{ê^ñÁ", get_yaku_han("hashimoto_shin_ya"),
			"ëŒÅXòa", "ê‚àÍñÂ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[CircleSeven] >= 1) &&
					(analysis->DuiziCount[BambooTwo] >= 1) &&
					(analysis->DuiziCount[BambooNine] >= 1);
			}
		));
	/* ëÂìÏêºèîìá */
	if (RuleData::chkRuleApplied("dai_nansei_shotou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ëÂìÏêºèîìá", get_yaku_han("dai_nansei_shotou"),
			"ëŒÅXòa", "éOêFìØçè", "ç¨òVì™",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1);
			}
		));
	/* îíàﬂÇÃìVég */
	if (RuleData::chkRuleApplied("hakuinotenshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"îíàﬂÇÃìVég", get_yaku_han("hakuinotenshi"),
			"ñîvÅEîí",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 7) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(countKangziOf(analysis, 5) >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind] >= 1);
			}
		));
	/* éµó¥éÏ */
	if (RuleData::chkRuleApplied("dragon_ball"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"éµó¥éÏ", get_yaku_han("dragon_ball"),
			"ëŒÅXòa", "ëÂéOå≥", "ç¨àÍêF",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] - analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1);
			}
		));
	/* éµå‹éO */
	if (RuleData::chkRuleApplied("shichigosan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"éµå‹éO", get_yaku_han("shichigosan"),
			"äÔêîëŒÅXòa", "äÔêîéµëŒéq", "ífõÙã„",
			[countTilesOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countTilesOf(analysis, 7) + countTilesOf(analysis, 5) + countTilesOf(analysis, 3) == NUM_OF_TILES_IN_HAND) &&
					(countTilesOf(analysis, 7) * countTilesOf(analysis, 5) * countTilesOf(analysis, 3));
			}
		));
	/* å‰ãÍòJÇ≥ÇÒ */
	if (RuleData::chkRuleApplied("gokurousan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"å‰ãÍòJÇ≥ÇÒ", get_yaku_han("gokurousan"),
			"ëŒÅXòa", "ãÿîvçè",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 5] >= 1) && (analysis->KeziCount[i + 9] >= 1) &&
						(analysis->KeziCount[i + 6] >= 1) && (analysis->KeziCount[i + 3] >= 1))
						flag = true;
				return flag;
			}
		));
	/* ÉjÉRÉjÉRññû */
	if (RuleData::chkRuleApplied("nikoniko_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ÉjÉRÉjÉRññû", get_yaku_han("nikoniko_yakuman"),
			"ëŒÅXòa", "ífõÙã„",
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 2) + countDuiziOf(analysis, 5) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* ïóê·ïÛëoïó */
	if (RuleData::chkRuleApplied("feng_xue_bao_shangfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ïóê·ïÛëoïó", get_yaku_han("feng_xue_bao_shangfeng"),
			"ëŒÅXòa", "ç¨òVì™",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (Wind2Tile(analysis->GameStat->GameRound / 4) != // É_ÉuïóÇ≈Ç»Ç≠Çƒ
					Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))) &&
					(analysis->DuiziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))] >= 1) && // é©ïóÇ™Ç†ÇË
					(analysis->DuiziCount[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1) && // èÍïóÇ™Ç†ÇË
					(analysis->DuiziCount[CircleOne] >= 1) && (analysis->DuiziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1);
			}
		));
	/* óyÇ©îﬁï˚ */
	if (RuleData::chkRuleApplied("haruka_kanata"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"óyÇ©îﬁï˚", get_yaku_han("haruka_kanata"),
			"ñîvÅEíÜ", "ñîvÅE·¢", "éOà√çè",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->AnKeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[RedDragon] >= 1) &&
					(analysis->AnKeziCount[WhiteDragon] >= 1);
			}
		));
	/* åÉélà√çè */
	if (RuleData::chkRuleApplied("geki_sianke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"åÉélà√çè", get_yaku_han("geki_sianke"),
			"élà√çè",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && // élà√çèÇ
					(analysis->TsumoHai->tile == CharacterOne); // àÍ‰›Ç≈òaóπÇÈ
			}
		));
	/* åÉëÂéOå≥ */
	if (RuleData::chkRuleApplied("geki_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"åÉëÂéOå≥", get_yaku_han("geki_daisangen"),
			"ëÂéOå≥",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiPenchan);
			}
		));
	/* åÉñk */
	if (RuleData::chkRuleApplied("geki_north"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"åÉñk", get_yaku_han("geki_north"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* ëÂã„òa */
	if (RuleData::chkRuleApplied("daikyuuhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ëÂã„òa", get_yaku_han("daikyuuhoh"),
			"éOêFìØçè", "ëŒÅXòa", "ç¨òVì™", "å‹ñÂêƒ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1);
			}
		));
	/* ëÂó—ä‘ */
	if (RuleData::chkRuleApplied("dairinkan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ëÂó—ä‘", get_yaku_han("dairinkan"),
			"ñîvÅEíÜ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ì°éqÉXÉyÉVÉÉÉã */
	if (RuleData::chkRuleApplied("fujikospecial"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ì°éqÉXÉyÉVÉÉÉã", get_yaku_han("fujikospecial"),
			"ñîvÅEîí",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 1; i < TILE_NONFLOWER_MAX; i++)
					if (analysis->GameStat->DoraFlag.Omote[i] && analysis->KeziCount[i])
						flag = true;
				return flag &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ÉtÉWÉeÉåÉr */
	if (RuleData::chkRuleApplied("fujitv"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ÉtÉWÉeÉåÉr", get_yaku_han("fujitv"),
			"éOêFìØçè",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->MianziDat[0].tile == CircleOne) &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1);
			}
		));
	/* ìåÉAÉWÉA */
	if (RuleData::chkRuleApplied("eastasia"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ìåÉAÉWÉA", get_yaku_han("eastasia"),
			"ñîvÅEíÜ", "ëŒÅXòa",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* ÉoÉbÉn */
	if (RuleData::chkRuleApplied("bach"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ÉoÉbÉn", get_yaku_han("bach"),
			"ìÒêFìØçè",
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				unsigned count = 0;
				for (int i = CircleOne; i <= CircleSeven; i++) {
					count += analysis->DuiziCount[i];
					count += analysis->ShunziCount[i];
				}
				count += analysis->DuiziCount[CircleNine];
				return (countKeziOf(analysis, 8) >= 2) &&
					(count >= 3);
			}
		));
	/* ñkìáéOòY */
	if (RuleData::chkRuleApplied("kitajima_saburoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ñkìáéOòY", get_yaku_han("kitajima_saburoh"),
			"ç¨àÍêF", "élãAàÍ",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool pairIsHonor = false; bool flag = false;
				for (int i = TILE_SUIT_HONORS; i < TILE_NONFLOWER_MAX; i++)
					if (analysis->MianziDat[0].tile == (tileCode)i)
						pairIsHonor = true;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP) {
					if ((analysis->KeziCount[i + 3] >= 1) &&
						(analysis->KeziCount[i + 6] >= 1) &&
						(analysis->KeziCount[NorthWind] >= 1) &&
						(pairIsHonor)) {
							if ((analysis->ShunziCount[i + 4] >= 1) &&
								(analysis->TsumoHai->tile == (tileCode)(i + 6)))
								flag = true;
							if ((analysis->ShunziCount[i + 3] >= 1) &&
								(analysis->TsumoHai->tile == (tileCode)(i + 3)))
								flag = true;
					}
				}
				return flag;
			}
		));
	/* ëÂäôÇÃéÄê_ */
	if (RuleData::chkRuleApplied("reaper"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ëÂäôÇÃéÄê_", get_yaku_han("reaper"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* âzíJ */
	if (RuleData::chkRuleApplied("koshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"âzíJ", get_yaku_han("koshigaya"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* ñkâzíJ */
	if (RuleData::chkRuleApplied("kitakoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ñkâzíJ", get_yaku_han("kitakoshigaya"),
			"âzíJ",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[NorthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* ìÏâzíJ */
	if (RuleData::chkRuleApplied("minamikoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ìÏâzíJ", get_yaku_han("minamikoshigaya"),
			"âzíJ",
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* TODO: òaómíÜññû */
	/* TODO: â[îNññû */
	/* TODO: Windows8 */
	/* TODO: êNó™ÉCÉJñ∫ */
	/* Ç†Ç∏Ç‹éıÇµ */
	if (RuleData::chkRuleApplied("azumazushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Ç†Ç∏Ç‹éıÇµ", get_yaku_han("azumazushi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == CharacterFour) ||
					(analysis->MianziDat[0].tile == CircleFour) ||
					(analysis->MianziDat[0].tile == BambooFour)) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* TODO: êºêVà‰ */
}
