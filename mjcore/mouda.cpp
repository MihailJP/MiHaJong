#include "mouda.h"

#include <cassert>
#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
#include "../common/strcode.h"
#include "logging.h"
#include "haifu.h"
#include "envtbl.h"
#include "ai/class.h"
#include "remote.h"
#include "../socket/socket.h"
#include "yaku/ykclass.h"
#include "yaku/yaku.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "fuuro.h"
#include "../graphic/graphic.h"
#include "ruletbl.h"

namespace {
	DiscardTileNum playerdahai(const GameTable* gameStat) { // プレイヤーの打牌
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubscenePlayerDahai);
#ifdef _WIN32
		DWORD result = mihajong_graphic::ui::WaitUI();
#else /*_WIN32*/
		uint32_t result = mihajong_graphic::ui::WaitUI();
#endif /*_WIN32*/
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
		DiscardTileNum discardTile(DiscardTileNum::fromSingleInt(result));

		uint8_t dahaiStreamCode = 0x00;
		if (discardTile.type == DiscardTileNum::Agari)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Tsumo;
		else if (discardTile.type == DiscardTileNum::Kyuushu)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Kyuushu;
		else if (discardTile.type == DiscardTileNum::Normal)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Normal_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Ankan)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Ankan_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Kakan)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Kakan_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Riichi)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Riichi_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Flower)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Flower_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::OpenRiichi)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_ORiichi_Offset + discardTile.id;

		if      (EnvTable::Instantiate()->GameMode == EnvTable::Server)
			mihajong_socket::server::send(dahaiStreamCode);
		else if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
			mihajong_socket::client::send(dahaiStreamCode);

		return discardTile;
	}
}

DiscardTileNum getdahai(GameTable* const gameStat) {
	DiscardTileNum DiscardTileIndex;
	/* COMが「カンニング」しないように処理 */
	GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Active);
	/* 打牌を取得する */
	if (gameStat->CurrentPlayer.Active == gameStat->PlayerID) {
		if (gameStat->statOfActive().AgariHouki) {
			debug(_T("プレイヤーのツモ番ですが残念ながらアガリ放棄です。"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NumOfTilesInHand - 1;
		} else if (EnvTable::Instantiate()->WatchModeFlag) {
			DiscardTileIndex = aiscript::compdahai(sandbox);
		} else {
			debug(_T("プレイヤーのツモ番です。"));
			DiscardTileIndex = playerdahai(gameStat);
		}
	} else if (
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag == -1) ||
		gameStat->statOfActive().AgariHouki) {
			debug(_T("アガリ放棄か回線切断したプレイヤーのツモ番です。"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NumOfTilesInHand - 1;
	} else if ((EnvTable::Instantiate()->GameMode == EnvTable::Client) ||
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag > 0)) {
			/* ネット対戦時の処理 */
			debug(_T("リモートプレイヤーのツモ番です。"));
			DiscardTileIndex = RemoteAction::remotedahai(gameStat);
	} else {
		debug(_T("AIのツモ番です。"));
		DiscardTileIndex = aiscript::compdahai(sandbox);
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			assert((DiscardTileIndex.type == DiscardTileNum::Kyuushu) ||
				(DiscardTileIndex.type == DiscardTileNum::Agari) ||
				((DiscardTileIndex.id >= 0) && (DiscardTileIndex.id < NumOfTilesInHand)));
			switch (DiscardTileIndex.type) {
			case DiscardTileNum::Kyuushu:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Kyuushu);
				break;
			case DiscardTileNum::Agari:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Tsumo);
				break;
			case DiscardTileNum::Normal:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Normal_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Ankan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Ankan_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Kakan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Kakan_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Riichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Riichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::OpenRiichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_ORiichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Flower:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Flower_Offset + DiscardTileIndex.id);
				break;
			default:
				error(_T("CPUの打牌番号がなんかおかしい"));
				break;
			}
		}
	}
	return DiscardTileIndex;
}

