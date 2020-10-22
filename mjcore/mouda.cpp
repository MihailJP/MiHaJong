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
#include <chrono>
#include "../common/sleep.h"

namespace {
	DiscardTileNum playerdahai(const GameTable* gameStat) { // プレイヤーの打牌
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::playerDahai);
#ifdef _WIN32
		DWORD result = mihajong_graphic::ui::WaitUI();
#else /*_WIN32*/
		uint32_t result = mihajong_graphic::ui::WaitUI();
#endif /*_WIN32*/
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::none);
		DiscardTileNum discardTile(DiscardTileNum::fromSingleInt(result));

		uint8_t dahaiStreamCode = 0x00;
		if (discardTile.type == DiscardType::agari)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Tsumo;
		else if (discardTile.type == DiscardType::kyuushu)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Kyuushu;
		else if (discardTile.type == DiscardType::normal)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Normal_Offset + discardTile.id;
		else if (discardTile.type == DiscardType::ankan)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Ankan_Offset + discardTile.id;
		else if (discardTile.type == DiscardType::kakan)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Kakan_Offset + discardTile.id;
		else if (discardTile.type == DiscardType::riichi)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Riichi_Offset + discardTile.id;
		else if (discardTile.type == DiscardType::flower)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Flower_Offset + discardTile.id;
		else if (discardTile.type == DiscardType::openRiichi)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_ORiichi_Offset + discardTile.id;

		if      (EnvTable::Instantiate()->GameMode == ClientType::server)
			mihajong_socket::server::send(dahaiStreamCode);
		else if (EnvTable::Instantiate()->GameMode == ClientType::client)
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
			DiscardTileIndex.type = DiscardType::normal;
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
			DiscardTileIndex.type = DiscardType::normal;
			DiscardTileIndex.id = NumOfTilesInHand - 1;
	} else if ((EnvTable::Instantiate()->GameMode == ClientType::client) ||
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag > 0)) {
			/* ネット対戦時の処理 */
			debug(_T("リモートプレイヤーのツモ番です。"));
			DiscardTileIndex = RemoteAction::remotedahai(gameStat);
	} else {
		debug(_T("AIのツモ番です。"));
		DiscardTileIndex = aiscript::compdahai(sandbox);
		if (EnvTable::Instantiate()->GameMode == ClientType::server) {
			assert((DiscardTileIndex.type == DiscardType::kyuushu) ||
				(DiscardTileIndex.type == DiscardType::agari) ||
				((DiscardTileIndex.id >= 0) && (DiscardTileIndex.id < NumOfTilesInHand)));
			switch (DiscardTileIndex.type) {
			case DiscardType::kyuushu:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Kyuushu);
				break;
			case DiscardType::agari:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Tsumo);
				break;
			case DiscardType::normal:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Normal_Offset + DiscardTileIndex.id);
				break;
			case DiscardType::ankan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Ankan_Offset + DiscardTileIndex.id);
				break;
			case DiscardType::kakan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Kakan_Offset + DiscardTileIndex.id);
				break;
			case DiscardType::riichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Riichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardType::openRiichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_ORiichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardType::flower:
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
		EndType RoundEndType = EndType::continuing;
		haifu::haifurectsumo(gameStat); // 牌譜に記録
		/* 八連荘判定用の変数 */
#ifndef GUOBIAO
		if (RuleData::chkRuleApplied("paarenchan")) {
			if (gameStat->LastAgariPlayer == gameStat->CurrentPlayer.Active) {
				++gameStat->AgariChain;
			} else if (RuleData::chkRuleApplied("breaking_paarenchan") && (gameStat->AgariChain == 7)) {
				gameStat->AgariChain = -1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			} else {
				gameStat->AgariChain = 1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			}
		}
