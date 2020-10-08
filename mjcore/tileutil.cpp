#include "tileutil.h"

#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <queue>
#include <algorithm>
#include <array>
#include "logging.h"
#include "ruletbl.h"
#include "shanten.h"
#include "func.h"
#include "envtbl.h"
#include "haifu.h"
#include "yaku/yaku.h"

/* 四人三麻用 */
PlayerID* tobePlayed(const GameTable* const gameStat) {
	static PlayerID tp[] = {
		static_cast<PlayerID>(gameStat->GameRound % Players),
		static_cast<PlayerID>((gameStat->GameRound + 1) % Players),
		static_cast<PlayerID>((gameStat->GameRound + 2) % Players)
	};
	if (gameStat->chkGameType(GameTypeID::sanma4)) return tp;
	else return nullptr;
}
PlayerID tobePlayed(const GameTable* const gameStat, int id) {
	assert((id >= 0)&&(id < 13));
	PlayerID *tp = tobePlayed(gameStat);
	return tp[id];
}

/* 一九字牌かどうか調べる */
bool isYaojiu(TileCode code) {
	bool ans = false;
	for (auto tile : Honor_Major_Tiles)
		if (code == tile) ans = true;
	return ans;
}

/* 手牌データをデバッグログに出力 */
static void logHand(const CodeConv::tstring& label , GameTable* const gameStat, PlayerID targetPlayer) {
	CodeConv::tostringstream o;
	o.str(_T("")); o << label << _T(" [");
	for (int i = 0; i < NumOfTilesInHand; i++)
		o << std::setw(2) << std::setfill(_T('0')) << static_cast<int>(gameStat->Player[targetPlayer].Hand[i].tile)
		<< ((i < (NumOfTilesInHand - 1)) ? _T(" ") : _T(""));
	o << _T("]"); trace(o.str());
}

/* 理牌する */
void lipai(GameTable* const gameStat, PlayerID targetPlayer) {
	// 理牌する

	CodeConv::tostringstream o;
	o.str(_T("")); o << _T("理牌を行います。プレイヤー [") << static_cast<int>(targetPlayer) << _T("]"); debug(o.str());

	/* 手牌データをデバッグログに出力：ビフォー */
	logHand(_T("理牌前の手牌"), gameStat, targetPlayer);

	/* ソートの準備として牌のない枠を処理 */
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (!gameStat->Player[targetPlayer].Hand[i]) {
			gameStat->Player[targetPlayer].Hand[i] = Tile(TileCode::tilePad);
		}
	}

	/* ソート作業 */
	std::sort(gameStat->Player[targetPlayer].Hand, gameStat->Player[targetPlayer].Hand + NumOfTilesInHand);

	/* 空欄だったところは元に戻してあげましょう */
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == TileCode::tilePad) {
			gameStat->Player[targetPlayer].Hand[i] = Tile();
		}
	}

	/* 手牌データをデバッグログに出力：アフター */
	logHand(_T("理牌後の手牌"), gameStat, targetPlayer);

	/* 理牌完了！ */
	return;
}

