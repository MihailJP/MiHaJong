#include "agari.h"

#include "func.h"

// -------------------------------------------------------------------------

/* 青天井対応の点棒処理 */
LNum endround::agari::agaricalc(const LNum& AgariPointRaw, int agariBairitsu, int agariBairitsu2, unsigned agariCount) {
	LNum agariPointArray = (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	for (unsigned i = 0; i < agariCount; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu2 + 99) / 100 * 100;
	return agariPointArray;
}
LNum endround::agari::agaricalc(const LNum& AgariPointRaw, rat agariBairitsu, rat agariBairitsu2, int agariCount, rat agariBairitsu3, int agariCount2) {
	LNum agariPointArray = (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	for (unsigned i = 0; i < agariCount; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu2.getNumerator() / agariBairitsu2.getDenominator() + 99) / 100 * 100;
	for (unsigned i = 0; i < agariCount2; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu3.getNumerator() / agariBairitsu3.getDenominator() + 99) / 100 * 100;
	return agariPointArray;
}

// -------------------------------------------------------------------------

namespace {
	using namespace endround::agari;

	void calcAgariPoints_Pao( // 包適用時
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int Mode)
	{
		const PLAYER_ID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : (PLAYER_ID)Mode;
		const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
		if (playerwind(AgariPlayer, gameStat->GameRound) == sEast) { // 親の和了り
			if (TsumoAgari) { // ツモアガリ(包の人が一人払い)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
				}
			} else { // ロンアガリ(包の人と振り込んだ人で折半)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 3, 3, 1);
						deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
						deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
					}
					if (cnt == gameStat->CurrentPlayer.Furikomi)
						deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
				}
			}
		} else {
			// 子の和了り
			if (TsumoAgari) { // ツモアガリ(包の人が一人払い)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 4, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 4, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 4, cnt);
				}
			} else { // ロンアガリ(包の人と振り込んだ人で折半)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
						deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
						deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					}
					if (cnt == gameStat->CurrentPlayer.Furikomi)
						deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
				}
			}
		}
	}

	void calcAgariPoints_Ron( // 通常時：ロン
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PLAYER_ID AgariPlayer)
	{
		if (playerwind(AgariPlayer, gameStat->GameRound) == sEast) { // 親の和了り
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
				}
				if (cnt == gameStat->CurrentPlayer.Furikomi)
					deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
			}
		} else { // 子の和了り
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 4, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, 4, cnt);
				}
				if (cnt == gameStat->CurrentPlayer.Furikomi)
					deltacalcminus(AgariPointRaw, PointDelta, 4, cnt);
			}
		}
	}

	void calcAgariPoints_Tsumo_Dealer( // 通常時：親のツモ
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PLAYER_ID AgariPlayer)
	{
		if (chkGameType(gameStat, Yonma) || (chkGameType(gameStat, Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// 四麻式ルール
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
				}
				else
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_omitted") || RuleData::chkRule("tsumo_payment", "add_1000")) {
			// 三麻自摸減りルール
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) { // 千点加符だったら
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else { // 三麻でツモ減りしないルール
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 3, 3, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
			}
		}
	}

	void calcAgariPoints_Tsumo_NonDealer( // 通常時：子のツモ
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PLAYER_ID AgariPlayer)
	{
		if (chkGameType(gameStat, Yonma) || (chkGameType(gameStat, Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// 四麻式ルール
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agaricalc(AgariPointRaw, 2, 1, 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
				}
				else if (playerwind(cnt, gameStat->GameRound) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
				else
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "fifty_fifty")) {
			// 三麻：親子関係なく折半ルール
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
				}
				else if (playerwind(cnt, gameStat->GameRound) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_omitted") || RuleData::chkRule("tsumo_payment", "add_1000")) {
			// 三麻：自摸減りルール、千点加符ルール
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) {
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (playerwind(cnt, gameStat->GameRound) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else if (RuleData::chkRule("tsumo_payment", "adjusted_payment")) {
			// 三麻：丸取りルール
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, Rat(8, 3), Rat(4, 3), 1, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
				}
				else if (playerwind(cnt, gameStat->GameRound) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
				else if (playerwind(cnt, gameStat->GameRound) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_halved")) {
			// 三麻：北家の分を折半
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1, Rat(1, 2), 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (playerwind(cnt, gameStat->GameRound) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcminus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcminus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				}
			}
		}
	}
}

void endround::agari::calcAgariPoints(
	const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw,
	InfoByPlayer<LNum>& PointDelta, int Mode)
{
	const PLAYER_ID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : (PLAYER_ID)Mode;
	const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
	if (isPaoAgari(gameStat, AgariPlayer)) // 包適用時
		calcAgariPoints_Pao(gameStat, agariPoint, AgariPointRaw, PointDelta, Mode);
	else if ((TsumoAgari) && (playerwind(AgariPlayer, gameStat->GameRound) == sEast)) // 通常時：親のツモアガリ
		calcAgariPoints_Tsumo_Dealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	else if (TsumoAgari) // 通常時：子のツモアガリ
		calcAgariPoints_Tsumo_NonDealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer); // PLACEHOLDER
	else // 通常時：ロンアガリ
		calcAgariPoints_Ron(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	return;
}

// -------------------------------------------------------------------------
