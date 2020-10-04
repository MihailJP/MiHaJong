#include "fuuro.h"

#include <sstream>
#include <cassert>
#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
#include "../sound/sound.h"
#include "../common/strcode.h"
#include "logging.h"
#include "haifu.h"
#include "ruletbl.h"
#include "envtbl.h"
#include "tileutil.h"
#include "discard.h"
#include "../common/bgmid.h"
#include "ai/class.h"
#include "../socket/socket.h"
#include "yaku/ykclass.h"
#include "yaku/yaku.h"
#include "remote.h"
#include "../graphic/graphic.h"
#include "../graphic/scenes/table/naki_id.h"
#include <chrono>
#include "../common/sleep.h"

namespace {

/* 牌譜への記録など準備処理 */
PlayerID PrepareFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex, FuuroType Mode) {
	PlayerID fuuroPlayer;
	switch (Mode) {
	case FuuroType::chii:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecchi(gameStat); // 牌譜に記録
		break;
	case FuuroType::pon:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecpon(gameStat); // 牌譜に記録
		break;
	case FuuroType::flower:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
#ifndef GUOBIAO
		gameStat->KangFlag.kangFlag = true; // 嶺上開花のフラグを立てる
#endif /* GUOBIAO */
		haifu::haifurecflower(gameStat, DiscardTileIndex); // 牌譜に記録
		break;
#ifndef GUOBIAO
	case FuuroType::north:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // 嶺上開花のフラグを立てる
		haifu::haifurecnorth(gameStat, DiscardTileIndex); // 牌譜に記録
		break;
#endif /* GUOBIAO */
	case FuuroType::daiminkan:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		gameStat->KangFlag.kangFlag = true; // 嶺上開花のフラグを立てる
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecminkan(gameStat);
#ifndef GUOBIAO
		gameStat->KangFlag.chainFlag++; // 連続槓の回数を記録
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = 1; /* 頭槓和のフラグ */
		if (RuleData::chkRule("minkan_penalty", "yes")) { /* 大明槓をすると1000点供託になるルール */
			/* TODO: 箱点のチェック */
			gameStat->Deposit++;
			gameStat->Player[fuuroPlayer].PlayerScore -= 1000;
		}
		if (gameStat->TurnRound == 1)
			gameStat->Player[fuuroPlayer].shokanFlag = true; /* 初槓のフラグ */
		else if (gameStat->TurnRound == 3)
			gameStat->Player[fuuroPlayer].kansanjunFlag = true; /* 槓三巡のフラグ */
#endif /* GUOBIAO */
		break;
	case FuuroType::ankan:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		/* FALLTHRU */
	case FuuroType::kakan:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = true; // 嶺上開花のフラグを立てる
		haifu::haifurecankan(gameStat, DiscardTileIndex); // 暗槓を仮に牌譜に記録
#ifndef GUOBIAO
		gameStat->KangFlag.chainFlag++; // 連続槓の回数を記録
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = true; /* 頭槓和のフラグ */
		if (gameStat->TurnRound == 1)
			gameStat->Player[fuuroPlayer].shokanFlag = true; /* 初槓のフラグ */
		else if (gameStat->TurnRound == 3)
			gameStat->Player[fuuroPlayer].kansanjunFlag = true; /* 槓三巡のフラグ */
#endif /* GUOBIAO */
		break;
	default:
		fuuroPlayer = -1;
	}
	return fuuroPlayer;
}

/* 暗槓・加槓を実行する(その他の鳴きの処理も行う) */
void MakeMeld(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex, FuuroType Mode, PlayerID kangPlayer) {
	unsigned nakiCount = 0;
	switch (Mode) {
	case FuuroType::ankan: /* 暗槓 */
		++gameStat->Player[kangPlayer].MeldPointer;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadConcealed;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		for (int i = 0; i < NumOfTilesInHand; i++) {
			if (gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) {
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[nakiCount] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i] = Tile();
				++nakiCount;
			}
		}
		if (nakiCount != 4) {
			CodeConv::tostringstream o;
			o << _T("暗槓するための牌が") <<
				((nakiCount < 4) ? _T("足りません") : _T("多すぎます")) <<
				_T("。牌コード [") << static_cast<int>(gameStat->CurrentDiscard.tile) << _T("] は [") <<
				nakiCount << _T("] 枚") <<
				((nakiCount < 4) ? _T("しかありません。") : _T("あります。"));
			error(o.str().c_str());
		}
		break;
	case FuuroType::kakan: /* 加槓 */
		for (unsigned i = 1; i <= (gameStat->Player[kangPlayer].MeldPointer); i++) {
			if (gameStat->Player[kangPlayer].Meld[i].tile == gameStat->CurrentDiscard.tile) {
				bool ProcessFlag = false;
				switch (gameStat->Player[kangPlayer].Meld[i].mstat) {
				case meldTripletExposedLeft:
					gameStat->Player[kangPlayer].Meld[i].mstat = meldQuadAddedLeft;
					ProcessFlag = true; break;
				case meldTripletExposedCenter:
					gameStat->Player[kangPlayer].Meld[i].mstat = meldQuadAddedCenter;
					ProcessFlag = true; break;
				case meldTripletExposedRight:
					gameStat->Player[kangPlayer].Meld[i].mstat = meldQuadAddedRight;
					ProcessFlag = true; break;
				}
				if (ProcessFlag) {
					gameStat->Player[kangPlayer].Meld[i].red[3] = gameStat->CurrentDiscard.red;
					++nakiCount;
				}
			}
		}
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id] = Tile();
		if (nakiCount != 1) {
			CodeConv::tostringstream o;
			o << _T("加槓するための明刻子が") <<
				(nakiCount ? _T("複数見つかりました。面子データに不整合が発生しています") : _T("見つかりません")) <<
				_T("。牌コード [") << static_cast<int>(gameStat->CurrentDiscard.tile) << _T("]");
			error(o.str().c_str());
		}
		break;
	case FuuroType::daiminkan: /* 大明槓 */
		for (int i = 0; i < NumOfTilesInHand; i++) {
			if ((gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) && (nakiCount < 3)) {
				// 鳴いた牌を純手牌から消去
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[nakiCount + 1] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i] = Tile();
				++nakiCount;
			}
		}
		if (nakiCount != 3) {
			CodeConv::tostringstream o;
			o << _T("明槓するための牌が") <<
				((nakiCount < 3) ? _T("足りません") : _T("多すぎます")) <<
				_T("。牌コード [") << static_cast<int>(gameStat->CurrentDiscard.tile) << _T("] は [") <<
				nakiCount << _T("] 枚") <<
				((nakiCount < 3) ? _T("しかありません。") : _T("あります。"));
			error(o.str().c_str());
		}
		/* 門前清フラグを降ろす */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* 槓子として晒す */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (playerRelative(gameStat->CurrentPlayer.Active, kangPlayer) == SeatRelative::left)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedLeft;
		else if (playerRelative(gameStat->CurrentPlayer.Active, kangPlayer) == SeatRelative::opposite)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedCenter;
		else if (playerRelative(gameStat->CurrentPlayer.Active, kangPlayer) == SeatRelative::right)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedRight;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[0] = gameStat->CurrentDiscard.red;
		/* 理牌する */
		lipai(gameStat, kangPlayer);
		break;
