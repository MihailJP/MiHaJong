#include "agari.h"

#include <functional>
#include <regex>
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "bgmid.h"
#include "func.h"
#include "yaku/yaku.h"
#include "haifu.h"
#include "envtbl.h"
#include "endround.h"
#include "chat.h"
#include "ruletbl.h"
#include "tileutil.h"

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

namespace {
	void forEachAgariPlayers(std::function<bool (int&)> f) {
		for (int cnt = 0; cnt < PLAYERS - 1; ++cnt) {
			if (RuleData::chkRule("multiple_mahjong", "single_mahjong_with_draw") || RuleData::chkRule("multiple_mahjong", "single_mahjong")) {
				if (cnt > 0) break; // 頭ハネ(ダブロンなし)ルールの場合
			} else if (RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw") || RuleData::chkRule("multiple_mahjong", "dual_mahjong")) {
				if (cnt > 1) break; // ダブロンあり・トリロンなしの場合
			}
			if (!f(cnt)) break;
		}
	}

	void chonboIfShibariUnmet(const GameTable* gameStat, const yaku::YAKUSTAT& yakuInfo, const MachihaiInfo& machiInfo, EndType& RoundEndType) {
		// 縛りを満たさないか、振聴のとき
		if ((yakuInfo.CoreHan <= (gameStat->ShibariFlag ? 1 : 0)) || // 縛りを満たしていないか、
			(!gameStat->TsumoAgariFlag && ( // 「ロンの時で
			machiInfo.FuritenFlag || // フリテンか
			gameStat->Player[gameStat->CurrentPlayer.Agari].DoujunFuriten)) || // 同順フリテンの時」、もしくは
			(RuleData::chkRule("riichi_shibari", "yes") && (!gameStat->Player[gameStat->CurrentPlayer.Agari].RichiFlag.RichiFlag))) // リーチ縛りを満たしていないならば
			RoundEndType = Chonbo; // チョンボにする
	}

	void verifyAgari(GameTable* gameStat, EndType& RoundEndType) {
		if (!gameStat->TsumoAgariFlag) {
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].red  = gameStat->CurrentDiscard.red;
		}
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
		MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
		// 縛りを満たさないか、振聴のときは錯和
		chonboIfShibariUnmet(gameStat, yakuInfo, machiInfo, RoundEndType);
	}

	enum OptionBool {oFalse, oTrue, oNull,};

	OptionBool procSecondaryRon(GameTable* gameStat, EndType& RoundEndType, int& cnt) {
		RoundEndType = Agari;
		gameStat->CurrentPlayer.Agari = (gameStat->CurrentPlayer.Agari + 1) % PLAYERS;
		if (gameStat->CurrentPlayer.Agari == gameStat->CurrentPlayer.Furikomi) return oFalse; // 一周した時点で抜ける
		if (gameStat->Player[gameStat->CurrentPlayer.Agari].DeclarationFlag.Ron) { // ロンしていれば
			verifyAgari(gameStat, RoundEndType);
		} else {
			--cnt; return oTrue;
		}
		return oNull;
	}

	bool isSomeoneDobon(const GameTable* gameStat) {
		bool flag = false;
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isDobon(gameStat, i)) flag = true;
		return flag;
	}

	void calcDobonDelta(const GameTable* gameStat, PLAYER_ID AgariPlayerPriority, int penalty) {
		endround::transfer::resetDelta();
		for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (isDobon(gameStat, cnt)) {
				endround::transfer::addDelta(cnt, -penalty);
				endround::transfer::addDelta(AgariPlayerPriority, penalty);
			}
		}
	}


	void dobonPenalty(GameTable* gameStat, PLAYER_ID AgariPlayerPriority) {
		int penalty = 0;
		std::string penaConf(RuleData::chkRule("penalty_negative"));
		std::smatch matchDat;
		if (std::regex_match(penaConf, matchDat, std::regex("(\\d+)pts"))) { // 点棒で精算
			penalty = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
			if (isSomeoneDobon(gameStat)) {
				calcDobonDelta(gameStat, AgariPlayerPriority, penalty);
				endround::transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValDobon, 3000);
			}
		} else if (std::regex_match(penaConf, matchDat, std::regex("chip(\\d+)"))) { // チップで精算
			if (RuleData::chkRuleApplied("chip")) {
				penalty = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
				if (isSomeoneDobon(gameStat)) {
					calcDobonDelta(gameStat, AgariPlayerPriority, penalty);
					endround::transfer::transferChip(gameStat, mihajong_graphic::tblSubsceneCallValDobon, 1500);
				}
			}
		}
		return;
	}

}

