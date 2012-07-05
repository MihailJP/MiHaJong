#include "tileutil.h"

/* 理牌する */
void lipai(GameTable* const gameStat, PLAYER_ID targetPlayer) {
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
			gameStat->Player[targetPlayer].Hand[i].tile = TilePad;
			gameStat->Player[targetPlayer].Hand[i].red = Normal;
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
		if (gameStat->Player[targetPlayer].Hand[i].tile == TilePad) {
			gameStat->Player[targetPlayer].Hand[i].tile = NoTile;
			gameStat->Player[targetPlayer].Hand[i].red = Normal;
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
__declspec(dllexport) void lipai(GameTable* const gameStat, int targetPlayer) {
	// 理牌する(HSP互換用)
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	lipai(gameStat, (PLAYER_ID)targetPlayer);
}

/* 場に見えてる個数 */
MJCORE Int8ByTile countseentiles(const GameTable* const gameStat) {
	// 場に見えている牌の数を数える
	std::ostringstream o; // エラーメッセージ用

	Int8ByTile seenTiles; // 計算する牌
	memset(&seenTiles, 0, sizeof(seenTiles)); // まずは、初期化

	// 捨牌で見えてる枚数
	for (int i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].DiscardPointer == 0) continue;
		for (int j = 1; j <= gameStat->Player[i].DiscardPointer; j++)
			seenTiles[gameStat->Player[i].Discard[j].tcode.tile]++;
	}

	// 誰かの副露で見えてる枚数
	for (int i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].MeldPointer == 0) continue;
		for (int j = 1; j <= gameStat->Player[i].MeldPointer; j++) {
			switch (gameStat->Player[i].Meld[j].mstat) {
			case meldSequenceExposedLower:
			case meldSequenceExposedMiddle:
			case meldSequenceExposedUpper:
				// 明順子(チー)
				// まず、正しいコードになっているかを確認する(デバッグ時)
				assert(gameStat->Player[i].Meld[j].tile % TILE_SUIT_STEP <= 7);
				assert(gameStat->Player[i].Meld[j].tile % TILE_SUIT_STEP > 0);
				assert(gameStat->Player[i].Meld[j].tile < TILE_SUIT_HONORS);
				// カウントアップ
				for (int k = gameStat->Player[i].Meld[j].tile;
					k <= gameStat->Player[i].Meld[j].tile + 2; k++)
					seenTiles[gameStat->Player[i].Meld[j].tile]++;
				// ここまで
				break;
			case meldTripletExposedLeft:
			case meldTripletExposedCenter:
			case meldTripletExposedRight:
				// 明刻子(ポン)
				seenTiles[gameStat->Player[i].Meld[j].tile] += 3;
				break;
			case meldQuadConcealed:
				// 暗槓
				if (getRule(RULE_ANKAN_CONCEAL) != 0) break; // 暗槓非開示ルールだったらカウントしない
				/* FALLTHRU */
			case meldQuadExposedLeft:   case meldQuadAddedLeft:
			case meldQuadExposedCenter: case meldQuadAddedCenter:
			case meldQuadExposedRight:  case meldQuadAddedRight:
				// 明槓
				seenTiles[gameStat->Player[i].Meld[j].tile] += 4;
				break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "副露データに暗順子、暗刻子、または不明な種類の面子が検出されました");
			}
		}
	}

	// ドラ表示牌で見えてる枚数
	for (int i = 0; i < 6; i++) {
		if (chkGameType(gameStat, AllSanma)) {
			if (gameStat->DoraPointer <= (102 - gameStat->ExtraRinshan - i * 2))
				seenTiles[gameStat->Deck[102 - gameStat->ExtraRinshan - i * 2].tile]++;
		} else {
			if (gameStat->DoraPointer <= (130 - i * 2))
				seenTiles[gameStat->Deck[130 - i * 2].tile]++;
		}
	}

	return seenTiles;
}
__declspec(dllexport) void countseentiles(int* const seenTiles, const GameTable* const gameStat) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	Int8ByTile tileCnt = countseentiles(gameStat);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) seenTiles[i] = (int)tileCnt[i];
}

