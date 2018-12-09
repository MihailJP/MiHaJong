#include "result.h"
#include "regex.h"
#include "../common/largenum.h"
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "sound.h"
#include "func.h"
#include "rankval.h"
#include "ruletbl.h"
#include "haifu.h"

namespace {

	/* 最後まで残っていた供託点棒はトップ取り */
	void withdrawDepoScore(GameTable* gameStat) {
		const PlayerRankList rank(calcRank(gameStat));
		for (PlayerID i = 0; i < Players; ++i) {
			if (rank[i] == 1) {
				gameStat->Player[i].PlayerScore += gameStat->Deposit * 1000;
				gameStat->Deposit = 0;
			}
		}
		return;
	}

	/* 1000で割って丸める */
	LargeNum roundScore(LargeNum val) {
#ifdef GUOBIAO
		return val;
#else /* GUOBIAO */
		/* TODO: ここに丸め処理を書く */
		return val / 1000;
#endif /* GUOBIAO */
	}

	/* ウマ計算 */
	void calcUma(const GameTable* gameStat, InfoByPlayer<LargeNum>& score) {
#ifndef GUOBIAO
		const PlayerRankList rank(calcRank(gameStat));
		int plusCount = 0;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isAboveBase(gameStat, i))
				++plusCount;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			score[i] += RankVal::Instantiate()->getRankVal(gameStat,
			RuleData::chkRule("point_basis"), plusCount, rank[i]) * 1000;
#endif /* GUOBIAO */
	}

	/* オカ計算 */
	void calcOka(const GameTable* gameStat, InfoByPlayer<LargeNum>& score) {
		const PlayerRankList rank(calcRank(gameStat));
		LargeNum sumOfScore = 0; // 不足分または過剰分
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			sumOfScore += score[i];
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) {
			if ((rank[i] == 1) && (sumOfScore <= (LargeNum)0)) // トップ賞
				score[i] -= sumOfScore;
			if ((rank[i] == ACTUAL_PLAYERS) && (sumOfScore > (LargeNum)0)) // マイナスオカ
				score[i] -= sumOfScore;
		}
	}

	/* チップレート */
	unsigned int chipRate() {
#ifdef GUOBIAO
		return 0;
#else /* GUOBIAO */
		if (RuleData::chkRule("chip", "no")) return 0; // チップなしルール
		const std::string chipRule(RuleData::chkRule("chip"));
		REGEX::smatch matchDat;
		if (REGEX::regex_match(chipRule, matchDat, REGEX::regex("chip_rate_(\\d+)"))) { // チップレート
			return atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
		} else { // 異常データ？
			CodeConv::tostringstream o;
			o << _T("認識できないチップレート設定です [") <<
				CodeConv::EnsureTStr(chipRule) << _T(']');
			warn(o.str().c_str());
			return 0;
		}
#endif /* GUOBIAO */
	}

	/* 得点計算 */
	void calcScore(const GameTable* gameStat) {
		InfoByPlayer<LargeNum> playerScore;
		for (PlayerID i = 0; i < Players; ++i) // 点数処理
			playerScore[i] = gameStat->Player[i].PlayerScore - (LargeNum)BasePoint();
		for (PlayerID i = 0; i < Players; ++i) // チップを反映
			playerScore[i] += gameStat->Player[i].playerChip * (signed)chipRate();
		calcUma(gameStat, playerScore); // ウマを加算する
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) // 丸め処理
			playerScore[i] = roundScore(playerScore[i]);
		calcOka(gameStat, playerScore); // オカを加算する
		mihajong_graphic::setFinalScore( // 表示処理用DLLに渡す
			playerScore[0], playerScore[1], playerScore[2], playerScore[3]);
	}

	/* 焼き鳥 */
	void yakitori(GameTable* gameStat) {
#ifndef GUOBIAO
		const PlayerRankList rank(calcRank(gameStat));
		const std::string yakitoriRule(RuleData::chkRule("yakitori"));
		const PlayerID winner = [rank]() -> PlayerID {
			for (PlayerID i = 0; i < Players; ++i) // Who is the winner?
				if (rank[i] == 1)                  // Are you the winner?
					return i;                      // This is the winner!
			return -1;                             // Nobody won: this shouldn't be occur...
		} ();
		REGEX::smatch matchDat;
		if (REGEX::regex_match(yakitoriRule, matchDat, REGEX::regex("(\\d+)pts"))) { // 点棒で支払う場合
			int yakitoriVal = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
			for (PlayerID i = 0; i < Players; ++i) {
				if (gameStat->Player[i].YakitoriFlag) {
					gameStat->Player[i].PlayerScore -= yakitoriVal;
					gameStat->Player[winner].PlayerScore += yakitoriVal;
				}
			}
		} else if (REGEX::regex_match(yakitoriRule, matchDat, REGEX::regex("chip(\\d+)"))) { // チップで支払う場合
			int yakitoriVal = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
			for (PlayerID i = 0; i < Players; ++i) {
				if (gameStat->Player[i].YakitoriFlag) {
					gameStat->Player[i].playerChip -= yakitoriVal;
					gameStat->Player[winner].playerChip += yakitoriVal;
				}
			}
		}
#endif /* GUOBIAO */
	}

}

void gameResult(GameTable* gameStat, int origTurn, int origHonba) {
	sound::util::bgmstop();
	withdrawDepoScore(gameStat); // 供託点棒の処理
	yakitori(gameStat); // 焼き鳥の処理
	mihajong_graphic::GameStatus::updateGameStat(gameStat); // 反映させる
	calcScore(gameStat);
	/*  */
	mihajong_graphic::Transit(mihajong_graphic::sceneResult);
	haifu::haifusave(gameStat); // 牌譜を保存する
	(void)mihajong_graphic::ui::WaitUI(); // 入力待ち
}
