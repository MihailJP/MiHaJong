#include "agari.h"

#include <functional>
#include <regex>
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "func.h"
#include "haifu.h"
#include "envtbl.h"
#include "endround.h"
#include "ruletbl.h"
#include "tileutil.h"
#include <cassert>

// -------------------------------------------------------------------------

/* 青天井対応の点棒処理 */
#ifndef GUOBIAO
LargeNum endround::agari::agaricalc(const LargeNum& AgariPointRaw, int agariBairitsu, int agariBairitsu2, unsigned agariCount) {
	LargeNum agariPointArray = (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	for (unsigned i = 0; i < agariCount; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu2 + 99) / 100 * 100;
	return agariPointArray;
}
LargeNum endround::agari::agaricalc(const LargeNum& AgariPointRaw, rat agariBairitsu, rat agariBairitsu2, int agariCount, rat agariBairitsu3, int agariCount2) {
	LargeNum agariPointArray = (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	for (int i = 0; i < agariCount; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu2.getNumerator() / agariBairitsu2.getDenominator() + 99) / 100 * 100;
	for (int i = 0; i < agariCount2; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu3.getNumerator() / agariBairitsu3.getDenominator() + 99) / 100 * 100;
	return agariPointArray;
}
#endif /* GUOBIAO */

// -------------------------------------------------------------------------

namespace {
	using namespace endround::agari;

#ifdef GUOBIAO

	void calcAgariPoints_Tsumo( // 中国ルール：ツモ
		const GameTable* gameStat, LargeNum& agariPoint, const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, PlayerID AgariPlayer)
	{
		agariPoint = AgariPointRaw;
		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (cnt == AgariPlayer)
				PointDelta[cnt] += (AgariPointRaw + 8) * 3;
			else
				PointDelta[cnt] -= AgariPointRaw + 8;
		}
	}

	void calcAgariPoints_Ron( // 中国ルール：ロン
		const GameTable* gameStat, LargeNum& agariPoint, const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, PlayerID AgariPlayer)
	{
		agariPoint = AgariPointRaw;
		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (cnt == AgariPlayer)
				PointDelta[cnt] += AgariPointRaw + (8 * 3);
			else if (cnt == gameStat->CurrentPlayer.Furikomi)
				PointDelta[cnt] -= AgariPointRaw + 8;
			else
				PointDelta[cnt] -= 8;
		}
	}

#else /* GUOBIAO */

	void calcAgariPoints_Pao( // 包適用時
		const GameTable* gameStat, LargeNum& agariPoint, const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, int Mode)
	{
		const PlayerID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : static_cast<PlayerID>(Mode);
		const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
		if (gameStat->playerwind(AgariPlayer) == sEast) { // 親の和了り
			if (TsumoAgari) { // ツモアガリ(包の人が一人払い)
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
				}
			} else { // ロンアガリ(包の人と振り込んだ人で折半)
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 4, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 4, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 4, cnt);
				}
			} else { // ロンアガリ(包の人と振り込んだ人で折半)
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
		const GameTable* gameStat, LargeNum& agariPoint, const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, PlayerID AgariPlayer)
	{
		if (gameStat->playerwind(AgariPlayer) == sEast) { // 親の和了り
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
				}
				if (cnt == gameStat->CurrentPlayer.Furikomi)
					deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
			}
		} else { // 子の和了り
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
		const GameTable* gameStat, LargeNum& agariPoint, const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, PlayerID AgariPlayer)
	{
		if (gameStat->chkGameType(Yonma) || (gameStat->chkGameType(Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// 四麻式ルール
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) { // 千点加符だったら
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (gameStat->playerwind(cnt) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else { // 三麻でツモ減りしないルール
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 3, 3, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
				} else if (gameStat->playerwind(cnt) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
			}
		}
	}

	void calcAgariPoints_Tsumo_NonDealer( // 通常時：子のツモ
		const GameTable* gameStat, LargeNum& agariPoint, const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, PlayerID AgariPlayer)
	{
		if (gameStat->chkGameType(Yonma) || (gameStat->chkGameType(Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// 四麻式ルール
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
				}
				else if (gameStat->playerwind(cnt) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
				else
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "fifty_fifty")) {
			// 三麻：親子関係なく折半ルール
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
				}
				else if (gameStat->playerwind(cnt) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_omitted") || RuleData::chkRule("tsumo_payment", "add_1000")) {
			// 三麻：自摸減りルール、千点加符ルール
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) {
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (gameStat->playerwind(cnt) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				} else if (gameStat->playerwind(cnt) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else if (RuleData::chkRule("tsumo_payment", "adjusted_payment")) {
			// 三麻：丸取りルール
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, Rat(8, 3), Rat(4, 3), 1, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
				}
				else if (gameStat->playerwind(cnt) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
				else if (gameStat->playerwind(cnt) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_halved")) {
			// 三麻：北家の分を折半
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1, Rat(1, 2), 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (gameStat->playerwind(cnt) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcminus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (gameStat->playerwind(cnt) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcminus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				}
			}
		}
	}
#endif /* GUOBIAO */
}

void endround::agari::calcAgariPoints(
	const GameTable* gameStat, LargeNum& agariPoint, const LargeNum& AgariPointRaw,
	InfoByPlayer<LargeNum>& PointDelta, int Mode)
{
	const PlayerID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : static_cast<PlayerID>(Mode);
	const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
#ifdef GUOBIAO
	if (TsumoAgari) // 中国ルール：ツモアガリ
		calcAgariPoints_Tsumo(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
#else /* GUOBIAO */
	if (isPaoAgari(gameStat, AgariPlayer)) // 包適用時
		calcAgariPoints_Pao(gameStat, agariPoint, AgariPointRaw, PointDelta, Mode);
	else if ((TsumoAgari) && (gameStat->playerwind(AgariPlayer) == sEast)) // 通常時：親のツモアガリ
		calcAgariPoints_Tsumo_Dealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	else if (TsumoAgari) // 通常時：子のツモアガリ
		calcAgariPoints_Tsumo_NonDealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
#endif /* GUOBIAO */
	else // 通常時：ロンアガリ
		calcAgariPoints_Ron(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	return;
}

// -------------------------------------------------------------------------

namespace {
	void forEachAgariPlayers(std::function<bool (int&)> f) {
		for (int cnt = 0; cnt < Players - 1; ++cnt) {
#ifndef GUOBIAO
			if (RuleData::chkRule("multiple_mahjong", "single_mahjong_with_draw") || RuleData::chkRule("multiple_mahjong", "single_mahjong")) {
#endif /* GUOBIAO */
				if (cnt > 0) break; // 頭ハネ(ダブロンなし)ルールの場合
#ifndef GUOBIAO
			} else if (RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw") || RuleData::chkRule("multiple_mahjong", "dual_mahjong")) {
				if (cnt > 1) break; // ダブロンあり・トリロンなしの場合
			}
#endif /* GUOBIAO */
			if (!f(cnt)) break;
		}
	}

	void chonboIfShibariUnmet(const GameTable* gameStat, const yaku::YAKUSTAT& yakuInfo, const MachihaiInfo& machiInfo, EndType& RoundEndType) {
		// 縛りを満たさないか、振聴のとき
#ifdef GUOBIAO
		if (yakuInfo.CoreHan < 8) // 中国ルールは和了に8点以上必要
			RoundEndType = EndType::chonbo;
#else /* GUOBIAO */
		if (((yakuInfo.CoreHan <= (gameStat->ShibariFlag ? 1 : 0)) && (yakuInfo.CoreSemiMangan <= 0)) || // 縛りを満たしていないか、
			(!gameStat->TsumoAgariFlag && ( // 「ロンの時で
			machiInfo.FuritenFlag || // フリテンか
			gameStat->statOfAgari().DoujunFuriten)) || // 同順フリテンの時」、もしくは
			(RuleData::chkRule("riichi_shibari", "yes") && (!gameStat->statOfAgari().RichiFlag.RichiFlag))) // リーチ縛りを満たしていないならば
			RoundEndType = EndType::chonbo; // チョンボにする
#endif /* GUOBIAO */
	}

	void chonboIfAgariForbidden(const GameTable* gameStat, const yaku::YAKUSTAT& yakuInfo, const MachihaiInfo& machiInfo, EndType& RoundEndType) {
#ifndef GUOBIAO
		if (RuleData::chkRule("furiten_riichi", "no") && // フリテン立直無しで
			gameStat->statOfAgari().RichiFlag.RichiFlag && // 立直していて
			machiInfo.FuritenFlag) // フリテンだった場合
			RoundEndType = EndType::chonbo; // チョンボにする
#endif /* GUOBIAO */
	}

	void verifyAgari(GameTable* gameStat, EndType& RoundEndType) {
		if (!gameStat->TsumoAgariFlag) {
			gameStat->statOfAgari().Tsumohai() = gameStat->CurrentDiscard;
		}
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
		MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
		// 縛りを満たさないか、振聴のときは錯和
		chonboIfShibariUnmet(gameStat, yakuInfo, machiInfo, RoundEndType);
		// 和了れない状況
		chonboIfAgariForbidden(gameStat, yakuInfo, machiInfo, RoundEndType);
	}

	enum OptionBool {oFalse, oTrue, oNull,};

	OptionBool procSecondaryRon(GameTable* gameStat, EndType& RoundEndType, int& cnt) {
		RoundEndType = EndType::agari;
		gameStat->CurrentPlayer.Agari = (gameStat->CurrentPlayer.Agari + 1) % Players;
		if (gameStat->CurrentPlayer.Agari == gameStat->CurrentPlayer.Furikomi) return oFalse; // 一周した時点で抜ける
		if (gameStat->statOfAgari().DeclarationFlag.Ron) { // ロンしていれば
			verifyAgari(gameStat, RoundEndType);
		} else {
			--cnt; return oTrue;
		}
		return oNull;
	}

	bool isSomeoneDobon(const GameTable* gameStat) {
		bool flag = false;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isDobon(gameStat, i)) flag = true;
		return flag;
	}

	void calcDobonDelta(const GameTable* gameStat, PlayerID AgariPlayerPriority, int penalty) {
		endround::transfer::resetDelta();
		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (isDobon(gameStat, cnt)) {
				endround::transfer::addDelta(cnt, -penalty);
				endround::transfer::addDelta(AgariPlayerPriority, penalty);
			}
		}
	}


	void dobonPenalty(GameTable* gameStat, PlayerID AgariPlayerPriority) {
#ifndef GUOBIAO
		int penalty = 0;
		std::string penaConf(RuleData::chkRule("penalty_negative"));
		std::smatch matchDat;
		if (std::regex_match(penaConf, matchDat, std::regex("(\\d+)pts"))) { // 点棒で精算
			penalty = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
			if (isSomeoneDobon(gameStat)) {
				calcDobonDelta(gameStat, AgariPlayerPriority, penalty);
				endround::transfer::transferPoints(gameStat, mihajong_graphic::TableSubsceneID::callValDobon, 3000);
			}
		} else if (std::regex_match(penaConf, matchDat, std::regex("chip(\\d+)"))) { // チップで精算
			if (RuleData::chkRuleApplied("chip")) {
				penalty = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
				if (isSomeoneDobon(gameStat)) {
					calcDobonDelta(gameStat, AgariPlayerPriority, penalty);
					endround::transfer::transferChip(gameStat, mihajong_graphic::TableSubsceneID::callValDobon, 1500);
				}
			}
		}
#endif /* GUOBIAO */
		return;
	}

}

void endround::agari::agariproc(EndType& RoundEndType, GameTable* gameStat, bool& tmpUraFlag, bool& tmpAliceFlag, CodeConv::tstring& ResultDesc) {
	bool tmpagariflag = false;
	PlayerID FirstAgariPlayer = gameStat->CurrentPlayer.Agari;
	int OyaAgari = -1;
	ResultDesc = _T("");
	tmpUraFlag = false;
	tmpAliceFlag = false;
	PlayerID AgariPlayerPriority = -1;
	std::uint16_t origDoraPointer = gameStat->DoraPointer;

	forEachAgariPlayers([&gameStat, &RoundEndType](int& cnt) -> bool {
		if (cnt > 0) { // ダブロン用の処理
			OptionBool result = procSecondaryRon(gameStat, RoundEndType, cnt);
			if (result == oFalse) return false;
			else if (result == oTrue) return true;
		}
		if (gameStat->TsumoAgariFlag) return false;
		return true;
	});

	forEachAgariPlayers([&, FirstAgariPlayer](int& cnt) -> bool {
		if (cnt == 0) {
			gameStat->CurrentPlayer.Agari = FirstAgariPlayer;
			verifyAgari(gameStat, RoundEndType);
		} else if (cnt > 0) {
			if (!gameStat->TsumoAgariFlag) {
				OptionBool result = procSecondaryRon(gameStat, RoundEndType, cnt);
				if (result == oFalse) return false;
				else if (result == oTrue) return true;
			}
		}
		haifu::haifualicedoraupd();
		/**************/
		/* 和了成立時 */
		/**************/
		if (RoundEndType == EndType::agari) {
			tmpagariflag = true;
			if (gameStat->statOfAgari().AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				RoundEndType = EndType::chonbo; // 和了り放棄時の処理→誤ロン・誤ツモとして罰符とする
		}
		if (RoundEndType == EndType::agari) {
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
			endround_agariproc(gameStat, ResultDesc, AgariPlayerPriority, origDoraPointer, yakuInfo, tmpUraFlag, tmpAliceFlag, OyaAgari);
		}
		/**************/
		/* 錯和発生時 */
		/**************/
		if (RoundEndType == EndType::chonbo)
			endround_chonboproc(gameStat, ResultDesc);

		if (gameStat->TsumoAgariFlag) return false; /* ツモ和了りの時は終了 */
#ifndef GUOBIAO
		if ((!RuleData::chkRule("getting_deposit", "riichidori")) || gameStat->statOfAgari().RichiFlag.RichiFlag)
			gameStat->Deposit = 0;
#endif /* GUOBIAO */
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		return true;
	});
	RoundEndType = tmpagariflag ? EndType::agari : EndType::chonbo;
	/* 連荘判定用のプレイヤー番号設定 */
#ifndef GUOBIAO
	if (RuleData::chkRule("simultaneous_mahjong", "renchan_if_dealer_mahjong"))
		gameStat->CurrentPlayer.Agari = (OyaAgari == -1) ? FirstAgariPlayer : OyaAgari;
	else if (RuleData::chkRule("simultaneous_mahjong", "renchan_if_dealer_upstream"))
		gameStat->CurrentPlayer.Agari = FirstAgariPlayer;
	else if (RuleData::chkRule("simultaneous_mahjong", "next_dealer"))
		gameStat->CurrentPlayer.Agari = FirstAgariPlayer + ((OyaAgari == FirstAgariPlayer) ? 1 : 0) % Players;
#endif /* GUOBIAO */

	dobonPenalty(gameStat, AgariPlayerPriority);
	return;
}

// -------------------------------------------------------------------------

namespace {
#ifndef GUOBIAO
	void deltawareme(PlayerID agariTmpPlayer, PlayerID agariTmpWareme) {
		LargeNum subtrahend = endround::transfer::getDelta()[agariTmpWareme];
		endround::transfer::addDelta(agariTmpPlayer, -subtrahend);
		return;
	}

	void calculateWareme(const GameTable* gameStat) {
		if (gameStat->WaremePlayer == gameStat->CurrentPlayer.Agari) {
			endround::transfer::doubleDelta(); // 割れ目の人が和了ったとき
		} else { // 割れ目の人以外が和了ったとき
			deltawareme(gameStat->CurrentPlayer.Agari, gameStat->WaremePlayer);
			endround::transfer::doubleDelta(gameStat->WaremePlayer);
		}
		return;
	}

	void calculateDoukasen(const GameTable* gameStat) {
		if (gameStat->DoukasenPlayer == gameStat->CurrentPlayer.Agari) {
			endround::transfer::doubleDelta(); // 導火線の人が和了ったとき
		} else { // 導火線の人以外が和了ったとき
			deltawareme(gameStat->CurrentPlayer.Agari, gameStat->DoukasenPlayer);
			endround::transfer::doubleDelta(gameStat->DoukasenPlayer);
		}
		return;
	}

	void calculateWaremeDelta(const GameTable* gameStat) { // 割れ目とか
		if (RuleData::chkRuleApplied("wareme"))
			calculateWareme(gameStat); // 割れ目ルール
		if ((RuleData::chkRule("wareme", "greater_wareme")) &&
			((gameStat->Dice[0].Number == gameStat->Dice[1].Number) || 
			((gameStat->Dice[2].Number != 0) && (gameStat->Dice[2].Number == gameStat->Dice[3].Number))))
			calculateWareme(gameStat); // サイコロがゾロ目の時はさらに倍
		if (RuleData::chkRuleApplied("doukasen"))
			calculateDoukasen(gameStat); // 導火線ルール
	}

	void calculateTsumibouDelta(const GameTable* gameStat) { // 積み棒の計算
		int tsumiboh_rate = 0;
		std::smatch matchDat;
		const std::string tsumiboh_rate_str(RuleData::chkRule("tsumiboh_rate"));
		// MEMORANDUM: 検索対象文字列をin-situで作ってはダメ 必ず引数に渡す前にオブジェクトを作っておくこと
		if (std::regex_match(tsumiboh_rate_str, matchDat, std::regex("counter_(\\d+)")))
			tsumiboh_rate = std::stoi(matchDat[1].str()) / (ACTUAL_PLAYERS - 1);
		endround::transfer::resetDelta();
		if (gameStat->TsumoAgariFlag) {
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == gameStat->CurrentPlayer.Agari)
					endround::transfer::addDelta(cnt, gameStat->Honba *   (ACTUAL_PLAYERS - 1)  * tsumiboh_rate);
				else
					endround::transfer::addDelta(cnt, gameStat->Honba * (-(ACTUAL_PLAYERS - 1)) * tsumiboh_rate);
			}
		} else {
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == gameStat->CurrentPlayer.Agari)
					endround::transfer::addDelta(cnt, gameStat->Honba *   (ACTUAL_PLAYERS - 1)  * tsumiboh_rate);
				else if (cnt == gameStat->CurrentPlayer.Furikomi)
					endround::transfer::addDelta(cnt, gameStat->Honba * (-(ACTUAL_PLAYERS - 1)) * tsumiboh_rate);
			}
		}
	}

	void chipTransfer(GameTable* gameStat, mihajong_graphic::TableSubsceneID subscene, int ChipAmount) {
		if ((ChipAmount <= 0) || (!RuleData::chkRuleApplied("chip"))) return;
		endround::transfer::resetDelta();
		const std::string limithand_bonus(RuleData::chkRule("limithand_bonus"));
		if ((!gameStat->TsumoAgariFlag) && (!std::regex_match(limithand_bonus, std::regex("chip_\\d+_each")))) {
			endround::transfer::addDelta(gameStat->CurrentPlayer.Furikomi, -ChipAmount);
			endround::transfer::addDelta(gameStat->CurrentPlayer.Agari   ,  ChipAmount);
		} else {
			for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
				endround::transfer::addDelta(i, -ChipAmount);
			endround::transfer::addDelta(gameStat->CurrentPlayer.Agari, ChipAmount * ACTUAL_PLAYERS);
		}
		endround::transfer::transferChip(gameStat, subscene, 1500);
		return;
	}

	int getChipAmount(const GameTable* gameStat, const YakuResult* yakuInfo) {
		if (!RuleData::chkRuleApplied("chip")) return 0; // チップ無しのルールならすぐ戻る
		int ChipAmount /* チップの量 */ = 0;
		/* 裏ドラ、アリス、一発祝儀 */
		if (gameStat->statOfAgari().MenzenFlag) {
			ChipAmount = yakuInfo->UraDoraQuantity + yakuInfo->AliceDora; // 裏ドラとアリスドラ
			if ((gameStat->statOfAgari().RichiFlag.IppatsuFlag) &&
				(RuleData::chkRuleApplied("riichi_ippatsu")))
				++ChipAmount; // 一発祝儀
		}
		/* 赤ドラ祝儀 */
		if (RuleData::chkRule("redtile_chip", "menzen_only") || RuleData::chkRule("redtile_chip", "menzen_only_non_dora")) { // 門前のみ赤ドラチップが付くルール
			if (gameStat->statOfAgari().MenzenFlag)
				ChipAmount += yakuInfo->AkaDoraQuantity;
		} else if (RuleData::chkRule("redtile_chip", "riichi_only") || RuleData::chkRule("redtile_chip", "riichi_only_non_dora")) { // リーチ時のみ赤ドラチップが付くルール
			if (gameStat->statOfAgari().RichiFlag.RichiFlag)
				ChipAmount += yakuInfo->AkaDoraQuantity;
		} else if (RuleData::chkRule("redtile_chip", "always") || RuleData::chkRule("redtile_chip", "always_but_non_dora")) { // 鳴いていても赤ドラチップが付くルール
			ChipAmount += yakuInfo->AkaDoraQuantity;
		}
		/* 青ドラ祝儀 */
		if (RuleData::chkRule("blue_chip", "menzen_only")) { // 門前のみ赤ドラチップが付くルール
			if (gameStat->statOfAgari().MenzenFlag)
				ChipAmount += yakuInfo->AoDoraQuantity;
		} else if (RuleData::chkRule("blue_chip", "riichi_only")) { // リーチ時のみ赤ドラチップが付くルール
			if (gameStat->statOfAgari().RichiFlag.RichiFlag)
				ChipAmount += yakuInfo->AoDoraQuantity;
		} else if (RuleData::chkRule("blue_chip", "always")) { // 鳴いていても赤ドラチップが付くルール
			ChipAmount += yakuInfo->AoDoraQuantity;
		}
		/* 合計を返す */
		return ChipAmount;
	}
#endif /* GUOBIAO */

	void agariscrproc(const GameTable* gameStat, const YakuResult* yakuInfo,
		const LargeNum* agariPointArray, int& ChipAmount, const CodeConv::tstring& ResultDesc, bool& tmpUraFlag)
	{
		sound::util::bgmstop();
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
#ifdef GUOBIAO
		mihajong_graphic::YakuResult::setYakuStat(yakuInfo, static_cast<LargeNum>(*agariPointArray), 0);
#else /* GUOBIAO */
		ChipAmount = getChipAmount(gameStat, yakuInfo);
		mihajong_graphic::YakuResult::setYakuStat(yakuInfo, static_cast<LargeNum>(*agariPointArray), ChipAmount);
		tmpUraFlag = gameStat->statOfAgari().MenzenFlag && gameStat->statOfAgari().RichiFlag.RichiFlag && (!RuleData::chkRule("uradora", "no"));
		if (tmpUraFlag)
			mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::agariUradora);
		else
#endif /* GUOBIAO */
			mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::agari);
		(void)mihajong_graphic::ui::WaitUI();
		if (EnvTable::Instantiate()->WatchModeFlag)
			mihajong_graphic::ui::CheckIfDemoTerminated();
	}

}

/* 和了成立時の処理 */
void endround::agari::endround_agariproc(GameTable* gameStat, CodeConv::tstring& ResultDesc, PlayerID& AgariPlayerPriority,
	std::uint16_t origDoraPointer, const yaku::YAKUSTAT& yakuInfo, bool& tmpUraFlag, bool& tmpAliceFlag, int& OyaAgari)
{
	LargeNum AgariPointRaw = yakuInfo.AgariPoints;
	if (AgariPlayerPriority == -1) AgariPlayerPriority = gameStat->CurrentPlayer.Agari;
	if (!ResultDesc.empty()) ResultDesc += _T("\n");
	switch (gameStat->playerwind(gameStat->CurrentPlayer.Agari)) {
		case sEast:  ResultDesc += _T("東家"); break;
		case sSouth: ResultDesc += _T("南家"); break;
		case sWest:  ResultDesc += _T("西家"); break;
		case sNorth: ResultDesc += _T("北家"); break;
	}
	if (gameStat->TsumoAgariFlag) {
		ResultDesc += _T("のツモ和了り");
	} else {
		switch (gameStat->playerwind(gameStat->CurrentPlayer.Furikomi)) {
			case sEast:  ResultDesc += _T("が東家からロン和了り"); break;
			case sSouth: ResultDesc += _T("が南家からロン和了り"); break;
			case sWest:  ResultDesc += _T("が西家からロン和了り"); break;
			case sNorth: ResultDesc += _T("が北家からロン和了り"); break;
		}
	}
	mihajong_graphic::ui::WaitUIWithTimeout(1500);
	if (EnvTable::Instantiate()->WatchModeFlag)
		mihajong_graphic::ui::CheckIfDemoTerminated();
	std::uint16_t tmpDoraPointer = origDoraPointer;
	const int AlicePointer = tmpDoraPointer - yakuInfo.AliceDora * 2 - 2;

#ifndef GUOBIAO
	if (gameStat->statOfAgari().MenzenFlag && RuleData::chkRuleApplied("alice")) { // めくっていく処理
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::alice); // 表示
		while (gameStat->DoraPointer > AlicePointer) {
			gameStat->DoraPointer -= 2;
			sound::Play(sound::IDs::sndMekuri);
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			mihajong_graphic::ui::WaitUIWithTimeout(1200);
			if (EnvTable::Instantiate()->WatchModeFlag)
				mihajong_graphic::ui::CheckIfDemoTerminated();
		}
		gameStat->DoraPointer = tmpDoraPointer;
		tmpAliceFlag = true;
	}
#endif /* GUOBIAO */

	sound::util::bgmstop();

	transfer::resetDelta();
	if (gameStat->playerwind(gameStat->CurrentPlayer.Agari) == sEast)
		OyaAgari = gameStat->CurrentPlayer.Agari; // 親の和了り
	LargeNum agariPoint;
	calcAgariPoints(gameStat, agariPoint, AgariPointRaw, transfer::getDelta(), -1);
	assert(agariPoint > 0);
#ifndef GUOBIAO
	calculateWaremeDelta(gameStat);
#endif /* GUOBIAO */
	int ChipAmount;
	agariscrproc(gameStat, &yakuInfo, &agariPoint, ChipAmount, ResultDesc, tmpUraFlag); /* 和了画面 */
	gameStat->statOfAgari().YakitoriFlag = false; // 焼き鳥フラグを下ろす
	/*if (gameStat->statOfAgari().MenzenFlag && RuleData::chkRuleApplied("alice"))
		gameStat->DoraPointer = AlicePointer;*/
	transfer::transferPoints(gameStat, mihajong_graphic::TableSubsceneID::callValAgariten, 1500);
	
#ifndef GUOBIAO
	if ((gameStat->Honba > 0) && RuleData::chkRuleApplied("tsumiboh_rate")) {
		calculateTsumibouDelta(gameStat);
		// 割れ目で積み棒も２倍になる
		calculateWaremeDelta(gameStat);
		/* 包の場合 */
		if (isPaoAgari(gameStat, gameStat->CurrentPlayer.Agari)) {
			PlayerID PaoPlayer = getPaoPlayer(gameStat, gameStat->CurrentPlayer.Agari);
			if (gameStat->TsumoAgariFlag) {
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if ((PaoPlayer != cnt) && (gameStat->CurrentPlayer.Agari != cnt)) {
						transfer::addDelta(gameStat->CurrentPlayer.Agari, transfer::getDelta()[cnt]);
						transfer::addDelta(cnt, -transfer::getDelta()[cnt]);
					}
				}
			} else {
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if ((PaoPlayer != cnt) && (gameStat->CurrentPlayer.Furikomi != cnt) && (gameStat->CurrentPlayer.Agari != cnt)) {
						transfer::addDelta(PaoPlayer, transfer::getDelta()[cnt]);
						transfer::addDelta(cnt, -transfer::getDelta()[cnt]);
					}
				}
			}
		}
		transfer::transferPoints(gameStat, mihajong_graphic::TableSubsceneID::callValTsumibou, 1500);
	}
	
	if (gameStat->Deposit) { // 供託点棒が場にある場合で、条件を満たしているなら
		if ((!RuleData::chkRule("getting_deposit", "riichidori")) || gameStat->statOfAgari().RichiFlag.RichiFlag) {
			transfer::resetDelta();
			transfer::addDelta(gameStat->CurrentPlayer.Agari, gameStat->Deposit * 1000);
			transfer::transferPoints(gameStat, mihajong_graphic::TableSubsceneID::callValKyoutaku, 1500);
		}
	}
	
	chipTransfer(gameStat, mihajong_graphic::TableSubsceneID::callValChip, ChipAmount);
	
	ChipAmount = 0;
	if ((yakuInfo.CoreSemiMangan + yakuInfo.BonusSemiMangan) >= 8) { // 役満祝儀
		std::smatch vals;
		std::string limithand_bonus(RuleData::chkRule("limithand_bonus"));
		if (std::regex_match(limithand_bonus, vals, std::regex("chip(\\d+)")))
			ChipAmount = std::atoi(vals[1].str().c_str());
		else if (std::regex_match(limithand_bonus, vals, std::regex("chip_tsumo(\\d+)each_ron(\\d)")))
			ChipAmount = std::atoi(vals[gameStat->TsumoAgariFlag ? 1 : 2].str().c_str());
		else if (std::regex_match(limithand_bonus, vals, std::regex("chip_(\\d+)_each")))
			ChipAmount = std::atoi(vals[1].str().c_str());
	}
	chipTransfer(gameStat, mihajong_graphic::TableSubsceneID::callValYakuman, ChipAmount);

	/* 四馬路が北家の放銃だった場合 */
	if (!gameStat->chkGameType(SanmaT)) {
		if (gameStat->playerwind(gameStat->CurrentPlayer.Furikomi) == sNorth) {
			if (std::regex_search(yakuInfo.yakuNameList, std::basic_regex<TCHAR>(_T("(^|\\r?\\n)四馬路(\r?\n|$)")))) {
				transfer::resetDelta();
				for (PlayerID i = 0; i < Players; ++i)
					transfer::addDelta(i, 1000);
				transfer::addDelta(gameStat->CurrentPlayer.Agari, -3000);
				transfer::transferPoints(gameStat, mihajong_graphic::TableSubsceneID::callValKitamakura, 1500);
			}
		}
	}