#ifndef GUOBIAO
	case FuuroType::north: /* 三麻抜きドラ */
		sound::Play(sound::IDs::voxFlower);
		++(gameStat->Player[kangPlayer].NorthFlag);
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].red = Normal;
		gameStat->TianHuFlag = false;
		mihajong_graphic::calltext::setCall(kangPlayer, mihajong_graphic::calltext::CallType::north);
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
		lipai(gameStat, kangPlayer);
		for (PlayerID i = 0; i < Players; i++)
			gameStat->Player[i].FirstDrawFlag = false;
		break;
#endif /* GUOBIAO */
	case FuuroType::flower: /* 花牌 */
		sound::Play(sound::IDs::voxFlower);
		switch (gameStat->CurrentDiscard.tile) {
			case Spring:        gameStat->Player[kangPlayer].FlowerFlag.Spring = true; break;
			case Summer:        gameStat->Player[kangPlayer].FlowerFlag.Summer = true; break;
			case Autumn:        gameStat->Player[kangPlayer].FlowerFlag.Autumn = true; break;
			case Winter:        gameStat->Player[kangPlayer].FlowerFlag.Winter = true; break;
			case Plum:          gameStat->Player[kangPlayer].FlowerFlag.Plum   = true; break;
			case Orchid:        gameStat->Player[kangPlayer].FlowerFlag.Orchid = true; break;
			case Chrysanthemum: gameStat->Player[kangPlayer].FlowerFlag.Chrys  = true; break;
			case Bamboo:        gameStat->Player[kangPlayer].FlowerFlag.Bamboo = true; break;
		default:
			{
				CodeConv::tostringstream o;
				o << _T("牌コード [") << static_cast<int>(gameStat->CurrentDiscard.tile) << _T("] は花牌ではありません。");
				warn(o.str().c_str());
			}
		}
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id] = Tile();
		gameStat->TianHuFlag = false;
		mihajong_graphic::calltext::setCall(kangPlayer, mihajong_graphic::calltext::CallType::flower);
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
		lipai(gameStat, kangPlayer);
		for (PlayerID i = 0; i < Players; i++)
			gameStat->Player[i].FirstDrawFlag = false;
		break;
	case FuuroType::pon: /* ポン */
		/* ポンした対子を消去 */
		nakiCount = 0;
		for (int i = 0; i < NumOfTilesInHand; i++) {
			if ((gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) && (nakiCount < 2)) {
				// 鳴いた牌を純手牌から消去
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[nakiCount + 1] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i] = Tile();
				++nakiCount;
			}
		}
		if (nakiCount < 2) {
			CodeConv::tostringstream o;
			o << _T("ポンするための牌が足りません。牌コード [") << static_cast<int>(gameStat->CurrentDiscard.tile) <<
				_T("] は [") << nakiCount << _T("] 枚しかありません。");
			error(o.str().c_str());
		}
		/* 門前清フラグを降ろす */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* 明刻として晒す */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (playerRelative(gameStat->CurrentPlayer.Active, kangPlayer) == SeatRelative::left)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedLeft;
		else if (playerRelative(gameStat->CurrentPlayer.Active, kangPlayer) == SeatRelative::opposite)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedCenter;
		else if (playerRelative(gameStat->CurrentPlayer.Active, kangPlayer) == SeatRelative::right)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedRight;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[0] = gameStat->CurrentDiscard.red;
		/* 理牌する */
		lipai(gameStat, kangPlayer);
		/* ポンを宣言する */
		sound::Play(sound::IDs::voxPon);
		mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Passive, mihajong_graphic::calltext::CallType::pon);
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
		/* 喰い替えの判定に使う変数を設定 */
