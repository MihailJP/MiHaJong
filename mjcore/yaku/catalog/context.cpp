#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
	/* ƒŠ[ƒ` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"—§’¼", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag)); // —§’¼‚µ‚Ä‚¢‚é
		}
	));
	/* ƒ_ƒuƒŠ[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ƒ_ƒuƒ‹—§’¼", yaku::yakuCalculator::Yaku::yval_2han_menzen,
		"—§’¼",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // —§’¼‚µ‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.DoubleFlag)); // ƒ_ƒuƒ‹—§’¼ƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
		}
	));
	/* ƒvƒ“ƒŠ[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ƒI[ƒvƒ“—§’¼", yaku::yakuCalculator::Yaku::yval_2han_menzen,
		"—§’¼",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // —§’¼‚µ‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.OpenFlag)); // ƒI[ƒvƒ“—§’¼ƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
		}
	));
	/* ƒ_ƒuƒvƒ“ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ƒ_ƒuƒ‹ŠJ—§’¼", yaku::yakuCalculator::Yaku::yval_3han_menzen,
		"—§’¼", "ƒ_ƒuƒ‹—§’¼", "ƒI[ƒvƒ“—§’¼",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // —§’¼‚µ‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.DoubleFlag) && // ƒ_ƒuƒ‹—§’¼ƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.OpenFlag)); // ƒI[ƒvƒ“—§’¼ƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
		}
	));
	/* ƒvƒ“ƒŠ[•úe */
	/* ‚ ‚Æ‚Å */
	/* ˆê”­(•ûŒ¾‚Å‚Í‘¦‚Æ‚à‚¢‚¤) */
	if ((getRule(RULE_RIICHI_IPPATSU) != 1)&&(getRule(RULE_RIICHI_IPPATSU) != 3)) { // ˆê”­‚ª–ð‚É‚È‚ç‚È‚¢ƒ‹[ƒ‹‚ðœŠO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ˆê”­",
			(getRule(RULE_RIICHI_IPPATSU) == 2) ? /* ˆê”­‚Í”›‚è‚ð–ž‚½‚³‚È‚¢(ƒŠƒƒƒ“ƒVƒo‚ÅƒŠ[‘¦‚Ì‚Ý‚Ì˜a—¹‚ð”F‚ß‚È‚¢)ƒ‹[ƒ‹ */
				yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
					return (gameStat->Player[analysis->player].MenzenFlag) ?
						yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
						yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han) :
						yaku::yakuCalculator::Yaku::YAKU_HAN();
			}) : yaku::yakuCalculator::Yaku::yval_1han_menzen, // ƒŠ[‘¦‚Ì‚Ý‚Å‚à˜a—¹‚Á‚Ä—Ç‚¢ƒ‹[ƒ‹
			/* •K‚¸—§’¼‚Æ•¡‡‚·‚é */
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // —§’¼‚µ‚Ä‚¢‚é
					(gameStat->Player[analysis->player].RichiFlag.IppatsuFlag)); // ˆê”­ƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
			}
		));
		/* ’´ˆê”­(ƒŠ[ƒ`ˆê”­‚ð‰º‰Æ‚©‚ç˜a—¹‚é) */
		if (getRule(RULE_CHOUIPPATSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"’´ˆê”­",
				(getRule(RULE_RIICHI_IPPATSU) == 2) ? /* ˆê”­‚Í”›‚è‚ð–ž‚½‚³‚È‚¢(ƒŠƒƒƒ“ƒVƒo‚ÅƒŠ[‘¦‚Ì‚Ý‚Ì˜a—¹‚ð”F‚ß‚È‚¢)ƒ‹[ƒ‹ */
					yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
						return (gameStat->Player[analysis->player].MenzenFlag) ?
							yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
							yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han) :
							yaku::yakuCalculator::Yaku::YAKU_HAN();
					}) : yaku::yakuCalculator::Yaku::yval_2han_menzen, // ƒŠ[‘¦‚Ì‚Ý‚Å‚à˜a—¹‚Á‚Ä—Ç‚¢ƒ‹[ƒ‹
				"ˆê”­", /* •K‚¸—§’¼‚Æ•¡‡‚·‚é */
				[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
						(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // —§’¼‚µ‚Ä‚¢‚é
						(gameStat->Player[analysis->player].RichiFlag.IppatsuFlag) && // ˆê”­ƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
						(!gameStat->TsumoAgariFlag) && // ƒƒ“‚Å‚ ‚é
						(
							(gameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sRight)) || // ‰º‰Æ‚©‚ç˜a—¹‚Á‚½
							((chkGameType(gameStat, SanmaT)) &&
								(gameStat->CurrentPlayer.Furikomi == 0) && (analysis->player == 2)) || // ŽO–ƒ‚Ì“Á—á
							((chkGameType(gameStat, Sanma4)) &&
								(gameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sOpposite)) &&
								(playerwind(gameStat, analysis->player, gameStat->GameRound) == sWest)) // ŽllŽO–ƒ‚Ì“Á—á
						));
				}
			));
	}
	/* ’¹ƒŠ[ƒ` */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"’¹ƒŠ[ƒ`", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		/* —§’¼‚Í•¡‡‚·‚é‚ç‚µ‚¢ */
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag)) // —§’¼‚µ‚Ä‚¢‚é
			{
				for (int i = 1; i < gameStat->Player[analysis->player].DiscardPointer; i++)
					if ((gameStat->Player[analysis->player].Discard[i].tcode.tile == BambooOne) &&
						((gameStat->Player[analysis->player].Discard[i].dstat == discardRiichi) ||
						(gameStat->Player[analysis->player].Discard[i].dstat == discardRiichiTaken)))
						return true;
				return false;
			}
			else return false;
		}
	));
	/* “V˜a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"“V˜a", (getRule(RULE_TENHOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : // “V˜a‚ªƒ_ƒuƒ‹–ð–ž‚Ìƒ‹[ƒ‹‚©
		yaku::yakuCalculator::Yaku::yval_yakuman_menzen, // •’Ê‚É–ð–ž‚É‚È‚é‚©
		"–å‘O´Ž©–Ì˜a",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(playerwind(gameStat, analysis->player, gameStat->GameRound) == sEast) && // e‚Å‚ ‚é(“Œ‰Æ‚Æ‚µ‚ÄŒˆ‚ß‘Å‚¿cc)
				(gameStat->Player[analysis->player].FirstDrawFlag)); // “V˜aE’n˜aƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
		}
	));
	/* ’n˜a */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"’n˜a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"–å‘O´Ž©–Ì˜a",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(playerwind(gameStat, analysis->player, gameStat->GameRound) != sEast) && // e‚Å‚ ‚é(“Œ‰Æ‚Æ‚µ‚ÄŒˆ‚ß‘Å‚¿cc)
				(gameStat->Player[analysis->player].FirstDrawFlag) && // “V˜aE’n˜aƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
				(gameStat->TsumoAgariFlag)); // ƒcƒ‚ƒAƒKƒŠ
		}
	));
	/* ‚QˆÊ‚¶‚á‘Ê–Ú‚È‚ñ‚Å‚·‚©H */
	if (getRule(RULE_RENHOH) != 0) {
		yaku::yakuCalculator::Yaku::MenzenHan renhoh_han;
		switch (getRule(RULE_RENHOH)) {
			case 1: renhoh_han = yaku::yakuCalculator::Yaku::yval_yakuman_menzen; break; // –ð–ž
			case 2: renhoh_han = yaku::yakuCalculator::Yaku::yval_baiman_menzen; break; // ”{–ž
			case 3: renhoh_han = yaku::yakuCalculator::Yaku::yval_mangan_menzen; break; // –žŠÑ
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"l˜a", renhoh_han, "–å‘O´Ž©–Ì˜a",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(playerwind(gameStat, analysis->player, gameStat->GameRound) != sEast) && // e‚Å‚ ‚é(“Œ‰Æ‚Æ‚µ‚ÄŒˆ‚ß‘Å‚¿cc)
					(gameStat->Player[analysis->player].FirstDrawFlag) && // “V˜aE’n˜aƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
					(!gameStat->TsumoAgariFlag)); // ƒƒ“ƒAƒKƒŠ
			}
		));
	}
	/* ƒcƒ‚ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"–å‘O´Ž©–Ì˜a", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(gameStat->Player[analysis->player].MenzenFlag) && // –å‘O‚Å‚ ‚é
				(gameStat->TsumoAgariFlag)); // ƒcƒ‚ƒAƒKƒŠ
		}
	));
	/* ŽOÇ“]•¢ */
	/* ‹NŽ€‰ñ¶ */
	/* ƒŠƒ“ƒVƒƒƒ“ƒcƒ‚ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"—äãŠJ‰Ô", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
				(gameStat->TsumoAgariFlag)); // ƒcƒ‚ƒAƒKƒŠ
		}
	));
	/* –¾žÈŠJ‰Ô */
	if ((getRule(RULE_MINKAN_PAO) >= 3) && (getRule(RULE_MINKAN_PAO) <= 5))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"–¾žÈŠJ‰Ô", yaku::yakuCalculator::Yaku::yval_2han,
			"—äãŠJ‰Ô",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(gameStat->PaoFlag[pyMinkan].paoPlayer >= 0)); // –¾žÈ‚Ì’¼Œã
			}
		));
	/* ˜AžÈŠJ‰Ô */
	if (getRule(RULE_RENKAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"˜AžÈŠJ‰Ô", yaku::yakuCalculator::Yaku::yval_2han,
			"—äãŠJ‰Ô",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(gameStat->KangFlag.chainFlag >= 2)); // ˜A‘±‚ÅžÈ‚µ‚½’¼Œã
			}
		));
	/* ŒÜ“›ŠJ‰Ô */
	if (getRule(RULE_UUPIN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŒÜ“›ŠJ‰Ô", (getRule(RULE_UUPIN_KAIHOH) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"—äãŠJ‰Ô",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleFive)); // ˜a—¹”v‚ªŒÜ“›
			}
		));
	/* ’†ãŠJ‰Ô */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"’†ãŠJ‰Ô", yaku::yakuCalculator::Yaku::yval_2han,
			"—äãŠJ‰Ô",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == RedDragon)); // ˜a—¹”v‚ª’†
			}
		));
	/* “ªžÈ˜a */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“ªžÈ˜a", yaku::yakuCalculator::Yaku::yval_yakuman,
			"—äãŠJ‰Ô",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(gameStat->KangFlag.topFlag)); // “ªžÈ˜aƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é
			}
		));
	/* ƒTƒˆƒiƒ‰ƒz[ƒ€ƒ‰ƒ“ */
	if (getRule(RULE_SAYONARA_HOMERUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ƒTƒˆƒiƒ‰ƒz[ƒ€ƒ‰ƒ“", yaku::yakuCalculator::Yaku::yval_3han,
			"—äãŠJ‰Ô",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(tilesLeft(gameStat) == 0)); // ‰¤”v‚ðœ‚¢‚½Žc‚èŽR”v‚ª0
			}
		));
	/* “Œ‰Ô‰€ */
	if (getRule(RULE_HIGASHI_HANAZONO_TRIPLETS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"“Œ‰Ô‰€", yaku::yakuCalculator::Yaku::yval_yakuman,
			"—äãŠJ‰Ô",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚ ‚é
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(gameStat->GameRound / 4 == 0) && // “Œê
					(playerwind(gameStat, analysis->player, gameStat->GameRound) == sEast) && // “Œ‰Æ
					(analysis->KangziCount[EastWind] >= 1)); // “Œ‚ÌžÈŽq‚ª‚ ‚é
			}
		));
	/* ŠC’êƒcƒ‚ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ŠC’ê–ÌŒŽ", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
				(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
				(tilesLeft(gameStat) == 0)); // ƒnƒCƒeƒC‚Å‚ ‚é
		}
	));
	/* ˆê“›–ÌŒŽ */
	if (getRule(RULE_IIPIN_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ˆê“›–ÌŒŽ", (getRule(RULE_IIPIN_MOYUE) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"ŠC’ê–ÌŒŽ", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(tilesLeft(gameStat) == 0) && // ƒnƒCƒeƒC‚Å‚ ‚é
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleOne)); // ˜a—¹”v‚ªˆê“›
			}
		));
	/* ŠC’êj */
	if (getRule(RULE_HAITEI_RAOTSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŠC’êj", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(tilesLeft(gameStat) == 0) && // ƒnƒCƒeƒC‚Å‚ ‚é
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CharacterOne)); // ˜a—¹”v‚ªˆêäÝ
			}
		));
	/* ŠC’ê‰Ô */
	if (getRule(RULE_HAITEI_RAOHWA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ŠC’ê‰Ô", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(tilesLeft(gameStat) == 0) && // ƒnƒCƒeƒC‚Å‚ ‚é
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleFive)); // ˜a—¹”v‚ªŒÜ“›
			}
		));
	/* ”­’ê–ÌŒŽ */
	if (getRule(RULE_HATSUTEI_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”­’ê–ÌŒŽ", yaku::yakuCalculator::Yaku::yval_2han,
			"ŠC’ê–ÌŒŽ", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
					(gameStat->TsumoAgariFlag) && // ƒcƒ‚ƒAƒKƒŠ
					(tilesLeft(gameStat) == 0) && // ƒnƒCƒeƒC‚Å‚ ‚é
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == GreenDragon)); // ˜a—¹”v‚ªá¢
			}
		));
	/* ‰Í’êƒƒ“ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"‰Í’ê‹›", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
				(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
				(!gameStat->TsumoAgariFlag) && // ƒƒ“ƒAƒKƒŠ
				(tilesLeft(gameStat) == 0)); // ƒnƒCƒeƒC‚Å‚ ‚é
		}
	));
	/* ‹ã“›‹› */
	if (getRule(RULE_CHUUPIN_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"‹ã“›‹›", (getRule(RULE_CHUUPIN_RAOYUI) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"‰Í’ê‹›", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
					(!gameStat->TsumoAgariFlag) && // ƒƒ“ƒAƒKƒŠ
					(tilesLeft(gameStat) == 0) && // ƒnƒCƒeƒC‚Å‚ ‚é
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleNine)); // ˜a—¹”v‚ª‹ã“›
			}
		));
	/* ”’’ê‹› */
	if (getRule(RULE_HAKUTEI_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"”’’ê‹›", yaku::yakuCalculator::Yaku::yval_2han,
			"‰Í’ê‹›", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // ‰½‚©‚ÌŽè‚Å˜a—¹‚É‚È‚Á‚Ä‚¢‚é
					(!gameStat->KangFlag.kangFlag) && // žÈ‚ð‚µ‚½’¼Œã‚Å‚Í‚È‚¢
					(!gameStat->TsumoAgariFlag) && // ƒƒ“ƒAƒKƒŠ
					(tilesLeft(gameStat) == 0) && // ƒnƒCƒeƒC‚Å‚ ‚é
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == WhiteDragon)); // ˜a—¹”v‚ª”’
			}
		));
}
