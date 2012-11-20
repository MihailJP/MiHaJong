#include "fuuro.h"

/* 牌譜への記録など準備処理 */
PLAYER_ID PrepareFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex, FuuroType Mode) {
	PLAYER_ID fuuroPlayer;
	switch (Mode) {
	case FuuroChii:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecchi(gameStat); // 牌譜に記録
		break;
	case FuuroPon:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecpon(gameStat); // 牌譜に記録
		break;
	case FuuroFlower:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // 嶺上開花のフラグを立てる
		haifu::haifurecflower(gameStat, DiscardTileIndex); // 牌譜に記録
		break;
	case FuuroNorth:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // 嶺上開花のフラグを立てる
		haifu::haifurecnorth(gameStat, DiscardTileIndex); // 牌譜に記録
		break;
	case FuuroDaiminkan:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		gameStat->KangFlag.kangFlag = 1; // 嶺上開花のフラグを立てる
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecminkan(gameStat);
		gameStat->KangFlag.chankanFlag++; // 連続槓の回数を記録
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = 1; /* 頭槓和のフラグ */
		if (RuleData::chkRule("minkan_penalty", "yes")) { /* 大明槓をすると1000点供託になるルール */
			/* TODO: 箱点のチェック */
			gameStat->Deposit++;
			gameStat->Player[fuuroPlayer].PlayerScore =
				gameStat->Player[fuuroPlayer].PlayerScore - LargeNum::fromInt(1000);
		}
		break;
	case FuuroKakan: case FuuroAnkan:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // 嶺上開花のフラグを立てる
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecankan(gameStat, DiscardTileIndex); // 暗槓を仮に牌譜に記録
		gameStat->KangFlag.chankanFlag++; // 連続槓の回数を記録
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = 1; /* 頭槓和のフラグ */
		break;
	default:
		fuuroPlayer = -1;
	}
	return fuuroPlayer;
}