#ifndef GUOBIAO
		gameStat->PreviousMeld.Discard = gameStat->CurrentDiscard.tile;
		gameStat->PreviousMeld.Stepped = NoTile;
		/* 包の判定 */
		checkpao(gameStat);
#endif /* GUOBIAO */
		break;
	case FuuroType::chii: /* チー */
		/* チーする牌を設定 */
		gameStat->Player[kangPlayer].Tsumohai() = gameStat->CurrentDiscard;
		/* 純手牌から鳴いた塔子を除去 */
		{
			bool nakiCount[3] = {false};
			for (int i = NumOfTilesInHand - 1; i < NumOfTilesInHand * 2 - 1; i++) {
				for (unsigned j = 0; j < 3; j++) {
					if ((!nakiCount[j]) && (gameStat->Player[kangPlayer].Hand[i % NumOfTilesInHand].tile ==
						(gameStat->CurrentDiscard.tile + j + 1 - static_cast<int>(gameStat->Player[kangPlayer].DeclarationFlag.Chi)))) {
							gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[j] = gameStat->Player[kangPlayer].Hand[i % NumOfTilesInHand].red;
							gameStat->Player[kangPlayer].Hand[i % NumOfTilesInHand] = Tile();
							nakiCount[j] = true;
					}
				}
			}
			assert(!gameStat->Player[kangPlayer].Tsumohai());
		}
		/* 門前フラグを降ろす */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* 順子を晒す */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == ChiiType::lower)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedLower;
		else if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == ChiiType::middle)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedMiddle;
		else if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == ChiiType::upper)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedUpper;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile =
			static_cast<TileCode>(gameStat->CurrentDiscard.tile + 1 - static_cast<int>(gameStat->Player[kangPlayer].DeclarationFlag.Chi));
		/* 自動理牌 */
		lipai(gameStat, kangPlayer);
		/* チーを宣言 */
		sound::Play(sound::IDs::voxChi);
		mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Passive, mihajong_graphic::calltext::CallType::chii);
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
#ifndef GUOBIAO
		/* 喰い替え判定用の変数を指定 */
		gameStat->PreviousMeld.Discard = gameStat->CurrentDiscard.tile;
		if (RuleData::chkRule("kuikae", "agari_houki") || RuleData::chkRule("kuikae", "chombo")) {
			switch (gameStat->Player[kangPlayer].DeclarationFlag.Chi) {
				case ChiiType::lower:  gameStat->PreviousMeld.Stepped = static_cast<TileCode>(gameStat->CurrentDiscard.tile + 3); break;
				case ChiiType::middle: gameStat->PreviousMeld.Stepped = NoTile; break;
				case ChiiType::upper:  gameStat->PreviousMeld.Stepped = static_cast<TileCode>(gameStat->CurrentDiscard.tile - 3); break;
			}
		} else {
			gameStat->PreviousMeld.Stepped = NoTile;
		}
#endif /* GUOBIAO */
		break;
	default:
		{
			CodeConv::tostringstream o;
			o << _T("モードが正しくありません [") << static_cast<int>(Mode) << _T("]");
			error(o.str().c_str());
		}
	}

	/* 暗槓・加槓を実行する */
	nakiCount = 0;
	if ((Mode == FuuroType::chii) || (Mode == FuuroType::pon) || (Mode == FuuroType::daiminkan)) {
		/* 鳴いた捨牌を河で非表示にする */
		PlayerTable* const activePlDat = &(gameStat->statOfActive());
		DiscardTile* const tmpSutehaiVar = &(activePlDat->Discard[activePlDat->DiscardPointer]);
		assert(tmpSutehaiVar->tcode.tile == gameStat->CurrentDiscard.tile); // [デバッグ用]本当に正しい牌なのか確認
		if (tmpSutehaiVar->dstat == discardNormal)
			tmpSutehaiVar->dstat = discardTaken;
		else if (tmpSutehaiVar->dstat == discardRiichi)
			tmpSutehaiVar->dstat = discardRiichiTaken;
	}
	/* カンを宣言、天和や地和のフラグが立っていれば降ろす */
	if ((Mode == FuuroType::ankan) || (Mode == FuuroType::kakan) || (Mode == FuuroType::daiminkan)) {
		gameStat->TianHuFlag = false;
		sound::Play(sound::IDs::voxKan);
		mihajong_graphic::calltext::setCall(kangPlayer, mihajong_graphic::calltext::CallType::kan);
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
		lipai(gameStat, kangPlayer);
		for (PlayerID i = 0; i < Players; i++)
			gameStat->Player[i].FirstDrawFlag = false;
	}
}