namespace MoveTile {

/* 手動理牌同期用キュー */
static std::array<std::queue<std::pair<int, int> >, Players> moveTileQueue;

void enqueue(PlayerID targetPlayer, int from, int to) {
	{ CodeConv::tostringstream o;
		o.str(_T("")); o << _T("牌の移動予約 プレイヤー [") << static_cast<int>(targetPlayer)
			<< _T("] from[") << from << _T("] to [") << to << _T("]"); debug(o.str());
	}
	if ((from < 0) || (from >= NumOfTilesInHand))
		throw std::out_of_range("'from' out of range");
	if ((to < 0) || (to >= NumOfTilesInHand))
		throw std::out_of_range("'to' out of range");
	moveTileQueue.at(targetPlayer).push(std::make_pair(from, to));
}

std::pair<int, int> dequeue(PlayerID targetPlayer) {
	if (moveTileQueue.at(targetPlayer).empty())
		throw std::runtime_error("LIPAI queue is empty");
	auto val(moveTileQueue.at(targetPlayer).front());
	moveTileQueue.at(targetPlayer).pop();
	return val;
}

void for_each(const std::function<void(std::pair<int, int>)>& f, PlayerID targetPlayer, bool preserve) {
	std::queue<std::pair<int, int> > tmpQueue;
	try {
		std::pair<int, int> indices;
		while (!moveTileQueue.empty()) {
			indices = dequeue(targetPlayer);
			tmpQueue.push(indices);
			f(indices);
		}
	} catch (std::runtime_error&) {}
	if (preserve) {
		while (!tmpQueue.empty()) {
			auto indices(tmpQueue.front());
			tmpQueue.pop();
			enqueue(targetPlayer, indices.first, indices.second);
		}
	}
}

void apply(GameTable* const gameStat, PlayerID targetPlayer, bool preserve) {
	for_each([gameStat, targetPlayer](std::pair<int, int> indices) {
		moveTile(gameStat, targetPlayer, false, indices.first);
		moveTile(gameStat, targetPlayer, true, indices.second);
	}, targetPlayer, preserve);
}

/* 手動理牌処理 */
void moveTile(GameTable* const gameStat, PlayerID targetPlayer, bool execute, int tileIndex) {
	static int index = 0;

	CodeConv::tostringstream o;
	o.str(_T("")); o << _T("牌の移動 プレイヤー [") << static_cast<int>(targetPlayer)
		<< _T("] 実行[") << execute << _T("] 位置 [") << tileIndex << _T("]"); debug(o.str());

	if ((tileIndex < 0) || (tileIndex > TsumohaiIndex)) {
		warn(_T("位置指定が正しくありません。無視します。"));
		return;
	}

	if (execute) { // 実行モード
		/* 手牌データをデバッグログに出力：ビフォー */
		logHand(_T("移動前の手牌"), gameStat, targetPlayer);

		/* ここから移動作業 */
		/* index     == 移動前の位置 */
		/* tileIndex == 移動後の位置 */
		Tile tmpTile = gameStat->Player[targetPlayer].Hand[index]; // 退避用
		if (tileIndex < index) { // 前に移動
			for (int i = index; i > tileIndex; --i)
				gameStat->Player[targetPlayer].Hand[i] = gameStat->Player[targetPlayer].Hand[i - 1];
		} else if (tileIndex > index) { // 後ろに移動
			for (int i = index; i < tileIndex; ++i)
				gameStat->Player[targetPlayer].Hand[i] = gameStat->Player[targetPlayer].Hand[i + 1];
		}
		gameStat->Player[targetPlayer].Hand[tileIndex] = tmpTile;
		index = tileIndex;

		/* 手牌データをデバッグログに出力：アフター */
		logHand(_T("移動後の手牌"), gameStat, targetPlayer);
	} else { // 移動元指定
		index = tileIndex;
	}

	/* 移動完了！ */
	return;
}

} /* namespace */

/* 場に見えてる個数 */
MJCORE Int8ByTile countseentiles(const GameTable* const gameStat) {
	// 場に見えている牌の数を数える
	CodeConv::tostringstream o; // エラーメッセージ用

	Int8ByTile seenTiles; // 計算する牌
	memset(&seenTiles, 0, sizeof(seenTiles)); // まずは、初期化

	// 捨牌で見えてる枚数
	for (int i = 0; i < Players; i++) {
		if (gameStat->Player[i].DiscardPointer == 0) continue;
		for (int j = 1; j <= gameStat->Player[i].DiscardPointer; j++)
			if ((gameStat->Player[i].Discard[j].dstat != DiscardStat::taken) && // 鳴かれた牌をここで数えると
				(gameStat->Player[i].Discard[j].dstat != DiscardStat::riichiTaken)) // 二重にカウントされることになるので数えない
				seenTiles[gameStat->Player[i].Discard[j].tcode.tile]++;
	}

	// 誰かの副露で見えてる枚数
	for (int i = 0; i < Players; i++) {
		if (gameStat->Player[i].MeldPointer == 0) continue;
		for (int j = 1; j <= gameStat->Player[i].MeldPointer; j++) {
			switch (gameStat->Player[i].Meld[j].mstat) {
			case MeldStat::sequenceExposedLower:
			case MeldStat::sequenceExposedMiddle:
			case MeldStat::sequenceExposedUpper:
				// 明順子(チー)
				// まず、正しいコードになっているかを確認する(デバッグ時)
				assert(getTileNumber(gameStat->Player[i].Meld[j].tile) <= 7);
				assert(getTileNumber(gameStat->Player[i].Meld[j].tile) > 0);
				assert(Tile(gameStat->Player[i].Meld[j].tile).isNumber());
				// カウントアップ
				for (int k = static_cast<int>(gameStat->Player[i].Meld[j].tile);
					k <= static_cast<int>(gameStat->Player[i].Meld[j].tile) + 2; k++)
					seenTiles[gameStat->Player[i].Meld[j].tile]++;
				// ここまで
				break;
			case MeldStat::tripletExposedLeft:
			case MeldStat::tripletExposedCenter:
			case MeldStat::tripletExposedRight:
				// 明刻子(ポン)
				seenTiles[gameStat->Player[i].Meld[j].tile] += 3;
				break;
			case MeldStat::quadConcealed:
				// 暗槓
#ifdef GUOBIAO
				break;
#else /* GUOBIAO */
				if (RuleData::chkRule("ankan_conceal", "closed")) break; // 暗槓非開示ルールだったらカウントしない
				/* FALLTHRU */
#endif /* GUOBIAO */
			case MeldStat::quadExposedLeft:   case MeldStat::quadAddedLeft:
			case MeldStat::quadExposedCenter: case MeldStat::quadAddedCenter:
			case MeldStat::quadExposedRight:  case MeldStat::quadAddedRight:
				// 明槓
				seenTiles[gameStat->Player[i].Meld[j].tile] += 4;
				break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("副露データに暗順子、暗刻子、または不明な種類の面子が検出されました"));
			}
		}
	}

