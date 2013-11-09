#include "endround.h"

#include "ruletbl.h"
#include "func.h"
#include "logging.h"
#include <functional>
#include <array>
#include <tuple>
#include <cassert>
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "../graphic/graphic.h"
#include "tileutil.h"
#include "haifu.h"
#include "chat.h"
#include "agari.h"
#include "envtbl.h"
#include <cmath>

namespace { // 内部処理に使う関数
	bool all_player(const GameTable* gameStat, std::function<bool (const PlayerTable*)> f) {
		bool flag = true;
		if (gameStat->chkGameType(Sanma4)) {
			for (PlayerID i = 0; i < Players; ++i)
				flag = flag && ((gameStat->playerwind(i) == sNorth) || f(&(gameStat->Player[i])));
		} else if (gameStat->chkGameType(SanmaT)) {
			for (PlayerID i = 0; i < 3; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		} else {
			for (PlayerID i = 0; i < Players; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		}
		return flag;
	}
	bool chksifeng(const GameTable* gameStat) {
		bool flag = false;
#ifndef GUOBIAO
		std::array<TileCode, 4> winds = {EastWind, SouthWind, WestWind, NorthWind,};
		for (auto k : winds) {
			if ((RuleData::chkRule("four_wind_ryuukyoku", "same_dealer_west") ||
				RuleData::chkRule("four_wind_ryuukyoku", "next_dealer_west")) &&
				(k != WestWind))
				continue; // 西以外は無視するルールの場合
			bool tmpflag = true;
			if (gameStat->chkGameType(Sanma4)) {
				for (PlayerID i = 0; i < Players; ++i)
					tmpflag = tmpflag && ((gameStat->playerwind(i) == sNorth) || (gameStat->Player[i].Discard[1].tcode.tile == k));
			} else if (gameStat->chkGameType(SanmaT)) {
				for (PlayerID i = 0; i < 3; ++i)
					tmpflag = tmpflag && (gameStat->Player[i].Discard[1].tcode.tile == k);
			} else {
				for (PlayerID i = 0; i < Players; ++i)
					tmpflag = tmpflag && (gameStat->Player[i].Discard[1].tcode.tile == k);
			}
			flag = flag || tmpflag;
		}
#endif /* GUOBIAO */
		return flag;
	}

#ifndef GUOBIAO
	void logKuikae(const GameTable* gameStat, bool chonbo) {
		CodeConv::tostringstream o;
		o << _T("喰い変えを検出しました。") <<
			(chonbo ? _T("チョンボ") : _T("和了り放棄")) << _T("として扱います。打牌 [") <<
			(int)gameStat->CurrentDiscard.tile << _T("] 鳴いた牌 [") <<
			(int)gameStat->PreviousMeld.Discard << _T("]");
		info(o.str().c_str());
		sound::Play(sound::IDs::sndCuohu);
	}
#endif /* GUOBIAO */
	bool chkKuikae(GameTable* gameStat) { // 喰い替えの場合の処理
#ifndef GUOBIAO
		if (((gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Discard) || // 現物の食い変えになっている場合か
			(gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Stepped)) && // 筋食い変えになっている場合で
			(!gameStat->statOfActive().AgariHouki)) { // まだアガリ放棄になっていないなら
				if (RuleData::chkRule("kuikae", "agari_houki") || RuleData::chkRule("kuikae", "agari_houki_if_in_kind")) {
					/* 和了り放棄とする設定 */
					logKuikae(gameStat, false);
					for (unsigned i = 0; i < 2; ++i) {
						mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Kuikae);
						mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); mihajong_graphic::ui::WaitUIWithTimeout(1500);
						mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::AgariHouki);
						mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); mihajong_graphic::ui::WaitUIWithTimeout(1500);
					}
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
					mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
					mihajong_graphic::GameStatus::updateGameStat(gameStat);
					/* 和了り放棄は以降強制ツモ切り、強制不聴扱いとなります */
					gameStat->statOfActive().AgariHouki = true;
				} else if (RuleData::chkRule("kuikae", "chombo") || RuleData::chkRule("kuikae", "chombo_if_in_kind")) {
					/* 直ちに錯和とする設定 */
					logKuikae(gameStat, true);
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Chonbo);
					mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); mihajong_graphic::ui::WaitUIWithTimeout(1500);
					/* 局を打ち切り、満貫罰符の支払いに進む */
					return true;
				}
		}