/* 暗槓・加槓を実行する(その他の鳴きの処理も行う) */
void MakeMeld(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex, FuuroType Mode, PLAYER_ID kangPlayer) {
	unsigned nakiCount = 0;
	switch (Mode) {
	case FuuroAnkan: /* 暗槓 */
		++gameStat->Player[kangPlayer].MeldPointer;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadConcealed;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if (gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) {
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[nakiCount] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i].tile = NoTile;
				gameStat->Player[kangPlayer].Hand[i].red = Normal;
				++nakiCount;
			}
		}
		if (nakiCount != 4) {
			CodeConv::tostringstream o;
			o << _T("暗槓するための牌が") <<
				((nakiCount < 4) ? _T("足りません") : _T("多すぎます")) <<
				_T("。牌コード [") << (int)gameStat->CurrentDiscard.tile << _T("] は [") <<
				nakiCount << _T("] 枚") <<
				((nakiCount < 4) ? _T("しかありません。") : _T("あります。"));
			error(o.str().c_str());
		}
		break;
	case FuuroKakan: /* 加槓 */
		for (unsigned i = 1; i <= gameStat->Player[kangPlayer].MeldPointer; i++) {
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
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].red = Normal;
		if (nakiCount != 1) {
			CodeConv::tostringstream o;
			o << _T("加槓するための明刻子が") <<
				(nakiCount ? _T("複数見つかりました。面子データに不整合が発生しています") : _T("見つかりません")) <<
				_T("。牌コード [") << (int)gameStat->CurrentDiscard.tile << _T("]");
			error(o.str().c_str());
		}
		break;
	case FuuroDaiminkan: /* 大明槓 */
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if ((gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) && (nakiCount < 3)) {
				// 鳴いた牌を純手牌から消去
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[nakiCount + 1] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i].tile = NoTile;
				gameStat->Player[kangPlayer].Hand[i].red = Normal;
				++nakiCount;
			}
		}
		if (nakiCount != 3) {
			CodeConv::tostringstream o;
			o << _T("明槓するための牌が") <<
				((nakiCount < 3) ? _T("足りません") : _T("多すぎます")) <<
				_T("。牌コード [") << (int)gameStat->CurrentDiscard.tile << _T("] は [") <<
				nakiCount << _T("] 枚") <<
				((nakiCount < 3) ? _T("しかありません。") : _T("あります。"));
			error(o.str().c_str());
		}
		/* 門前清フラグを降ろす */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* 槓子として晒す */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sLeft)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedLeft;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sOpposite)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedCenter;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sRight)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedRight;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[0] = gameStat->CurrentDiscard.red;
		/* 理牌する */
		lipai(gameStat, kangPlayer);
		break;
	case FuuroNorth: /* 三麻抜きドラ */
		/* TODO: 効果音を鳴らす dsplay@ VOX_FLOWER */
		++(gameStat->Player[kangPlayer].NorthFlag);
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].red = Normal;
		gameStat->TianHuFlag = false;
		/* TODO: 発声 setCall kangPlayer, "北" */
		lipai(gameStat, kangPlayer);
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			gameStat->Player[i].FirstDrawFlag = false;
		break;
	case FuuroFlower: /* 花牌 */
		/* TODO: 効果音を鳴らす dsplay@ VOX_FLOWER */
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
				o << _T("牌コード [") << (int)gameStat->CurrentDiscard.tile << _T("] は花牌ではありません。");
				warn(o.str().c_str());
			}
		}
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].red = Normal;
		gameStat->TianHuFlag = false;
		/* TODO: 発声 setCall kangPlayer, "花" */
		lipai(gameStat, kangPlayer);
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			gameStat->Player[i].FirstDrawFlag = false;
		break;
	case FuuroPon: /* ポン */
		/* ポンした対子を消去 */
		nakiCount = 0;
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if ((gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) && (nakiCount < 2)) {
				// 鳴いた牌を純手牌から消去
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[nakiCount + 1] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i].tile = NoTile;
				gameStat->Player[kangPlayer].Hand[i].red = Normal;
				++nakiCount;
			}
		}
		if (nakiCount < 2) {
			CodeConv::tostringstream o;
			o << _T("ポンするための牌が足りません。牌コード [") << (int)gameStat->CurrentDiscard.tile <<
				_T("] は [") << nakiCount << _T("] 枚しかありません。");
			error(o.str().c_str());
		}
		/* 門前清フラグを降ろす */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* 明刻として晒す */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sLeft)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedLeft;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sOpposite)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedCenter;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sRight)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedRight;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[0] = gameStat->CurrentDiscard.red;
		/* 理牌する */
		lipai(gameStat, kangPlayer);
		/* ポンを宣言する */
		/* TODO: 効果音を鳴らす dsplay@ VOX_PON */
		/* TODO: 発声 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE), "ポン"*/
		/* 喰い替えの判定に使う変数を設定 */
		gameStat->PreviousMeld.Discard = gameStat->CurrentDiscard.tile;
		gameStat->PreviousMeld.Stepped = NoTile;
		/* 包の判定 */
		/* TODO: 包の判定 checkpao GameStat */
		break;
	case FuuroChii: /* チー */
		/* チーする牌を設定 */
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->CurrentDiscard.red;
		/* 純手牌から鳴いた塔子を除去 */
		{
			bool nakiCount[3] = {false};
			for (int i = NUM_OF_TILES_IN_HAND - 1; i < NUM_OF_TILES_IN_HAND * 2 - 1; i++) {
				for (unsigned j = 0; j < 3; j++) {
					if ((!nakiCount[j]) && (gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].tile ==
						(gameStat->CurrentDiscard.tile + j + 1 - gameStat->Player[kangPlayer].DeclarationFlag.Chi))) {
							gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[j] = gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].red;
							gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].tile = NoTile;
							gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].red = Normal;
							nakiCount[j] = true;
					}
				}
			}
			assert(gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile);
		}
		/* 門前フラグを降ろす */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* 順子を晒す */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == 1)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedLower;
		else if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == 2)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedMiddle;
		else if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == 3)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedUpper;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile =
			(tileCode)(gameStat->CurrentDiscard.tile + 1 - gameStat->Player[kangPlayer].DeclarationFlag.Chi);
		/* 自動理牌 */
		lipai(gameStat, kangPlayer);
		/* チーを宣言 */
		/* TODO: 効果音を鳴らす dsplay@ VOX_CHI */
		/* TODO: 発声 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE), "チー" */
		/* 喰い替え判定用の変数を指定 */
		gameStat->PreviousMeld.Discard = gameStat->CurrentDiscard.tile;
		if (RuleData::chkRule("kuikae", "agari_houki") || RuleData::chkRule("kuikae", "chombo")) {
			switch (gameStat->Player[kangPlayer].DeclarationFlag.Chi) {
				case 1: gameStat->PreviousMeld.Stepped = (tileCode)(gameStat->CurrentDiscard.tile + 3); break;
				case 2: gameStat->PreviousMeld.Stepped = NoTile; break;
				case 3: gameStat->PreviousMeld.Stepped = (tileCode)(gameStat->CurrentDiscard.tile - 3); break;
			}
		} else {
			gameStat->PreviousMeld.Stepped = NoTile;
		}
		break;
	default:
		{
			CodeConv::tostringstream o;
			o << _T("モードが正しくありません [") << Mode << _T("]");
			error(o.str().c_str());
		}
	}

	/* 暗槓・加槓を実行する */
	nakiCount = 0;
	if ((Mode == FuuroChii) || (Mode == FuuroPon) || (Mode == FuuroDaiminkan)) {
		/* 鳴いた捨牌を河で非表示にする */
		discardTile* tmpSutehaiVar = &gameStat->Player[gameStat->CurrentPlayer.Active].Discard[gameStat->Player[gameStat->CurrentPlayer.Active].DiscardPointer];
		if (tmpSutehaiVar->dstat == discardNormal)
			tmpSutehaiVar->dstat = discardTaken;
		else if (tmpSutehaiVar->dstat == discardRiichi)
			tmpSutehaiVar->dstat = discardRiichiTaken;
	}
	/* カンを宣言、天和や地和のフラグが立っていれば降ろす */
	if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
		gameStat->TianHuFlag = false;
		/* TODO: 効果音を鳴らす dsplay@ VOX_KAN */
		/* TODO: 発声 setCall kangPlayer, "カン" */
		lipai(gameStat, kangPlayer);
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			gameStat->Player[i].FirstDrawFlag = false;
	}
}