#endif /* GUOBIAO */
		/* 自摸和したことを変数に設定 */
		gameStat->TsumoAgariFlag = true;
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Active);
#ifndef GUOBIAO
		if ((!yaku::yakuCalculator::checkShibari(gameStat, &yakuInfo)) ||
			(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->statOfActive().RichiFlag.RichiFlag)) ||
			((gameStat->PaoFlag[PaoYakuPage::minkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong")) ||
			((!RuleData::chkRuleApplied("kataagari")) && (!isKataagari(gameStat, gameStat->CurrentPlayer.Active))))
			RoundEndType = EndType::chonbo; /* 縛りを満たしていない場合(役が無いなど)…錯和として局を終了する */
		else
#endif /* GUOBIAO */
			RoundEndType = EndType::agari; /* 縛りを満たすなら和了りとして成立 */
		gameStat->TsumoAgariFlag = true;
		gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Active;
		mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active,
			gameStat->TianHuFlag ?
			mihajong_graphic::calltext::CallType::ronQualified : //天和の時はロンと言う慣わし
			mihajong_graphic::calltext::CallType::tsumo
			);
		mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
		gameStat->statOfAgari().HandStat = HandStatCode::exposed;
		if (gameStat->TianHuFlag) sound::Play(sound::IDs::voxRon);
		else sound::Play(sound::IDs::voxTsumo);
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		return RoundEndType;
	}
	EndType procDahaiSubKyuushu(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 九種九牌が宣言された場合 */
#ifndef GUOBIAO
		DiscardTileIndex.type = DiscardType::normal;
		DiscardTileIndex.id = NumOfTilesInHand; // 九種流しができない時はツモ切りとみなす
		if (RuleData::chkRuleApplied("nine_terminals") &&
			chkdaopaiability(gameStat, gameStat->CurrentPlayer.Active) &&
			gameStat->statOfActive().FirstDrawFlag) {
				mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::CallType::kyuushu);
				mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
				gameStat->statOfActive().HandStat = HandStatCode::exposed;
				sound::Play(sound::IDs::voxKyuushu);
				mihajong_graphic::GameStatus::updateGameStat(gameStat);
				return EndType::kyuushuKyuuhai;
		} else {
#endif /* GUOBIAO */
			warn(_T("九種九牌はできません。ツモ切りとみなします。"));
			return EndType::continuing;
#ifndef GUOBIAO
		}
#endif /* GUOBIAO */
	}
	EndType procDahaiSubFlower(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 花牌を抜いた場合の処理 */
		if ((DiscardTileIndex.type == DiscardType::ankan) &&
			(gameStat->statOfActive().Hand[DiscardTileIndex.id].isFlower())) {
				DiscardTileIndex.type = DiscardType::flower;
				info(_T("花牌の処理に移ります。打牌コードを補正しました。"));
		}
		if (DiscardTileIndex.type == DiscardType::flower) {
			EndType RoundEndType;
			if (gameStat->chkGameType(GameTypeID::sanmaX)) {
				/* ガリ三麻ルールで北風牌を抜いたときの処理 */
				/* このゲームではどんな手でも(国士や大四喜でなくてもいい)
					抜き北をロンできるルール */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroType::north))
					return RoundEndType;
				else return EndType::drawRinshan;
			} else {
				/* 花牌を抜いたときの処理 */
				/* このゲームは七搶一がないので花牌でロンされることは無い */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroType::flower))
					return RoundEndType;
				else return EndType::drawRinshan;
			}
		}
		return EndType::continuing;
	}
	EndType procDahaiSubKan(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 暗槓・加槓するときの処理 */
#ifdef GUOBIAO
		constexpr unsigned kanLim = 16;
#else /* GUOBIAO */
		const unsigned kanLim = (RuleData::chkRuleApplied("fifth_kong") ? 5 : 4);
#endif /* GUOBIAO */
		if ((gameStat->TilePointer < (gameStat->RinshanPointer - (gameStat->DeadTiles - 1))) && // ハイテイでない
			(gameStat->KangNum < kanLim)) { // 合計数の制限内である
				if ((DiscardTileIndex.type == DiscardType::ankan) ||
					(DiscardTileIndex.type == DiscardType::kakan)) {
#ifndef GUOBIAO
						if (RuleData::chkRule("minkan_pao", "yes") || RuleData::chkRule("minkan_pao", "yes_2han")) {
							gameStat->PaoFlag[PaoYakuPage::minkan].paoPlayer =
								gameStat->PaoFlag[PaoYakuPage::minkan].agariPlayer = -1;
						}
#endif /* GUOBIAO */
						/* 槓をすると嶺上牌の分自摸が増えるので次の打牌へ */
						EndType roundEndType;
						if (fuuroproc(gameStat, &roundEndType, DiscardTileIndex,
							(DiscardTileIndex.type == DiscardType::ankan) ?
							FuuroType::ankan : FuuroType::kakan))
							return roundEndType;
						else return EndType::drawRinshan;
				}
		}
		return EndType::continuing;
	}
	void procDahaiSubRiichi(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 立直をするときの処理 */
#ifdef GUOBIAO
		DiscardTileIndex.type = DiscardType::normal;
		warn(_T("リーチが指定されましたが、これを無視します。"));
#else /* GUOBIAO */
		if (gameStat->tilesLeft() < ACTUAL_PLAYERS) {
			// 残り４枚未満の時はリーチ無効
			DiscardTileIndex.type = DiscardType::normal;
			warn(_T("山牌の残数要件を満たしていません。通常の打牌とみなします。"));
		} else if (!isRichiReqSatisfied(gameStat, gameStat->CurrentPlayer.Active)) {
			// 点棒条件を満たしていない時はリーチ無効
			DiscardTileIndex.type = DiscardType::normal;
			warn(_T("持ち点の要件を満たしていません。通常の打牌とみなします。"));
		}
		if ((!RuleData::chkRuleApplied("open_riichi")) && (DiscardTileIndex.type == DiscardType::openRiichi)) {
			// オープン立直無しの時
			DiscardTileIndex.type = DiscardType::riichi;
			warn(_T("オープン立直はできません。通常の立直とみなします。"));
		}
		/* 立直を宣言する */
		if (DiscardTileIndex.type == DiscardType::openRiichi) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::CallType::riichi);
			mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag))
				sound::util::bgmplay(sound::IDs::musOpenrichi);
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			threadSleep(1000);
			gameStat->statOfActive().HandStat = HandStatCode::openRiichi;
			gameStat->statOfActive().RichiFlag.OpenFlag = true;
		}
		if (DiscardTileIndex.type == DiscardType::riichi) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::CallType::riichi);
			mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::call); // 発声表示処理
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
			threadSleep(1000);
		}