#endif /* GUOBIAO */
		return false;
	}

	void chkTahai(GameTable* gameStat) {
		/* 多牌や少牌をしていないかのチェック */
		unsigned tmptilecnt = 0;
		for (unsigned i = 0; i < NumOfTilesInHand; ++i)
			if (gameStat->statOfActive().Hand[i].tile != NoTile)
				++tmptilecnt;
		tmptilecnt += gameStat->statOfActive().MeldPointer * 3;
		if ((tmptilecnt != (NumOfTilesInHand - 1)) && (!gameStat->statOfActive().AgariHouki)) {
			// 多牌や少牌の場合の処理(通常起きることはないはずだが…)
			if (tmptilecnt > (NumOfTilesInHand - 1))
				warn(_T("多牌を検出しました。和了り放棄として扱いますが、摸打の処理で不整合が起きていた可能性があります。"));
			else
				warn(_T("少牌を検出しました。和了り放棄として扱いますが、摸打の処理で不整合が起きていた可能性があります。"));
			sound::Play(sound::IDs::sndCuohu);
			for (unsigned i = 0; i < 2; ++i) {
				if (tmptilecnt > (NumOfTilesInHand - 1))
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Tahai);
				else
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Shouhai);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); mihajong_graphic::ui::WaitUIWithTimeout(1500);
				mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::AgariHouki);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); mihajong_graphic::ui::WaitUIWithTimeout(1500);
			}
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			gameStat->statOfActive().AgariHouki = true;
		}
		return;
	}
}

EndType endround::checkroundabort(GameTable* gameStat) { // 局終了条件の判定
#ifndef GUOBIAO
	/* 四開槓なら流す */
	if (RuleData::chkRuleApplied("four_kong_ryuukyoku") && (gameStat->KangNum == 4) &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->NumberOfQuads < 4;})))
		return Suukaikan;
	// 四人立直・三人立直の判定
	if (RuleData::chkRuleApplied("four_riichi_ryuukyoku") &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->RichiFlag.RichiFlag;})))
		return SuuchaRiichi;
	// 四風連打・三風連打の判定
	if (RuleData::chkRuleApplied("four_wind_ryuukyoku") &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->DiscardPointer == 1;})) &&
		(chksifeng(gameStat)))
		return SuufonRenda;
#endif /* GUOBIAO */

	for (PlayerID i = 0; i < Players; ++i) gameStat->Player[i].Tsumohai().tile = NoTile; // バグ防止のため
#ifndef GUOBIAO
	if (chkKuikae(gameStat)) { // 喰い替えの場合の処理
		gameStat->AgariSpecialStat = agariKuikae;
		return Chonbo;
	}
#endif /* GUOBIAO */
	/* 多牌や少牌をしていないかのチェック */
	chkTahai(gameStat);
	/* 荒牌の場合ここで終了する(河底牌は吃ポンできないがロンはできる) */
	if (gameStat->TilePointer == (gameStat->RinshanPointer - (gameStat->DeadTiles - 1)))
		return Ryuukyoku;
	/* 何事もなかった場合 */
	return Continuing;
}

// -------------------------------------------------------------------------

namespace {
	std::array<bool, Players> chkNagashiMangan(const GameTable* gameStat, EndType& RoundEndType) { /* 流し満貫の判定 */
		std::array<bool, Players> NagashiManganFlag = {false,};
#ifndef GUOBIAO
		if (RoundEndType == Ryuukyoku) {
			for (unsigned i = 0; i < ACTUAL_PLAYERS; ++i) {
				if (gameStat->chkGameType(Sanma4) && (gameStat->playerwind(i) == sNorth))
					continue; // 四人三麻の場合北家は無視
				if (RuleData::chkRuleApplied("nagashi_mangan") && isNagashiMangan(gameStat, i)) {
					NagashiManganFlag[i] = true; RoundEndType = NagashiMangan;
				}
			}
		}
#endif /* GUOBIAO */
		return NagashiManganFlag;
	}

