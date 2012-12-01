#include "fuuro.h"

namespace {

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
		gameStat->KangFlag.chainFlag++; // 連続槓の回数を記録
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = 1; /* 頭槓和のフラグ */
		if (RuleData::chkRule("minkan_penalty", "yes")) { /* 大明槓をすると1000点供託になるルール */
			/* TODO: 箱点のチェック */
			gameStat->Deposit++;
			gameStat->Player[fuuroPlayer].PlayerScore -= (LNum)1000;
		}
		break;
	case FuuroKakan: case FuuroAnkan:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // 嶺上開花のフラグを立てる
		for (int i = 0; i < 4; i++) // 赤ドラバグ回避のため
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecankan(gameStat, DiscardTileIndex); // 暗槓を仮に牌譜に記録
		gameStat->KangFlag.chainFlag++; // 連続槓の回数を記録
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
		sound::Play(sound::IDs::voxFlower);
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
		sound::Play(sound::IDs::voxPon);
		/* TODO: 発声 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE), "ポン"*/
		/* 喰い替えの判定に使う変数を設定 */
		gameStat->PreviousMeld.Discard = gameStat->CurrentDiscard.tile;
		gameStat->PreviousMeld.Stepped = NoTile;
		/* 包の判定 */
		checkpao(gameStat);
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
		sound::Play(sound::IDs::voxChi);
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
		sound::Play(sound::IDs::voxKan);
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
					if (Mode == FuuroAnkan) gameStat->KangFlag.chankanFlag = chankanOfAnkan;
					else if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = chankanOfNorth;
					else gameStat->KangFlag.chankanFlag = chankanRegular;
					*RoundEndType = ronhuproc(gameStat);
					if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = chankanNone;
					if (*RoundEndType == TripleRon) return true;
					if (RonPlayers(gameStat) > 0) {
						info(_T("搶槓が宣言されました。ループから出ます。"));
						return true;
					}
					gameStat->KangFlag.chankanFlag = chankanNone;
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
			gameStat->PaoFlag[pyMinkan].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyMinkan].agariPlayer = gameStat->CurrentPlayer.Passive;
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
		sound::Play(sound::IDs::sndTsumo);
		if (tilesLeft(gameStat) < 10)
			sound::Play(sound::IDs::sndCountdown);
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			/* 槓ドラをめくる */
			if (RuleData::chkRuleApplied("kandora")) {
				gameStat->DoraPointer -= 2;
				sound::Play(sound::IDs::sndMekuri);
				setdora(gameStat, 0); // 表ドラを設定する
				if (RuleData::chkRuleApplied("kandora") && RuleData::chkRuleApplied("uradora")) {
					setdora(gameStat, 1); // 裏ドラを設定する
				}
				haifu::haifurecdorap();
			}
			/* 包の判定 */
			checkpao(gameStat);
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

} /* namespace */

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

/* 包を判定する */
void checkpao(GameTable* const gameStat) {
	// 包の判定 ここから
	debug(_T("包の条件を判定します。"));
	unsigned DragonPons = 0, WindPons = 0, NumOfKangs = 0;
	const PlayerTable* playerStat = &gameStat->Player[gameStat->CurrentPlayer.Passive];
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
			o << _T("プレイヤー [") << (int)gameStat->CurrentPlayer.Active << _T("] は、大三元を包になります。");
			debug(o.str().c_str());
			gameStat->PaoFlag[pyDaisangen].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyDaisangen].agariPlayer = gameStat->CurrentPlayer.Passive;
		}
		if ((WindPons == 4) && (gameStat->PaoFlag[pyDaisixi].paoPlayer == -1)) {
			o << _T("プレイヤー [") << (int)gameStat->CurrentPlayer.Active << _T("] は、大四喜を包になります。");
			debug(o.str().c_str());
			gameStat->PaoFlag[pyDaisixi].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyDaisixi].agariPlayer = gameStat->CurrentPlayer.Passive;
		}
	}
	if ((RuleData::chkRule("suukantsu", "yakuman_with_pao") || RuleData::chkRule("suukantsu", "double_yakuman_with_pao")) &&
		(NumOfKangs == 4) && (gameStat->PaoFlag[pySikang].paoPlayer == -1)) {
		o << _T("プレイヤー [") << (int)gameStat->CurrentPlayer.Active << _T("] は、四槓子を包になります。");
		debug(o.str().c_str());
		gameStat->PaoFlag[pySikang].paoPlayer = gameStat->CurrentPlayer.Active;
		gameStat->PaoFlag[pySikang].agariPlayer = gameStat->CurrentPlayer.Passive;
	}
	// 包の判定 ここまで
	return;
}