/* 手牌に存在する牌を種類別にカウントする */
MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PLAYER_ID playerID) {
	// 手牌に存在する牌を種類別にカウントする（鳴き面子・暗槓は除く）
	Int8ByTile count; memset(&count, 0, sizeof(count)); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = gameStat->Player[playerID].Hand[i].tile) != NoTile)
			count[tmpTC]++;
	}
	return count;
}
__declspec(dllexport) void countTilesInHand(int* const tileCount, const GameTable* const gameStat, int playerID) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	Int8ByTile tCount = countTilesInHand(gameStat, (PLAYER_ID)playerID);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) tileCount[i] = (int)tCount[i];
}

/* 手牌に存在する赤ドラを種類別にカウントする */
MJCORE Int8ByTile countRedTilesInHand(const GameTable* const gameStat, PLAYER_ID playerID, int doraCol) {
	assert((doraCol == 1)||(doraCol == 2));
	// 手牌に存在する牌を種類別にカウントする（鳴き面子・暗槓は除く）
	Int8ByTile count; memset(&count, 0, sizeof(count)); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = tileCode(gameStat->Player[playerID].Hand[i].tile)) != NoTile)
			if (tileCode(gameStat->Player[playerID].Hand[i].red) == doraCol)
				count[tmpTC]++;
	}
	// 赤ドラのみ数えるときは便宜のため、鳴き面子も数える
	for (int i = 1; i < gameStat->Player[playerID].MeldPointer; i++) {
		switch (gameStat->Player[playerID].Meld[i].mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle:
		case meldSequenceExposedUpper:
			// 順子の時
			if (gameStat->Player[playerID].Meld[i].red[0] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			if (gameStat->Player[playerID].Meld[i].red[1] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile + 1]++;
			if (gameStat->Player[playerID].Meld[i].red[2] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile + 2]++;
			break;
		case meldQuadExposedLeft: case meldQuadAddedLeft:
		case meldQuadExposedCenter: case meldQuadAddedCenter:
		case meldQuadExposedRight: case meldQuadAddedRight:
		case meldQuadConcealed: // 暗槓も数えてあげましょう……
			// 槓子の時
			if (gameStat->Player[playerID].Meld[i].red[3] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			/* FALLTHRU */
		case meldTripletExposedLeft: case meldTripletExposedCenter:
		case meldTripletExposedRight:
			// 刻子の時(槓子も含む)
			if (gameStat->Player[playerID].Meld[i].red[0] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			if (gameStat->Player[playerID].Meld[i].red[1] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			if (gameStat->Player[playerID].Meld[i].red[2] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			break;
		default:
			// 異常データ
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "副露データに暗順子、暗刻子、または不明な種類の面子が検出されました");
		}
	}
	return count;
}
__declspec(dllexport) void countRedTilesInHand(int* const tileCount, const GameTable* const gameStat, int playerID) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	Int8ByTile tCount = countRedTilesInHand(gameStat, (PLAYER_ID)playerID, 1);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) tileCount[i] = (int)tCount[i];
}


/* 手牌のうちある１枚に注目する */
/* あとで書く */

/* 振聴でないかのチェック・待ち牌を数える処理も含む */
MJCORE MachihaiInfo chkFuriten(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	trace("振聴かどうか・待ち牌を調べます。");
	Int8ByTile seenTiles = countseentiles(gameStat); // 見えてる牌の数を数える
	Int8ByTile countTiles = countTilesInHand(gameStat, targetPlayer); // 手の内の牌の数を数える
	MachihaiInfo machihaiInfo; memset(&machihaiInfo, 0, sizeof(machihaiInfo)); // 初期化
	GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(GameTable)); // テンポラリ卓情報オブジェクト(ここからは仮定法の世界……)

	for (tileCode i = CharacterOne; i < Flower; i = (tileCode)((int)i + 1)) {
		if ((int)i % TILE_SUIT_STEP == 0) continue; // ない牌だったら戻る
		tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = i;
		if (calcShanten(&tmpGameStat, targetPlayer, shantenAll) == -1) { // 待ちになっていたら
			machihaiInfo.MachiMen++; machihaiInfo.Machihai[i].MachihaiFlag = true; // フラグをセットしましょう
			machihaiInfo.MachihaiTotal += // カウントを加算しましょう
				(machihaiInfo.Machihai[i].MachihaiCount =
				(4 - seenTiles[i] - countTiles[i]));
			// フリテンかどうかを判断する
			for (int j = 1; j <= tmpGameStat.Player[targetPlayer].DiscardPointer; j++) {
				if (tmpGameStat.Player[targetPlayer].Discard[j].tcode.tile == i) // 同じ種類の捨て牌が見つかったら
					machihaiInfo.FuritenFlag = true; // フリテンと判断します
			}
			if (getRule(RULE_KAKAN_FURITEN) == 1) { // 加槓の牌をフリテンとみなすルールなら
				for (int j = 1; j <= tmpGameStat.Player[targetPlayer].MeldPointer; j++) {
					switch (tmpGameStat.Player[targetPlayer].Meld[j].mstat) {
					case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight: // 加槓で、なおかつ
						if (tmpGameStat.Player[targetPlayer].Meld[j].tile == i) // 同じ種類の捨て牌が見つかったら
							machihaiInfo.FuritenFlag = true; // フリテンと判断します
						break;
					}
				}
			}
		}
	}

	// オープン立直者の手牌も数える
	for (int p = 0; p < PLAYERS; p++) {
		if ((p != targetPlayer) && (gameStat->Player[p].RichiFlag.OpenFlag)) { // 自分以外のオープンリーチ者の時、
			for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) { // それぞれの手牌について、
				if (gameStat->Player[p].Hand[i].tile != NoTile) { // ブランクではなくて、
					if (machihaiInfo.Machihai[gameStat->Player[p].Hand[i].tile].MachihaiCount > 0) { // 1枚以上あるはずの待ち牌と認識されている時
						machihaiInfo.Machihai[gameStat->Player[p].Hand[i].tile].MachihaiCount--;
						machihaiInfo.MachihaiTotal--; // 待ち牌の数を減らす
					}
				}
			}
		}
	}

	/* 計算完了、値を返しましょう */
	return machihaiInfo;
}
__declspec(dllexport) void chkFuriten(
	int* const furitenFlag, int* const machihaiFlag, int* const machihaiCount,
	int* const machihaiTotal, int* const machiMen, const GameTable* const gameStat,
	int targetPlayer) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		MachihaiInfo machihaiInfo = chkFuriten(gameStat, (PLAYER_ID)targetPlayer);
		*furitenFlag = machihaiInfo.FuritenFlag ? 1 : 0;
		for (int i = 0; i < TILE_NONFLOWER_MAX; i++) {
			machihaiFlag[i] = machihaiInfo.Machihai[i].MachihaiFlag ? 1 : 0;
			machihaiCount[i] = machihaiInfo.Machihai[i].MachihaiFlag ?
				(int)machihaiInfo.Machihai[i].MachihaiCount : (-1);
		}
		*machihaiTotal = (int)machihaiInfo.MachihaiTotal;
		*machiMen = (int)machihaiInfo.MachiMen;
}

