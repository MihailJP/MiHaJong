#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
	/* リーチ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"立直", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag)); // 立直している
		}
	));
	/* ダブリー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"ダブル立直", yaku::yakuCalculator::Yaku::yval_2han_menzen,
		"立直",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 立直している
				(gameStat->Player[analysis->player].RichiFlag.DoubleFlag)); // ダブル立直フラグが立っている
		}
	));
	if (getRule(RULE_OPEN_RIICHI) != 0) {
		/* プンリー */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"オープン立直", yaku::yakuCalculator::Yaku::yval_2han_menzen,
			"立直",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 立直している
					(gameStat->Player[analysis->player].RichiFlag.OpenFlag)); // オープン立直フラグが立っている
			}
		));
		/* ダブプン */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ダブル開立直", yaku::yakuCalculator::Yaku::yval_3han_menzen,
			"立直", "ダブル立直", "オープン立直",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 立直している
					(gameStat->Player[analysis->player].RichiFlag.DoubleFlag) && // ダブル立直フラグが立っている
					(gameStat->Player[analysis->player].RichiFlag.OpenFlag)); // オープン立直フラグが立っている
			}
		));
		/* プンリー放銃 */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"開立直栄和", yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
				return (gameStat->Player[analysis->player].MenzenFlag) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(11, yaku::yakuCalculator::hanUnit::Han),
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN();
			}),
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 立直している
					(gameStat->Player[analysis->player].RichiFlag.OpenFlag) && // オープン立直フラグが立っている
					(!gameStat->TsumoAgariFlag) && // ロン
					(gameStat->CurrentPlayer.Furikomi >= 0) &&
					(!gameStat->Player[gameStat->CurrentPlayer.Furikomi].RichiFlag.RichiFlag)); // 非リーチ者からの和了
			}
		));
	}
	/* 一発(方言では即ともいう) */
	if ((getRule(RULE_RIICHI_IPPATSU) != 1)&&(getRule(RULE_RIICHI_IPPATSU) != 3)) { // 一発が役にならないルールを除外
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一発",
			(getRule(RULE_RIICHI_IPPATSU) == 2) ? /* 一発は縛りを満たさない(リャンシバでリー即のみの和了を認めない)ルール */
				yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
					return (gameStat->Player[analysis->player].MenzenFlag) ?
						yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
						yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han) :
						yaku::yakuCalculator::Yaku::YAKU_HAN();
			}) : yaku::yakuCalculator::Yaku::yval_1han_menzen, // リー即のみでも和了って良いルール
			/* 必ず立直と複合する */
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 立直している
					(gameStat->Player[analysis->player].RichiFlag.IppatsuFlag)); // 一発フラグが立っている
			}
		));
		/* 超一発(リーチ一発を下家から和了る) */
		if (getRule(RULE_CHOUIPPATSU) != 0)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"超一発",
				(getRule(RULE_RIICHI_IPPATSU) == 2) ? /* 一発は縛りを満たさない(リャンシバでリー即のみの和了を認めない)ルール */
					yaku::yakuCalculator::Yaku::HANFUNC( [](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) {
						return (gameStat->Player[analysis->player].MenzenFlag) ?
							yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
							yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han) :
							yaku::yakuCalculator::Yaku::YAKU_HAN();
					}) : yaku::yakuCalculator::Yaku::yval_2han_menzen, // リー即のみでも和了って良いルール
				"一発", /* 必ず立直と複合する */
				[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
						(gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 立直している
						(gameStat->Player[analysis->player].RichiFlag.IppatsuFlag) && // 一発フラグが立っている
						(!gameStat->TsumoAgariFlag) && // ロンである
						(
							(gameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sRight)) || // 下家から和了った
							((chkGameType(gameStat, SanmaT)) &&
								(gameStat->CurrentPlayer.Furikomi == 0) && (analysis->player == 2)) || // 三麻の特例
							((chkGameType(gameStat, Sanma4)) &&
								(gameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, sOpposite)) &&
								(playerwind(gameStat, analysis->player, gameStat->GameRound) == sWest)) // 四人三麻の特例
						));
				}
			));
	}
	/* 鳥リーチ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"鳥リーチ", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		/* 立直は複合するらしい */
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			if ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(gameStat->Player[analysis->player].RichiFlag.RichiFlag)) // 立直している
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
	/* 天和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"天和", (getRule(RULE_TENHOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : // 天和がダブル役満のルールか
		yaku::yakuCalculator::Yaku::yval_yakuman_menzen, // 普通に役満になるか
		"門前清自摸和",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(playerwind(gameStat, analysis->player, gameStat->GameRound) == sEast) && // 親である(東家として決め打ち……)
				(gameStat->Player[analysis->player].FirstDrawFlag)); // 天和・地和フラグが立っている
		}
	));
	/* 地和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"地和", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"門前清自摸和",
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(playerwind(gameStat, analysis->player, gameStat->GameRound) != sEast) && // 親である(東家として決め打ち……)
				(gameStat->Player[analysis->player].FirstDrawFlag) && // 天和・地和フラグが立っている
				(gameStat->TsumoAgariFlag)); // ツモアガリ
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
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(playerwind(gameStat, analysis->player, gameStat->GameRound) != sEast) && // 親である(東家として決め打ち……)
					(gameStat->Player[analysis->player].FirstDrawFlag) && // 天和・地和フラグが立っている
					(!gameStat->TsumoAgariFlag)); // ロンアガリ
			}
		));
	}
	/* ツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"門前清自摸和", yaku::yakuCalculator::Yaku::yval_1han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(gameStat->Player[analysis->player].MenzenFlag) && // 門前である
				(gameStat->TsumoAgariFlag)); // ツモアガリ
		}
	));
	/* 三隻転覆 */
	if (getRule(RULE_SANSEKI_TEMPUKU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三隻転覆", yaku::yakuCalculator::Yaku::yval_3han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(!gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 自分はリーチしていない
					(gameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.RichiFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.RichiFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.RichiFlag) // 他家全員立直
					);
			}
		));
	/* 起死回生 */
	if (getRule(RULE_KISHI_KAISEI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"起死回生", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(!gameStat->Player[analysis->player].RichiFlag.RichiFlag) && // 自分はリーチしていない
					(gameStat->Player[RelativePositionOf(analysis->player, sRight)].RichiFlag.DoubleFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sOpposite)].RichiFlag.DoubleFlag) &&
					(gameStat->Player[RelativePositionOf(analysis->player, sLeft)].RichiFlag.DoubleFlag) // 他家全員ダブリー(!)
					);
			}
		));
	/* リンシャンツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"嶺上開花", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(gameStat->KangFlag.kangFlag) && // 槓をした直後である
				(gameStat->TsumoAgariFlag)); // ツモアガリ
		}
	));
	/* 明槓開花 */
	if ((getRule(RULE_MINKAN_PAO) >= 3) && (getRule(RULE_MINKAN_PAO) <= 5))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"明槓開花", yaku::yakuCalculator::Yaku::yval_2han,
			"嶺上開花",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->KangFlag.kangFlag) && // 槓をした直後である
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(gameStat->PaoFlag[pyMinkan].paoPlayer >= 0)); // 明槓の直後
			}
		));
	/* 連槓開花 */
	if (getRule(RULE_RENKAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"連槓開花", yaku::yakuCalculator::Yaku::yval_2han,
			"嶺上開花",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->KangFlag.kangFlag) && // 槓をした直後である
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(gameStat->KangFlag.chainFlag >= 2)); // 連続で槓した直後
			}
		));
	/* 五筒開花 */
	if (getRule(RULE_UUPIN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"五筒開花", (getRule(RULE_UUPIN_KAIHOH) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"嶺上開花",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->KangFlag.kangFlag) && // 槓をした直後である
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleFive)); // 和了牌が五筒
			}
		));
	/* 中上開花 */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中上開花", yaku::yakuCalculator::Yaku::yval_2han,
			"嶺上開花",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->KangFlag.kangFlag) && // 槓をした直後である
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == RedDragon)); // 和了牌が中
			}
		));
	/* 頭槓和 */
	if (getRule(RULE_CHUNSHAN_KAIHOH) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"頭槓和", yaku::yakuCalculator::Yaku::yval_yakuman,
			"嶺上開花",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->KangFlag.kangFlag) && // 槓をした直後である
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(gameStat->KangFlag.topFlag)); // 頭槓和フラグが立っている
			}
		));
	/* サヨナラホームラン */
	if (getRule(RULE_SAYONARA_HOMERUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"サヨナラホームラン", yaku::yakuCalculator::Yaku::yval_3han,
			"嶺上開花",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->KangFlag.kangFlag) && // 槓をした直後である
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(gameStat) == 0)); // 王牌を除いた残り山牌が0
			}
		));
	/* 東花園 */
	if (getRule(RULE_HIGASHI_HANAZONO_TRIPLETS) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東花園", yaku::yakuCalculator::Yaku::yval_yakuman,
			"嶺上開花",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(gameStat->KangFlag.kangFlag) && // 槓をした直後である
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(gameStat->GameRound / 4 == 0) && // 東場
					(playerwind(gameStat, analysis->player, gameStat->GameRound) == sEast) && // 東家
					(analysis->KangziCount[EastWind] >= 1)); // 東の槓子がある
			}
		));
	/* 海底ツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"海底摸月", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
				(gameStat->TsumoAgariFlag) && // ツモアガリ
				(tilesLeft(gameStat) == 0)); // ハイテイである
		}
	));
	/* 一筒摸月 */
	if (getRule(RULE_IIPIN_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一筒摸月", (getRule(RULE_IIPIN_MOYUE) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"海底摸月", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleOne)); // 和了牌が一筒
			}
		));
	/* 海底撈針 */
	if (getRule(RULE_HAITEI_RAOTSEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"海底撈針", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CharacterOne)); // 和了牌が一萬
			}
		));
	/* 海底撈花 */
	if (getRule(RULE_HAITEI_RAOHWA) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"海底撈花", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleFive)); // 和了牌が五筒
			}
		));
	/* 発底摸月 */
	if (getRule(RULE_HATSUTEI_MOYUE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"発底摸月", yaku::yakuCalculator::Yaku::yval_2han,
			"海底摸月", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(gameStat->TsumoAgariFlag) && // ツモアガリ
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == GreenDragon)); // 和了牌が發
			}
		));
	/* 河底ロン */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"河底撈魚", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
				(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
				(!gameStat->TsumoAgariFlag) && // ロンアガリ
				(tilesLeft(gameStat) == 0)); // ハイテイである
		}
	));
	/* 九筒撈魚 */
	if (getRule(RULE_CHUUPIN_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"九筒撈魚", (getRule(RULE_CHUUPIN_RAOYUI) == 1) ?
			yaku::yakuCalculator::Yaku::yval_yakuman : yaku::yakuCalculator::Yaku::yval_4han,
			"河底撈魚", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(!gameStat->TsumoAgariFlag) && // ロンアガリ
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == CircleNine)); // 和了牌が九筒
			}
		));
	/* 白底撈魚 */
	if (getRule(RULE_HAKUTEI_RAOYUI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白底撈魚", yaku::yakuCalculator::Yaku::yval_2han,
			"河底撈魚", 
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(!gameStat->TsumoAgariFlag) && // ロンアガリ
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == WhiteDragon)); // 和了牌が白
			}
		));
	/* 泥底撈蟲 */
	if (getRule(RULE_NITEI_RAOCHUN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"泥底撈蟲", yaku::yakuCalculator::Yaku::yval_1han,
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(tilesLeft(gameStat) == 0) && // ハイテイである
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // 裸単騎である
			}
		));
	/* ラストオーダー麻婆豆腐 */
	if (getRule(RULE_MAABOODOUFU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ラストオーダー麻婆豆腐", yaku::yakuCalculator::Yaku::yval_yakuman,
			"白底撈魚",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile == WhiteDragon)); // 和了牌が白
			}
		));
	/* 石の上にも三年 */
	if (getRule(RULE_THREE_YEARS_ON_STONE) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"石の上にも三年", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ダブル立直", "ダブル開立直", "海底摸月", "河底撈魚",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenAll] == -1) && // 何かの手で和了になっている
					(!gameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(tilesLeft(gameStat) == 0) && // ハイテイである
					(gameStat->Player[analysis->player].RichiFlag.DoubleFlag)); // ダブル立直している
			}
		));
}