#ifndef GUOBIAO
	// ドラ表示牌で見えてる枚数
	for (int i = 0; i < 6; i++) {
		if (gameStat->chkGameType(GameTypeID::allSanma)) {
			if (gameStat->DoraPointer <= (102 - gameStat->ExtraRinshan - i * 2))
				seenTiles[gameStat->Deck[102 - gameStat->ExtraRinshan - i * 2].tile]++;
		} else {
			if (gameStat->DoraPointer <= (130 - i * 2))
				seenTiles[gameStat->Deck[130 - i * 2].tile]++;
		}
	}
#endif /* GUOBIAO */

	return seenTiles;
}

/* 手牌に存在する牌を種類別にカウントする */
MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PlayerID playerID) {
	// 手牌に存在する牌を種類別にカウントする（鳴き面子・暗槓は除く）
	Int8ByTile count; memset(&count, 0, sizeof(count)); TileCode tmpTC;
	if ((playerID < 0) || (playerID > Players)) return count;
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if ((tmpTC = gameStat->Player[playerID].Hand[i].tile) != TileCode::noTile)
			count[tmpTC]++;
	}
	return count;
}

/* 手牌に存在する赤ドラを種類別にカウントする */
MJCORE Int8ByTile countRedTilesInHand(const GameTable* const gameStat, PlayerID playerID, DoraCol doraCol) {
	assert((doraCol == DoraCol::akaDora)||(doraCol == DoraCol::aoDora));
	// 手牌に存在する牌を種類別にカウントする（鳴き面子・暗槓は除く）
	Int8ByTile count; memset(&count, 0, sizeof(count)); TileCode tmpTC;
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if ((tmpTC = TileCode(gameStat->Player[playerID].Hand[i].tile)) != TileCode::noTile)
			if (gameStat->Player[playerID].Hand[i].red == doraCol)
				count[tmpTC]++;
	}
	// 赤ドラのみ数えるときは便宜のため、鳴き面子も数える
	for (int i = 1; i < gameStat->Player[playerID].MeldPointer; i++) {
		switch (gameStat->Player[playerID].Meld[i].mstat) {
		case MeldStat::sequenceExposedLower: case MeldStat::sequenceExposedMiddle:
		case MeldStat::sequenceExposedUpper:
			// 順子の時
			if (gameStat->Player[playerID].Meld[i].red[0] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			if (gameStat->Player[playerID].Meld[i].red[1] == doraCol)
				count[offsetTileNumber(gameStat->Player[playerID].Meld[i].tile, 1)]++;
			if (gameStat->Player[playerID].Meld[i].red[2] == doraCol)
				count[offsetTileNumber(gameStat->Player[playerID].Meld[i].tile, 2)]++;
			break;
		case MeldStat::quadExposedLeft: case MeldStat::quadAddedLeft:
		case MeldStat::quadExposedCenter: case MeldStat::quadAddedCenter:
		case MeldStat::quadExposedRight: case MeldStat::quadAddedRight:
		case MeldStat::quadConcealed: // 暗槓も数えてあげましょう……
			// 槓子の時
			if (gameStat->Player[playerID].Meld[i].red[3] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			/* FALLTHRU */
		case MeldStat::tripletExposedLeft: case MeldStat::tripletExposedCenter:
		case MeldStat::tripletExposedRight:
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("副露データに暗順子、暗刻子、または不明な種類の面子が検出されました"));
		}
	}
	return count;
}