void endround::agari::agariproc(EndType& RoundEndType, GameTable* gameStat, bool& tmpUraFlag, bool& tmpAliceFlag, CodeConv::tstring& ResultDesc) {
	bool tmpagariflag = false;
	PLAYER_ID FirstAgariPlayer = gameStat->CurrentPlayer.Agari;
	int OyaAgari = -1;
	ResultDesc = _T("");
	tmpUraFlag = 0;
	tmpAliceFlag = 0;
	int AgariPlayerPriority = -1;
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
		if (RoundEndType == Agari) {
			tmpagariflag = true;
			if (gameStat->Player[gameStat->CurrentPlayer.Agari].AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				RoundEndType = Chonbo; // 和了り放棄時の処理→誤ロン・誤ツモとして罰符とする
		}
		//if (RoundEndType == Agari)
			/* TODO: ここの移植 endround_agariproc GameStat, GameEnv, ResultDesc, AgariPlayerPriority, origDoraPointer, YakuInfo, tmpAliceFlag */
		/**************/
		/* 錯和発生時 */
		/**************/
		if (RoundEndType == Chonbo)
			endround_chonboproc(gameStat, ResultDesc);

		if (gameStat->TsumoAgariFlag) return false; /* ツモ和了りの時は終了 */
		if ((!RuleData::chkRule("getting_deposit", "riichidori")) || gameStat->Player[gameStat->CurrentPlayer.Agari].RichiFlag.RichiFlag)
			gameStat->Deposit = 0;
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		return true;
	});
	RoundEndType = tmpagariflag ? Agari : Chonbo;
	/* 連荘判定用のプレイヤー番号設定 */
	if (RuleData::chkRule("simultaneous_mahjong", "renchan_if_dealer_mahjong"))
		gameStat->CurrentPlayer.Agari = (OyaAgari == -1) ? FirstAgariPlayer : OyaAgari;
	else if (RuleData::chkRule("simultaneous_mahjong", "renchan_if_dealer_upstream"))
		gameStat->CurrentPlayer.Agari = FirstAgariPlayer;
	else if (RuleData::chkRule("simultaneous_mahjong", "next_dealer"))
		gameStat->CurrentPlayer.Agari = FirstAgariPlayer + ((OyaAgari == FirstAgariPlayer) ? 1 : 0) % PLAYERS;

	dobonPenalty(gameStat, AgariPlayerPriority);
	return;
}

// -------------------------------------------------------------------------

/* 錯和発生時の処理 */
void endround::agari::endround_chonboproc(GameTable* gameStat, CodeConv::tstring& ResultDesc) {
	if (!ResultDesc.empty()) ResultDesc += _T("\n");
	CodeConv::tstring tmpResultDesc;
	switch (playerwind(gameStat, gameStat->CurrentPlayer.Agari, gameStat->GameRound)) {
		case sEast:  tmpResultDesc += _T("東家のチョンボ"); break;
		case sSouth: tmpResultDesc += _T("南家のチョンボ"); break;
		case sWest:  tmpResultDesc += _T("西家のチョンボ"); break;
		case sNorth: tmpResultDesc += _T("北家のチョンボ"); break;
	}
	ResultDesc += tmpResultDesc + _T("です");
	writeChat(tmpResultDesc);
	/* TODO: これを廃止するかの決定 statmes tmpResultDesc */
	Sleep(1500);
	sound::Play(sound::IDs::musRyuukyoku);
	/* 誤ロンまたは誤ツモ */
	if ((gameStat->AgariSpecialStat == 0) || (gameStat->AgariSpecialStat == 1)) {
		if ((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong"))
			ResultDesc += _T("(大明槓の嶺上牌での和了り)"); // 大明槓の嶺上開花禁止ルールの場合
		else if ((!gameStat->Player[gameStat->CurrentPlayer.Agari].RichiFlag.RichiFlag) && RuleData::chkRuleApplied("riichi_shibari"))
			ResultDesc += _T("(ダマ聴での和了り)"); // 立直縛りで立直してないなら錯和
		else if (!isTenpai(gameStat, gameStat->CurrentPlayer.Agari))
			ResultDesc += _T("(成立していない和了り)"); // 和了りが成立していない場合錯和
		else {
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
			MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
			if (machiInfo.FuritenFlag || gameStat->Player[gameStat->CurrentPlayer.Agari].DoujunFuriten)
				ResultDesc += _T("(振聴でのロン和了り)"); // 振聴なのにロンした場合は錯和
			else if (yakuInfo.CoreHan == 0)
				ResultDesc += _T("(役のない和了り)"); // 役がなかった場合は錯和
			else if ((yakuInfo.CoreHan == 1) && gameStat->ShibariFlag)
				ResultDesc += _T("(一飜しかない和了り)"); // 二飜縛りで１飜しかないなら錯和
			else if (gameStat->Player[gameStat->CurrentPlayer.Agari].AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				ResultDesc += _T("(和了り放棄適用中)"); // 和了り放棄なのに和了ろうとした
		}
		sound::Play(sound::IDs::sndCuohu);
	}
	/* 誤ロン、誤ツモ以外の錯和 */
	else {
		if (gameStat->AgariSpecialStat == AGARI_KUIKAE)
			ResultDesc += _T("(喰い替え)"); // 喰い替えをしたとき
		sound::Play(sound::IDs::sndPage);
	}
	mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneChonbo);
	Sleep(5000);

	transferChonboPenalty(gameStat, gameStat->CurrentPlayer.Agari);
	return;
}