/* オープン立直の待ち牌 */
void chkOpenMachi(GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// オープンリーチの待ち牌情報を更新する
	assert(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) {
		/* 変な牌で計算しないようにしましょう */
		if (i % TILE_SUIT_STEP == 0) continue;
		if (i % TILE_SUIT_STEP > RedDragon) continue;
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
__declspec(dllexport) void chkOpenMachi(GameTable* const gameStat, int targetPlayer) {
	// オープンリーチの待ち牌情報を更新する(HSP互換用)
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	chkOpenMachi(gameStat, (PLAYER_ID)targetPlayer);
}

/* 九種九牌流しが可能かどうかのチェック */
/* あとで */

/* ドラを設定する */
/* あとで書く。牌譜への記録命令とかあるし…… */

/* 立直後の暗槓の可否 */
namespace chkAnkanAbilityTools { // chkAnkanAbility関数用の処理
	bool CheckForNumOfTiles(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* ツモった牌が４枚揃ってないなら当然に槓は不可能 */
		/* 立直後であり送り槓はできないのでツモった牌だけ調べればよい */
		std::ostringstream o;
		Int8ByTile tlCount = countTilesInHand(gameStat, targetPlayer);
		if (tlCount[gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile] < 4) {
			o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] は、4枚揃っていません。"; debug(o.str());
			return true;
		} else {
			o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] は、手牌に合わせて4枚あります。"; debug(o.str());
			return false;
		}
	}
	int CheckForTileClass(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* 字牌に順子はないのでこれ以降のチェックをせずとも槓が可能とわかる */
		std::ostringstream o;
		switch (gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
		case EastWind: case SouthWind: case WestWind: case NorthWind:
		case WhiteDragon: case GreenDragon: case RedDragon:
			o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] は字牌です。"; debug(o.str());
			return 1;
		case Spring: case Summer: case Autumn: case Winter:
		case Plum: case Orchid: case Chrysanthemum: case Bamboo:
			/* 花牌を槓？　ご冗談を */
			o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] は花牌です。"; debug(o.str());
			return 2;
		default:
			o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] は数牌です。"; debug(o.str());
			return 0;
		}
	}
	bool CheckForAtama(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* ツモった牌が雀頭になりうるか調べる */
		std::ostringstream o;
		GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(tmpGameStat));

		// 2枚除去
		int tmpTakenCount = 0;
		for (int i = 0; i < (NUM_OF_TILES_IN_HAND-1); i++) {
			if (tmpGameStat.Player[targetPlayer].Hand[i].tile ==
				tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
					tmpGameStat.Player[targetPlayer].Hand[i].tile = NoTile;
					tmpTakenCount++;
			}
			if (tmpTakenCount == 2) break;
		}
		tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
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
		SHANTEN shanten = calcShanten(&tmpGameStat, targetPlayer, shantenAll);
		if (shanten == 1) {
			o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] は、アタマ候補です。"; debug(o.str());
			return true;
		} else {
			o.str(""); o << "ツモ牌 [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] は、アタマ候補ではありません。"; debug(o.str());
			return false;
		}
	}
	bool CheckIfMachihaiMatches(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* 待ち牌が一致するかどうか調べる */
		std::ostringstream o;

		/* 暗槓する前の待ち牌を調べる */
		MachihaiInfo machiInfo_Before = chkFuriten(gameStat, targetPlayer);
		/* 暗槓したあとの待ち牌を調べる */
		int tmpTakenCount = 0; // 牌の数の確認用
		GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(GameTable));
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if (tmpGameStat.Player[targetPlayer].Hand[i].tile ==
				tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
					tmpGameStat.Player[targetPlayer].Hand[i].tile = NoTile;
					tmpTakenCount++;
			}
		}
		assert(tmpTakenCount == 4); // デバッグ用：ちょうど4枚だったか確認
		tmpGameStat.Player[targetPlayer].Meld[++ tmpGameStat.Player[targetPlayer].MeldPointer].tile =
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile; /* ツモった牌を */
		tmpGameStat.Player[targetPlayer].Meld[tmpGameStat.Player[targetPlayer].MeldPointer].mstat =
			meldQuadConcealed; /* 暗槓したとみなす */
		MachihaiInfo machiInfo_After = chkFuriten(&tmpGameStat, targetPlayer);

		/* 待ち牌は一致するか？ */
		bool machihaiDiffers = false;
		for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
			if (machiInfo_Before.Machihai[i].MachihaiFlag != machiInfo_After.Machihai[i].MachihaiFlag)
				machihaiDiffers = true;
		if (machihaiDiffers) { // 待ち牌が一致しないなら、槓できない
			debug("槓の前後で待ち牌が一致しません。");
			return false;
		} else { // 条件を満たす暗槓
			debug("槓の前後で待ち牌が一致しました。");
			return true;
		}
	}
}

MJCORE bool chkAnkanAbility(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
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

	/* ツモった牌が４枚揃ってないなら当然に槓は不可能 */
	/* 立直後であり送り槓はできないのでツモった牌だけ調べればよい */
	if (chkAnkanAbilityTools::CheckForNumOfTiles(gameStat, targetPlayer)) return false;

	/* 字牌に順子はないのでこれ以降のチェックをせずとも槓が可能とわかる */
	switch (chkAnkanAbilityTools::CheckForTileClass(gameStat, targetPlayer)) {
		case 1: return true; // 字牌
		case 2: return false; // 花牌
	}

	/* ツモった牌が雀頭になりうるなら戻る */
	if (chkAnkanAbilityTools::CheckForAtama(gameStat, targetPlayer)) return false;

	/* 待ち牌が一致するかどうか調べる */
	return (chkAnkanAbilityTools::CheckIfMachihaiMatches(gameStat, targetPlayer));
}
__declspec(dllexport) int chkAnkanAbility(const GameTable* const gameStat, int targetPlayer) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	return chkAnkanAbility(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}