/* 手牌のうちある１枚に注目する */
MJCORE TileStatus gettilestatus(
	const GameTable* const gameStat, PlayerID targetPlayer, int targetTile, bool CheckMode)
{
	const TileCode* const theTile = &gameStat->Player[targetPlayer].Hand[targetTile].tile;

	TileStatus tlStat; memset(&tlStat, 0, sizeof(tlStat)); // 初期化
	if (*theTile == TileCode::noTile) return tlStat; // なかったら戻る

	tlStat.isExistent = true; // ここにたどり着いたら、それは存在する牌です
	const auto tlCount = countTilesInHand(gameStat, targetPlayer); // 牌を数えろ、話はそれからだ

	if (tlCount[*theTile] == 4) tlStat.canFormQuad = true; // 暗槓が可能な牌
	if (tlCount[*theTile] >= 3) tlStat.formsTriplet = true; // 暗刻を形成している場合
	
	if (Tile(*theTile).isNumber()) { // 順子を形成している場合
		if ((tlCount[*theTile] >= 1)&&(tlCount[offsetTileNumber(*theTile, 1)] >= 1)&&(tlCount[offsetTileNumber(*theTile, 2)] >= 1))
			tlStat.formsSequence = true;
		else if ((tlCount[*theTile] >= 1)&&(tlCount[offsetTileNumber(*theTile, 1)] >= 1)&&(tlCount[offsetTileNumber(*theTile, -1)] >= 1))
			tlStat.formsSequence = true;
		else if ((*theTile > static_cast<TileCode>(1))&&(tlCount[*theTile] >= 1)&&(tlCount[offsetTileNumber(*theTile, -2)] >= 1)&&(tlCount[offsetTileNumber(*theTile, -1)] >= 1))
			tlStat.formsSequence = true;
	}

	if ((tlCount[*theTile] >= 2)&&(!tlStat.formsTriplet)) tlStat.formsPair = true; // 対子を形成している場合

	if (Tile(*theTile).isNumber()) { // 辺張を形成している場合
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[offsetTileNumber(*theTile, 1)] >= 1)&&(getTileNumber(*theTile) == 1))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[offsetTileNumber(*theTile, 1)] >= 1)&&(getTileNumber(*theTile) == 8))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[offsetTileNumber(*theTile, -1)] >= 1)&&(getTileNumber(*theTile) == 2))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[offsetTileNumber(*theTile, -1)] >= 1)&&(getTileNumber(*theTile) == 9))
				tlStat.seqSingleSideWait = true;
		}
	}
	
	if (Tile(*theTile).isNumber()) { // 両面塔子を形成している場合
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[offsetTileNumber(*theTile, 1)] >= 1)&&(getTileNumber(*theTile) != 1)&&
				(getTileNumber(*theTile) != 8)) tlStat.seqDoubleSideWait = true;
			else if ((tlCount[offsetTileNumber(*theTile, -1)] >= 1)&&(getTileNumber(*theTile) != 2)&&
				(getTileNumber(*theTile) != 9)) tlStat.seqDoubleSideWait = true;
		}
	}
	
	if (Tile(*theTile).isNumber()) { // 嵌張を形成している場合
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[offsetTileNumber(*theTile, 2)] >= 1)&&(getTileNumber(*theTile) != 9))
				tlStat.seqMidWait = true;
			else if ((*theTile > static_cast<TileCode>(1))&&(tlCount[offsetTileNumber(*theTile, -2)] >= 1)&&(getTileNumber(*theTile) != 1))
				tlStat.seqMidWait = true;
		}
	}

	return tlStat;
}

