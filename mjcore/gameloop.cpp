#include "gameloop.h"


/* 半荘の進行 */
EndType doTableTurn(GameTable* const gameStat) {
	{
		CodeConv::tostringstream o;
		o << _T("プレイヤー [") << (int)gameStat->CurrentPlayer.Active << _T("] のツモ番です。");
		info(o.str().c_str());
	}
	/* 変数の初期化 */
	calcdoukasen(gameStat); // 導火線の位置を更新
	if (!gameStat->KangFlag.kangFlag) {
		/* 連続槓の回数と頭槓和フラグをリセット */
		gameStat->KangFlag.chainFlag = 0;
		gameStat->KangFlag.topFlag = false;
	}
	gameStat->CurrentDiscard.tile = NoTile;
	gameStat->CurrentDiscard.red = Normal;
	/* ウォッチモードの時は視点をツモ番の人に移す */
	if (EnvTable::Instantiate()->WatchModeFlag)
		gameStat->PlayerID = gameStat->CurrentPlayer.Active;
	/* 再描画 */
	/* TODO: redrscreen */
	/* 摸打の処理 */
	DiscardTileNum DiscardTileIndex = getdahai(gameStat);
	if (DiscardTileIndex.type == DiscardTileNum::Disconnect)
		return Disconnect;
	/* ウェイトを入れる */
	/* await 0 */
	EndType RoundEndType = procdahai(gameStat, DiscardTileIndex);
	if (RoundEndType != Continuing)
		return RoundEndType;
	/* await 80 */
	/* 栄和の処理 */
	RoundEndType = ronhuproc(gameStat); // 栄和の処理
	if (RoundEndType != Continuing) return RoundEndType;
	/* await 0 */
	/* 途中流局の判定 */
	/* TODO: ここを移植する
	checkroundabort GameStat, GameEnv
	if (stat >= 0) {RoundEndType = stat: break}
	*/
	/* 捨牌をポン、または大明槓する場合の処理 */
	if (executeFuuro(gameStat, DiscardTileIndex))
		return Continuing; /* 鳴きがあった場合、鳴いたプレーヤーに順番を移して戻る */
	/* ウェイトを入れる */
	/* await 100 */
	/* 次のプレイヤーが牌を自摸る */
	tsumoproc(gameStat);
	// 打牌へ戻る
	return Continuing;
}

/* 半荘の進行 */
bool doTableRound(GameTable* const gameStat) {
	{
		CodeConv::tostringstream o;
		o << _T("局番号 [") << gameStat->GameRound << _T("] を開始しました。");
		info(o.str().c_str());
	}
	/* TODO: チャットの同期
	if (getGameMode(GameEnv) == GAMEMODE_CLIENT) {
		//sockput "8\n", SOCK_CHAT+0 // チャットを同期
	}
	*/
	/* ウォッチモードの時は視点を親に移す */
	gameStat->CurrentPlayer.Active = -1;
	if (EnvTable::Instantiate()->WatchModeFlag)
		gameStat->PlayerID = gameStat->GameRound % PLAYERS;
	/* 破回八連荘の後始末 */
	if (gameStat->AgariChain == -1) gameStat->AgariChain = 1;
	/* 半荘の初期化と配牌を行なう */
	tableinit(gameStat);
	/* 配牌終了時の処理 */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		gameStat->Player[i].HandStat = handUpright;
		lipai(gameStat, i);
	}
	gameStat->CurrentPlayer.Active = gameStat->GameRound % PLAYERS; // 最初に親から捨牌を行なう
	info(_T("配牌を完了しました。"));
	gameStat->TurnRound = 1; // 配牌が終わったら1巡目
	/* 摸打ループ */
	EndType roundEndType = Continuing;
	do {
		roundEndType = doTableTurn(gameStat);
	} while ((roundEndType == DrawRinshan) || (roundEndType == Continuing));
	int OrigHonba = gameStat->Honba, OrigTurn = gameStat->GameRound;
	/* TODO: 終了時の処理 endround GameStat, GameEnv, RoundEndType, OrigTurn, OrigHonba */
	/* TODO: await 5000 */
	// 半荘終了判定
	/* TODO: これを移植する
	nextRound GameStat, OrigTurn
	if (stat) {break}
	*/
	return false; // 仮置き
}

/* ゲーム開始処理 */
void startgame(gameTypeID gameType) {
	while (true) {
		/* 変数類の初期化 */
		EnvTable::TableEnvInit();
		GameTable* gameStat = initializeGameTable(gameType);
		/* TODO: 画面初期化 vanish */
		info(_T("ゲーム情報を初期化しました。"));
		mihajong_socket::server::rotation_reset();
		sound::Play(sound::IDs::musTitle); // タイトル曲を流す
		/* TODO: gosub *selmode */ // タイトル画面
		/* TODO: gosub *chrshuffle */ // 親決めの処理
		/* TODO: gosub *gameinit */ // 半荘の初期化処理

		/* 半荘の進行 */
		bool endFlag = false;
		do {
			bool endFlag = doTableRound(gameStat);
		} while (!endFlag);
		// 半荘終了時
		/* TODO: 終了時の処理 endgame GameStat, GameEnv, origTurn, origHonba */
		/* TODO: 入力待ち
		buttonPressed = 0
		repeat
			redrscreen
			await 1000
			if (buttonPressed) {break}
		loop
		*/
	}
}
