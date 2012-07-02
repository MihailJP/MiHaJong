#include "tileutil.h"

/* 理牌する */
void lipai(GameTable* gameStat, PLAYER_ID targetPlayer) {
	// 理牌する

	std::ostringstream o;
	o.str(""); o << "理牌を行います。プレイヤー [" << (int)targetPlayer << "]"; debug(o.str());

	/* 手牌データをデバッグログに出力：ビフォー */
	o.str(""); o << "理牌前の手牌 [";
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++)
		o << std::setw(2) << std::setfill('0') << (int)gameStat->Player[targetPlayer].Hand[i].tile
		<< ((i < (NUM_OF_TILES_IN_HAND - 1)) ? " " : "");
	o << "]"; trace(o.str());

	/* ソートの準備として牌のない枠を処理 */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == NoTile) {
			gameStat->Player[targetPlayer].Hand[i].tile = (tileCode)999;
			gameStat->Player[targetPlayer].Hand[i].red = 0;
		}
	}

	/* ここからソート作業 */
	TILE tmpTile;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		for (int j = i + 1; j < NUM_OF_TILES_IN_HAND; j++) {
			if (gameStat->Player[targetPlayer].Hand[i].tile >
				gameStat->Player[targetPlayer].Hand[j].tile) {
					/* TILE構造体はPODだから、memcpyしちゃってもいいよね？ */
					memcpy(&tmpTile, &(gameStat->Player[targetPlayer].Hand[i]), sizeof(TILE));
					memcpy( &(gameStat->Player[targetPlayer].Hand[i]),
						&(gameStat->Player[targetPlayer].Hand[j]), sizeof(TILE));
					memcpy(&(gameStat->Player[targetPlayer].Hand[j]), &tmpTile, sizeof(TILE));
			}
			else if ((gameStat->Player[targetPlayer].Hand[i].tile ==
				gameStat->Player[targetPlayer].Hand[j].tile) &&
				(gameStat->Player[targetPlayer].Hand[i].red <
				gameStat->Player[targetPlayer].Hand[j].red)) {
					/* 同じ種類の牌でも赤ドラかそうでないかで並べ替える */
					memcpy(&tmpTile, &(gameStat->Player[targetPlayer].Hand[i]), sizeof(TILE));
					memcpy( &(gameStat->Player[targetPlayer].Hand[i]),
						&(gameStat->Player[targetPlayer].Hand[j]), sizeof(TILE));
					memcpy(&(gameStat->Player[targetPlayer].Hand[j]), &tmpTile, sizeof(TILE));
			}
		}
	}

	/* 空欄だったところは元に戻してあげましょう */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == (tileCode)999) {
			gameStat->Player[targetPlayer].Hand[i].tile = NoTile;
			gameStat->Player[targetPlayer].Hand[i].red = 0;
		}
	}

	/* 手牌データをデバッグログに出力：アフター */
	o.str(""); o << "理牌後の手牌 [";
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++)
		o << std::setw(2) << std::setfill('0') << (int)gameStat->Player[targetPlayer].Hand[i].tile
		<< ((i < (NUM_OF_TILES_IN_HAND - 1)) ? " " : "");
	o << "]"; trace(o.str());

	/* 理牌完了！ */
	return;
}
__declspec(dllexport) void lipai(GameTable* gameStat, int targetPlayer) {
	// 理牌する(HSP互換用)
	lipai(gameStat, (PLAYER_ID)targetPlayer);
}

