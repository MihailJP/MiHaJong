#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
	/* リーチ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"立直", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(analysis->PlayerStat->RichiFlag.RichiFlag)); // 立直している
		}
	));
	/* ダブリー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ダブル立直", yaku::yakuCalculator::Yaku::yval_2han_menzen,
		"立直",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
				(analysis->PlayerStat->RichiFlag.DoubleFlag)); // ダブル立直フラグが立っている
		}
	));
	if (getRule(RULE_OPEN_RIICHI) != 0) {
		/* プンリー */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"オープン立直", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			"立直",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // オープン立直フラグが立っている
			}
		));
		/* ダブプン */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ダブル開立直", yaku::yakuCalculator::Yaku::yval_3han_menzen,
			"立直", "ダブル立直", "オープン立直",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.DoubleFlag) && // ダブル立直フラグが立っている
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // オープン立直フラグが立っている
			}
		));
		/* プンリー放銃 */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"開立直栄和", yaku::yakuCalculator::Yaku::HANFUNC( [](const MENTSU_ANALYSIS* const analysis) {
				return (*analysis->MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(11, yaku::yakuCalculator::hanUnit::Han),
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN();
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.OpenFlag) && // オープン立直フラグが立っている
					(!*analysis->TsumoAgariFlag) && // ロン
					(analysis->GameStat->CurrentPlayer.Furikomi >= 0) &&
					(!analysis->GameStat->Player[analysis->GameStat->CurrentPlayer.Furikomi].RichiFlag.RichiFlag)); // 非リーチ者からの和了
			}
		));
	}
	/* 一発(方言では即ともいう) */
	if ((getRule(RULE_RIICHI_IPPATSU) != 1)&&(getRule(RULE_RIICHI_IPPATSU) != 3)) { // 一発が役にならないルールを除外
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一発", (getRule(RULE_RIICHI_IPPATSU) == 2) ?
			yaku::yakuCalculator::Yaku::yval_1han_menzen_dependent : /* 一発は縛りを満たさない(リャンシバでリー即のみの和了を認めない)ルール */
			yaku::yakuCalculator::Yaku::yval_1han_menzen, // リー即のみでも和了って良いルール
			/* 必ず立直と複合する */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.IppatsuFlag)); // 一発フラグが立っている
			}
		));
		/* 超一発(リーチ一発を下家から和了る) */
		if (getRule(RULE_CHOUIPPATSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"超一発", (getRule(RULE_RIICHI_IPPATSU) == 2) ?
				yaku::yakuCalculator::Yaku::yval_2han_menzen_dependent : /* 一発は縛りを満たさない(リャンシバでリー即のみの和了を認めない)ルール */
				yaku::yakuCalculator::Yaku::yval_2han_menzen, // リー即のみでも和了って良いルール
				"一発", /* 必ず立直と複合する */
				[](const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
						(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
						(analysis->PlayerStat->RichiFlag.IppatsuFlag) && // 一発フラグが立っている
						(!*analysis->TsumoAgariFlag) && // ロンである
						(
							(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sRight)) || // 下家から和了った
							((chkGameType(analysis->GameStat, SanmaT)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == 0) && (analysis->player == 2)) || // 三麻の特例
							((chkGameType(analysis->GameStat, Sanma4)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sOpposite)) &&
								(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sWest)) // 四人三麻の特例
						));
				}
			));
	}
	/* 鳥リーチ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"鳥リーチ", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		/* 立直は複合するらしい */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(analysis->PlayerStat->RichiFlag.RichiFlag)) // 立直している
			{
				for (int i = 1; i < analysis->PlayerStat->DiscardPointer; i++)
					if ((analysis->PlayerStat->Discard[i].tcode.tile == BambooOne) &&
						((analysis->PlayerStat->Discard[i].dstat == discardRiichi) ||
						(analysis->PlayerStat->Discard[i].dstat == discardRiichiTaken)))
						return true;
				return false;
			}
			else return false;
		}
	));

	// ---------------------------------------------------------------------

	/* 天和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"天和", (getRule(RULE_TENHOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : // 天和がダブル役満のルールか
		yaku::yakuCalculator::Yaku::yval_yakuman_menzen, // 普通に役満になるか
		"門前清自摸和",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sEast) && // 親である(東家として決め打ち……)
				(analysis->PlayerStat->FirstDrawFlag)); // 天和・地和フラグが立っている
		}
	));
	/* 地和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"地和", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"門前清自摸和",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) != sEast) && // 親である(東家として決め打ち……)
				(analysis->PlayerStat->FirstDrawFlag) && // 天和・地和フラグが立っている
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));
	/* ２位じゃ駄目なんですか？ */
	if (getRule(RULE_RENHOH) != 0) {
		yaku::yakuCalculator::Yaku::MenzenHan renhoh_han;
		switch (getRule(RULE_RENHOH)) {
			case 1: renhoh_han = yaku::yakuCalculator::Yaku::yval_yakuman_menzen; break; // 役満
			case 2: renhoh_han = yaku::yakuCalculator::Yaku::yval_baiman_menzen; break; // 倍満
			case 3: renhoh_han = yaku::yakuCalculator::Yaku::yval_mangan_menzen; break; // 満貫
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"人和", renhoh_han, "門前清自摸和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) != sEast) && // 親である(東家として決め打ち……)
					(analysis->PlayerStat->FirstDrawFlag) && // 天和・地和フラグが立っている
					(!*analysis->TsumoAgariFlag)); // ロンアガリ
			}
		));
	}
	/* ツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"門前清自摸和", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(*analysis->MenzenFlag) && // 門前である
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));

	// ---------------------------------------------------------------------

	/* 三隻転覆 */
	if (getRule(RULE_SANSEKI_TEMPUKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三隻転覆", yaku::yakuCalculator::Yaku::yval_3han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(!analysis->PlayerStat->RichiFlag.RichiFlag) && // 自分はリーチしていない
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.RichiFlag) // 他家全員立直
					);
			}
		));
	/* 起死回生 */
	if (getRule(RULE_KISHI_KAISEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"起死回生", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(!analysis->PlayerStat->RichiFlag.RichiFlag) && // 自分はリーチしていない
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.DoubleFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.DoubleFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.DoubleFlag) // 他家全員ダブリー(!)
					);
			}
		));

	// ---------------------------------------------------------------------

	/* リンシャンツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"嶺上開花", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));
	/* 明槓開花 */
	if ((getRule(RULE_MINKAN_PAO) >= 3) && (getRule(RULE_MINKAN_PAO) <= 5))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"明槓開花", yaku::yakuCalculator::Yaku::yval_2han,
			"嶺上開花",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->PaoFlag[pyMinkan].paoPlayer >= 0)); // 明槓の直後
			}
		));
	/* 連槓開花 */
	if (getRule(RULE_RENKAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"連槓開花", yaku::yakuCalculator::Yaku::yval_2han,
			"嶺上開花",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->KangFlag.chainFlag >= 2)); // 連続で槓した直後
			}
		));
	/* 五筒開花 */
	if (getRule(RULE_UUPIN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"五筒開花", (getRule(RULE_UUPIN_KAIHOH) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"嶺上開花",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->TsumoHai->tile == CircleFive)); // 和了牌が五筒
			}
		));
	/* 中上開花 */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中上開花", yaku::yakuCalculator::Yaku::yval_2han,
			"嶺上開花",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->TsumoHai->tile == RedDragon)); // 和了牌が中
			}
		));
	/* 頭槓和 */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"頭槓和", yaku::yakuCalculator::Yaku::yval_yakuman,
			"嶺上開花",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->KangFlag.topFlag)); // 頭槓和フラグが立っている
			}
		));
	/* サヨナラホームラン */
	if (getRule(RULE_SAYONARA_HOMERUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"サヨナラホームラン", yaku::yakuCalculator::Yaku::yval_3han,
			"嶺上開花",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(analysis->GameStat) == 0)); // 王牌を除いた残り山牌が0
			}
		));
	/* 東花園 */
	if (getRule(RULE_HIGASHI_HANAZONO_TRIPLETS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東花園", yaku::yakuCalculator::Yaku::yval_yakuman,
			"嶺上開花",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->GameRound / 4 == 0) && // 東場
					(playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound) == sEast) && // 東家
					(analysis->KangziCount[EastWind] >= 1)); // 東の槓子がある
			}
		));

	// ---------------------------------------------------------------------

	/* 海底ツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"海底摸月", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
				(*analysis->TsumoAgariFlag) && // ツモアガリ
				(tilesLeft(analysis->GameStat) == 0)); // ハイテイである
		}
	));
	/* 一筒摸月 */
	if (getRule(RULE_IIPIN_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一筒摸月", (getRule(RULE_IIPIN_MOYUE) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"海底摸月", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CircleOne)); // 和了牌が一筒
			}
		));
	/* 海底撈針 */
	if (getRule(RULE_HAITEI_RAOTSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"海底撈針", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CharacterOne)); // 和了牌が一萬
			}
		));
	/* 海底撈花 */
	if (getRule(RULE_HAITEI_RAOHWA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"海底撈花", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CircleFive)); // 和了牌が五筒
			}
		));
	/* 発底摸月 */
	if (getRule(RULE_HATSUTEI_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"発底摸月", yaku::yakuCalculator::Yaku::yval_2han,
			"海底摸月", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->TsumoHai->tile == GreenDragon)); // 和了牌が發
			}
		));
	/* 河底ロン */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"河底撈魚", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
				(!*analysis->TsumoAgariFlag) && // ロンアガリ
				(tilesLeft(analysis->GameStat) == 0)); // ハイテイである
		}
	));
	/* 九筒撈魚 */
	if (getRule(RULE_CHUUPIN_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"九筒撈魚", (getRule(RULE_CHUUPIN_RAOYUI) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"河底撈魚", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(!*analysis->TsumoAgariFlag) && // ロンアガリ
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CircleNine)); // 和了牌が九筒
			}
		));
	/* 白底撈魚 */
	if (getRule(RULE_HAKUTEI_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白底撈魚", yaku::yakuCalculator::Yaku::yval_2han,
			"河底撈魚", 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(!*analysis->TsumoAgariFlag) && // ロンアガリ
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->TsumoHai->tile == WhiteDragon)); // 和了牌が白
			}
		));
	/* 泥底撈蟲 */
	if (getRule(RULE_NITEI_RAOCHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"泥底撈蟲", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // 裸単騎である
			}
		));
	/* ラストオーダー麻婆豆腐 */
	if (getRule(RULE_MAABOODOUFU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ラストオーダー麻婆豆腐", yaku::yakuCalculator::Yaku::yval_yakuman,
			"白底撈魚",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->TsumoHai->tile == WhiteDragon)); // 和了牌が白
			}
		));
	/* 石の上にも三年 */
	if (getRule(RULE_THREE_YEARS_ON_STONE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"石の上にも三年", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ダブル立直", "ダブル開立直", "海底摸月", "河底撈魚",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(tilesLeft(analysis->GameStat) == 0) && // ハイテイである
					(analysis->PlayerStat->RichiFlag.DoubleFlag)); // ダブル立直している
			}
		));

	// ---------------------------------------------------------------------

	/* 槓振り */
	if (getRule(RULE_KAMBURI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"槓振り", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.chainFlag >= 1) && // 槓をした直後である
					(!*analysis->TsumoAgariFlag)); // ロンアガリ
			}
		));
	/* 槍槓は正確には木偏ではなく手偏 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"搶槓", yaku::yakuCalculator::Yaku::yval_1han,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(analysis->GameStat->KangFlag.chankanFlag)); // 槍槓フラグが立っている
		}
	));
	/* 二索搶槓 */
	if (getRule(RULE_RYANZOH_CHANKAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二索搶槓",  (getRule(RULE_RYANZOH_CHANKAN) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"搶槓",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.chankanFlag) && // 槍槓フラグが立っている
					(analysis->TsumoHai->tile == BambooTwo)); // 和了牌が二索
			}
		));
	/* 金鶏奪食 */
	if (getRule(RULE_JINJI_DUOSHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金鶏奪食", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.chankanFlag) && // 槍槓フラグが立っている
					((analysis->SeenTiles[analysis->TsumoHai->tile] +
					analysis->TileCount[analysis->TsumoHai->tile]) >=
					(*analysis->TsumoAgariFlag ? 4 : 5)) && // ラス牌かどうか判定
					(analysis->MachiInfo.MachiMen == 1) // 1面待ちである
					);
			}
		));

	// ---------------------------------------------------------------------

	/* 八連荘 */
	if (getRule(RULE_PAARENCHAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"八連荘", yaku::yakuCalculator::Yaku::yval_yakuman_dependent, // 移植時仕様変更：八連荘自体が縛りを満たさないようにする
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->AgariChain == 8)); // 和了ったのが連続8回目
			}
		));
	/* 八連殺し(パーレンブレイカー) */
	if (getRule(RULE_BREAKING_PAARENCHAN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"破回八連荘", yaku::yakuCalculator::Yaku::yval_yakuman_dependent, // いいぜ、てめえがこの局で八連荘を和了れるっていうなら
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // まずはその
					(analysis->GameStat->AgariChain == -1)); // ふざけた和了を頭ハネ(じゃなくてもいいけど)！
			}
		));

	// ---------------------------------------------------------------------

	/* 燕返し(立直宣言牌でロンすると役が付く) */
	if (getRule(RULE_TSUBAME_GAESHI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"燕返し", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->RichiCounter)); // フラグが立っている
			}
		));
	/* 飛燕(ダブリー宣言牌でロンすると役が付く) */
	if (getRule(RULE_HIEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"飛燕", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"燕返し",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->DoubleRichiCounter)); // フラグが立っている
			}
		));

	// ---------------------------------------------------------------------

	/* 四馬路 */
	if (getRule(RULE_SUMARO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"四馬路", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile == CharacterFour) && // 和了牌が四萬
					(!*analysis->TsumoAgariFlag) && // ロンアガリ
					(analysis->PlayerStat->SumaroFlag)); // 解禁フラグが立っている
			}
		));
	/* 美鈴 */
	if (getRule(RULE_MEILING) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"美鈴", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile == BambooTwo) && // 和了牌が二索
					(!*analysis->TsumoAgariFlag)); // ロンアガリ
			}
		));
	/* 客来和(オタ風の出和了) */
	if (getRule(RULE_OTAKAZE_RON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"客来和", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag;
				switch (tileCode tc = analysis->TsumoHai->tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind: // 風牌だった
					yakuFlag = (!*analysis->TsumoAgariFlag); // 仮にフラグ設定
					if (tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->player, analysis->GameStat->GameRound))) // 自風だった
						yakuFlag = false;
					if (tc == Wind2Tile(analysis->GameStat->GameRound / 4)) // 場風だった
							yakuFlag = false;
					if ((getRule(RULE_KAIMENKAZE) != 0) &&
						(tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, analysis->GameStat->WaremePlayer, analysis->GameStat->GameRound)))) // 開門風だった
							yakuFlag = false;
					if ((getRule(RULE_URAKAZE) != 0) &&
						(tc == Wind2Tile((uint8_t)playerwind(analysis->GameStat, RelativePositionOf(analysis->player, sOpposite), analysis->GameStat->GameRound)))) // 裏風だった
							yakuFlag = false;
					break;
				default: // 風牌じゃなかった
					yakuFlag = false; break;
				}
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 鳳還巣系列の役 */
	{
		const int Rules = 10;
		const RuleCode tmpRuleCodeList[Rules] = {
			RULE_HOUKANSOU, RULE_GEKKANJUN, RULE_SHINRIISHOU, RULE_KINKI_KIKAN, RULE_HOKUTO_SHOUKAN,
			RULE_DAIJA_KANKETSU, RULE_DONGFENGCHUI, RULE_NANFENGCHUI, RULE_XIFENGCHUI, RULE_BEIFENGCHUI,
		};
		const tileCode tmpTileCodeList[Rules] = {
			BambooOne, CircleOne, CharacterOne, BambooSeven, CircleSeven,
			CharacterSeven, EastWind, SouthWind, WestWind, NorthWind,
		};
		const char tmpYakuNameList[Rules][16] = {
			"鳳還巣", "月還巡", "針戻手", "金亀帰還", "北斗召還",
			"大蛇還穴", "東風吹", "南風吹", "西風吹", "北風吹",
		};
		for (int i = 0; i < Rules; i++) {
			if (getRule(tmpRuleCodeList[i]) != 0) {
				tileCode tc = tmpTileCodeList[i];
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					tmpYakuNameList[i], (getRule(tmpRuleCodeList[i]) == 2) ?
					yaku::yakuCalculator::Yaku::yval_2han : yaku::yakuCalculator::Yaku::yval_1han,
					[tc](const MENTSU_ANALYSIS* const analysis) -> bool {
						bool yakuFlag = false;
						for (int i = 0; i < analysis->PlayerStat->DiscardPointer; i++) {
							if (analysis->PlayerStat->Discard[i].tcode.tile == tc) {
								yakuFlag = true; break; // 現物フリテンである
							}
						}
						return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
							(analysis->TsumoHai->tile == tc) && // 和了牌
							(*analysis->TsumoAgariFlag) && (yakuFlag) && // フリテンツモ
							(analysis->Machi == yaku::yakuCalculator::machiTanki)); // 単騎待ちである
					}
				));
			}
		}
	}

	// ---------------------------------------------------------------------

	/* 金龍天和 */
	if (getRule(RULE_GOLDDRAGON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金龍天和", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile >= CircleOne) && // 和了牌が筒子
					(analysis->TsumoHai->tile <= CircleNine) &&
					(analysis->GameStat->TurnRound <= 3)); // 3巡目以内
			}
		));
	/* 旋風栄和 */
	if (getRule(RULE_WHIRLWIND) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"旋風栄和", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile >= EastWind) && // 和了牌が風牌
					(analysis->TsumoHai->tile <= NorthWind) &&
					(analysis->GameStat->TurnRound <= 3)); // 3巡目以内
			}
		));
	/* 青天の霹靂 */
	if (getRule(RULE_BOLT_FROM_THE_BLUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"青天の霹靂", yaku::yakuCalculator::Yaku::yval_4han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->Player[0].RichiFlag.RichiFlag) && // 誰もリーチしていない
					(!analysis->GameStat->Player[1].RichiFlag.RichiFlag) &&
					(!analysis->GameStat->Player[2].RichiFlag.RichiFlag) &&
					(!analysis->GameStat->Player[3].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[0].DiscardPointer == 0) && // 誰も鳴いていない
					(analysis->GameStat->Player[1].DiscardPointer == 0) &&
					(analysis->GameStat->Player[2].DiscardPointer == 0) &&
					(analysis->GameStat->Player[3].DiscardPointer == 0) &&
					(analysis->GameStat->TurnRound <= 4)); // 4巡目以内
			}
		));
	/* 猫まっしぐら */
	if (getRule(RULE_GOLDDRAGON) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"猫まっしぐら", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenRegular] == -1) && // 面子手で和了になっている
					(analysis->TsumoHai->tile == BambooThree) && // 和了牌が三索
					(analysis->Machi == yaku::yakuCalculator::machiKanchan)); // 嵌張待ち
			}
		));

}