namespace { /* 内部処理分割用 */
	EndType procDahaiSubAgari(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* 自摸和の処理 */
		EndType RoundEndType = Continuing;
		haifu::haifurectsumo(gameStat); // 牌譜に記録
		/* 八連荘判定用の変数 */
		if (RuleData::chkRuleApplied("paarenchan")) {
			if (gameStat->LastAgariPlayer == gameStat->CurrentPlayer.Active) {
				++gameStat->AgariChain;
			} else if (RuleData::chkRuleApplied("breaking_paarenchan") && (gameStat->AgariChain == 7)) {
				gameStat->AgariChain = -1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			} else {
				gameStat->AgariChain = 1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			}
		}
		/* 自摸和したことを変数に設定 */
		gameStat->TsumoAgariFlag = true;
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Active);
		if (((yakuInfo.CoreHan < (gameStat->ShibariFlag ? 2 : 1)) && (yakuInfo.CoreSemiMangan == 0)) ||
			(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->statOfActive().RichiFlag.RichiFlag)) ||
			((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong")))
			RoundEndType = Chonbo; /* 縛りを満たしていない場合(役が無いなど)…錯和として局を終了する */
		else
			RoundEndType = Agari; /* 縛りを満たすなら和了りとして成立 */
		gameStat->TsumoAgariFlag = true;
		gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Active;
		mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active,
			gameStat->TianHuFlag ?
			mihajong_graphic::calltext::RonQualified : //天和の時はロンと言う慣わし
			mihajong_graphic::calltext::Tsumo
			);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // 発声表示処理
		gameStat->statOfAgari().HandStat = handExposed;
		if (gameStat->TianHuFlag) sound::Play(sound::IDs::voxRon);
		else sound::Play(sound::IDs::voxTsumo);
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		return RoundEndType;
	}
	EndType procDahaiSubKyuushu(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 九種九牌が宣言された場合 */
		DiscardTileIndex.type = DiscardTileNum::Normal;
		DiscardTileIndex.id = NumOfTilesInHand; // 九種流しができない時はツモ切りとみなす
		if (RuleData::chkRuleApplied("nine_terminals") &&
			chkdaopaiability(gameStat, gameStat->CurrentPlayer.Active) &&
			gameStat->statOfActive().FirstDrawFlag) {
				mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Kyuushu);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // 発声表示処理
				gameStat->statOfActive().HandStat = handExposed;
				sound::Play(sound::IDs::voxKyuushu);
				mihajong_graphic::GameStatus::updateGameStat(gameStat);
				return KyuushuKyuuhai;
		} else {
			warn(_T("九種九牌はできません。ツモ切りとみなします。"));
			return Continuing;
		}
	}
	EndType procDahaiSubFlower(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 花牌を抜いた場合の処理 */
		if ((DiscardTileIndex.type == DiscardTileNum::Ankan) &&
			(gameStat->statOfActive().Hand[DiscardTileIndex.id].tile > TileSuitFlowers)) {
				DiscardTileIndex.type = DiscardTileNum::Flower;
				info(_T("花牌の処理に移ります。打牌コードを補正しました。"));
		}
		if (DiscardTileIndex.type == DiscardTileNum::Flower) {
			EndType RoundEndType;
			if (gameStat->chkGameType(SanmaX)) {
				/* ガリ三麻ルールで北風牌を抜いたときの処理 */
				/* このゲームではどんな手でも(国士や大四喜でなくてもいい)
					抜き北をロンできるルール */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroNorth))
					return RoundEndType;
				else return DrawRinshan;
			} else {
				/* 花牌を抜いたときの処理 */
				/* このゲームは七搶一がないので花牌でロンされることは無い */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroFlower))
					return RoundEndType;
				else return DrawRinshan;
			}
		}
		return Continuing;
	}
	EndType procDahaiSubKan(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 暗槓・加槓するときの処理 */
		const unsigned kanLim = (RuleData::chkRuleApplied("fifth_kong") ? 5 : 4);
		if ((gameStat->TilePointer < (gameStat->RinshanPointer - (gameStat->DeadTiles - 1))) && // ハイテイでない
			(gameStat->KangNum < kanLim)) { // 合計数の制限内である
				if ((DiscardTileIndex.type == DiscardTileNum::Ankan) ||
					(DiscardTileIndex.type == DiscardTileNum::Kakan)) {
						if (RuleData::chkRule("minkan_pao", "yes") || RuleData::chkRule("minkan_pao", "yes_2han")) {
							gameStat->PaoFlag[pyMinkan].paoPlayer =
								gameStat->PaoFlag[pyMinkan].agariPlayer = -1;
						}
						/* 槓をすると嶺上牌の分自摸が増えるので次の打牌へ */
						EndType roundEndType;
						if (fuuroproc(gameStat, &roundEndType, DiscardTileIndex,
							(DiscardTileIndex.type == DiscardTileNum::Ankan) ?
							FuuroAnkan : FuuroKakan))
							return roundEndType;
						else return DrawRinshan;
				}
		}
		return Continuing;
	}
	void procDahaiSubRiichi(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 立直をするときの処理 */
		if (gameStat->tilesLeft() < ACTUAL_PLAYERS) {
			// 残り４枚未満の時はリーチ無効
			DiscardTileIndex.type = DiscardTileNum::Normal;
			warn(_T("山牌の残数要件を満たしていません。通常の打牌とみなします。"));
		} else if (!isRichiReqSatisfied(gameStat, gameStat->CurrentPlayer.Active)) {
			// 点棒条件を満たしていない時はリーチ無効
			DiscardTileIndex.type = DiscardTileNum::Normal;
			warn(_T("持ち点の要件を満たしていません。通常の打牌とみなします。"));
		}
		if ((!RuleData::chkRuleApplied("open_riichi")) && (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)) {
			// オープン立直無しの時
			DiscardTileIndex.type = DiscardTileNum::Riichi;
			warn(_T("オープン立直はできません。通常の立直とみなします。"));
		}
		/* 立直を宣言する */
		if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Riichi);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // 発声表示処理
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag))
				sound::util::bgmplay(sound::IDs::musOpenrichi);
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
#ifdef _WIN32
			Sleep(1000);