	void ryuukyokuScreen(unsigned soundNum, CodeConv::tstring* ResultDesc, unsigned subsceneCode, unsigned waittime = 3000, unsigned bgmNum = sound::IDs::musRyuukyoku) {
		using namespace mihajong_graphic;
		using namespace CodeConv;
		if (ResultDesc) {
			sound::Play(soundNum);
			writeChat(*ResultDesc);
			Subscene(subsceneCode);
			mihajong_graphic::ui::WaitUIWithTimeout(3000);
		}
		for (PlayerID i = 0; i < Players; ++i)
			calltext::setCall(i, calltext::None);
		sound::Play(sound::IDs::sndPingju);
		sound::util::bgmplay(bgmNum);
		Subscene(tblSubsceneRyuukyoku);
		mihajong_graphic::ui::WaitUIWithTimeout(waittime);
	}

	unsigned checkTenpai(GameTable* gameStat, CodeConv::tstring& ResultDesc, unsigned OrigTurn) {
		unsigned TenpaiCnt = 0;
		for (PlayerID i = 0; i < Players; ++i) {
			if (gameStat->chkGameType(Sanma4) && (gameStat->playerwind(i, OrigTurn) == sNorth))
				continue; // 四人三麻の北家は無視
			if (isTenpai(gameStat, i)) { // 聴牌の時
				++TenpaiCnt; gameStat->Player[i].HandStat = handExposed;
			} else if (gameStat->Player[i].RichiFlag.RichiFlag) { // 錯和(不聴)リーチの時
				gameStat->Player[i].HandStat = handExposed;
			} else { // 不聴の時は伏せる
				gameStat->Player[i].HandStat = handHidden;
			}
		}
		CodeConv::tstring TenpaiCountTxt;
		switch (TenpaiCnt) {
			case 0: TenpaiCountTxt = _T("全員不聴"); break;
			case 1: TenpaiCountTxt = _T("１人聴牌"); break;
			case 2: TenpaiCountTxt = _T("２人聴牌"); break;
			case 3: TenpaiCountTxt = _T("３人聴牌"); break;
			case 4: TenpaiCountTxt = _T("４人聴牌"); break;
		}
		ResultDesc = _T("荒牌流局、") + TenpaiCountTxt;
		writeChat(ResultDesc + _T("です"));
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneChkTenpai);
		mihajong_graphic::ui::WaitUIWithTimeout(5000);
		return TenpaiCnt;
	}

#ifndef GUOBIAO
	void transferNotenBappu(GameTable* gameStat, unsigned OrigTurn, unsigned TenpaiCnt) {
		using namespace endround::transfer;
		resetDelta();
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) {
			if (gameStat->chkGameType(Sanma4) && (gameStat->playerwind(i, OrigTurn) == sNorth))
				continue; // 四人三麻の北家は無視
			if (gameStat->chkGameType(AllSanma)) { // 三麻の場合
				if ((TenpaiCnt > 0) && (TenpaiCnt < 3)) {
					if (isTenpai(gameStat, i))
						addDelta(i, 3000 / TenpaiCnt);
					else addDelta(i, -3000 / (signed)(3 - TenpaiCnt));
				}
			} else { // 四麻の場合
				if ((TenpaiCnt > 0) && (TenpaiCnt < 4)) {
					if (isTenpai(gameStat, i))
						addDelta(i, 3000 / TenpaiCnt);
					else addDelta(i, -3000 / (signed)(4 - TenpaiCnt));
				}
			}
		}
		if ((TenpaiCnt > 0) && (TenpaiCnt < ACTUAL_PLAYERS))
			transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValNotenBappu, 2500);
	}

	void showRenchanFlag(GameTable* gameStat, bool RenchanFlag) {
		using namespace mihajong_graphic::calltext;
		for (PlayerID cnt = 0; cnt < Players; ++cnt)
			setCall(cnt, None);
		if (RenchanFlag)
			setCall(gameStat->GameRound % Players, Renchan);
		else
			setCall(gameStat->GameRound % Players, Oyanagare);
		sound::Play(sound::IDs::sndPage);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade);
		return;
	}