/* 槍槓の判定 */
bool CheckChankan(GameTable* const gameStat, EndType* RoundEndType, FuuroType Mode) {
	if ((Mode != FuuroDaiminkan) && (Mode != FuuroPon) && (Mode != FuuroChii)) {
		/* 国士無双に限り、暗槓を搶槓可能 */
		/* 断幺牌の暗槓は搶槓しようがないんですが、面倒なので区別せず処理
		   暗槓非開示設定の時は何を暗槓したか分からないので処理をスキップ*/
		/* 搶槓の判定 ここから */
		/* 搶槓の際の注意事項。元々のポンが自分の捨牌のものの場合
		   振聴となるので搶槓はできません */
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroNorth)) {
			if ((Mode == FuuroKakan) || (Mode == FuuroNorth) ||
				(RuleData::chkRule("ankan_conceal", "open") && RuleData::chkRule("ankan_chankan", "yes"))) {
					debug(_T("搶槓の判定をします。"));
					if (Mode == FuuroAnkan) gameStat->KangFlag.chankanFlag = 2;
					else if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = 3;
					else gameStat->KangFlag.chankanFlag = 1;
					/* TODO: 出和了り判定 ronhuproc GameStat, GameEnv: RoundEndType = stat */
					if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = 0;
					if (*RoundEndType == TripleRon) return true;
					if (RonPlayers(gameStat) > 0) {
						info(_T("搶槓が宣言されました。ループから出ます。"));
						return true;
					}
					gameStat->KangFlag.chankanFlag = 0;
			}
		}
		/* 搶槓の判定 ここまで */
		debug(_T("副露に成功しました。"));
		switch (Mode) {
			case FuuroAnkan: case FuuroKakan:
				haifu::haifureckanproc(gameStat); // 搶槓が無い場合、牌譜に記録
				break;
			case FuuroNorth:
				haifu::haifurecnorthproc(gameStat);
				break;
		}
	}
	if (Mode == FuuroDaiminkan) {
		/* バグ防止の為 */
		for (PLAYER_ID i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].tile = NoTile;
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].red = Normal;
		}
		/* 大明槓の包判定用 */
		if ((!RuleData::chkRule("minkan_pao", "no")) && (!RuleData::chkRule("minkan_pao", "no_but_2han"))) {
			/* TODO: 包の設定
				setPao GameStat, PAO_PLAYER_PAO, PAO_YAKU_MINKAN, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
				setPao GameStat, PAO_PLAYER_AGARI, PAO_YAKU_MINKAN, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
			*/
		}
	}
	return false;
}