/* 振聴でないかのチェック・待ち牌を数える処理も含む */
MJCORE MachihaiInfo chkFuriten(const GameTable* const gameStat, PlayerID targetPlayer) {
	trace(_T("振聴かどうか・待ち牌を調べます。"));
	const Int8ByTile seenTiles = countseentiles(gameStat); // 見えてる牌の数を数える
	const Int8ByTile countTiles = countTilesInHand(gameStat, targetPlayer); // 手の内の牌の数を数える
	MachihaiInfo machihaiInfo; memset(&machihaiInfo, 0, sizeof(machihaiInfo)); // 初期化
	GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(GameTable)); // テンポラリ卓情報オブジェクト(ここからは仮定法の世界……)

	for (auto i : AllTiles) {
		tmpGameStat.Player[targetPlayer].Tsumohai().tile = i;
		if (ShantenAnalyzer::calcShanten(&tmpGameStat, targetPlayer, ShantenType::all) == -1) { // 待ちになっていたら
			machihaiInfo.MachiMen++; machihaiInfo.Machihai[i].MachihaiFlag = true; // フラグをセットしましょう
			machihaiInfo.MachihaiTotal += // カウントを加算しましょう
				(machihaiInfo.Machihai[i].MachihaiCount =
				(4 - seenTiles[i] - countTiles[i]));
			// フリテンかどうかを判断する
			for (int j = 1; j <= tmpGameStat.Player[targetPlayer].DiscardPointer; j++) {
				if (tmpGameStat.Player[targetPlayer].Discard[j].tcode.tile == i) // 同じ種類の捨て牌が見つかったら
					machihaiInfo.FuritenFlag = true; // フリテンと判断します
			}
			if (RuleData::chkRuleApplied("kakan_furiten")) { // 加槓の牌をフリテンとみなすルールなら
				for (int j = 1; j <= tmpGameStat.Player[targetPlayer].MeldPointer; j++) {
					switch (tmpGameStat.Player[targetPlayer].Meld[j].mstat) {
					case MeldStat::quadAddedLeft: case MeldStat::quadAddedCenter: case MeldStat::quadAddedRight: // 加槓で、なおかつ
						if (tmpGameStat.Player[targetPlayer].Meld[j].tile == i) // 同じ種類の捨て牌が見つかったら
							machihaiInfo.FuritenFlag = true; // フリテンと判断します
						break;
					}
				}
			}
		}
	}

	// オープン立直者の手牌も数える
	for (int p = 0; p < Players; p++) {
		if ((p != targetPlayer) && (gameStat->Player[p].RichiFlag.OpenFlag)) { // 自分以外のオープンリーチ者の時、
			for (int i = 0; i < NumOfTilesInHand; i++) { // それぞれの手牌について、
				if (gameStat->Player[p].Hand[i]) { // ブランクではなくて、
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

/* オープン立直の待ち牌 */
void chkOpenMachi(GameTable* const gameStat, PlayerID targetPlayer) {
	// オープンリーチの待ち牌情報を更新する
	assert(!gameStat->Player[targetPlayer].Tsumohai());
	for (auto i : AllTiles) {
		/* まずは、ある牌をツモったと仮定します */
		gameStat->Player[targetPlayer].Tsumohai().tile = i;
		/* もしそれが和了になっていたら、フラグをセットしましょう */
		if (ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenType::all) == -1)
			gameStat->OpenRichiWait[i] = true;
		/* これをすべての牌について試行しましょう */
	}
	/* もとに戻すのを忘れないようにしましょう。さもなくば多牌になってしまいます */
	gameStat->Player[targetPlayer].Tsumohai().tile = TileCode::noTile;

	/* これで処理が終わりました。戻りましょう */
	return;
}

/* 九種九牌流しが可能かどうかのチェック */
MJCORE bool chkdaopaiability(const GameTable* const gameStat, PlayerID targetPlayer) {
	if (RuleData::chkRule("nine_terminals", "continue")) return false; // 九種九牌なしのルールだったら戻る
	Int8ByTile TileCount = countTilesInHand(gameStat, targetPlayer);
	int YaojiuCount = 0; bool AtamaFlag = false;
	for (auto tile : Honor_Major_Tiles) // ヤオ九牌１種類につき、１をカウントする。
		if (TileCount[tile] >= 1) YaojiuCount++;
	return (YaojiuCount >= 9);
}

/* ドラを設定する */
namespace setdora_tools {
	TileCode getNextOf(const GameTable* const gameStat, TileCode tc) { // ネクスト牌
		TileCode ans = static_cast<TileCode>(static_cast<int>(tc) + 1);
		if ((gameStat->chkGameType(GameTypeID::sanmaX))&&(ans == TileCode::characterTwo)) ans = TileCode::characterNine;
		else if ((gameStat->chkGameType(GameTypeID::sanmaSeto))&&(ans == TileCode::bambooTwo)) ans = TileCode::bambooNine;
		else if (ans == static_cast<TileCode>(10)) ans = TileCode::characterOne;
		else if (ans == static_cast<TileCode>(20)) ans = TileCode::circleOne;
		else if (ans == static_cast<TileCode>(30)) ans = TileCode::bambooOne;
		else if (ans == static_cast<TileCode>(35)) ans = TileCode::eastWind;
		else if (ans == static_cast<TileCode>(38)) ans = TileCode::whiteDragon;
		return ans;
	}

	TileCode getPrevOf(const GameTable* const gameStat, TileCode tc) { // 前の牌
		TileCode ans = static_cast<TileCode>(static_cast<int>(tc) - 1);
		if ((gameStat->chkGameType(GameTypeID::sanmaX))&&(ans == TileCode::characterEight)) ans = TileCode::characterOne;
		else if ((gameStat->chkGameType(GameTypeID::sanmaSeto))&&(ans == TileCode::bambooEight)) ans = TileCode::bambooOne;
		else if (ans == static_cast<TileCode>(0)) ans = TileCode::characterNine;
		else if (ans == static_cast<TileCode>(10)) ans = TileCode::circleNine;
		else if (ans == static_cast<TileCode>(20)) ans = TileCode::bambooNine;
		else if (ans == static_cast<TileCode>(30)) ans = TileCode::northWind;
		else if (ans == static_cast<TileCode>(34)) ans = TileCode::redDragon;
		return ans;
	}

	void addDora(GameTable* const gameStat, TileCode tc, int Mode) {
		for (int i = (RuleData::chkRuleApplied("nagatacho") ? static_cast<int>(tc) % 10 : static_cast<int>(tc));
			i <= ((RuleData::chkRuleApplied("nagatacho") && Tile(tc).isNumber()) ? static_cast<int>(TileSuit::honors) : static_cast<int>(tc));
			i += 10) {
				CodeConv::tostringstream o;
				if (Mode) gameStat->DoraFlag.Ura[i]++;	// ドラを設定する
				else gameStat->DoraFlag.Omote[i]++;	// ドラを設定する
				o << _T("牌コード [") << static_cast<int>(tc) << _T("] をドラにしました。");
				debug(o.str().c_str());
				if (Mode) haifu::haifurecuradora(static_cast<TileCode>(i));
				else haifu::haifurecdora(static_cast<TileCode>(i));
		}
	}
}

void setdora(GameTable* const gameStat, int Mode) {
	CodeConv::tostringstream o;
	o << _T("ドラの追加 ポインタ [") << gameStat->DoraPointer <<
		_T("] 牌コード [") << static_cast<int>(gameStat->Deck[gameStat->DoraPointer + Mode].tile) <<
		_T("] モード [") << Mode << _T("]");
	debug(o.str().c_str());
	if (gameStat->Deck[gameStat->DoraPointer + Mode].isFlower()) {
		// 花牌がドラ表示牌になったとき
		setdora_tools::addDora(gameStat, TileCode::flower, Mode);
	} else {
		if (RuleData::chkRule("dora_indicator", "dora_around_indicator")) {
			// 前の牌がドラ（超インフレ用）
			if ((gameStat->Deck[gameStat->DoraPointer + Mode].getSuit() != TileSuit::characters) ||
				(!gameStat->chkGameType(GameTypeID::sanmaX)))
					setdora_tools::addDora(gameStat,
						setdora_tools::getPrevOf(gameStat, gameStat->Deck[gameStat->DoraPointer + Mode].tile),
						Mode);
		}
		if ((RuleData::chkRule("dora_indicator", "dora_indicator_itself"))||(RuleData::chkRule("dora_indicator", "dora_around_indicator"))) {
			// 現物ドラ
			setdora_tools::addDora(gameStat,
				gameStat->Deck[gameStat->DoraPointer + Mode].tile,
				Mode);
		}
		if (!RuleData::chkRule("dora_indicator", "dora_indicator_itself")) {
			// ネクストドラ
			setdora_tools::addDora(gameStat,
				setdora_tools::getNextOf(gameStat, gameStat->Deck[gameStat->DoraPointer + Mode].tile),
				Mode);
		}
	}
}

/* 立直後の暗槓の可否 */
namespace chkAnkanAbilityTools { // chkAnkanAbility関数用の処理
	bool CheckForNumOfTiles(const GameTable* const gameStat, PlayerID targetPlayer) {
		/* ツモった牌が４枚揃ってないなら当然に槓は不可能 */
		/* 立直後であり送り槓はできないのでツモった牌だけ調べればよい */
		CodeConv::tostringstream o;
		Int8ByTile tlCount = countTilesInHand(gameStat, targetPlayer);
		if (tlCount[gameStat->Player[targetPlayer].Tsumohai().tile] < 4) {
			o.str(_T("")); o << _T("ツモ牌 [") << std::setw(2) << std::setfill(_T('0')) <<
				static_cast<int>(gameStat->Player[targetPlayer].Tsumohai().tile) <<
				_T("] は、4枚揃っていません。"); debug(o.str());
			return true;
		} else {
			o.str(_T("")); o << _T("ツモ牌 [") << std::setw(2) << std::setfill(_T('0')) <<
				static_cast<int>(gameStat->Player[targetPlayer].Tsumohai().tile) <<
				_T("] は、手牌に合わせて4枚あります。"); debug(o.str());
			return false;
		}
	}
	bool CheckForAtama(const GameTable* const gameStat, PlayerID targetPlayer) {
		/* ツモった牌が雀頭になりうるか調べる */
		CodeConv::tostringstream o;
		GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(tmpGameStat));

		// 2枚除去
		int tmpTakenCount = 0;
		for (int i = 0; i < (NumOfTilesInHand-1); i++) {
			if (tmpGameStat.Player[targetPlayer].Hand[i].tile ==
				tmpGameStat.Player[targetPlayer].Tsumohai().tile) {
					tmpGameStat.Player[targetPlayer].Hand[i] = Tile();
					tmpTakenCount++;
			}
			if (tmpTakenCount == 2) break;
		}
		tmpGameStat.Player[targetPlayer].Tsumohai() = Tile();
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
		Shanten shanten = ShantenAnalyzer::calcShanten(&tmpGameStat, targetPlayer, ShantenType::all);
		if (shanten == 1) {
			o.str(_T("")); o << _T("ツモ牌 [") << std::setw(2) << std::setfill(_T('0')) <<
				static_cast<int>(gameStat->Player[targetPlayer].Tsumohai().tile) <<
				_T("] は、アタマ候補です。"); debug(o.str());
			return true;
		} else {
			o.str(_T("")); o << _T("ツモ牌 [") << std::setw(2) << std::setfill(_T('0')) <<
				static_cast<int>(gameStat->Player[targetPlayer].Tsumohai().tile) <<
				_T("] は、アタマ候補ではありません。"); debug(o.str());
			return false;
		}
	}
	bool CheckIfMachihaiMatches(const GameTable* const gameStat, PlayerID targetPlayer) {
		/* 待ち牌が一致するかどうか調べる */
		CodeConv::tostringstream o;

		/* 暗槓する前の待ち牌を調べる */
		MachihaiInfo machiInfo_Before = chkFuriten(gameStat, targetPlayer);
		/* 暗槓したあとの待ち牌を調べる */
		int tmpTakenCount = 0; // 牌の数の確認用
		GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(GameTable));
		for (int i = 0; i < NumOfTilesInHand; i++) {
			if (tmpGameStat.Player[targetPlayer].Hand[i].tile ==
				tmpGameStat.Player[targetPlayer].Tsumohai().tile) {
					tmpGameStat.Player[targetPlayer].Hand[i] = Tile();
					tmpTakenCount++;
			}
		}
		assert(tmpTakenCount == 4); // デバッグ用：ちょうど4枚だったか確認
		tmpGameStat.Player[targetPlayer].Meld[++ tmpGameStat.Player[targetPlayer].MeldPointer].tile =
			gameStat->Player[targetPlayer].Tsumohai().tile; /* ツモった牌を */
		tmpGameStat.Player[targetPlayer].Meld[tmpGameStat.Player[targetPlayer].MeldPointer].mstat =
			MeldStat::quadConcealed; /* 暗槓したとみなす */
		MachihaiInfo machiInfo_After = chkFuriten(&tmpGameStat, targetPlayer);

		/* 待ち牌は一致するか？ */
		bool machihaiDiffers = false;
		for (int i = 0; i < TileNonflowerMax; i++)
			if (machiInfo_Before.Machihai[i].MachihaiFlag != machiInfo_After.Machihai[i].MachihaiFlag)
				machihaiDiffers = true;
		if (machihaiDiffers) { // 待ち牌が一致しないなら、槓できない
			debug(_T("槓の前後で待ち牌が一致しません。"));
			return false;
		} else { // 条件を満たす暗槓
			debug(_T("槓の前後で待ち牌が一致しました。"));
			return true;
		}
	}
}

MJCORE bool chkAnkanAbility(const GameTable* const gameStat, PlayerID targetPlayer) {
	// リーチ後の暗槓ができるかどうかを判定する

	/*
	 * リーチ後の暗槓が許される条件：
	 * 1) 送り槓(例えば555567と持っている時8を自摸って5を槓すること)をしてはいけない
	 * 2) 面子構成が変わってはいけない
	 * 条件(2)を実装しやすいように書き換えると次のようになる：
	 * 2a) 雀頭候補となる牌(例：22234という形における2)を暗槓してはいけない
	 * 2b) 待ちが変わってはいけない
	 */

	CodeConv::tostringstream o;
	o.str(_T("")); o << _T("暗槓の可否を判定します。プレイヤー [") << static_cast<int>(targetPlayer) << _T("]"); debug(o.str());

	/* ツモった牌が４枚揃ってないなら当然に槓は不可能 */
	/* 立直後であり送り槓はできないのでツモった牌だけ調べればよい */
	if (chkAnkanAbilityTools::CheckForNumOfTiles(gameStat, targetPlayer)) return false;

	/* 字牌に順子はないのでこれ以降のチェックをせずとも槓が可能とわかる */
	if      (gameStat->Player[targetPlayer].Tsumohai().isHonor())  return true;  // 字牌
	else if (gameStat->Player[targetPlayer].Tsumohai().isFlower()) return false; // 花牌

	/* ツモった牌が雀頭になりうるなら戻る */
	if (chkAnkanAbilityTools::CheckForAtama(gameStat, targetPlayer)) return false;

	/* 待ち牌が一致するかどうか調べる */
	return (chkAnkanAbilityTools::CheckIfMachihaiMatches(gameStat, targetPlayer));
}

/* 導火線の位置を調べる */
void calcdoukasen(GameTable* const gameStat) {
	/* 導火線の位置を計算する */
	if (RuleData::chkRuleApplied("doukasen")) {
		if (gameStat->chkGameType(GameTypeID::sanma4)) {
			PlayerID* tmpDoukasen = new PlayerID(
				((30 - ((gameStat->diceSum() - 1) * 36 * 2 + 
				(gameStat->diceSum() + gameStat->diceSum2()) * 2 + gameStat->TilePointer - 1) / 36) + 30) % 3);
			gameStat->DoukasenPlayer = tobePlayed(gameStat, *tmpDoukasen);
			delete tmpDoukasen;
		} else if (gameStat->chkGameType(GameTypeID::sanmaT)) {
			gameStat->DoukasenPlayer =
				((30 - ((gameStat->diceSum() - 1 +
				(gameStat->GameRound - (gameStat->GameRound / 4))) * 36 * 2 +
				(gameStat->diceSum() + gameStat->diceSum2()) * 2 + gameStat->TilePointer - 1) / 36) + 30) % 3;
		} else {
			int* tmp;
			if (RuleData::chkRule("flower_tiles", "8tiles"))
				gameStat->DoukasenPlayer =
					((40 - ((gameStat->diceSum() - 1 +
					(gameStat->GameRound % Players)) * 36 * 3 + 
					(gameStat->diceSum() + gameStat->diceSum2()) * 2 + gameStat->TilePointer - 1) / 36) + 40)
					% Players;
			else if (RuleData::chkRule("flower_tiles", "no"))
				gameStat->DoukasenPlayer =
					((40 - ((gameStat->diceSum() - 1 +
					(gameStat->GameRound % Players)) * 34 * 3 +
					(gameStat->diceSum() + gameStat->diceSum2()) * 2 + gameStat->TilePointer - 1) / 34) + 40)
					% Players;
			else {
				gameStat->DoukasenPlayer =
					((40 - ((gameStat->diceSum() - 1 +
					(gameStat->GameRound % Players)) * 70 * 3 / 2 +
					(gameStat->diceSum() + gameStat->diceSum2()) * 2 + gameStat->TilePointer - 1) / 35) + 40)
					% Players;
				tmp = new int(
					((gameStat->diceSum() - 1 + (gameStat->GameRound % Players)) * 70 * 3 / 2 +
					(gameStat->diceSum() + gameStat->diceSum2()) * 2 + gameStat->TilePointer - 1) % 70
					);
				if (*tmp == (((gameStat->diceSum() + gameStat->GameRound) % 2) * 35))
					gameStat->DoukasenPlayer = (40 + gameStat->DoukasenPlayer + 1) % Players;
				delete tmp;
			}
		}
	}
}

/* 聴牌かどうか調べる */
bool isTenpai(const GameTable* const gameStat, PlayerID targetPlayer) {
	Shanten shanten = ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenType::all);
	if (gameStat->Player[targetPlayer].AgariHouki) shanten = 1; // アガリ放棄なら強制不聴
	if (EnvTable::Instantiate()->PlayerDat[targetPlayer].RemotePlayerFlag == -1)
		return false;
	return (shanten <= 0);
}