/* 槍槓の判定 */
bool CheckChankan(GameTable* const gameStat, EndType* RoundEndType, FuuroType Mode) {
	if ((Mode != FuuroType::daiminkan) && (Mode != FuuroType::pon) && (Mode != FuuroType::chii)) {
		/* 国士無双に限り、暗槓を搶槓可能 */
		/* 断幺牌の暗槓は搶槓しようがないんですが、面倒なので区別せず処理
		   暗槓非開示設定の時は何を暗槓したか分からないので処理をスキップ*/
		/* 搶槓の判定 ここから */
		/* 搶槓の際の注意事項。元々のポンが自分の捨牌のものの場合
		   振聴となるので搶槓はできません */
#ifdef GUOBIAO
		if (Mode == FuuroType::kakan) {
#else /* GUOBIAO */
		if ((Mode == FuuroType::ankan) || (Mode == FuuroType::kakan) || (Mode == FuuroType::north)) {
			if ((Mode == FuuroType::kakan) || (Mode == FuuroType::north) ||
				(RuleData::chkRule("ankan_conceal", "open") && RuleData::chkRule("ankan_chankan", "yes"))) {
#endif /* GUOBIAO */
					debug(_T("搶槓の判定をします。"));
					if (Mode == FuuroType::ankan) gameStat->KangFlag.chankanFlag = ChankanStat::ankan;
					else if (Mode == FuuroType::north) gameStat->KangFlag.chankanFlag = ChankanStat::north;
					else gameStat->KangFlag.chankanFlag = ChankanStat::regular;
					*RoundEndType = ronhuproc(gameStat);
					if (Mode == FuuroType::north) gameStat->KangFlag.chankanFlag = ChankanStat::none;
					if (*RoundEndType == EndType::tripleRon) return true;
					if (RonPlayers(gameStat) > 0) {
						info(_T("搶槓が宣言されました。ループから出ます。"));
						return true;
					}
					gameStat->KangFlag.chankanFlag = ChankanStat::none;
#ifndef GUOBIAO
			}
#endif /* GUOBIAO */
		}
		/* 搶槓の判定 ここまで */
		debug(_T("副露に成功しました。"));
		switch (Mode) {
			case FuuroType::ankan: case FuuroType::kakan:
				haifu::haifureckanproc(gameStat); // 搶槓が無い場合、牌譜に記録
				break;
			case FuuroType::north:
				haifu::haifurecnorthproc(gameStat);
				break;
		}
	}
	if (Mode == FuuroType::daiminkan) {
		/* バグ防止の為 */
		for (PlayerID i = 0; i < Players; i++) {
			gameStat->Player[i].Hand[TsumohaiIndex] = Tile();
		}
#ifndef GUOBIAO
		/* 大明槓の包判定用 */
		if ((!RuleData::chkRule("minkan_pao", "no")) && (!RuleData::chkRule("minkan_pao", "no_but_2han"))) {
			gameStat->PaoFlag[pyMinkan].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyMinkan].agariPlayer = gameStat->CurrentPlayer.Passive;
		}
#endif /* GUOBIAO */
	}
	return false;
}

/* 嶺上の処理 */
bool ProcRinshan(GameTable* const gameStat, EndType* RoundEndType, FuuroType Mode, PlayerID kangPlayer) {
	if ((Mode != FuuroType::pon) && (Mode != FuuroType::chii)) {
		/* 四槓子の聴牌者がいて、５回目の槓があると流局とするルールの場合
		   その条件を判定する*/
#ifndef GUOBIAO
		if ((Mode == FuuroType::ankan) || (Mode == FuuroType::kakan) || (Mode == FuuroType::daiminkan)) {
			if (gameStat->KangNum >= 4) {
				debug(_T("5個目の槓なので直ちに流局とし、ループから出ます。"));
				*RoundEndType = EndType::uukaikan; return true;
			}
		}
#endif /* GUOBIAO */
		if (gameStat->TilePointer >= (gameStat->RinshanPointer - (gameStat->DeadTiles - 1))) {
			*RoundEndType = EndType::ryuukyoku; return true; /* 荒牌なら終了 */
		}
		/* 嶺上牌を自摸る */
		gameStat->Player[kangPlayer].Tsumohai() = gameStat->Deck[gameStat->RinshanPointer];
		--gameStat->RinshanPointer;
		sound::Play(sound::IDs::sndTsumo);
		if (gameStat->tilesLeft() < 10)
			sound::Play(sound::IDs::sndCountdown);
#ifndef GUOBIAO
		if ((Mode == FuuroType::ankan) || (Mode == FuuroType::kakan) || (Mode == FuuroType::daiminkan)) {
			/* 槓ドラをめくる */
			if (RuleData::chkRuleApplied("kandora")) {
				gameStat->DoraPointer -= 2;
				sound::Play(sound::IDs::sndMekuri);
				setdora(gameStat, 0); // 表ドラを設定する
				if (RuleData::chkRuleApplied("kandora") && RuleData::chkRuleApplied("uradora")) {
					setdora(gameStat, 1); // 裏ドラを設定する
				}
			}
			/* 包の判定 */
			checkpao(gameStat);
		}
		/* 槓子をカウントする(四開槓の判定のため) */
		if ((Mode == FuuroType::ankan) || (Mode == FuuroType::kakan) || (Mode == FuuroType::daiminkan)) {
			++gameStat->Player[kangPlayer].NumberOfQuads;
			++gameStat->KangNum;
			CodeConv::tostringstream o;
			o << _T("プレイヤー [") << static_cast<int>(kangPlayer) << _T("] が [") <<
				static_cast<int>(gameStat->Player[kangPlayer].NumberOfQuads) << _T("] 回開槓、全員の合計で [") <<
				static_cast<int>(gameStat->KangNum) << _T("] 回開槓しています。");
			debug(o.str().c_str());
		}
#endif /* GUOBIAO */
	}
	return false;
}

} /* namespace */