#endif /* GUOBIAO */

	void ryuukyokuProc(GameTable* gameStat, bool RenchanFlag) {
#ifndef GUOBIAO
		showRenchanFlag(gameStat, RenchanFlag);
		++(gameStat->Honba);
		if (!RenchanFlag)
#endif /* GUOBIAO */
			++(gameStat->GameRound);
		gameStat->AgariChain = 0; gameStat->LastAgariPlayer = -1;
		return;
	}
}

void endround::endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba) {
	EndType RoundEndType = roundEndType;
	bool tmpUraFlag = false, tmpAliceFlag = false;
	CodeConv::tstring ResultDesc; 
	{
		CodeConv::tostringstream o;
		o << _T("局を終了 終了コード [") << (int)RoundEndType << _T(']');
	}
#ifndef GUOBIAO
	/* 流し満貫の判定 */
	auto NagashiManganFlag = chkNagashiMangan(gameStat, RoundEndType);
#endif /* GUOBIAO */
	/************/
	/* 和了処理 */
	/************/
	if ((RoundEndType == Agari) || (RoundEndType == Chonbo))
		agari::agariproc(RoundEndType, gameStat, tmpUraFlag, tmpAliceFlag, ResultDesc);
	bool RenchanFlag = false;
	switch (RoundEndType) {
	/**************/
	/* 荒牌流局時 */
	/**************/
	case Ryuukyoku:
		ResultDesc = _T("荒牌流局");
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);
#ifndef GUOBIAO
		transferNotenBappu(gameStat, OrigTurn,
			checkTenpai(gameStat, ResultDesc, OrigTurn));
#endif /* GUOBIAO */

#ifndef GUOBIAO
		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			MachihaiInfo machiInfo = chkFuriten(gameStat, cnt);
			bool chonboFlag = false;
			if (gameStat->Player[cnt].RichiFlag.RichiFlag) {
				if (!isTenpai(gameStat, cnt))
					chonboFlag = true; // 錯和立直（不聴立直）の場合
				if (RuleData::chkRule("furiten_riichi", "no") && machiInfo.FuritenFlag)
					chonboFlag = true; // 振聴立直の場合
			}
			if (chonboFlag) {
				transferChonboPenalty(gameStat, cnt);
				mihajong_graphic::ui::WaitUIWithTimeout(500);
			}
		}
#endif /* GUOBIAO */
		
#ifdef GUOBIAO
		RenchanFlag = false;
#else /* GUOBIAO */
		if (RuleData::chkRule("round_continuation", "renchan_if_ready")) {
			if (isTenpai(gameStat, gameStat->GameRound % Players)) RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_always")) {
			RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_ready_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % Players) ||
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_mahjong_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % Players) &&
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		}
#endif /* GUOBIAO */
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* 和了成立時 */
	/**************/
	case Agari: {
#ifdef GUOBIAO
		const bool RenchanFlag = false;
#else /* GUOBIAO */
		const bool RenchanFlag =
			(gameStat->playerwind(gameStat->CurrentPlayer.Agari) == sEast) &&
			(!RuleData::chkRule("round_continuation", "renchan_never"));
#endif /* GUOBIAO */
#ifndef GUOBIAO
		showRenchanFlag(gameStat, RenchanFlag);
		if (RenchanFlag) {
			++(gameStat->Honba);
		} else {
#endif /* GUOBIAO */
			++(gameStat->GameRound); gameStat->Honba = 0;
#ifndef GUOBIAO
		}
#endif /* GUOBIAO */
		gameStat->Deposit = 0;
		// 八連荘成立時、カウンタをリセット
		if (gameStat->AgariChain == 8)
			gameStat->AgariChain = 0;
	}
		break;
	/**************/
	/* 錯和発生時 */
	/**************/
	case Chonbo:
		gameStat->AgariChain = 0; gameStat->LastAgariPlayer = -1;
		break;
