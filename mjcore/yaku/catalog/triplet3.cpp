#include "../catalog.h"

#include "../../func.h"

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

	/* ƒCƒiƒoƒEƒA[ */
	if (RuleData::chkRuleApplied("inabauer"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ƒCƒiƒoƒEƒA["), get_yaku_han("inabauer"),
			_T("´ˆêF"), _T("‘ÎX˜a"), _T("‹Ø”v"),
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
	/* ŽlŒŽ”nŽ­ */
	if (RuleData::chkRuleApplied("april_fool")) {
		auto isApril1st =
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 4) && (nowTime.wDay == 1);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ŽlŒŽ”nŽ­"), get_yaku_han("april_fool"),
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
	/* ’†“Œ‚Ì“J */
	if (RuleData::chkRuleApplied("chuutounofue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("’†“Œ‚Ì“J"), get_yaku_han("chuutounofue"),
			_T("–ð”vE’†"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1));
			}
		));
	/* ŽO‘Žu */
	if (RuleData::chkRuleApplied("sangokushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ŽO‘Žu"), get_yaku_han("sangokushi"),
			_T("‘ÎX˜a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (bool)
					(countKeziOf(analysis, 3) * countKeziOf(analysis, 5) *
					countKeziOf(analysis, 9) * countKeziOf(analysis, 4));
			}
		));
	/* ˆê‹x‚³‚ñ */
	if (RuleData::chkRuleApplied("ikkyuusan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ˆê‹x‚³‚ñ"), get_yaku_han("ikkyuusan"),
			_T("‘ÎX˜a"),
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 1) > 0) &&
					(countDuiziOf(analysis, 9) > 0) &&
					(countDuiziOf(analysis, 3) > 0) &&
					(countDuiziOf(analysis, 1) + countDuiziOf(analysis, 9) +
					countDuiziOf(analysis, 3) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* ‘å–‚_ */
	if (RuleData::chkRuleApplied("daimajin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘å–‚_"), get_yaku_han("daimajin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKeziCount[WhiteDragon] +
					analysis->AnKeziCount[GreenDragon] +
					analysis->AnKeziCount[RedDragon]) >= 1) &&
					(analysis->TsumoHai->tile == BambooFive) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki);
			}
		));
	/* ¼’|”~ */
	if (RuleData::chkRuleApplied("shouchikubai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("¼’|”~"), get_yaku_han("shouchikubai"),
			_T("–ð”vEá¢"),
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
	/* ƒpƒ“ƒ_ */
	if (RuleData::chkRuleApplied("panda"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ƒpƒ“ƒ_"), get_yaku_han("panda"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* •ŠøŒR */
	if (RuleData::chkRuleApplied("kokkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("•ŠøŒR"), get_yaku_han("kokkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1);
			}
		));
	/* ”’ŠøŒR */
	if (RuleData::chkRuleApplied("hakkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("”’ŠøŒR"), get_yaku_han("hakkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* gŠøŒR */
	if (RuleData::chkRuleApplied("kohkigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("gŠøŒR"), get_yaku_han("kohkigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* —•ŠøŒR */
	if (RuleData::chkRuleApplied("rankigun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("—•ŠøŒR"), get_yaku_han("rankigun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* “Œ•—‰™’–ö */
	if (RuleData::chkRuleApplied("dongfeng_yan_jian_liu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“Œ•—‰™’–ö"), get_yaku_han("dongfeng_yan_jian_liu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* ‹´–{^–ç */
	if (RuleData::chkRuleApplied("hashimoto_shin_ya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‹´–{^–ç"), get_yaku_han("hashimoto_shin_ya"),
			_T("‘ÎX˜a"), _T("âˆê–å"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[CircleSeven] >= 1) &&
					(analysis->DuiziCount[BambooTwo] >= 1) &&
					(analysis->DuiziCount[BambooNine] >= 1);
			}
		));
	/* ‘å“ì¼”“‡ */
	if (RuleData::chkRuleApplied("dai_nansei_shotou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘å“ì¼”“‡"), get_yaku_han("dai_nansei_shotou"),
			_T("‘ÎX˜a"), _T("ŽOF“¯"), _T("¬˜V“ª"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1);
			}
		));
	/* ”’ˆß‚Ì“VŽg */
	if (RuleData::chkRuleApplied("hakuinotenshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("”’ˆß‚Ì“VŽg"), get_yaku_han("hakuinotenshi"),
			_T("–ð”vE”’"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 7) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(countKangziOf(analysis, 5) >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind] >= 1);
			}
		));
	/* Žµ—´Žì */
	if (RuleData::chkRuleApplied("dragon_ball"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Žµ—´Žì"), get_yaku_han("dragon_ball"),
			_T("‘ÎX˜a"), _T("‘åŽOŒ³"), _T("¬ˆêF"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CircleOne] - analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1);
			}
		));
	/* ŽµŒÜŽO */
	if (RuleData::chkRuleApplied("shichigosan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ŽµŒÜŽO"), get_yaku_han("shichigosan"),
			_T("Šï”‘ÎX˜a"), _T("Šï”Žµ‘ÎŽq"), _T("’f›ô‹ã"),
			[countTilesOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countTilesOf(analysis, 7) + countTilesOf(analysis, 5) + countTilesOf(analysis, 3) == NUM_OF_TILES_IN_HAND) &&
					(countTilesOf(analysis, 7) * countTilesOf(analysis, 5) * countTilesOf(analysis, 3));
			}
		));
	/* Œä‹ê˜J‚³‚ñ */
	if (RuleData::chkRuleApplied("gokurousan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Œä‹ê˜J‚³‚ñ"), get_yaku_han("gokurousan"),
			_T("‘ÎX˜a"), _T("‹Ø”v"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 5] >= 1) && (analysis->KeziCount[i + 9] >= 1) &&
						(analysis->KeziCount[i + 6] >= 1) && (analysis->KeziCount[i + 3] >= 1))
						flag = true;
				return flag;
			}
		));
	/* ƒjƒRƒjƒR–ð–ž */
	if (RuleData::chkRuleApplied("nikoniko_yakuman"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ƒjƒRƒjƒR–ð–ž"), get_yaku_han("nikoniko_yakuman"),
			_T("‘ÎX˜a"), _T("’f›ô‹ã"),
			[countDuiziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countDuiziOf(analysis, 2) + countDuiziOf(analysis, 5) == SIZE_OF_MELD_BUFFER);
			}
		));
	/* •—á•ó‘o•— */
	if (RuleData::chkRuleApplied("feng_xue_bao_shangfeng"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("•—á•ó‘o•—"), get_yaku_han("feng_xue_bao_shangfeng"),
			_T("‘ÎX˜a"), _T("¬˜V“ª"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (Wind2Tile(analysis->GameStat->GameRound / 4) != // ƒ_ƒu•—‚Å‚È‚­‚Ä
					Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))) &&
					(analysis->DuiziCount[Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))] >= 1) && // Ž©•—‚ª‚ ‚è
					(analysis->DuiziCount[Wind2Tile(analysis->GameStat->GameRound / 4)] >= 1) && // ê•—‚ª‚ ‚è
					(analysis->DuiziCount[CircleOne] >= 1) && (analysis->DuiziCount[BambooOne] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1);
			}
		));
	/* —y‚©”Þ•û */
	if (RuleData::chkRuleApplied("haruka_kanata"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("—y‚©”Þ•û"), get_yaku_han("haruka_kanata"),
			_T("–ð”vE’†"), _T("–ð”vEá¢"), _T("ŽOˆÃ"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->AnKeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[RedDragon] >= 1) &&
					(analysis->AnKeziCount[WhiteDragon] >= 1);
			}
		));
	/* ŒƒŽlˆÃ */
	if (RuleData::chkRuleApplied("geki_sianke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ŒƒŽlˆÃ"), get_yaku_han("geki_sianke"),
			_T("ŽlˆÃ"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) && (!*analysis->TsumoAgariFlag)) ?
					1 : 0) == 4) && // ŽlˆÃ‚ð
					(analysis->TsumoHai->tile == CharacterOne); // ˆêäÝ‚Å˜a—¹‚é
			}
		));
	/* Œƒ‘åŽOŒ³ */
	if (RuleData::chkRuleApplied("geki_daisangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Œƒ‘åŽOŒ³"), get_yaku_han("geki_daisangen"),
			_T("‘åŽOŒ³"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiPenchan);
			}
		));
	/* Œƒ–k */
	if (RuleData::chkRuleApplied("geki_north"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Œƒ–k"), get_yaku_han("geki_north"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* ‘å‹ã˜a */
	if (RuleData::chkRuleApplied("daikyuuhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘å‹ã˜a"), get_yaku_han("daikyuuhoh"),
			_T("ŽOF“¯"), _T("‘ÎX˜a"), _T("¬˜V“ª"), _T("ŒÜ–åÄ"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[WestWind] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1);
			}
		));
	/* ‘å—ÑŠÔ */
	if (RuleData::chkRuleApplied("dairinkan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘å—ÑŠÔ"), get_yaku_han("dairinkan"),
			_T("–ð”vE’†"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* “¡ŽqƒXƒyƒVƒƒƒ‹ */
	if (RuleData::chkRuleApplied("fujikospecial"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“¡ŽqƒXƒyƒVƒƒƒ‹"), get_yaku_han("fujikospecial"),
			_T("–ð”vE”’"),
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
	/* ƒtƒWƒeƒŒƒr */
	if (RuleData::chkRuleApplied("fujitv"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ƒtƒWƒeƒŒƒr"), get_yaku_han("fujitv"),
			_T("ŽOF“¯"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->MianziDat[0].tile == CircleOne) &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1);
			}
		));
	/* “ŒƒAƒWƒA */
	if (RuleData::chkRuleApplied("eastasia"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“ŒƒAƒWƒA"), get_yaku_han("eastasia"),
			_T("–ð”vE’†"), _T("‘ÎX˜a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
	/* ƒoƒbƒn */
	if (RuleData::chkRuleApplied("bach"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ƒoƒbƒn"), get_yaku_han("bach"),
			_T("“ñF“¯"),
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
	/* –k“‡ŽO˜Y */
	if (RuleData::chkRuleApplied("kitajima_saburoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("–k“‡ŽO˜Y"), get_yaku_han("kitajima_saburoh"),
			_T("¬ˆêF"), _T("Žl‹Aˆê"),
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
	/* ‘åŠ™‚ÌŽ€_ */
	if (RuleData::chkRuleApplied("reaper"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘åŠ™‚ÌŽ€_"), get_yaku_han("reaper"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* ‰z’J */
	if (RuleData::chkRuleApplied("koshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‰z’J"), get_yaku_han("koshigaya"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* –k‰z’J */
	if (RuleData::chkRuleApplied("kitakoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("–k‰z’J"), get_yaku_han("kitakoshigaya"),
			_T("‰z’J"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[NorthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* “ì‰z’J */
	if (RuleData::chkRuleApplied("minamikoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“ì‰z’J"), get_yaku_han("minamikoshigaya"),
			_T("‰z’J"),
			[countKeziOf, countKangziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(countKeziOf(analysis, 5) >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(countKangziOf(analysis, 8) >= 1);
			}
		));
	/* ˜a—m’†–ð–ž */
	if (RuleData::chkRuleApplied("wayouchuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("˜a—m’†–ð–ž"), get_yaku_han("wayouchuu"),
			_T("‘ÎX˜a"), _T("–ð”vE’†"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(countKeziOf(analysis, 4) >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ‰[”N–ð–ž */
	if (RuleData::chkRuleApplied("leap_year"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‰[”N–ð–ž"),
			yaku::yakuCalculator::Yaku::HANFUNC(
				[](const MENTSU_ANALYSIS* const analysis) -> yaku::yakuCalculator::Yaku::YAKU_HAN {
					SYSTEMTIME nowTime; GetLocalTime(&nowTime);
					if ((nowTime.wMonth == 2) && (nowTime.wDay == 29))
						return yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman;
					else return yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman;
				}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				bool isLeapYear = (nowTime.wYear % 400 == 0) || ((nowTime.wYear % 4 == 0) && (nowTime.wYear % 100 != 0));
				if (!isLeapYear) return false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - _T('0')) * TILE_SUIT_STEP + 2] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 2] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 9] >= 1))
						return true;
				return false;
			}
		));
	/* Windows8 */
	if (RuleData::chkRuleApplied("windows8"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Windows8"), get_yaku_han("windows8"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->TotalAnKezi - (
					((analysis->Machi == yaku::yakuCalculator::machiShanpon) &&
					(!*analysis->TsumoAgariFlag)) ? 1 : 0) >= 3);
			}
		));
	/* N—ªƒCƒJ–º */
	if (RuleData::chkRuleApplied("ikamusume"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("N—ªƒCƒJ–º"), get_yaku_han("ikamusume"),
			_T("–ð”vE”’"), _T("–ð”vEá¢"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKeziCount[EastWind] - analysis->AnKangziCount[EastWind] +
					analysis->AnKeziCount[SouthWind] - analysis->AnKangziCount[SouthWind] +
					analysis->AnKeziCount[WestWind] - analysis->AnKangziCount[WestWind] +
					analysis->AnKeziCount[NorthWind] - analysis->AnKangziCount[NorthWind]) >= 1) &&
					((analysis->AnKeziCount[WhiteDragon] - analysis->AnKangziCount[WhiteDragon]) >= 1) &&
					((analysis->AnKeziCount[GreenDragon] - analysis->AnKangziCount[GreenDragon]) >= 1);
			}
		));
	/* ‚ ‚¸‚ÜŽõ‚µ */
	if (RuleData::chkRuleApplied("azumazushi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‚ ‚¸‚ÜŽõ‚µ"), get_yaku_han("azumazushi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->MianziDat[0].tile == CharacterFour) ||
					(analysis->MianziDat[0].tile == CircleFour) ||
					(analysis->MianziDat[0].tile == BambooFour)) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	/* ¼Vˆä */
	if (RuleData::chkRuleApplied("nishiarai"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("¼Vˆä"), get_yaku_han("nishiarai"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					((analysis->TsumoHai->tile == CharacterOne) ||
					(analysis->TsumoHai->tile == CircleOne) ||
					(analysis->TsumoHai->tile == BambooOne) ||
					(analysis->TsumoHai->tile == CharacterTwo) ||
					(analysis->TsumoHai->tile == CircleTwo) ||
					(analysis->TsumoHai->tile == BambooTwo));
			}
		));
	/* ‹à„‘g */
	if (RuleData::chkRuleApplied("kongougumi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‹à„‘g"), get_yaku_han("kongougumi"),
			_T("‘ÎX˜a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (countKeziOf(analysis, 5) >= 2) &&
					(countKeziOf(analysis, 9) >= 1) &&
					(countKeziOf(analysis, 3) >= 1);
			}
		));
	/* “Œ“ú–{•œ‹» */
	if (RuleData::chkRuleApplied("eastjapan_fukkou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“Œ“ú–{•œ‹»"), get_yaku_han("eastjapan_fukkou"),
			_T("‘ÎX˜a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(countKeziOf(analysis, 2) >= 1) &&
					(countKeziOf(analysis, 5) >= 1);
			}
		));
	/* ³˜IŠÛ */
	if (RuleData::chkRuleApplied("seirogan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("³˜IŠÛ"), get_yaku_han("seirogan"),
			_T("–ð”vEá¢"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* Žé–å */
	if (RuleData::chkRuleApplied("suzakumon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Žé–å"), get_yaku_han("suzakumon"),
			_T("‘ÎX˜a"), _T("¬ˆêF"), _T("–ð”vE’†"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[BambooFour] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->MianziDat[0].tile == SouthWind);
			}
		));
	/* ‘S˜Z */
	if (RuleData::chkRuleApplied("quanliuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘S˜Z"), get_yaku_han("quanliuke"),
			_T("‘ÎX˜a"), _T("–ð”vEá¢"), _T("ŽOF“¯"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSix] >= 1) &&
					(analysis->KeziCount[CircleSix] >= 1) &&
					(analysis->KeziCount[BambooSix] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* ‘S‹è */
	if (RuleData::chkRuleApplied("quanjiuke"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘S‹è"), get_yaku_han("quanjiuke"),
			_T("‘ÎX˜a"), _T("–ð”vE’†"), _T("ŽOF“¯"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ”’’¹ŒÎ */
	if (RuleData::chkRuleApplied("swanlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("”’’¹ŒÎ"), get_yaku_han("swanlake"),
			_T("‘ÎX˜a"), _T("–ð”vE”’"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));
	/* •’¹ŒÎ */
	if (RuleData::chkRuleApplied("blackbirdlake"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("•’¹ŒÎ"), get_yaku_han("blackbirdlake"),
			_T("‘ÎX˜a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* ‘å“ú–{Žj */
	if (RuleData::chkRuleApplied("dainihonshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘å“ú–{Žj"), get_yaku_han("dainihonshi"),
			_T("‘ÎX˜a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1) &&
					(analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[BambooFour] >= 1);
			}
		));
	/* ‘å‰»‚Ì‰üV */
	if (RuleData::chkRuleApplied("taikanokaishin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("‘å‰»‚Ì‰üV"), get_yaku_han("taikanokaishin"),
			_T("‘ÎX˜a"), _T("–ð”vE’†"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag = false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - _T('0')) * TILE_SUIT_STEP + 6] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 4] >= 1) &&
						(analysis->KeziCount[((*k)[2] - _T('0')) * TILE_SUIT_STEP + 5] >= 1))
						flag = true;
				return flag &&
					(analysis->KeziCount[RedDragon] >= 1);
			}
		));
	/* ÂŽR‰¡–kŠs”’…ç«“Œé */
	if (RuleData::chkRuleApplied("song_youren"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ÂŽR‰¡–kŠs”’…ç«“Œé"), get_yaku_han("song_youren"),
			_T("‘ÎX˜a"), _T("–ð”vE”’"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[BambooThree] - analysis->AnKeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[NorthWind] - analysis->AnKeziCount[NorthWind] >= 1);
			}
		));
	/* —•ˆêF */
	if (RuleData::chkRuleApplied("lanyise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("—•ˆêF"), get_yaku_han("lanyise"),
			_T("‘ÎX˜a"), _T("¬ˆêF"), _T("¬ŽO•—"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1) &&
					((analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind]) >= (SIZE_OF_MELD_BUFFER - 2));
			}
		));
	/* “ì‘å’Ã”g */
	if (RuleData::chkRuleApplied("minami_ootsunami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“ì‘å’Ã”g"), get_yaku_han("minami_ootsunami"),
			_T("‘ÎX˜a"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1);
			}
		));
	/* ¼Žð‹è */
	if (RuleData::chkRuleApplied("xijiujiu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("¼Žð‹è"), get_yaku_han("xijiujiu"),
			_T("‘ÎX˜a"), _T("ŽOF“¯"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) &&
					(analysis->MianziDat[0].tile == BambooThree);
			}
		));
	/* “ï”gƒ~ƒiƒ~ */
	if (RuleData::chkRuleApplied("namba_minami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("“ï”gƒ~ƒiƒ~"), get_yaku_han("namba_minami"),
			_T("‘ÎX˜a"),
			[countKeziOf](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag1 = false; bool flag2 = false;
				for (auto k = parsedat_trichrome3.begin(); k != parsedat_trichrome3.end(); ++k)
					if ((analysis->KeziCount[((*k)[0] - _T('0')) * TILE_SUIT_STEP + 3] >= 1) &&
						(analysis->KeziCount[((*k)[1] - _T('0')) * TILE_SUIT_STEP + 7] >= 1) &&
						(analysis->KeziCount[((*k)[2] - _T('0')) * TILE_SUIT_STEP + 3] >= 1))
						flag1 = true;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if ((analysis->KeziCount[i + 8] >= 1) &&
						(analysis->TsumoHai->tile == (tileCode)(i + 8)))
						flag2 = true;
				return flag1 && flag2 &&
					(analysis->MianziDat[0].tile == SouthWind);
			}
		));
	/* Žµ_‹Y–k“l */
	if (RuleData::chkRuleApplied("qishen_xi_beidou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("Žµ_‹Y–k“l"), get_yaku_han("qishen_xi_beidou"),
			_T("‘ÎX˜a"), _T("ŽOF“¯"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
}