/* 副露の処理 */
bool fuuroproc(GameTable* const gameStat, EndType* RoundEndType, const DiscardTileNum& DiscardTileIndex, FuuroType Mode) {
	{
		CodeConv::tostringstream o;
		o << _T("副露の処理を開始します。モード[") << static_cast<int>(Mode) << _T("]");
		info(o.str().c_str());
	}
	PlayerID fuuroPlayer = PrepareFuuro(gameStat, DiscardTileIndex, Mode);

	if ((Mode != FuuroType::pon)&&(Mode != FuuroType::chii)&&(Mode != FuuroType::daiminkan)) { /* カンする牌を設定 */
		gameStat->CurrentDiscard = gameStat->Player[fuuroPlayer].Hand[DiscardTileIndex.id];
	}

	MakeMeld(gameStat, DiscardTileIndex, Mode, fuuroPlayer);
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
	if (CheckChankan(gameStat, RoundEndType, Mode)) return true;
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
	threadSleep((gameStat->KangFlag.chankanFlag != ChankanStat::none) ? 500 : 1000);
	if (ProcRinshan(gameStat, RoundEndType, Mode, fuuroPlayer)) return true;
	/* 事後処理 */
	for (PlayerID i = 0; i < Players; ++i)
		mihajong_graphic::calltext::setCall(i, mihajong_graphic::calltext::CallType::none); /* 発声文字列を消去 */
	mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::none); // 発声表示から戻る
	/* 再描画 */
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
	/* 鳴きが成立したので、一発のフラグを降ろす */
	for (PlayerID i = 0; i < Players; i++)
		gameStat->Player[i].RichiFlag.IppatsuFlag =
		gameStat->Player[i].FirstDrawFlag = false;
	return false;
}

/* 包を判定する */
void checkpao(GameTable* const gameStat) {
#ifndef GUOBIAO
	// 包の判定 ここから
	debug(_T("包の条件を判定します。"));
	unsigned DragonPons = 0, WindPons = 0, NumOfKangs = 0;
	const PlayerTable* playerStat = &gameStat->statOfPassive();
	for (unsigned i = 1; i <= playerStat->MeldPointer; i++) {
		switch (playerStat->Meld[i].tile) {
		case WhiteDragon: case GreenDragon: case RedDragon:
			++DragonPons; break;
		case EastWind: case SouthWind: case WestWind: case NorthWind:
			++WindPons; break;
		}
		switch (playerStat->Meld[i].mstat) {
		case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight:
		case meldQuadAddedLeft:   case meldQuadAddedCenter:   case meldQuadAddedRight:
		case meldQuadConcealed:
			++NumOfKangs; break;
		}
	}
	CodeConv::tostringstream o;
	if (RuleData::chkRuleApplied("normal_pao")) {
		if ((DragonPons == 3) && (gameStat->PaoFlag[pyDaisangen].paoPlayer == -1)) {
			o << _T("プレイヤー [") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("] は、大三元を包になります。");
			debug(o.str().c_str());
			gameStat->PaoFlag[pyDaisangen].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyDaisangen].agariPlayer = gameStat->CurrentPlayer.Passive;
		}
		if ((WindPons == 4) && (gameStat->PaoFlag[pyDaisixi].paoPlayer == -1)) {
			o << _T("プレイヤー [") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("] は、大四喜を包になります。");
			debug(o.str().c_str());
			gameStat->PaoFlag[pyDaisixi].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyDaisixi].agariPlayer = gameStat->CurrentPlayer.Passive;
		}
	}
	if ((RuleData::chkRule("suukantsu", "yakuman_with_pao") || RuleData::chkRule("suukantsu", "double_yakuman_with_pao")) &&
		(NumOfKangs == 4) && (gameStat->PaoFlag[pySikang].paoPlayer == -1)) {
		o << _T("プレイヤー [") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("] は、四槓子を包になります。");
		debug(o.str().c_str());
		gameStat->PaoFlag[pySikang].paoPlayer = gameStat->CurrentPlayer.Active;
		gameStat->PaoFlag[pySikang].agariPlayer = gameStat->CurrentPlayer.Passive;
	}
	// 包の判定 ここまで
	return;
#endif /* GUOBIAO */
}

namespace {
	void playerfuuro(GameTable* gameStat) {
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		PlayerTable* const playerStat = &(gameStat->statOfMine());
		using namespace mihajong_graphic;
		using namespace mihajong_graphic::naki;
		if (gameStat->KangFlag.chankanFlag != ChankanStat::none) {
			threadSleep(500);
			Subscene(TableSubsceneID::playerChankan);
		} else {
			Subscene(TableSubsceneID::playerNaki);
		}
#ifdef _WIN32
		DWORD result = ui::WaitUI();
#else /*_WIN32*/
		uint32_t result = ui::WaitUI();
#endif /*_WIN32*/
		if (gameStat->KangFlag.chankanFlag != ChankanStat::none) {
			Subscene(TableSubsceneID::callChankanPre);
		} else {
			Subscene(TableSubsceneID::none);
		}
		if (EnvTable::Instantiate()->GameMode == ClientType::client)
			MoveTile::for_each([](std::pair<int, int> p) {
				mihajong_socket::client::send(mihajong_socket::protocol::Lipai_From + p.first);
				mihajong_socket::client::send(mihajong_socket::protocol::Lipai_To + p.second);
			}, gameStat->CurrentPlayer.Passive, true);
		switch (static_cast<NakiTypeID>(result)) {
		case nakiRon:
			debug(_T("プレイヤーからの応答：ロン"));
			playerStat->DeclarationFlag.Ron = true;
			playerStat->Tsumohai() = gameStat->CurrentDiscard;
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Ron);
			break;
		case nakiKan:
			debug(_T("プレイヤーからの応答：カン"));
			playerStat->DeclarationFlag.Kan = true;
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Kan);
			break;
		case nakiPon:
			debug(_T("プレイヤーからの応答：ポン"));
			playerStat->DeclarationFlag.Pon = true;
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Pon);
			break;
		case nakiChiLower:
			debug(_T("プレイヤーからの応答：チー(小さい側)"));
			playerStat->DeclarationFlag.Chi = ChiiType::lower;
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Chii_Lower);
			break;
		case nakiChiMiddle:
			debug(_T("プレイヤーからの応答：チー(嵌塔子)"));
			playerStat->DeclarationFlag.Chi = ChiiType::middle;
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Chii_Middle);
			break;
		case nakiChiUpper:
			debug(_T("プレイヤーからの応答：チー(大きい側)"));
			playerStat->DeclarationFlag.Chi = ChiiType::upper;
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Chii_Upper);
			break;
		case nakiNone:
			debug(_T("プレイヤーからの応答：通し"));
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Ignore);
			break;
		default:
			{
				CodeConv::tostringstream o;
				o << _T("インターフェイスからの戻り値が異常です！！ [") <<
					result << _T(']');
				error(o.str().c_str());
			}
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Ignore);
			break;
		}
	}
}