#ifndef GUOBIAO
	/**************/
	/* 九種流局時 */
	/**************/
	case KyuushuKyuuhai:
		if      (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sEast ) % Players))
			ResultDesc = _T("東家の九種九牌");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sSouth) % Players))
			ResultDesc = _T("南家の九種九牌");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sWest ) % Players))
			ResultDesc = _T("西家の九種九牌");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sNorth) % Players))
			ResultDesc = _T("北家の九種九牌");
		else ResultDesc = _T("九種九牌"); // ←決して実行されないはず
		writeChat(ResultDesc);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);

		if (RuleData::chkRule("nine_terminals", "next_dealer") == 0)
			RenchanFlag = ((!RuleData::chkRule("nine_terminals", "renchan_if_dealer_kyuushu")) || (gameStat->CurrentPlayer.Active == (gameStat->GameRound % Players)));
		else
			RenchanFlag = false;
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* 四槓流局時 */
	/**************/
	case Suukaikan:
		ResultDesc = _T("四開槓");
		ryuukyokuScreen(sound::IDs::voxSikang, &ResultDesc, mihajong_graphic::tblSubsceneSikang);
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_kong_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* 三家和の時 */
	/**************/
	case TripleRon:
		mihajong_graphic::ui::WaitUIWithTimeout(1300);
		ResultDesc = gameStat->chkGameType(AllSanma) ? _T("二家和") : _T("三家和");
		ryuukyokuScreen(sound::IDs::voxSanjiahu, &ResultDesc, mihajong_graphic::tblSubsceneTripleRon);

		if (RuleData::chkRule("triple_mahjong", "renchan_if_nondealer_furikomi"))
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi != (gameStat->GameRound % Players));
		else if (RuleData::chkRule("triple_mahjong", "renchan_if_north_furikomi") || RuleData::chkRule("triple_mahjong", "renchan_if_west_furikomi"))
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi == ((gameStat->GameRound + (gameStat->chkGameType(AllSanma) ? sWest : sNorth)) % Players));
		else if (RuleData::chkRule("triple_mahjong", "same_dealer"))
			RenchanFlag = true;
		else if (RuleData::chkRule("triple_mahjong", "next_dealer"))
			RenchanFlag = false;
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* 四風流局時 */
	/**************/
	case SuufonRenda:
		ResultDesc = gameStat->chkGameType(AllSanma) ? _T("三風連打") : _T("四風連打");
		ryuukyokuScreen(sound::IDs::voxSifeng, &ResultDesc, mihajong_graphic::tblSubsceneSifeng);
		ryuukyokuProc(gameStat, !(RuleData::chkRule("four_wind_ryuukyoku", "next_dealer") || RuleData::chkRule("four_wind_ryuukyoku", "next_dealer_west")));
		break;
	/**************/
	/* 四人立直時 */
	/**************/
	case SuuchaRiichi:
		ResultDesc = gameStat->chkGameType(AllSanma) ? _T("三家立直") : _T("四家立直");
		ryuukyokuScreen(sound::IDs::voxSifeng, &ResultDesc, mihajong_graphic::tblSubsceneFourRiichi, 1500u);
		checkTenpai(gameStat, ResultDesc, OrigTurn);
		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (gameStat->chkGameType(Sanma4) && (gameStat->playerwind(cnt) == sNorth))
				continue;
			// 錯和立直（不聴立直）の者がいた場合
			if (!isTenpai(gameStat, cnt)) {
				transferChonboPenalty(gameStat, cnt);
				mihajong_graphic::ui::WaitUIWithTimeout(500);
			}
		}
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_riichi_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* 流し満貫時 */
	/**************/
	case NagashiMangan:
	{
		const bool agariBgmSet = !(NagashiManganFlag[gameStat->PlayerID] || EnvTable::Instantiate()->WatchModeFlag); // 自分の流し満貫ならtrue
		const unsigned bgmNum =
			RuleData::chkRule("nagashi_mangan", "yakuman") ? (agariBgmSet ? sound::IDs::musAgariSelf3 : sound::IDs::musAgariFurikomi3)
			: (agariBgmSet ? sound::IDs::musAgariSelf2 : sound::IDs::musAgariFurikomi2);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u, bgmNum);
		transfer::resetDelta();
		{
			CodeConv::tstring ResultDesc(_T(""));
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (isNagashiMangan(gameStat, cnt)) {
					mihajong_graphic::calltext::setCall(cnt, mihajong_graphic::calltext::NagashiMangan);
					if (!ResultDesc.empty()) ResultDesc += _T("、");
					switch (gameStat->playerwind(cnt)) {
						case sEast:  ResultDesc += _T("東家"); break;
						case sSouth: ResultDesc += _T("南家"); break;
						case sWest:  ResultDesc += _T("西家"); break;
						case sNorth: ResultDesc += _T("北家"); break;
					}
					LNum AgariPointRaw = 0;
					if      (RuleData::chkRule("nagashi_mangan", "mangan"))  AgariPointRaw = 2000;
					else if (RuleData::chkRule("nagashi_mangan", "haneman")) AgariPointRaw = 3000;
					else if (RuleData::chkRule("nagashi_mangan", "baiman"))  AgariPointRaw = 4000;
					else if (RuleData::chkRule("nagashi_mangan", "yakuman")) AgariPointRaw = 8000;
					LNum agariPoint; // ダミー。この変数は使用しない。
					agari::calcAgariPoints(gameStat, agariPoint, AgariPointRaw, transfer::getDelta(), cnt);
				}
			}
			ResultDesc += _T("の流し満貫");
			writeChat(ResultDesc);
		}
		sound::Play(sound::IDs::sndPage);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall);
		mihajong_graphic::ui::WaitUIWithTimeout(1500);
		transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValNagashiMangan, 1500);
		ryuukyokuProc(gameStat, true);
	}
		break;
	/**************/
	/* 四槓流局時 */
	/**************/
	case Uukaikan:
		ResultDesc = _T("四開槓(５回目の槓での流局)");
		ryuukyokuScreen(sound::IDs::voxSikang, &ResultDesc, mihajong_graphic::tblSubsceneSikang);
		ryuukyokuProc(gameStat, !RuleData::chkRule("fifth_kong", "next_dealer"));
		break;