#endif /* GUOBIAO */
	}
	void procDahaiSubPost(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* 事後処理 */
		/* 打牌を記録する */
		DiscardTile* const newDiscard = &(gameStat->statOfActive().Discard[++gameStat->statOfActive().DiscardPointer]);
		newDiscard->tcode = gameStat->CurrentDiscard = gameStat->statOfActive().Hand[DiscardTileIndex.id];
#ifndef GUOBIAO
		if (DiscardTileIndex.type == DiscardType::riichi) /* 立直宣言牌の場合 */
			newDiscard->dstat = DiscardStat::riichi;
		else if (DiscardTileIndex.type == DiscardType::openRiichi) /* オープン立直宣言牌の場合 */
			newDiscard->dstat = DiscardStat::riichi;
		else /* それ以外の場合 */
#endif /* GUOBIAO */
			newDiscard->dstat = DiscardStat::normal;
		newDiscard->isDiscardThrough = (DiscardTileIndex.id == NumOfTilesInHand - 1) && (!gameStat->TianHuFlag);
		gameStat->statOfActive().Hand[DiscardTileIndex.id] = Tile();
		/* 一発のフラグを降ろす */
		gameStat->statOfActive().RichiFlag.IppatsuFlag = false;
		/* 自動的に理牌を行なう */
		lipai(gameStat, gameStat->CurrentPlayer.Active);
		// このとき牌を捨てているはず 表示バグ防止のため
		for (PlayerID i = 0; i < Players; i++) {
			gameStat->Player[i].Tsumohai() = Tile();
		}
#ifndef GUOBIAO
		/* 立直をした直後の場合、千点を供託し一発のフラグを立てる */
		if ((DiscardTileIndex.type == DiscardType::riichi) || (DiscardTileIndex.type == DiscardType::openRiichi)) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::CallType::none);
			mihajong_graphic::Subscene(mihajong_graphic::TableSubsceneID::none); // 発声文字列を消去
			gameStat->statOfActive().RichiFlag.RichiFlag =
				gameStat->statOfActive().RichiFlag.IppatsuFlag = true;
			gameStat->statOfActive().RichiFlag.DoubleFlag =
				gameStat->statOfActive().FirstDrawFlag;
			gameStat->statOfActive().RichiFlag.OpenFlag =
				(DiscardTileIndex.type == DiscardType::openRiichi);
			++gameStat->Deposit;
			gameStat->statOfActive().PlayerScore -= 1000;
			if (DiscardTileIndex.type == DiscardType::openRiichi)
				chkOpenMachi(gameStat, gameStat->CurrentPlayer.Active);
		}
		/* 天和や地和のフラグを降ろす */
		gameStat->statOfActive().FirstDrawFlag =
			gameStat->TianHuFlag = false;
#endif /* GUOBIAO */
		/* 打牌するときの音を鳴らす */
		/* ドラを捨てる時は強打の音にする */
#ifndef GUOBIAO
		if (gameStat->CurrentDiscard.isFlower())
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == DoraCol::akaDora) || (gameStat->DoraFlag.Omote[gameStat->CurrentDiscard.tile] > 0))
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == DoraCol::aoDora) && (!RuleData::chkRule("blue_tiles", "-1han")))
			sound::Play(sound::IDs::sndDahai2);
		else
#endif /* GUOBIAO */
			sound::Play(sound::IDs::sndDahai1);
		/* このとき牌を捨てているはずなので、バグ防止のための処理 */
		for (PlayerID i = 0; i < Players; i++) {
			gameStat->Player[i].Tsumohai() = Tile();
		}
		/* 再描画 */
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
	}
}