/* リアクションを問い合わせる */
void askReaction(GameTable* const gameStat) {
	info(_T("リアクションを問い合わせます。"));
	/* 栄和や鳴き仕掛けをするかどうか問い合わせる */
	for (int pl = 0; pl < Players; pl++) {
		gameStat->Player[pl].DeclarationFlag.Ron =
			gameStat->Player[pl].DeclarationFlag.Pon =
			gameStat->Player[pl].DeclarationFlag.Kan = false;
		gameStat->Player[pl].DeclarationFlag.Chi = ChiiType::none;
	}
	for (PlayerID i = 0; i < Players; i++) {
		if (gameStat->CurrentPlayer.Active != i) {
			gameStat->CurrentPlayer.Passive = i;
			if ((!EnvTable::Instantiate()->WatchModeFlag) && (gameStat->CurrentPlayer.Passive == gameStat->PlayerID)) {
				mihajong_graphic::GameStatus::updateGameStat(gameStat);
				playerfuuro(gameStat);
			} else if (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Passive].RemotePlayerFlag == 0) {
				/* COMが「カンニング」しないように処理 */
				GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Passive);
				/* 処理 */
				aiscript::compfuuro(sandbox);
				for (PlayerID j = 0; j < Players; j++) {
					if (!gameStat->Player[j].DeclarationFlag.Ron) gameStat->Player[j].DeclarationFlag.Ron = sandbox->Player[j].DeclarationFlag.Ron;
					if (!gameStat->Player[j].DeclarationFlag.Kan) gameStat->Player[j].DeclarationFlag.Kan = sandbox->Player[j].DeclarationFlag.Kan;
					if (!gameStat->Player[j].DeclarationFlag.Pon) gameStat->Player[j].DeclarationFlag.Pon = sandbox->Player[j].DeclarationFlag.Pon;
					if (gameStat->Player[j].DeclarationFlag.Chi == ChiiType::none) gameStat->Player[j].DeclarationFlag.Chi = sandbox->Player[j].DeclarationFlag.Chi;
				}
			}
		} else if (i == gameStat->PlayerID) {
			if (EnvTable::Instantiate()->GameMode == ClientType::client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Ignore);
		}
	}
	/* 通信対戦時の処理 */
	if (EnvTable::Instantiate()->GameMode != ClientType::standalone)
		RemoteAction::remotenaki(gameStat);
	{
		CodeConv::tostringstream o;
		o << _T("ロン [");
		for (PlayerID i = 0; i < Players; i++)
			o << (i ? _T(" ") : _T("")) << static_cast<int>(gameStat->Player[i].DeclarationFlag.Ron);
		o << _T("] カン [");
		for (PlayerID i = 0; i < Players; i++)
			o << (i ? _T(" ") : _T("")) << static_cast<int>(gameStat->Player[i].DeclarationFlag.Kan);
		o << _T("] ポン [");
		for (PlayerID i = 0; i < Players; i++)
			o << (i ? _T(" ") : _T("")) << static_cast<int>(gameStat->Player[i].DeclarationFlag.Pon);
		o << _T("] チー [");
		for (PlayerID i = 0; i < Players; i++)
			o << (i ? _T(" ") : _T("")) << static_cast<int>(gameStat->Player[i].DeclarationFlag.Chi);
		o << _T("]"); trace(o.str().c_str());
	}
	/* 並べ替えを適用 */
	for (PlayerID i = 0; i < Players; i++)
		MoveTile::apply(gameStat, i);
	/* 和了り放棄の時は宣言を無効にする */
	for (PlayerID i = 0; i < Players; i++) {
		if (gameStat->Player[i].AgariHouki) {
			{
				CodeConv::tostringstream o;
				o << _T("プレイヤー [") << static_cast<int>(i) << _T("] は和了り放棄です。宣言フラグを下ろします。");
				debug(o.str().c_str());
			}
			gameStat->Player[i].DeclarationFlag.Ron = gameStat->Player[i].DeclarationFlag.Pon =
				gameStat->Player[i].DeclarationFlag.Kan = false;
			gameStat->Player[i].DeclarationFlag.Chi = ChiiType::none;
		}
	}
	/* 当たり牌見逃しを同順フリテンにする処理 */