#endif /* GUOBIAO */
	}
#ifdef GUOBIAO
	if (RoundEndType != Chonbo)
#endif /* GUOBIAO */
	haifu::haifuwritebuffer(gameStat, OrigTurn, OrigHonba, tmpUraFlag, tmpAliceFlag, ResultDesc.c_str(), RoundEndType);
	return;
}

// -------------------------------------------------------------------------

void endround::transferChonboPenalty(GameTable* gameStat, PlayerID targetPlayer) {
	transfer::resetDelta();
#ifdef GUOBIAO
	LNum AgariPoint = 0, AgariPointRaw = 10 - 8;
#else /* GUOBIAO */
	LNum AgariPoint = 0, AgariPointRaw = 2000;
#endif /* GUOBIAO */
	agari::calcAgariPoints(gameStat, AgariPoint, AgariPointRaw, transfer::getDelta(), targetPlayer);
	transfer::negateDelta();
	/* なぜわざわざ一旦プラスで求めて符号を反転するという回りくどいことをしているのかというと
	   点パネの計算時に天井函数(数値として大きい方に丸める)的な処理を行っているため、
	   引数をマイナスで与えると(特に三麻で丸取り設定にしてるときとか)チョンボ料が減る虞があるからです */
	transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValChonboBappu, 1500);
	return;
}

// -------------------------------------------------------------------------

#ifdef GUOBIAO
namespace { /* 中国ルール用の席替え処理 */
	void swapSeat(GameTable* gameStat, std::array<int, Players>& swapSeatList) {
		// 退避
		InfoByPlayer<EnvTable::PlayerLabel> TmpPlayerDat;
		InfoByPlayer<PlayerTable> TmpPlayerTable;
		for (PlayerID i = 0; i < Players; i++) {
			TmpPlayerDat[i].PlayerName = EnvTable::Instantiate()->PlayerDat[i].PlayerName;
			TmpPlayerDat[i].RemotePlayerFlag = EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag;
			memcpy(&(TmpPlayerTable[i]), &(gameStat->Player[i]), sizeof (PlayerTable));
		}
		// 並べ替え結果を書き込み
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++) {
			EnvTable::Instantiate()->PlayerDat[swapSeatList[i]].PlayerName = TmpPlayerDat[i].PlayerName;
			EnvTable::Instantiate()->PlayerDat[swapSeatList[i]].RemotePlayerFlag = TmpPlayerDat[i].RemotePlayerFlag;
			memcpy(&(gameStat->Player[swapSeatList[i]]), &(TmpPlayerTable[i]), sizeof (PlayerTable));
		}
		gameStat->PlayerID = swapSeatList[gameStat->PlayerID];
	}
}
#endif /* GUOBIAO */