/* 場に見えてる個数 */
/* あとで書く */
MJCORE void countseentiles(Int8ByTile* seenTiles, GameTable* gameStat) {
	// 場に見えている牌の数を数える

	
}
#if 0
/* 場に見えてる個数 */
#module
#include "const.hsp"
#include "mjcore.hsp"
#deffunc countseentiles array SeenTiles, var GameStat
/*
		countseentiles p1, p2
		場に見えている牌の数を数える

		p1 : 結果を格納する配列
		p2 : 卓の状況を格納した構造体

		それぞれの牌が場に見えている数を数えます。
*/
	dim SeenTiles, TILE_CODE_MAXIMUM // 計算する牌
	// 捨牌で見えてる枚数
	repeat NUM_OF_PLAYERS: tmp = cnt
		if (DiscardPointer(GameStat, tmp) == 0) {continue}
		repeat DiscardPointer(GameStat, tmp), 1
			SeenTiles(getDiscard(GameStat, DISCARD_TILECODE, cnt, tmp))++
		loop
	loop
	// 誰かの副露で見えてる枚数
	repeat NUM_OF_PLAYERS: tmp = cnt
		if (MeldPointer(GameStat, tmp) == 0) {continue}
		repeat MeldPointer(GameStat, tmp), 1
			if (getMeld(GameStat, MELD_TILECODE, cnt, tmp) < MELD_TRIPLET*MELD_TYPE_STEP) {
				// 明順子
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+0)++
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+1)++
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+2)++
			} else: if (getMeld(GameStat, MELD_TILECODE, cnt, tmp) < MELD_QUAD*MELD_TYPE_STEP) {
				// 明刻子
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+0) += 3
			} else {
				if ((getMeld(GameStat, MELD_TILECODE, cnt, tmp)/MELD_TYPE_STEP != MELD_QUAD_CONCEALED)||(getRule(RULE_ANKAN_CONCEAL) == 0)) {
					// 槓子 (暗槓非開示設定で見えてない暗槓までカウントしないように条件式を細工しておく)
					SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+0) += 4
				}
			}
		loop
	loop
	// ドラ表示牌で見えてる枚数
	repeat 6
#ifdef ALLSANMA
		if (getDoraPointer(GameStat) <= (102-getRinshanExtension(GameStat)-cnt*2)) {SeenTiles(getWall(GameStat, WALL_TILECODE, 102-getRinshanExtension(GameStat)-cnt*2))++}
#else
		if (getDoraPointer(GameStat) <= (130-cnt*2)) {SeenTiles(getWall(GameStat, WALL_TILECODE, 130-cnt*2))++}
#endif
	loop
return
#global
#endif


/* 手牌のうちある１枚に注目する */
/* あとで書く */

/* 振聴でないかのチェック・待ち牌を数える処理も含む */
/* あとで書く */
std::unique_ptr<MachihaiInfo> chkFuriten(GameTable* gameStat, PLAYER_ID targetPlayer) {
	std::unique_ptr<MachihaiInfo> machihaiInfo(new MachihaiInfo);
	trace("振聴かどうか・待ち牌を調べます。");
	/* countseentiles呼び出し */

	/* 未完成・仮置き */
	return machihaiInfo;
}
#if 0
	countseentiles SeenTiles, GameStat
	countTilesInHand TileCountCFTTmp, GameStat, targetPlayer
	dim MachihaiCount, TILE_NONFLOWER_STRICT_MAX+1: MachihaiTotal = 0: MachiMen = 0
	dim MachihaiFlag, TILE_NONFLOWER_STRICT_MAX+1
	FuritenFlag = 0
	tmpTsumoHai = TsumoTile(GameStat, targetPlayer)
	repeat TILE_NONFLOWER_STRICT_MAX,1
		MachihaiCount(cnt) = -1
		if (cnt\TILE_SUIT_STEP == 0) {continue}
		setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, targetPlayer, cnt
		Shanten = countshanten(GameStat, targetPlayer, SHANTEN_ALL)
		if (Shanten == -1) {
			MachiMen++: MachihaiFlag(cnt) = 1
			MachihaiCount(cnt) = (4 - SeenTiles(cnt) - TileCountCFTTmp(cnt))
			MachihaiTotal += (4 - SeenTiles(cnt) - TileCountCFTTmp(cnt))
			tmpHai = cnt
			repeat DiscardPointer(GameStat, targetPlayer), 1
				if ((getDiscard(GameStat, DISCARD_TILECODE, cnt, targetPlayer)\TILE_CODE_MAXIMUM) == tmpHai) {
					FuritenFlag = 1
				}
			loop
			if ((MeldPointer(GameStat, targetPlayer) > 0)&&(getRule(RULE_KAKAN_FURITEN) == 1)) {
				repeat MeldPointer(GameStat, targetPlayer), 1
					if (((getMeld(GameStat, MELD_TILECODE, cnt, targetPlayer)/MELD_TYPE_STEP) >= MELD_QUAD_ADDED_MIN)&&((getMeld(GameStat, MELD_TILECODE, cnt, targetPlayer)\TILE_CODE_MAXIMUM) == tmpHai)) {
						FuritenFlag = 1
					}
				loop
			}
			repeat DiscardPointer(GameStat, targetPlayer), 1
				if ((getDiscard(GameStat, DISCARD_TILECODE, cnt, targetPlayer)\TILE_CODE_MAXIMUM) == tmpHai) {
					FuritenFlag = 1
				}
			loop
		}
		setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, targetPlayer, tmpTsumoHai
	loop
	repeat NUM_OF_PLAYERS
		tmpooooo = cnt
		// オープン立直者の手牌も数える
		if ((cnt != targetPlayer)&&(getRichiFlag(GameStat, RICHI_OPENFLAG, cnt) > 0)) {
			repeat NUM_OF_TILES_IN_HAND-1
				if (getHand(GameStat, HAND_TILECODE, cnt, tmpooooo) > 0) {
					if (MachihaiFlag(getHand(GameStat, HAND_TILECODE, cnt, tmpooooo)) == 1) {
						MachihaiCount(getHand(GameStat, HAND_TILECODE, cnt, tmpooooo))--
						MachihaiTotal--
					}
				}
			loop
		}
	loop
