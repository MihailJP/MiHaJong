#include "mouda.h"

DiscardTileNum getdahai(GameTable* const gameStat) {
	DiscardTileNum DiscardTileIndex;
	/* COMが「カンニング」しないように処理 */
	GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Active);
	/* 打牌を取得する */
	if (gameStat->CurrentPlayer.Active == gameStat->PlayerID) {
		if (gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki) {
			debug(_T("プレイヤーのツモ番ですが残念ながらアガリ放棄です。"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1;
		} else if (EnvTable::Instantiate()->WatchModeFlag) {
			DiscardTileIndex = aiscript::compdahai(sandbox);
		} else {
			debug(_T("プレイヤーのツモ番です。"));
			/* TODO: プレイヤー打牌選択 playerdahai GameStat, GameEnv: DiscardTileIndex = stat */
			DiscardTileIndex.type = DiscardTileNum::Normal; // テストダブル
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1; // (ツモ切り)
		}
	} else if (
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag == -1) ||
		gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki) {
			debug(_T("アガリ放棄か回線切断したプレイヤーのツモ番です。"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1;
	} else if ((EnvTable::Instantiate()->GameMode == EnvTable::Client) ||
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag > 0)) {
			/* ネット対戦時の処理 */
			debug(_T("リモートプレイヤーのツモ番です。"));
			DiscardTileIndex = RemoteAction::remotedahai(gameStat);
			/* TODO: 切断時の処理
			if (getGameMode(GameEnv) == GAMEMODE_CLIENT) {
				if (ReceivedMsg == 1023) {
					chatappend "*** ホストとの接続が切れました。\n"
					chatappend "*** この局はツモ切り、次局からCPUが代走します。\n"
					repeat NUM_OF_PLAYERS
						if (cnt != getPlayer(GameStat)) {
							setDisconnectFlag GameStat, cnt, 1
						}
					loop
				}
			}
			*/
	} else {
		debug(_T("AIのツモ番です。"));
		DiscardTileIndex = aiscript::compdahai(sandbox);
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			assert((DiscardTileIndex.type == DiscardTileNum::Kyuushu) ||
				(DiscardTileIndex.type == DiscardTileNum::Agari) ||
				((DiscardTileIndex.id >= 0) && (DiscardTileIndex.id < NUM_OF_TILES_IN_HAND)));
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
			(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag)) ||
			((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong")))
			RoundEndType = Chonbo; /* 縛りを満たしていない場合(役が無いなど)…錯和として局を終了する */
		else
			RoundEndType = Agari; /* 縛りを満たすなら和了りとして成立 */
		gameStat->TsumoAgariFlag = true;
		gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Active;
		/* TODO: 発声
		setCall getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), "ツモ"
		if (getHeavenHandFlag(GameStat) == 1) {setCall getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), "ロン"} //天和の時はロンと言う慣わし
		*/
		gameStat->Player[gameStat->CurrentPlayer.Agari].HandStat = handExposed;
		if (gameStat->TianHuFlag) sound::Play(sound::IDs::voxRon);
		else sound::Play(sound::IDs::voxTsumo);
		/* TODO: 画面更新 redrscreen */
		return RoundEndType;
	}
	EndType procDahaiSubKyuushu(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 九種九牌が宣言された場合 */
		DiscardTileIndex.type = DiscardTileNum::Normal;
		DiscardTileIndex.id = NUM_OF_TILES_IN_HAND; // 九種流しができない時はツモ切りとみなす
		if (RuleData::chkRuleApplied("nine_terminals") &&
			chkdaopaiability(gameStat, gameStat->CurrentPlayer.Active) &&
			gameStat->Player[gameStat->CurrentPlayer.Active].FirstDrawFlag) {
				/* TODO: 発声 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "九種九牌" */
				gameStat->Player[gameStat->CurrentPlayer.Active].HandStat = handExposed;
				sound::Play(sound::IDs::voxKyuushu);
				/* TODO: 画面更新 redrscreen */
				return KyuushuKyuuhai;
		} else {
			warn(_T("九種九牌はできません。ツモ切りとみなします。"));
			return Continuing;
		}
	}
	EndType procDahaiSubFlower(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* 花牌を抜いた場合の処理 */
		if ((DiscardTileIndex.type == DiscardTileNum::Ankan) &&
			(gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile > TILE_SUIT_FLOWERS)) {
				DiscardTileIndex.type = DiscardTileNum::Flower;
				info(_T("花牌の処理に移ります。打牌コードを補正しました。"));
		}
		if (DiscardTileIndex.type == DiscardTileNum::Flower) {
			EndType RoundEndType;
			if (chkGameType(gameStat, SanmaX)) {
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
		if (tilesLeft(gameStat) < ACTUAL_PLAYERS) {
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
			/* TODO: 発声 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "リーチ" */
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag))
				sound::Play(sound::IDs::musOpenrichi);
			/* TODO: 画面更新 redrscreen */
			/* TODO: 1秒待つ await 1000 */
			gameStat->Player[gameStat->CurrentPlayer.Active].HandStat = handOpenRiichi;
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.OpenFlag = true;
			/* TODO: 一部ボタンの無効化 vanish2@ */
		}
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) {
			/* TODO: 発声 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "リーチ" */
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag)) {
					if (!(gameStat->Player[0].RichiFlag.RichiFlag || gameStat->Player[1].RichiFlag.RichiFlag ||
						gameStat->Player[2].RichiFlag.RichiFlag || gameStat->Player[3].RichiFlag.RichiFlag)) {
							if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
								sound::Play(sound::IDs::musRichi2);
							else
								sound::Play(sound::IDs::musRichi1);
					} else if (!EnvTable::Instantiate()->WatchModeFlag) {
						if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
							sound::Play(sound::IDs::musRichi3);
					}
			}
			/* TODO: 画面更新 redrscreen */
			/* TODO: 1秒待つ await 1000 */
			/* TODO: 一部ボタンの無効化 vanish2@ */
		}
	}
	void procDahaiSubPost(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* 事後処理 */
		/* 打牌を記録する */
		discardTile* const newDiscard = &(gameStat->Player[gameStat->CurrentPlayer.Active].Discard[++gameStat->Player[gameStat->CurrentPlayer.Active].DiscardPointer]);
		newDiscard->tcode.tile = gameStat->CurrentDiscard.tile = gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile;
		newDiscard->tcode.red  = gameStat->CurrentDiscard.red  = gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].red;
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) /* 立直宣言牌の場合 */
			newDiscard->dstat = discardRiichi;
		else if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi) /* オープン立直宣言牌の場合 */
			newDiscard->dstat = discardRiichi;
		else /* それ以外の場合 */
			newDiscard->dstat = discardNormal;
		newDiscard->isDiscardThrough = (DiscardTileIndex.id == NUM_OF_TILES_IN_HAND - 1) && (!gameStat->TianHuFlag);
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].red  = Normal;
		/* 一発のフラグを降ろす */
		gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag = false;
		/* 自動的に理牌を行なう */
		lipai(gameStat, gameStat->CurrentPlayer.Active);
		// このとき牌を捨てているはず 表示バグ防止のため
		for (PLAYER_ID i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].red  = Normal;
		}
		/* 立直をした直後の場合、千点を供託し一発のフラグを立てる */
		if ((DiscardTileIndex.type == DiscardTileNum::Riichi) || (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)) {
			/* TODO: 発声文字列を消去 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "" */
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag =
				gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag = true;
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.DoubleFlag =
				gameStat->Player[gameStat->CurrentPlayer.Active].FirstDrawFlag;
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.OpenFlag =
				(DiscardTileIndex.type == DiscardTileNum::OpenRiichi);
			++gameStat->Deposit;
			gameStat->Player[gameStat->CurrentPlayer.Active].PlayerScore += LargeNum::fromInt(-1000);
			if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)
				chkOpenMachi(gameStat, gameStat->CurrentPlayer.Active);
		}
		/* 天和や地和のフラグを降ろす */
		gameStat->Player[gameStat->CurrentPlayer.Active].FirstDrawFlag =
			gameStat->TianHuFlag = false;
		/* 打牌するときの音を鳴らす */
		/* ドラを捨てる時は強打の音にする */
		if (gameStat->CurrentDiscard.tile > TILE_SUIT_FLOWERS)
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AkaDora) || (gameStat->DoraFlag.Omote[gameStat->CurrentDiscard.tile] > 0))
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AoDora) && (!RuleData::chkRule("blue_tiles", "-1han")))
			sound::Play(sound::IDs::sndDahai2);
		else
			sound::Play(sound::IDs::sndDahai1);
		/* このとき牌を捨てているはずなので、バグ防止のための処理 */
		for (PLAYER_ID i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].red  = Normal;
		}
		/* 再描画 */
		/* TODO: 再描画 redrscreen */
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
	if (!gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag)
		gameStat->Player[gameStat->CurrentPlayer.Active].DoujunFuriten = false;
	/* 自摸和の処理 */
	if (DiscardTileIndex.type == DiscardTileNum::Agari) {
		RoundEndType = procDahaiSubAgari(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* 九種九牌が宣言された場合 */
	if ((!chkGameType(gameStat, SanmaS)) && (DiscardTileIndex.type == DiscardTileNum::Kyuushu)) {
		RoundEndType = procDahaiSubKyuushu(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* 打牌を牌譜に記録する */
	if ((DiscardTileIndex.type == DiscardTileNum::Normal) ||
		(DiscardTileIndex.type == DiscardTileNum::Riichi) ||
		(DiscardTileIndex.type == DiscardTileNum::OpenRiichi))
		haifu::haifurecmota(gameStat, DiscardTileIndex);
	/* 花牌を抜いた場合の処理 */
	if (!chkGameType(gameStat, SanmaS)) {
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
	/* 事後処理 */
	procDahaiSubPost(gameStat, DiscardTileIndex);
	return Continuing;
}

void tsumoproc(GameTable* const gameStat) {
	/* 次のプレイヤーが牌を自摸る */
	++gameStat->TilePointer; gameStat->TianHuFlag = false;
	if (chkGameType(gameStat, SanmaT)) {
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % ACTUAL_PLAYERS;
	} else {
		if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound) == sWest)) /* 四人三麻の場合は北家をスキップ */
			gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % PLAYERS;
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % PLAYERS;
	}
	/* 東家の順番が回ってきたら次の巡目となる */
	if (playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound) == sEast)
		++gameStat->TurnRound;
	gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].tile =
		gameStat->Deck[gameStat->TilePointer].tile;
	gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].red =
		gameStat->Deck[gameStat->TilePointer].red;
	gameStat->PreviousMeld.Discard = gameStat->PreviousMeld.Stepped = NoTile;
	sound::Play(sound::IDs::sndTsumo);
	if (tilesLeft(gameStat) < 10)
		sound::Play(sound::IDs::sndCountdown);
	return;
}