/* 栄和のときの処理 */
EndType ronhuproc(GameTable* const gameStat) {
	EndType RoundEndType = Continuing;
	info(_T("リアクションを問い合わせます。"));
	/* 栄和や鳴き仕掛けをするかどうか問い合わせる */
	for (int pl = 0; pl < PLAYERS; pl++) {
		gameStat->Player[pl].DeclarationFlag.Ron =
			gameStat->Player[pl].DeclarationFlag.Pon =
			gameStat->Player[pl].DeclarationFlag.Kan = false;
		gameStat->Player[pl].DeclarationFlag.Chi = chiiNone;
	}
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->CurrentPlayer.Active != i) {
			gameStat->CurrentPlayer.Passive = i;
			if (gameStat->CurrentPlayer.Passive == gameStat->PlayerID) {
				/* TODO: プレイヤー鳴き選択 playerfuuro GameStat, GameEnv */
			} else if (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Passive].RemotePlayerFlag == 0) {
				/* COMが「カンニング」しないように処理 */
				GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Passive);
				/* 処理 */
				aiscript::compfuuro(sandbox);
				for (PLAYER_ID j = 0; j < PLAYERS; j++) {
					if (!gameStat->Player[j].DeclarationFlag.Ron) gameStat->Player[j].DeclarationFlag.Ron = sandbox->Player[j].DeclarationFlag.Ron;
					if (!gameStat->Player[j].DeclarationFlag.Kan) gameStat->Player[j].DeclarationFlag.Kan = sandbox->Player[j].DeclarationFlag.Kan;
					if (!gameStat->Player[j].DeclarationFlag.Pon) gameStat->Player[j].DeclarationFlag.Pon = sandbox->Player[j].DeclarationFlag.Pon;
					if (gameStat->Player[j].DeclarationFlag.Chi == chiiNone) gameStat->Player[j].DeclarationFlag.Chi = sandbox->Player[j].DeclarationFlag.Chi;
				}
			}
		} else if (i == gameStat->PlayerID) {
			if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Ignore);
		}
	}
	/* 通信対戦時の処理 */
	if (EnvTable::Instantiate()->GameMode != EnvTable::Standalone)
		RemoteAction::remotenaki(gameStat);
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
	{
		CodeConv::tostringstream o;
		o << _T("ロン [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Ron;
		o << _T("] カン [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Kan;
		o << _T("] ポン [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Pon;
		o << _T("] チー [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Chi;
		o << _T("]"); trace(o.str().c_str());
	}
	/* 和了り放棄の時は宣言を無効にする */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].AgariHouki) {
			{
				CodeConv::tostringstream o;
				o << _T("プレイヤー [") << (int)i << _T("] は和了り放棄です。宣言フラグを下ろします。");
				debug(o.str().c_str());
			}
			gameStat->Player[i].DeclarationFlag.Ron = gameStat->Player[i].DeclarationFlag.Pon =
				gameStat->Player[i].DeclarationFlag.Kan = false;
			gameStat->Player[i].DeclarationFlag.Chi = chiiNone;
		}
	}
	/* ロンしようとする人を表示(頭ハネで蹴られるような人も含む) */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].DeclarationFlag.Ron) {
			{
				CodeConv::tostringstream o;
				o << _T("プレイヤー [") << (int)i << _T("] は、栄和を宣言します。");
				debug(o.str().c_str());
			}
			// 優先権のないロンも表示されるようにする
			/* TODO: 発声 setCall cnt, " ロン " */
			gameStat->Player[i].HandStat = handExposed;
		}
	}
	/* 実際にロンできる人を表示 */
	unsigned roncount = 0;
	for (int i = 0; i < (PLAYERS - 1); i++) {
		if (gameStat->Player[RelativePositionOf(gameStat->CurrentPlayer.Active, (seatRelative)(i + 1))].DeclarationFlag.Ron) {
			bool accept = false;
			switch (roncount) {
			case 0:
				accept = true; break;
			case 1:
				accept =
					RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw") ||
					RuleData::chkRule("multiple_mahjong", "dual_mahjong") ||
					RuleData::chkRule("multiple_mahjong", "trial_mahjong");
				break;
			case 2:
				accept = RuleData::chkRule("multiple_mahjong", "trial_mahjong");
				break;
			}
			if (accept) {
				CodeConv::tostringstream o;
				o << _T("プレイヤー [") << (int)i << _T("] は、栄和できます。");
				debug(o.str().c_str());
				/* TODO: 発声 setCall RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1), "ロン" */
			}
			++roncount;
		}
	}
	/* 実際に栄和を行なう処理 */
	for (int i = 0; i < (PLAYERS - 1); i++) {
		Sleep(1); PLAYER_ID pl = RelativePositionOf(gameStat->CurrentPlayer.Active, (seatRelative)(i + 1));
		if (gameStat->Player[pl].DeclarationFlag.Ron) {
			/* ウォッチモードの場合は和了った人に視点を向ける */
			if (EnvTable::Instantiate()->WatchModeFlag)
				gameStat->PlayerID = pl;
			/* 栄和したことを変数に記録 */
			RoundEndType = Agari; gameStat->TsumoAgariFlag = false;
			gameStat->CurrentPlayer.Furikomi = gameStat->CurrentPlayer.Active;
			gameStat->CurrentPlayer.Agari = pl;
			/* 八連荘の判定に使う変数 */
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
			/* 和了り牌を設定 */
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->CurrentDiscard.red;
			/* 立直宣言牌での放銃の場合、立直を無効とし供託点棒を返却する */
			if (gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag) {
				if (gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.DoubleFlag) {
					gameStat->DoubleRichiCounter = true;
					trace(_T("ダブル立直宣言牌での放銃のため、立直棒を返還します。"));
				} else {
					gameStat->RichiCounter = true;
					trace(_T("立直宣言牌での放銃のため、立直棒を返還します。"));
				}
				gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag =
					gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.DoubleFlag =
					gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.OpenFlag =
					gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag = false;
				--gameStat->Deposit;
				gameStat->Player[gameStat->CurrentPlayer.Active].PlayerScore += (LNum)1000;
			}
			/* 役や振聴の判定 */
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, pl);
			MachihaiInfo mInfo = chkFuriten(gameStat, pl);
			// 縛りを満たさないか、振聴のとき
			if (((yakuInfo.CoreHan < (gameStat->ShibariFlag ? 2 : 1)) && (yakuInfo.CoreSemiMangan == 0)) ||
				(mInfo.FuritenFlag) || (gameStat->Player[pl].DoujunFuriten) ||
				(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->Player[pl].RichiFlag.RichiFlag))) {
					trace(_T("縛りを満たさないか振聴です。次の処理をチョンボ用に切り替えます。"));
					RoundEndType = Chonbo; // チョンボにする
			}
			// ロンをしたことを表示
			/* TODO: 発声 setCall getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), "ロン" */
			gameStat->Player[gameStat->CurrentPlayer.Agari].HandStat = handExposed;
			/* 和了り牌を設定 */
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->CurrentDiscard.red;
			/* 栄和のサウンドを鳴らす */
			if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
				sound::Play(sound::IDs::voxRonFurikomi);
			else
				sound::Play(sound::IDs::voxRon);
			/* 画面更新して戻る */
			/* TODO: 画面更新 redrscreen */
			break;
		}
	}
	/* 牌譜に記録 */
	roncount = 0;
	if (RonPlayers(gameStat) > 0) {
		if (gameStat->KangFlag.chankanFlag != chankanNone) {
			if (gameStat->KangFlag.chankanFlag == chankanOfNorth)
				haifu::haifurecchanpei(gameStat);
			else
				haifu::haifurecchankan(gameStat);
		} else {
			haifu::haifurecfurikomi(gameStat);
		}
	} else if (!gameStat->KangFlag.kangFlag) { // 抜き北で牌譜がずれるのを抑止
		haifu::haifurecfurikomi(gameStat);
	}
	if (chkGameType(gameStat, AllSanma)) {
		// 二家和の判定
		if ((RonPlayers(gameStat) >= 2) && RuleData::chkRule("multiple_mahjong", "aborted"))
			return TripleRon;
	} else {
		// 三家和の判定
		if ((RonPlayers(gameStat) >= 3) &&
			(RuleData::chkRule("multiple_mahjong", "single_mahjong_with_draw") ||
			RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw")))
			return TripleRon;
	}
	return RoundEndType;
}

bool executeFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	/* 捨牌をポン、または大明槓する場合の処理 */
	/* 同じ牌は４枚しかないので、ポンと明槓は同時に起こることがない */
	unsigned declCount = 0; EndType roundEndType = Continuing;
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].DeclarationFlag.Pon) ++declCount;
		if (gameStat->Player[i].DeclarationFlag.Kan) ++declCount;
	}
	if (declCount > 1)
		error(_T("複数同時のポン・槓が宣言されています。"));
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		Sleep(1);
		/* 捨牌をポンする場合 */
		if (gameStat->Player[i].DeclarationFlag.Pon) {
			gameStat->CurrentPlayer.Passive = i; // 鳴いたプレイヤーを設定
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroPon);
			break;
		}
		/* 明槓の場合 */
		const unsigned kanLim = (RuleData::chkRuleApplied("fifth_kong") ? 5 : 4);
		if (gameStat->Player[i].DeclarationFlag.Kan && (gameStat->KangNum < kanLim)) {
			gameStat->CurrentPlayer.Passive = i; // 鳴いたプレイヤーを設定
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroDaiminkan);
			break;
		}
	}
	/* 吃とポンが同時に起こった場合、ポンを優先する */
	if (declCount > 0) {
		/* ポンや槓の時はツモ順を飛ばしたとみなして数え、北家→東家をまたいだ場合は次の巡目として扱う */
		if (playerwind(gameStat, gameStat->CurrentPlayer.Passive, gameStat->GameRound) < playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound))
			++gameStat->TurnRound;
		gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive;
		return true;
	} else if (!chkGameType(gameStat, AllSanma)) {
		/* 吃の処理 */
		/* 三人打ちでは吃なし */
		if (gameStat->Player[RelativePositionOf(gameStat->CurrentPlayer.Passive, sRight)].DeclarationFlag.Chi > 0) {
			/* ポンや槓の時はツモ順を飛ばしたとみなして数え、北家→東家をまたいだ場合は次の巡目として扱う */
			gameStat->CurrentPlayer.Passive = RelativePositionOf(gameStat->CurrentPlayer.Passive, sRight); // 吃ができるのは上家の捨牌のみ
			if (playerwind(gameStat, gameStat->CurrentPlayer.Passive, gameStat->GameRound) < playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound))
				++gameStat->TurnRound;
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroChii);
			gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive;
			return true;
		}
	}
	/* バグ防止のアレ */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].tile = NoTile;
		gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].red = Normal;
	}
	return false;
}
