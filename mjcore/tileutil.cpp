#include "tileutil.h"

/* 四人三麻用 */
PLAYER_ID* tobePlayed(const GameTable* const gameStat) {
	static PLAYER_ID tp[] = {
		(gameStat->GameRound % PLAYERS),
		((gameStat->GameRound + 1) % PLAYERS),
		((gameStat->GameRound + 2) % PLAYERS)
	};
	if (chkGameType(gameStat, Sanma4)) return tp;
	else return NULL;
}
PLAYER_ID tobePlayed(const GameTable* const gameStat, int id) {
	assert((id >= 0)&&(id < 13));
	PLAYER_ID *tp = tobePlayed(gameStat);
	return tp[id];
}

/* 一九字牌のコード一覧 */
tileCode* Honor_Major_Tiles() {
	static const tileCode YaojiuPai[] = {
		CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	return (tileCode*)&YaojiuPai;
}
tileCode Honor_Major_Tiles(int code) {
	assert((code >= 0)&&(code < 13));
	static const tileCode* YaojiuPai = Honor_Major_Tiles();
	return YaojiuPai[code];
}
/* 一九字牌かどうか調べる */
bool isYaojiu(tileCode code) {
	bool ans = false;
	for (int i = 0; i < 13; i++)
		if (code == Honor_Major_Tiles(i)) ans = true;
	return ans;
}

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
				if (RuleData::getRule("ankan_conceal") != 0) break; // 暗槓非開示ルールだったらカウントしない
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
MJCORE TileStatus gettilestatus(
	const GameTable* const gameStat, PLAYER_ID targetPlayer, int targetTile, bool CheckMode)
{
	const tileCode* const theTile = &gameStat->Player[targetPlayer].Hand[targetTile].tile;

	TileStatus tlStat; memset(&tlStat, 0, sizeof(tlStat)); // 初期化
	if (*theTile == NoTile) return tlStat; // なかったら戻る

	tlStat.isExistent = true; // ここにたどり着いたら、それは存在する牌です
	auto tlCount = countTilesInHand(gameStat, targetPlayer); // 牌を数えろ、話はそれからだ

	if (tlCount[*theTile] == 4) tlStat.canFormQuad = true; // 暗槓が可能な牌
	if (tlCount[*theTile] >= 3) tlStat.formsTriplet = true; // 暗刻を形成している場合
	
	if (*theTile < TILE_SUIT_HONORS) { // 順子を形成している場合
		if ((tlCount[*theTile] >= 1)&&(tlCount[int(*theTile) + 1] >= 1)&&(tlCount[int(*theTile) + 2] >= 1))
			tlStat.formsSequence = true;
		else if ((tlCount[*theTile] >= 1)&&(tlCount[int(*theTile) + 1] >= 1)&&(tlCount[int(*theTile) - 1] >= 1))
			tlStat.formsSequence = true;
		else if ((*theTile > 1)&&(tlCount[*theTile] >= 1)&&(tlCount[int(*theTile) - 2] >= 1)&&(tlCount[int(*theTile) - 1] >= 1))
			tlStat.formsSequence = true;
	}

	if ((tlCount[*theTile] >= 2)&&(!tlStat.formsTriplet)) tlStat.formsPair = true; // 対子を形成している場合

	if (*theTile < TILE_SUIT_HONORS) { // 辺張を形成している場合
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[int(*theTile) + 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 1))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[int(*theTile) + 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 8))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[int(*theTile) - 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 2))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[int(*theTile) - 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 9))
				tlStat.seqSingleSideWait = true;
		}
	}
	
	if (*theTile < TILE_SUIT_HONORS) { // 両面塔子を形成している場合
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[int(*theTile) + 1] >= 1)&&(*theTile % TILE_SUIT_STEP != 1)&&
				(*theTile % TILE_SUIT_STEP != 8)) tlStat.seqDoubleSideWait = true;
			else if ((tlCount[int(*theTile) - 1] >= 1)&&(*theTile % TILE_SUIT_STEP != 2)&&
				(*theTile % TILE_SUIT_STEP != 9)) tlStat.seqDoubleSideWait = true;
		}
	}
	
	if (*theTile < TILE_SUIT_HONORS) { // 嵌張を形成している場合
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[int(*theTile) + 2] >= 1)&&(*theTile % TILE_SUIT_STEP != 9))
				tlStat.seqMidWait = true;
			else if ((*theTile > 1)&&(tlCount[int(*theTile) - 2] >= 1)&&(*theTile % TILE_SUIT_STEP != 1))
				tlStat.seqMidWait = true;
		}
	}

	return tlStat;
}
__declspec(dllexport) int gettilestatus(
	const GameTable* const gameStat, int targetPlayer, int targetTile, int CheckMode)
{
	auto ans = gettilestatus(gameStat, (PLAYER_ID)targetPlayer, targetTile, (bool)CheckMode);
	return
		(ans.isExistent ?
		(ans.canFormQuad ? 0x40 : 0) +
		(ans.seqMidWait ? 0x20 : 0) +
		(ans.seqDoubleSideWait ? 0x10 : 0) +
		(ans.seqSingleSideWait ? 0x08 : 0) +
		(ans.formsPair ? 0x04 : 0) +
		(ans.formsSequence ? 0x02 : 0) +
		(ans.formsTriplet ? 0x01 : 0)
		: -999);
}

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
		if (ShantenAnalyzer::calcShanten(&tmpGameStat, targetPlayer, ShantenAnalyzer::shantenAll) == -1) { // 待ちになっていたら
			machihaiInfo.MachiMen++; machihaiInfo.Machihai[i].MachihaiFlag = true; // フラグをセットしましょう
			machihaiInfo.MachihaiTotal += // カウントを加算しましょう
				(machihaiInfo.Machihai[i].MachihaiCount =
				(4 - seenTiles[i] - countTiles[i]));
			// フリテンかどうかを判断する
			for (int j = 1; j <= tmpGameStat.Player[targetPlayer].DiscardPointer; j++) {
				if (tmpGameStat.Player[targetPlayer].Discard[j].tcode.tile == i) // 同じ種類の捨て牌が見つかったら
					machihaiInfo.FuritenFlag = true; // フリテンと判断します
			}
			if (RuleData::getRule("kakan_furiten") == 1) { // 加槓の牌をフリテンとみなすルールなら
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
		if (ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenAll) == -1)
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
MJCORE bool chkdaopaiability(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	Int8ByTile TileCount = countTilesInHand(gameStat, targetPlayer);
	int YaojiuCount = 0; bool AtamaFlag = false;
	for (int i = 0; i < 13; i++) // ヤオ九牌１種類につき、１をカウントする。
		if (TileCount[Honor_Major_Tiles(i)] >= 1) YaojiuCount++;
	return (YaojiuCount >= 9);
}
__declspec(dllexport) int chkdaopaiability(const GameTable* const gameStat, int targetPlayer) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	return chkdaopaiability(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

/* ドラを設定する */
namespace setdora_tools {
	tileCode getNextOf(const GameTable* const gameStat, tileCode tc) { // ネクスト牌
		tileCode ans = (tileCode)((int)tc + 1);
		if ((chkGameType(gameStat, SanmaX))&&(ans == CharacterTwo)) ans = CharacterNine;
		else if (ans == (tileCode)10) ans = CharacterOne;
		else if (ans == (tileCode)20) ans = CircleOne;
		else if (ans == (tileCode)30) ans = BambooOne;
		else if (ans == (tileCode)35) ans = EastWind;
		else if (ans == (tileCode)38) ans = WhiteDragon;
		return ans;
	}

	tileCode getPrevOf(const GameTable* const gameStat, tileCode tc) { // 前の牌
		tileCode ans = (tileCode)((int)tc - 1);
		if ((chkGameType(gameStat, SanmaX))&&(ans == CharacterEight)) ans = CharacterOne;
		else if (ans == (tileCode)0) ans = CharacterNine;
		else if (ans == (tileCode)10) ans = CircleNine;
		else if (ans == (tileCode)20) ans = BambooNine;
		else if (ans == (tileCode)30) ans = NorthWind;
		else if (ans == (tileCode)34) ans = RedDragon;
		return ans;
	}

	void addDora(GameTable* const gameStat, tileCode tc, int Mode) {
		for (int i = ((RuleData::getRule("nagatacho") != 0) ? tc % 10 : tc);
			i <= (((RuleData::getRule("nagatacho") != 0) && (tc < TILE_SUIT_HONORS)) ? TILE_SUIT_HONORS : tc);
			i += 10) {
				std::ostringstream o;
				if (Mode) gameStat->DoraFlag.Ura[i]++;	// ドラを設定する
				else gameStat->DoraFlag.Omote[i]++;	// ドラを設定する
				o << "牌コード [" << (int)tc << "] をドラにしました。";
				debug(o.str().c_str());
				if (Mode) haifu::haifurecuradora((tileCode)i);
				else haifu::haifurecdora((tileCode)i);
		}
	}
}

__declspec(dllexport) void setdora(GameTable* const gameStat, int Mode) {
	std::ostringstream o;
	o << "ドラの追加 ポインタ [" << gameStat->DoraPointer <<
		"] 牌コード [" << (int)gameStat->Deck[gameStat->DoraPointer + Mode].tile <<
		"] モード [" << Mode << "]";
	debug(o.str().c_str());
	if (gameStat->Deck[gameStat->DoraPointer + Mode].tile > TILE_SUIT_FLOWERS) {
		// 花牌がドラ表示牌になったとき
		setdora_tools::addDora(gameStat, Flower, Mode);
	} else {
		if (RuleData::getRule("dora_indicator") == 2) {
			// 前の牌がドラ（超インフレ用）
			if ((gameStat->Deck[gameStat->DoraPointer + Mode].tile >= 10) ||
				(!chkGameType(gameStat, SanmaX)))
					setdora_tools::addDora(gameStat,
						setdora_tools::getPrevOf(gameStat, gameStat->Deck[gameStat->DoraPointer + Mode].tile),
						Mode);
		}
		if ((RuleData::getRule("dora_indicator") == 1)||(RuleData::getRule("dora_indicator") == 2)) {
			// 現物ドラ
			setdora_tools::addDora(gameStat,
				gameStat->Deck[gameStat->DoraPointer + Mode].tile,
				Mode);
		}
		if (RuleData::getRule("dora_indicator") != 1) {
			// ネクストドラ
			setdora_tools::addDora(gameStat,
				setdora_tools::getNextOf(gameStat, gameStat->Deck[gameStat->DoraPointer + Mode].tile),
				Mode);
		}
	}
}

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
		SHANTEN shanten = ShantenAnalyzer::calcShanten(&tmpGameStat, targetPlayer, ShantenAnalyzer::shantenAll);
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

/* 導火線の位置を調べる */
__declspec(dllexport) void calcdoukasen(GameTable* const gameStat) {
	/* 導火線の位置を計算する */
	if (RuleData::getRule("doukasen") != 0) {
		if (chkGameType(gameStat, Sanma4)) {
			PLAYER_ID* tmpDoukasen = new PLAYER_ID(
				((30 - ((diceSum(gameStat) - 1) * 36 * 2 + 
				diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 36) + 30) % 3);
			gameStat->DoukasenPlayer = tobePlayed(gameStat, *tmpDoukasen);
			delete tmpDoukasen;
		} else if (chkGameType(gameStat, SanmaT)) {
			gameStat->DoukasenPlayer =
				((30 - ((diceSum(gameStat) - 1 +
				(gameStat->GameRound - (gameStat->GameRound / 4))) * 36 * 2 +
				diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 36) + 30) % 3;
		} else {
			int* tmp;
			switch (RuleData::getRule("flower_tiles")) {
			case 3:
				gameStat->DoukasenPlayer =
					((40 - ((diceSum(gameStat) - 1 +
					(gameStat->GameRound % PLAYERS)) * 36 * 3 + 
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 36) + 40)
					% PLAYERS;
				break;
			case 0:
				gameStat->DoukasenPlayer =
					((40 - ((diceSum(gameStat) - 1 +
					(gameStat->GameRound % PLAYERS)) * 34 * 3 +
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 34) + 40)
					% PLAYERS;
				break;
			default:
				gameStat->DoukasenPlayer =
					((40 - ((diceSum(gameStat) - 1 +
					(gameStat->GameRound % PLAYERS)) * 70 * 3 / 2 +
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 35) + 40)
					% PLAYERS;
				tmp = new int(
					((diceSum(gameStat) - 1 + (gameStat->GameRound % PLAYERS)) * 70 * 3 / 2 +
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) % 70
					);
				if (*tmp == (((diceSum(gameStat) + gameStat->GameRound) % 2) * 35))
					gameStat->DoukasenPlayer = (40 + gameStat->DoukasenPlayer + 1) % PLAYERS;
				delete tmp;
			}
		}
	}
}

/* 聴牌かどうか調べる */
bool isTenpai(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	auto Shanten = ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenAll);
	if (gameStat->Player[targetPlayer].AgariHouki) Shanten = 1; // アガリ放棄なら強制不聴
	if (EnvTable::Instantiate()->PlayerDat[targetPlayer].RemotePlayerFlag == -1)
		return false;
	return (Shanten <= 0);
}
__declspec(dllexport) int isTenpai(const GameTable* const gameStat, void *, int targetPlayer) {
	return isTenpai(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

/* 流し満貫かどうか調べる */
bool isNagashiMangan(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	if (gameStat->Player[targetPlayer].AgariHouki) return false; // 和了り放棄時の処理→流し満貫を認めない
	if (EnvTable::Instantiate()->PlayerDat[targetPlayer].RemotePlayerFlag == -1)
		return false;
	int YaojiuSutehai = 0;
	for (int i = 1; i <= gameStat->Player[targetPlayer].DiscardPointer; i++) {
		// 鳴かれた牌だったら無視
		if (gameStat->Player[targetPlayer].Discard[i].dstat == discardTaken) continue;
		if (gameStat->Player[targetPlayer].Discard[i].dstat == discardRiichiTaken) continue;
		// 鳴かれていない牌
		if (isYaojiu(gameStat->Player[targetPlayer].Discard[i].tcode.tile)) YaojiuSutehai++;
	}
	// 全部該当する牌だったらtrue
	return (YaojiuSutehai == gameStat->Player[targetPlayer].DiscardPointer);
}
__declspec(dllexport) int isNagashiMangan(const GameTable* const gameStat, void *, int targetPlayer) {
	return isNagashiMangan(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}