/* 流し満貫かどうか調べる */
bool isNagashiMangan(const GameTable* const gameStat, PlayerID targetPlayer) {
	if (gameStat->Player[targetPlayer].AgariHouki) return false; // 和了り放棄時の処理→流し満貫を認めない
	if (EnvTable::Instantiate()->PlayerDat[targetPlayer].RemotePlayerFlag == -1)
		return false;
	int YaojiuSutehai = 0;
	for (int i = 1; i <= gameStat->Player[targetPlayer].DiscardPointer; i++) {
		// 鳴かれた牌だったら無視
		if (gameStat->Player[targetPlayer].Discard[i].dstat == DiscardStat::taken) continue;
		if (gameStat->Player[targetPlayer].Discard[i].dstat == DiscardStat::riichiTaken) continue;
		// 鳴かれていない牌
		if (isYaojiu(gameStat->Player[targetPlayer].Discard[i].tcode.tile)) YaojiuSutehai++;
	}
	// 全部該当する牌だったらtrue
	return (YaojiuSutehai == gameStat->Player[targetPlayer].DiscardPointer);
}

/* 片和りかどうか調べる */
bool isKataagari(const GameTable* gameStat, PlayerID targetPlayer) {
	MachihaiInfo machiInfo = chkFuriten(gameStat, targetPlayer);
	GameTable tmpStat; memcpy(&tmpStat, gameStat, sizeof tmpStat);
	for (int i = 0; i < TileNonflowerMax; ++i) {
		if (machiInfo.Machihai[i].MachihaiFlag) {
			tmpStat.Player[targetPlayer].Hand[TsumohaiIndex].tile = static_cast<TileCode>(i);
			yaku::YAKUSTAT yakuStat = yaku::yakuCalculator::countyaku(&tmpStat, targetPlayer);
			if (!yaku::yakuCalculator::checkShibari(&tmpStat, &yakuStat))
				return true;
		}
	}
	return false;
}
