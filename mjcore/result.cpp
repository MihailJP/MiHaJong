#include "result.h"
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "sound.h"
#include "func.h"
#include "largenum.h"
#include "rankval.h"
#include "ruletbl.h"
#include "haifu.h"

namespace {

	/* 最後まで残っていた供託点棒はトップ取り */
	void withdrawDepoScore(GameTable* gameStat) {
		PlayerRankList rank(calcRank(gameStat));
		for (PlayerID i = 0; i < Players; ++i) {
			if (rank[i] == 1) {
				gameStat->Player[i].PlayerScore += gameStat->Deposit * 1000;
				gameStat->Deposit = 0;
			}
		}
		return;
	}

	/* 1000で割って丸める */
	LNum roundScore(LNum val) {
		/* TODO: ここに丸め処理を書く */
		return val / 1000;
	}

	/* ウマ計算 */
	void calcUma(const GameTable* gameStat, InfoByPlayer<LNum>& score) {
		PlayerRankList rank(calcRank(gameStat));
		int plusCount = 0;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isAboveBase(gameStat, i))
				++plusCount;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			score[i] += RankVal::Instantiate()->getRankVal(gameStat,
			RuleData::chkRule("point_basis"), plusCount, rank[i]) * 1000;
	}

	/* オカ計算 */
	void calcOka(const GameTable* gameStat, InfoByPlayer<LNum>& score) {
		PlayerRankList rank(calcRank(gameStat));
		LNum sumOfScore = 0; // 不足分または過剰分
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			sumOfScore += score[i];
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) {
			if ((rank[i] == 1) && (sumOfScore <= (LNum)0)) // トップ賞
				score[i] -= sumOfScore;
			if ((rank[i] == ACTUAL_PLAYERS) && (sumOfScore > (LNum)0)) // マイナスオカ
				score[i] -= sumOfScore;
		}
	}

	/* 得点計算 */
	void calcScore(const GameTable* gameStat) {
		InfoByPlayer<LNum> playerScore;
		for (PlayerID i = 0; i < Players; ++i)
			playerScore[i] = gameStat->Player[i].PlayerScore - BasePoint();
		calcUma(gameStat, playerScore); // ウマを加算する
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) // 丸め処理
			playerScore[i] = roundScore(playerScore[i]);
		/* TODO: チップの反映 */
		calcOka(gameStat, playerScore); // オカを加算する
		mihajong_graphic::setFinalScore( // 表示処理用DLLに渡す
			playerScore[0], playerScore[1], playerScore[2], playerScore[3]);
	}
}

void gameResult(GameTable* gameStat, int origTurn, int origHonba) {
	sound::util::bgmstop(); /* TODO: BGM選択 */
	withdrawDepoScore(gameStat); // 供託点棒の処理
	mihajong_graphic::GameStatus::updateGameStat(gameStat); // 反映させる
	calcScore(gameStat);
	/*  */
	mihajong_graphic::Transit(mihajong_graphic::sceneResult);
	haifu::haifusave(gameStat); // 牌譜を保存する
	/* TODO: 終了時の処理 */
	/* TODO: 入力待ち
	buttonPressed = 0
	repeat
		redrscreen
		await 1000
		if (buttonPressed) {break}
	loop
	*/
	Sleep(INFINITE);
}