#ifndef GUOBIAO
	for (PlayerID i = 0; i < Players; ++i) {
		const TileCode xTile = gameStat->Player[i].Tsumohai().tile;
		gameStat->Player[i].Tsumohai().tile = gameStat->CurrentDiscard.tile;
		const Shanten tmpShanten = ShantenAnalyzer::calcShanten(gameStat, i, ShantenType::all);
		if ((tmpShanten == -1) && (!(gameStat->Player[i].DeclarationFlag.Ron)))
			gameStat->Player[i].DoujunFuriten = true;
		gameStat->Player[i].Tsumohai().tile = xTile;
	}
#endif /* GUOBIAO */
}

/* 栄和のときの処理 */
EndType ronhuproc(GameTable* const gameStat) {
	EndType RoundEndType = EndType::continuing;
	/* ロンしようとする人を表示(頭ハネで蹴られるような人も含む) */
	for (PlayerID i = 0; i < Players; i++) {
		if (gameStat->Player[i].DeclarationFlag.Ron) {
			{
				CodeConv::tostringstream o;
				o << _T("プレイヤー [") << static_cast<int>(i) << _T("] は、栄和を宣言します。");
				debug(o.str().c_str());
			}
			// 優先権のないロンも表示されるようにする
			mihajong_graphic::calltext::setCall(i, mihajong_graphic::calltext::CallType::ron);
			gameStat->Player[i].HandStat = HandStatCode::exposed;
		}
	}
	/* 実際にロンできる人を表示 */
	unsigned roncount = 0;
	for (int i = 0; i < (Players - 1); i++) {
		if (gameStat->Player[RelativePositionOf(gameStat->CurrentPlayer.Active, static_cast<SeatRelative>(i + 1))].DeclarationFlag.Ron) {
			bool accept = false;
			switch (roncount) {
			case 0:
				accept = true; break;
#ifdef GUOBIAO
			default:
				accept = false; break;
#else /* GUOBIAO */
			case 1:
				accept =
					RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw") ||
					RuleData::chkRule("multiple_mahjong", "dual_mahjong") ||
					RuleData::chkRule("multiple_mahjong", "trial_mahjong");
				break;
			case 2:
				accept = RuleData::chkRule("multiple_mahjong", "trial_mahjong");
				break;
#endif /* GUOBIAO */
			}
			if (accept) {
				CodeConv::tostringstream o;
				o << _T("プレイヤー [") << static_cast<int>(i) << _T("] は、栄和できます。");
				debug(o.str().c_str());
				mihajong_graphic::calltext::setCall(
					RelativePositionOf(gameStat->CurrentPlayer.Active, static_cast<SeatRelative>(i + 1)),
					mihajong_graphic::calltext::CallType::ronQualified);
			}
			++roncount;
		}
	}
	/* 実際に栄和を行なう処理 */
	for (int i = 0; i < (Players - 1); i++) {
		threadYield();
		PlayerID pl = RelativePositionOf(gameStat->CurrentPlayer.Active, static_cast<SeatRelative>(i + 1));
		if (gameStat->Player[pl].DeclarationFlag.Ron) {
			/* 栄和したことを変数に記録 */
			RoundEndType = EndType::agari; gameStat->TsumoAgariFlag = false;
			gameStat->CurrentPlayer.Furikomi = gameStat->CurrentPlayer.Active;
			gameStat->CurrentPlayer.Agari = pl;
			/* 八連荘の判定に使う変数 */
#ifndef GUOBIAO
			if (RuleData::chkRuleApplied("paarenchan")) {
				if (gameStat->LastAgariPlayer == gameStat->CurrentPlayer.Agari) {
					++gameStat->AgariChain;
				} else if (RuleData::chkRuleApplied("breaking_paarenchan") && (gameStat->AgariChain == 7)) {
					gameStat->AgariChain = -1;
					gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Agari;
				} else { /* FIXME: 八連荘ありの時にダブロン・トリロンした時の処理 */
					gameStat->AgariChain = 1;
					gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Agari;
				}
			}
#endif /* GUOBIAO */
			/* 和了り牌を設定 */
			gameStat->statOfAgari().Tsumohai() = gameStat->CurrentDiscard;
			/* 立直宣言牌での放銃の場合、立直を無効とし供託点棒を返却する */
#ifndef GUOBIAO
			if (gameStat->statOfActive().RichiFlag.IppatsuFlag) {
				if (gameStat->statOfActive().RichiFlag.DoubleFlag) {
					gameStat->DoubleRichiCounter = true;
					trace(_T("ダブル立直宣言牌での放銃のため、立直棒を返還します。"));
				} else {
					gameStat->RichiCounter = true;
					trace(_T("立直宣言牌での放銃のため、立直棒を返還します。"));
				}
				gameStat->statOfActive().RichiFlag.IppatsuFlag =
					gameStat->statOfActive().RichiFlag.DoubleFlag =
					gameStat->statOfActive().RichiFlag.OpenFlag =
					gameStat->statOfActive().RichiFlag.RichiFlag = false;
				--gameStat->Deposit;
				gameStat->statOfActive().PlayerScore += 1000;
			}
#endif /* GUOBIAO */
			/* 役や振聴の判定 */
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, pl);
			MachihaiInfo mInfo = chkFuriten(gameStat, pl);
			// 縛りを満たさないか、振聴のとき
#ifdef GUOBIAO
			if (!yaku::yakuCalculator::checkShibari(gameStat, &yakuInfo)) {
					trace(_T("縛りを満たさない和了です。次の処理をチョンボ用に切り替えます。"));
#else /* GUOBIAO */
			if ((!yaku::yakuCalculator::checkShibari(gameStat, &yakuInfo)) ||
				(mInfo.FuritenFlag) || (gameStat->Player[pl].DoujunFuriten) ||
				(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->Player[pl].RichiFlag.RichiFlag)) ||
				((!RuleData::chkRuleApplied("kataagari")) && (!isKataagari(gameStat, gameStat->CurrentPlayer.Active)))) {
					trace(_T("縛りを満たさないか振聴です。次の処理をチョンボ用に切り替えます。"));
#endif /* GUOBIAO */
					RoundEndType = EndType::chonbo; // チョンボにする
			}
			// ロンをしたことを表示
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Agari, mihajong_graphic::calltext::CallType::ronQualified);
			gameStat->statOfAgari().HandStat = HandStatCode::exposed;
			/* 和了り牌を設定 */
			gameStat->statOfAgari().Tsumohai() = gameStat->CurrentDiscard;
			/* 栄和のサウンドを鳴らす */
			if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
				sound::Play(sound::IDs::voxRonFurikomi);
			else
				sound::Play(sound::IDs::voxRon);
			/* 画面更新して戻る */
			if (gameStat->KangFlag.chankanFlag != ChankanStat::none) {
				mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::callChankan); // 発声表示処理
			} else {
				mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
			}
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			break;
		}
	}
	/* 牌譜に記録 */
	roncount = 0;
	if (RonPlayers(gameStat) > 0) {
		if (gameStat->KangFlag.chankanFlag != ChankanStat::none) {
			if (gameStat->KangFlag.chankanFlag == ChankanStat::north)
				haifu::haifurecchanpei(gameStat);
			else
				haifu::haifurecchankan(gameStat);
		} else {
			haifu::haifurecfurikomi(gameStat);
		}
	} else if (!gameStat->KangFlag.kangFlag) { // 抜き北で牌譜がずれるのを抑止
		haifu::haifurecfurikomi(gameStat);
	}