#else /*_WIN32*/
			usleep(1000000);
#endif /*_WIN32*/
			gameStat->statOfActive().HandStat = handOpenRiichi;
			gameStat->statOfActive().RichiFlag.OpenFlag = true;
		}
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Riichi);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // 発声表示処理
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag)) {
					if (!(gameStat->Player[0].RichiFlag.RichiFlag || gameStat->Player[1].RichiFlag.RichiFlag ||
						gameStat->Player[2].RichiFlag.RichiFlag || gameStat->Player[3].RichiFlag.RichiFlag)) {
							if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
								sound::util::bgmplay(sound::IDs::musRichi2);
							else
								sound::util::bgmplay(sound::IDs::musRichi1);
					} else if (!EnvTable::Instantiate()->WatchModeFlag) {
						if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
							sound::util::bgmplay(sound::IDs::musRichi3);
					}
			}
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
#ifdef _WIN32
			Sleep(1000);
#else /*_WIN32*/
			usleep(1000000);
#endif /*_WIN32*/
		}
	}
	void procDahaiSubPost(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* 事後処理 */
		/* 打牌を記録する */
		DiscardTile* const newDiscard = &(gameStat->statOfActive().Discard[++gameStat->statOfActive().DiscardPointer]);
		newDiscard->tcode.tile = gameStat->CurrentDiscard.tile = gameStat->statOfActive().Hand[DiscardTileIndex.id].tile;
		newDiscard->tcode.red  = gameStat->CurrentDiscard.red  = gameStat->statOfActive().Hand[DiscardTileIndex.id].red;
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) /* 立直宣言牌の場合 */
			newDiscard->dstat = discardRiichi;
		else if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi) /* オープン立直宣言牌の場合 */
			newDiscard->dstat = discardRiichi;
		else /* それ以外の場合 */
			newDiscard->dstat = discardNormal;
		newDiscard->isDiscardThrough = (DiscardTileIndex.id == NumOfTilesInHand - 1) && (!gameStat->TianHuFlag);
		gameStat->statOfActive().Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->statOfActive().Hand[DiscardTileIndex.id].red  = Normal;
		/* 一発のフラグを降ろす */
		gameStat->statOfActive().RichiFlag.IppatsuFlag = false;
		/* 自動的に理牌を行なう */
		lipai(gameStat, gameStat->CurrentPlayer.Active);
		// このとき牌を捨てているはず 表示バグ防止のため
		for (PlayerID i = 0; i < Players; i++) {
			gameStat->Player[i].Tsumohai().tile = NoTile;
			gameStat->Player[i].Tsumohai().red  = Normal;
		}
		/* 立直をした直後の場合、千点を供託し一発のフラグを立てる */
		if ((DiscardTileIndex.type == DiscardTileNum::Riichi) || (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone); // 発声文字列を消去
			gameStat->statOfActive().RichiFlag.RichiFlag =
				gameStat->statOfActive().RichiFlag.IppatsuFlag = true;
			gameStat->statOfActive().RichiFlag.DoubleFlag =
				gameStat->statOfActive().FirstDrawFlag;
			gameStat->statOfActive().RichiFlag.OpenFlag =
				(DiscardTileIndex.type == DiscardTileNum::OpenRiichi);
			++gameStat->Deposit;
			gameStat->statOfActive().PlayerScore -= 1000;
			if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)
				chkOpenMachi(gameStat, gameStat->CurrentPlayer.Active);
		}
		/* 天和や地和のフラグを降ろす */
		gameStat->statOfActive().FirstDrawFlag =
			gameStat->TianHuFlag = false;
		/* 打牌するときの音を鳴らす */
		/* ドラを捨てる時は強打の音にする */
		if (gameStat->CurrentDiscard.tile > TileSuitFlowers)
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AkaDora) || (gameStat->DoraFlag.Omote[gameStat->CurrentDiscard.tile] > 0))
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AoDora) && (!RuleData::chkRule("blue_tiles", "-1han")))
			sound::Play(sound::IDs::sndDahai2);
		else
			sound::Play(sound::IDs::sndDahai1);
		/* このとき牌を捨てているはずなので、バグ防止のための処理 */
		for (PlayerID i = 0; i < Players; i++) {
			gameStat->Player[i].Tsumohai().tile = NoTile;
			gameStat->Player[i].Tsumohai().red  = Normal;
		}
		/* 再描画 */
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
	}
}