return
#global
#endif

/* オープン立直の待ち牌 */
void chkOpenMachi(GameTable* gameStat, PLAYER_ID targetPlayer) {
	// オープンリーチの待ち牌情報を更新する
	assert(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) {
		/* 変な牌で計算しないようにしましょう */
		if (i % TILE_SUIT_STEP == 0) continue;
		if (i % TILE_SUIT_STEP > tileCode::RedDragon) continue;
		/* まずは、ある牌をツモったと仮定します */
		gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = (tileCode)i;
		/* もしそれが和了になっていたら、フラグをセットしましょう */
		if (calcShanten(gameStat, targetPlayer, shantenAll) == -1)
			gameStat->OpenRichiWait[i] = true;
		/* これをすべての牌について試行しましょう */
	}
	/* もとに戻すのを忘れないようにしましょう。さもなくば多牌になってしまいます */
	gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;

	/* これで処理が終わりました。戻りましょう */
	return;
}
__declspec(dllexport) void chkOpenMachi(GameTable* gameStat, int targetPlayer) {
	// オープンリーチの待ち牌情報を更新する(HSP互換用)
	chkOpenMachi(gameStat, (PLAYER_ID)targetPlayer);
}

/* ドラを設定する */
/* あとで書く。牌譜への記録命令とかあるし…… */