#ifndef GUOBIAO
	if (gameStat->chkGameType(GameTypeID::allSanma)) {
		// 二家和の判定
		if ((RonPlayers(gameStat) >= 2) && RuleData::chkRule("multiple_mahjong", "aborted"))
			return EndType::tripleRon;
	} else {
		// 三家和の判定
		if ((RonPlayers(gameStat) >= 3) &&
			(RuleData::chkRule("multiple_mahjong", "single_mahjong_with_draw") ||
			RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw")))
			return EndType::tripleRon;
	}
#endif /* GUOBIAO */
	return RoundEndType;
}

bool executeFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	/* 捨牌をポン、または大明槓する場合の処理 */
	/* 同じ牌は４枚しかないので、ポンと明槓は同時に起こることがない */
	unsigned declCount = 0; EndType roundEndType = EndType::continuing;
	for (PlayerID i = 0; i < Players; i++) {
		if (gameStat->Player[i].DeclarationFlag.Pon) ++declCount;
		if (gameStat->Player[i].DeclarationFlag.Kan) ++declCount;
	}
	if (declCount > 1)
		error(_T("複数同時のポン・槓が宣言されています。"));
	for (PlayerID i = 0; i < Players; i++) {
		threadYield();
		/* 捨牌をポンする場合 */
		if (gameStat->Player[i].DeclarationFlag.Pon) {
			gameStat->CurrentPlayer.Passive = i; // 鳴いたプレイヤーを設定
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroType::pon);
			break;
		}
		/* 明槓の場合 */
		const unsigned kanLim = (RuleData::chkRuleApplied("fifth_kong") ? 5 : 4);
		if (gameStat->Player[i].DeclarationFlag.Kan && (gameStat->KangNum < kanLim)) {
			gameStat->CurrentPlayer.Passive = i; // 鳴いたプレイヤーを設定
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroType::daiminkan);
			break;
		}
	}
	/* 吃とポンが同時に起こった場合、ポンを優先する */
	if (declCount > 0) {
		/* ポンや槓の時はツモ順を飛ばしたとみなして数え、北家→東家をまたいだ場合は次の巡目として扱う */
		if (gameStat->playerwind(gameStat->CurrentPlayer.Passive) < gameStat->playerwind(gameStat->CurrentPlayer.Active))
			++gameStat->TurnRound;
		gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive;
		return true;
	} else if (!gameStat->chkGameType(GameTypeID::allSanma)) {
		/* 吃の処理 */
		/* 三人打ちでは吃なし */
		if (gameStat->Player[RelativePositionOf(gameStat->CurrentPlayer.Active, SeatRelative::right)].DeclarationFlag.Chi != ChiiType::none) {
			/* ポンや槓の時はツモ順を飛ばしたとみなして数え、北家→東家をまたいだ場合は次の巡目として扱う */
			gameStat->CurrentPlayer.Passive = RelativePositionOf(gameStat->CurrentPlayer.Active, SeatRelative::right); // 吃ができるのは上家の捨牌のみ
			if (gameStat->playerwind(gameStat->CurrentPlayer.Passive) < gameStat->playerwind(gameStat->CurrentPlayer.Active))
				++gameStat->TurnRound;
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroType::chii);
			gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive;
			return true;
		}
	}
	/* バグ防止のアレ */
	for (PlayerID i = 0; i < Players; i++) {
		gameStat->Player[i].Hand[TsumohaiIndex] = Tile();
	}
	return false;
}