bool endround::nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn) { // 次の局へ(終了する場合はtrue)
#ifndef GUOBIAO
	// ハコ割れ終了
	if (RuleData::chkRuleApplied("buttobi_border"))
		for (PlayerID i = 0; i < (gameStat->chkGameType(SanmaT) ? 3 : 4); ++i)
			if (isDobon(gameStat, i)) return true;
	// 天辺終了
	if (RuleData::chkRuleApplied("teppen"))
		for (PlayerID i = 0; i < (gameStat->chkGameType(SanmaT) ? 3 : 4); ++i)
			if (isTeppen(gameStat, i)) return true;
	// 和了り止め
	if (RuleData::chkRuleApplied("agariyame")) {
		if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((OrigTurn + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((RoundEndType == Agari) || (RuleData::chkRule("agariyame", "yes_also_ready")))) {
				PlayerRankList Rank = calcRank(gameStat);
				if ((Rank[gameStat->GameRound % (int)Players] == 1) &&
					(gameStat->Player[gameStat->GameRound % (int)Players].PlayerScore >= (LNum)BasePoint()))
					return true;
		}
	}
	// 三麻の場合
	if (gameStat->chkGameType(SanmaT) &&
		((gameStat->GameRound % Players) == 3))
		++(gameStat->GameRound);
#endif /* GUOBIAO */
#ifdef GUOBIAO
	/* 席替え処理 */
	switch (gameStat->GameRound) {
	case 4: case 12: // 南入・北入
		{
			std::array<int, Players> playerSwitch = {1, 0, 3, 2};
			swapSeat(gameStat, playerSwitch);
		}
		break;
	case 8: // 西入
		{
			std::array<int, Players> playerSwitch = {2, 3, 1, 0};
			swapSeat(gameStat, playerSwitch);
		}
		break;
	}
#else /* GUOBIAO */
	// 南入した場合……
	if (gameStat->GameRound == 4) {
		if (RuleData::chkRule("game_length", "east_north_game")) // 東北廻しのとき
			gameStat->GameRound = 12;
		else if (RuleData::chkRule("game_length", "east_west_game")) // 東西廻しのとき
			gameStat->GameRound = 8;
		else if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game")) // 東々廻しのとき
			gameStat->GameRound = 16;
	}
#endif /* GUOBIAO */
	// 通常の半荘終了時（トップが３００００点未満だと西入サドンデス）
	if ((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) > gameStat->GameLength) {
#ifdef GUOBIAO
		return true;
#else /* GUOBIAO */
		for (PlayerID i = 0; i < (gameStat->chkGameType(SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].PlayerScore >= (LNum)BasePoint())
				return true;
		// 延長戦なし設定
		if (RuleData::chkRule("sudden_death_length", "no")) return true;
#endif /* GUOBIAO */
	}
	// 延長戦の長さに制限がある場合
#ifndef GUOBIAO
	if (RuleData::chkRule("sudden_death_length", "one_extra_round")) {
		if ((gameStat->GameRound == 16) && // 東々廻しのとき
			(RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game"))) {
				if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) >= (gameStat->GameLength + roundLoopRate())))
					return true;
		} else { // それ以外のとき
			if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) >= (gameStat->GameLength + 5)))
				return true;
		}
	}
#endif /* GUOBIAO */
	// 北場終了の場合は帰り東へ
	if (gameStat->GameRound == roundLoopRate()) {
#ifndef GUOBIAO
		if (RuleData::chkRule("sudden_death_length", "no")) { // 延長戦無しで終了
#endif /* GUOBIAO */
			return true;
#ifndef GUOBIAO
		} else { // 返り東
			++(gameStat->LoopRound); gameStat->GameRound = 0;
		}
#endif /* GUOBIAO */
	}