EndType procdahai(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) {
	EndType RoundEndType = Continuing;
	{
		CodeConv::tostringstream o;
		o << _T("プレイヤー [") << (int)gameStat->CurrentPlayer.Active <<
			_T("] 打牌タイプ [") << (int)DiscardTileIndex.type <<
			_T("] 手牌番号 [") << (int)DiscardTileIndex.id << _T("]");
		info(o.str().c_str());
	}
	/* 立直していない同順振聴ならその期限のため振聴を解除する */
	if (!gameStat->statOfActive().RichiFlag.RichiFlag)
		gameStat->statOfActive().DoujunFuriten = false;
	/* 自摸和の処理 */
	if (DiscardTileIndex.type == DiscardTileNum::Agari) {
		RoundEndType = procDahaiSubAgari(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* 九種九牌が宣言された場合 */
	if ((!gameStat->chkGameType(SanmaS)) && (DiscardTileIndex.type == DiscardTileNum::Kyuushu)) {
		RoundEndType = procDahaiSubKyuushu(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* 打牌を牌譜に記録する */
	if ((DiscardTileIndex.type == DiscardTileNum::Normal) ||
		(DiscardTileIndex.type == DiscardTileNum::Riichi) ||
		(DiscardTileIndex.type == DiscardTileNum::OpenRiichi))
		haifu::haifurecmota(gameStat, DiscardTileIndex);
	/* 花牌を抜いた場合の処理 */
	if (!gameStat->chkGameType(SanmaS)) {
		RoundEndType = procDahaiSubFlower(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* 暗槓・加槓するときの処理 */
	{
		RoundEndType = procDahaiSubKan(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	gameStat->KangFlag.kangFlag = false; // 嶺上開花のフラグを降ろす
	gameStat->PaoFlag[pyMinkan].paoPlayer = gameStat->PaoFlag[pyMinkan].agariPlayer = -1;
	/* 立直をするときの処理 */
	if ((DiscardTileIndex.type == DiscardTileNum::Riichi) ||
		(DiscardTileIndex.type == DiscardTileNum::OpenRiichi))
		procDahaiSubRiichi(gameStat, DiscardTileIndex);
	/* 戻牌天和フラグ */
	if ((gameStat->statOfActive().renpaiTenhohStat == 0) &&
		(ShantenAnalyzer::calcShanten(gameStat, gameStat->CurrentPlayer.Active, shantenAll) == -1))
		gameStat->statOfActive().renpaiTenhohStat = 1;
	else if (gameStat->statOfActive().renpaiTenhohStat == 1)
		gameStat->statOfActive().renpaiTenhohStat = -1;
	/* 事後処理 */
	procDahaiSubPost(gameStat, DiscardTileIndex);
	return Continuing;
}

void tsumoproc(GameTable* const gameStat) {
	/* 次のプレイヤーが牌を自摸る */
	gameStat->TianHuFlag = false;
	if (gameStat->chkGameType(SanmaT)) {
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % ACTUAL_PLAYERS;
	} else {
		if (gameStat->chkGameType(Sanma4) && (gameStat->playerwind(gameStat->CurrentPlayer.Active) == sWest)) /* 四人三麻の場合は北家をスキップ */
			gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % Players;
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % Players;
	}
	/* 東家の順番が回ってきたら次の巡目となる */
	if (gameStat->playerwind(gameStat->CurrentPlayer.Active) == sEast)
		++gameStat->TurnRound;
	gameStat->statOfActive().Tsumohai().tile =
		gameStat->Deck[gameStat->TilePointer].tile;
	gameStat->statOfActive().Tsumohai().red =
		gameStat->Deck[gameStat->TilePointer].red;
	gameStat->PreviousMeld.Discard = gameStat->PreviousMeld.Stepped = NoTile;
	++gameStat->TilePointer;
	sound::Play(sound::IDs::sndTsumo);
	if (gameStat->tilesLeft() < 10)
		sound::Play(sound::IDs::sndCountdown);
	return;
}