/* 嶺上の処理 */
bool ProcRinshan(GameTable* const gameStat, EndType* RoundEndType, FuuroType Mode, PLAYER_ID kangPlayer) {
	if ((Mode != FuuroPon) && (Mode != FuuroChii)) {
		/* 四槓子の聴牌者がいて、５回目の槓があると流局とするルールの場合
		   その条件を判定する*/
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			if (gameStat->KangNum >= 4) {
				debug(_T("5個目の槓なので直ちに流局とし、ループから出ます。"));
				*RoundEndType = Uukaikan; return true;
			}
		}
		if (gameStat->TilePointer >= (gameStat->RinshanPointer - (gameStat->DeadTiles - 1))) {
			*RoundEndType = Ryuukyoku; return true; /* 荒牌なら終了 */
		}
		/* 嶺上牌を自摸る */
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->Deck[gameStat->RinshanPointer].tile;
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->Deck[gameStat->RinshanPointer].red;
		--gameStat->RinshanPointer;
		/* TODO: 効果音を鳴らす
		dsplay@ SND_TSUMO
		if (tilesLeft(GameStat) < 10) {
			dsplay@ SND_COUNTDOWN
		}
		*/
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			/* 槓ドラをめくる */
			if (RuleData::chkRuleApplied("kandora")) {
				gameStat->DoraPointer -= 2;
				/* TODO: 効果音を鳴らす dsplay@ SND_MEKURI */
				setdora(gameStat, 0); // 表ドラを設定する
				if (RuleData::chkRuleApplied("kandora") && RuleData::chkRuleApplied("uradora")) {
					setdora(gameStat, 1); // 裏ドラを設定する
				}
				haifu::haifurecdorap();
			}
			/* 包の判定 */
			/* TODO: 包の判定 checkpao GameStat */
		}
		/* 槓子をカウントする(四開槓の判定のため) */
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			++gameStat->Player[kangPlayer].NumberOfQuads;
			++gameStat->KangNum;
			CodeConv::tostringstream o;
			o << _T("プレイヤー [") << (int)kangPlayer << _T("] が [") <<
				(int)gameStat->Player[kangPlayer].NumberOfQuads << _T("] 回開槓、全員の合計で [") <<
				(int)gameStat->KangNum << _T("] 回開槓しています。");
			debug(o.str().c_str());
		}
	}
	return false;
}

/* 副露の処理 */
bool fuuroproc(GameTable* const gameStat, EndType* RoundEndType, const DiscardTileNum& DiscardTileIndex, FuuroType Mode) {
	{
		CodeConv::tostringstream o;
		o << _T("副露の処理を開始します。モード[") << (int)Mode << _T("]");
		info(o.str().c_str());
	}
	PLAYER_ID fuuroPlayer = PrepareFuuro(gameStat, DiscardTileIndex, Mode);

	if ((Mode != FuuroPon)&&(Mode != FuuroChii)&&(Mode != FuuroDaiminkan)) { /* カンする牌を設定 */
		gameStat->CurrentDiscard.tile = gameStat->Player[fuuroPlayer].Hand[DiscardTileIndex.id].tile;
		gameStat->CurrentDiscard.red = gameStat->Player[fuuroPlayer].Hand[DiscardTileIndex.id].red;
	}

	MakeMeld(gameStat, DiscardTileIndex, Mode, fuuroPlayer);
	/* TODO: 画面の再構築 redrscreen */
	if (CheckChankan(gameStat, RoundEndType, Mode)) return true;
	/* TODO: 画面の再構築 redrscreen */
	/* TODO: 1秒待ち await 1000 */
	if (ProcRinshan(gameStat, RoundEndType, Mode, fuuroPlayer)) return true;
	/* 事後処理 */
	/* TODO: 発声文字列を消去 repeat NUM_OF_PLAYERS: setCall cnt, "": loop */
	/* 再描画 */
	/* TODO: 画面の再構築 redrscreen */
	/* 鳴きが成立したので、一発のフラグを降ろす */
	for (PLAYER_ID i = 0; i < PLAYERS; i++)
		gameStat->Player[i].RichiFlag.IppatsuFlag =
		gameStat->Player[i].FirstDrawFlag = false;
	return false;
}