#ifndef GUOBIAO
	// 焼き鳥復活ルールの場合
	if (RuleData::chkRuleApplied("yakitori") && RuleData::chkRuleApplied("yakitori_again")) {
		bool flag = true;
		for (PlayerID i = 0; i < (gameStat->chkGameType(SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].YakitoriFlag) flag = false;
		if (flag)
			for (PlayerID i = 0; i < (gameStat->chkGameType(SanmaT) ? 3 : 4); ++i)
				gameStat->Player[i].YakitoriFlag = true;
	}
#endif /* GUOBIAO */
	return false;
}

// -------------------------------------------------------------------------

namespace {
	InfoByPlayer<LNum> delta;

	std::tuple<bool, signed short> checkExponent(PlayerID player, unsigned group, unsigned digit) {
		if (((LargeNum)delta[player]).digitGroup[group] / (int)pow(10.0, (int)digit) != 0) {
			if (digit == 0) {
				assert(group != 0);
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] % 10) * 100 + ((LargeNum)delta[player]).digitGroup[group - 1] / 1000000);
			} else if (digit == 1) {
				assert(group != 0);
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] % 100) * 10 + ((LargeNum)delta[player]).digitGroup[group - 1] / 10000000);
			} else {
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] / ((int)pow(10.0, (int)digit - 2))) % 1000);
			}
		}
		else if ((group == 0) && (digit == 2))
			return std::make_tuple(true, ((LargeNum)delta[player]).digitGroup[0]);
		else return std::make_tuple(false, 0);
	}

	void setTransferParam() {
		bool finishFlag = false; signed short mantissa = 0;
		for (PlayerID i = 0; i < Players; ++i) {
			mihajong_graphic::callvalue::CallValue callVal = {0, 0u};
			for (int j = DIGIT_GROUPS - 1; j >= 0; --j) {
				for (int k = (j == DIGIT_GROUPS - 1) ? 9 : 7; k >= 0; --k) {
					std::tie(finishFlag, mantissa) = checkExponent(i, j, k);
					if (finishFlag) {
						callVal.Exponent = j * 8 + k;
						callVal.Mantissa = mantissa;
						break;
					}
				}
				if (finishFlag) break;
			}
			if (mantissa != 0)
				mihajong_graphic::callvalue::setVal(i, callVal.Mantissa, callVal.Exponent);
			else
				mihajong_graphic::callvalue::setVal(i, 0, 0u);
		}
	}
}

InfoByPlayer<LNum>& endround::transfer::getDelta() {
	return delta;
}
void endround::transfer::resetDelta() {
	for (PlayerID i = 0; i < Players; ++i)
		delta[i] = 0;
}
void endround::transfer::addDelta(PlayerID player, const LNum& deltaVal) {
	delta[player] += deltaVal;
}
void endround::transfer::negateDelta() {
	for (PlayerID i = 0; i < Players; ++i)
		delta[i] *= -1;
}
void endround::transfer::doubleDelta(PlayerID player) {
	delta[player] *= 2;
}
void endround::transfer::doubleDelta() {
	for (PlayerID i = 0; i < Players; ++i)
		delta[i] *= 2;
}
void endround::transfer::transferPoints(GameTable* gameStat, unsigned subscene, unsigned wait) {
	setTransferParam();
	mihajong_graphic::Subscene(subscene);
	sound::Play(sound::IDs::sndPage);
	mihajong_graphic::ui::WaitUIWithTimeout(wait);
	for (PlayerID i = 0; i < Players; ++i)
		gameStat->Player[i].PlayerScore += (LargeNum)delta[i];
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
}
void endround::transfer::transferChip(GameTable* gameStat, unsigned subscene, unsigned wait) {
	setTransferParam();
	mihajong_graphic::Subscene(subscene);
	sound::Play(sound::IDs::sndPage);
	mihajong_graphic::ui::WaitUIWithTimeout(wait);
	for (PlayerID i = 0; i < Players; ++i)
		gameStat->Player[i].playerChip += delta[i];
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
}