/* 立直後の暗槓の可否 */
MJCORE bool chkAnkanAbility(GameTable* gameStat, PLAYER_ID targetPlayer) {
	// リーチ後の暗槓ができるかどうかを判定する

	/*
	 * リーチ後の暗槓が許される条件：
	 * 1) 送り槓(例えば555567と持っている時8を自摸って5を槓すること)をしてはいけない
	 * 2) 面子構成が変わってはいけない
	 * 条件(2)を実装しやすいように書き換えると次のようになる：
	 * 2a) 雀頭候補となる牌(例：22234という形における2)を暗槓してはいけない
	 * 2b) 待ちが変わってはいけない
	 */

	std::ostringstream o;
	o.str(""); o << "暗槓の可否を判定します。プレイヤー [" << (int)targetPlayer << "]"; debug(o.str());

	/* 手牌を調べる */
	TileCount tlCount = countTilesInHand(gameStat, targetPlayer);

	/* 手牌を退避する */
	HAND_TILES tmpHand;
	memcpy(tmpHand, gameStat->Player[targetPlayer].Hand, sizeof(HAND_TILES));

	/* ツモった牌が４枚揃ってないなら当然に槓は不可能 */
	/* 立直後であり送り槓はできないのでツモった牌だけ調べればよい */
	if (tlCount[gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile] < 4) {
		o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] は、4枚揃っていません。"; debug(o.str());
		return false;
	} else {
		o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] は、手牌に合わせて4枚あります。"; debug(o.str());
	}

	/* 字牌に順子はないのでこれ以降のチェックをせずとも槓が可能とわかる */
	switch (gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
	case EastWind: case SouthWind: case WestWind: case NorthWind:
	case WhiteDragon: case GreenDragon: case RedDragon:
		o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] は字牌です。"; debug(o.str());
		return true;
	case Spring: case Summer: case Autumn: case Winter:
	case Plum: case Orchid: case Chrysanthemum: case Bamboo:
		/* 花牌を槓？　ご冗談を */
		o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] は花牌です。"; debug(o.str());
		return false;
	default:
		o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] は数牌です。"; debug(o.str());
		break;
	}

	/* ツモった牌が雀頭になりうるなら戻る */
	int tmpTakenCount = 0;
	for (int i = 0; i < (NUM_OF_TILES_IN_HAND-1); i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile ==
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
				gameStat->Player[targetPlayer].Hand[i].tile = NoTile;
				tmpTakenCount++;
		}
		if (tmpTakenCount == 2) break;
	}
	gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
	/*	
		例：
		面子を２、対子や塔子を１、雀頭候補を①と定義する。
		面子オーバーは除外すること。合計が８なら聴牌、９なら和了
		１１１３４４４東東東発発発 ←この場合
		+①++１++-２-++-２-++-２-+ … ８(聴牌)
		＊＊１３４４４東東東発発発 ←１を雀頭に取ると
		    +１++-２-++-２-++-２-+ … ７(一向聴)
		１１１３４４４東東東発＊＊ ←発を取ろうとすると
		+①++１++-２-++-２-+×     … ６(二向聴)
	*/
	SHANTEN shanten = calcShanten(gameStat, targetPlayer, shantenAll);
	memcpy(gameStat->Player[targetPlayer].Hand, tmpHand, sizeof(HAND_TILES)); /* 手牌を書き戻す */
	if (shanten == 1) {
		o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] は、アタマ候補です。"; debug(o.str());
		return false;
	} else {
		o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] は、アタマ候補ではありません。"; debug(o.str());
	}

	/* 暗槓する前の待ち牌を調べる */
	/* chkfuriten関数呼び出しをここに書く */
	
	/* 未完成・仮置き */
	return false;
}
#if 0
	/* 暗槓する前の待ち牌を調べる */
	chkfuriten FuritenFlag, MachihaiFlag, MachihaiCount, MachihaiTotal, MachiMen, GameStat, targetPlayer
	dim tmpchkMachihaiFlag, TILE_NONFLOWER_STRICT_MAX+1
	repeat TILE_NONFLOWER_STRICT_MAX+1: tmpchkMachihaiFlag(cnt) = MachihaiFlag(cnt): loop
	/* 暗槓したあとの待ち牌を調べる */
	repeat NUM_OF_TILES_IN_HAND
		if (getHand(GameStat, HAND_TILECODE, cnt, targetPlayer) == TsumoTile(GameStat, targetPlayer)) {
			setHand GameStat, HAND_TILECODE, cnt, targetPlayer, 0: tmptakencount++
		}
	loop
	setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, targetPlayer, 0
	MeldPointerIncrement GameStat, targetPlayer
	chkfuriten FuritenFlag, MachihaiFlag, MachihaiCount, MachihaiTotal, MachiMen, GameStat, targetPlayer
	/* 手牌を書き戻す */
	MeldPointerDecrement GameStat, targetPlayer: repeat NUM_OF_TILES_IN_HAND /* 手牌を書き戻す */
		setHand GameStat, HAND_TILECODE, cnt, targetPlayer, tmpkanhand(cnt)
	loop
	/* 待ち牌が一致しないなら戻る */
	MachiHaiDifference = 0
	repeat TILE_NONFLOWER_STRICT_MAX+1
		if (tmpchkMachihaiFlag(cnt) != MachihaiFlag(cnt)) {
			MachiHaiDifference = 1
		}
	loop
	if (MachiHaiDifference == 1) {
		debug "槓の前後で待ち牌が一致しません。"
		return 0
	}
	/* 条件を満たす暗槓 */
	debug "槓の前後で待ち牌が一致しました。"
	return 1
#global
#endif