EndType procdahai(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) {
	EndType RoundEndType = EndType::continuing;
	{
		CodeConv::tostringstream o;
		o << _T("プレイヤー [") << static_cast<int>(gameStat->CurrentPlayer.Active) <<
			_T("] 打牌タイプ [") << static_cast<int>(DiscardTileIndex.type) <<
			_T("] 手牌番号 [") << static_cast<int>(DiscardTileIndex.id) << _T("]");
		info(o.str().c_str());
	}
	/* 立直していない同順振聴ならその期限のため振聴を解除する */
	if (!gameStat->statOfActive().RichiFlag.RichiFlag)
		gameStat->statOfActive().DoujunFuriten = false;
	/* 自摸和の処理 */
	if (DiscardTileIndex.type == DiscardType::agari) {
		RoundEndType = procDahaiSubAgari(gameStat, DiscardTileIndex);
		if (RoundEndType != EndType::continuing) return RoundEndType;
	}
	/* 九種九牌が宣言された場合 */
	if ((!gameStat->chkGameType(GameTypeID::sanmaS)) && (DiscardTileIndex.type == DiscardType::kyuushu)) {
		RoundEndType = procDahaiSubKyuushu(gameStat, DiscardTileIndex);
		if (RoundEndType != EndType::continuing) return RoundEndType;
	}
	/* 打牌を牌譜に記録する */
	if ((DiscardTileIndex.type == DiscardType::normal) ||
		(DiscardTileIndex.type == DiscardType::riichi) ||
		(DiscardTileIndex.type == DiscardType::openRiichi))
		haifu::haifurecmota(gameStat, DiscardTileIndex);
	/* 花牌を抜いた場合の処理 */
	if (!gameStat->chkGameType(GameTypeID::sanmaS)) {
		RoundEndType = procDahaiSubFlower(gameStat, DiscardTileIndex);
		if (RoundEndType != EndType::continuing) return RoundEndType;
	}
	/* 暗槓・加槓するときの処理 */
	{
		RoundEndType = procDahaiSubKan(gameStat, DiscardTileIndex);
		if (RoundEndType != EndType::continuing) return RoundEndType;
	}
	gameStat->KangFlag.kangFlag = false; // 嶺上開花のフラグを降ろす
	gameStat->PaoFlag[PaoYakuPage::minkan].paoPlayer = gameStat->PaoFlag[PaoYakuPage::minkan].agariPlayer = -1;
#ifndef GUOBIAO
	/* 立直をするときの処理 */
	if ((DiscardTileIndex.type == DiscardType::riichi) ||
		(DiscardTileIndex.type == DiscardType::openRiichi))
		procDahaiSubRiichi(gameStat, DiscardTileIndex);
	/* 戻牌天和フラグ */
	if ((gameStat->statOfActive().renpaiTenhohStat == 0) &&
		(ShantenAnalyzer::calcShanten(gameStat, gameStat->CurrentPlayer.Active, ShantenType::all) == -1))
		gameStat->statOfActive().renpaiTenhohStat = 1;
	else if (gameStat->statOfActive().renpaiTenhohStat == 1)
		gameStat->statOfActive().renpaiTenhohStat = -1;
#endif /* GUOBIAO */
	/* 事後処理 */
	procDahaiSubPost(gameStat, DiscardTileIndex);
	return EndType::continuing;
}

void tsumoproc(GameTable* const gameStat) {
	/* 次のプレイヤーが牌を自摸る */
	gameStat->TianHuFlag = false;
	if (gameStat->chkGameType(GameTypeID::sanmaT)) {
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % ACTUAL_PLAYERS;
	} else {
		if (gameStat->chkGameType(GameTypeID::sanma4) && (gameStat->playerwind(gameStat->CurrentPlayer.Active) == SeatAbsolute::west)) /* 四人三麻の場合は北家をスキップ */
			gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % Players;
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % Players;
	}
	/* 東家の順番が回ってきたら次の巡目となる */
	if (gameStat->playerwind(gameStat->CurrentPlayer.Active) == SeatAbsolute::east)
		++gameStat->TurnRound;
	gameStat->statOfActive().Tsumohai() = gameStat->Deck[gameStat->TilePointer];
	gameStat->PreviousMeld.Discard = gameStat->PreviousMeld.Stepped = TileCode::noTile;
	++gameStat->TilePointer;
	sound::Play(sound::IDs::sndTsumo);
	if (gameStat->tilesLeft() < 10)
		sound::Play(sound::IDs::sndCountdown);
	return;
}