#endif /* GUOBIAO */
	return;
}

// -------------------------------------------------------------------------

/* 錯和発生時の処理 */
void endround::agari::endround_chonboproc(GameTable* gameStat, CodeConv::tstring& ResultDesc) {
#ifdef GUOBIAO
	/* 中国ルールでの処理。基本的にアガリ放棄で続行する */
	mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Agari, mihajong_graphic::calltext::CallType::chonbo);
	mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::callFade);
	sound::Play(sound::IDs::sndCuohu);
	mihajong_graphic::ui::WaitUIWithTimeout(1500);
	if (EnvTable::Instantiate()->WatchModeFlag)
		mihajong_graphic::ui::CheckIfDemoTerminated();
	gameStat->statOfAgari().AgariHouki = true;
	mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Agari, mihajong_graphic::calltext::CallType::none);
#else /* GUOBIAO */
	if (!ResultDesc.empty()) ResultDesc += _T("\n");
	switch (gameStat->playerwind(gameStat->CurrentPlayer.Agari)) {
		case sEast:  ResultDesc += _T("東家のチョンボ"); break;
		case sSouth: ResultDesc += _T("南家のチョンボ"); break;
		case sWest:  ResultDesc += _T("西家のチョンボ"); break;
		case sNorth: ResultDesc += _T("北家のチョンボ"); break;
	}
	mihajong_graphic::ui::WaitUIWithTimeout(1500);
	if (EnvTable::Instantiate()->WatchModeFlag)
		mihajong_graphic::ui::CheckIfDemoTerminated();
	sound::util::bgmplay(sound::IDs::musRyuukyoku);
	/* 誤ロンまたは誤ツモ */
	if ((gameStat->AgariSpecialStat == 0) || (gameStat->AgariSpecialStat == 1)) {
		if ((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong"))
			ResultDesc += _T("(大明槓の嶺上牌での和了り)"); // 大明槓の嶺上開花禁止ルールの場合
		else if ((!gameStat->statOfAgari().RichiFlag.RichiFlag) && RuleData::chkRuleApplied("riichi_shibari"))
			ResultDesc += _T("(ダマ聴での和了り)"); // 立直縛りで立直してないなら錯和
		else if (!isTenpai(gameStat, gameStat->CurrentPlayer.Agari))
			ResultDesc += _T("(成立していない和了り)"); // 和了りが成立していない場合錯和
		else {
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
			MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
			if (machiInfo.FuritenFlag || gameStat->statOfAgari().DoujunFuriten)
				ResultDesc += _T("(振聴でのロン和了り)"); // 振聴なのにロンした場合は錯和
			else if (yakuInfo.CoreHan == 0)
				ResultDesc += _T("(役のない和了り)"); // 役がなかった場合は錯和
			else if ((yakuInfo.CoreHan == 1) && gameStat->ShibariFlag)
				ResultDesc += _T("(一飜しかない和了り)"); // 二飜縛りで１飜しかないなら錯和
			else if (gameStat->statOfAgari().AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				ResultDesc += _T("(和了り放棄適用中)"); // 和了り放棄なのに和了ろうとした
		}
		sound::Play(sound::IDs::sndCuohu);
	}
	/* 誤ロン、誤ツモ以外の錯和 */
	else {
		if (gameStat->AgariSpecialStat == agariKuikae)
			ResultDesc += _T("(喰い替え)"); // 喰い替えをしたとき
		sound::Play(sound::IDs::sndPage);
	}
	mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::chonbo);
	mihajong_graphic::ui::WaitUIWithTimeout(5000);
	if (EnvTable::Instantiate()->WatchModeFlag)
		mihajong_graphic::ui::CheckIfDemoTerminated();
#endif /* GUOBIAO */

	transferChonboPenalty(gameStat, gameStat->CurrentPlayer.Agari);
	return;
}
