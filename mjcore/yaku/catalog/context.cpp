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
	/* あとで */
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
}
