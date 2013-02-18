#include "gameloop.h"

#include "../common/strcode.h"
#include "logging.h"
#include "envtbl.h"
#include "../socket/socket.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "tileutil.h"
#include "mouda.h"
#include "fuuro.h"
#include "prepare.h"
#include "../graphic/graphic.h"

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
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
	/* 摸打の処理 */
	DiscardTileNum DiscardTileIndex = getdahai(gameStat);
	if (DiscardTileIndex.type == DiscardTileNum::Disconnect)
		return Disconnect;
	/* ウェイトを入れる */
	Sleep(1);
	EndType RoundEndType = procdahai(gameStat, DiscardTileIndex);
	if (RoundEndType != Continuing)
		return RoundEndType;
	Sleep(80);
	/* 栄和の処理 */
	RoundEndType = ronhuproc(gameStat); // 栄和の処理
	if (RoundEndType != Continuing) return RoundEndType;
	Sleep(1);
	/* 途中流局の判定 */
	EndType round_abort_type = endround::checkroundabort(gameStat);
	if (round_abort_type != Continuing) return round_abort_type;
	/* 捨牌をポン、または大明槓する場合の処理 */
	if (executeFuuro(gameStat, DiscardTileIndex))
		return Continuing; /* 鳴きがあった場合、鳴いたプレーヤーに順番を移して戻る */
	/* ウェイトを入れる */
	Sleep(100);
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
		gameStat->PlayerID = gameStat->GameRound % Players;
	/* 破回八連荘の後始末 */
	if (gameStat->AgariChain == -1) gameStat->AgariChain = 1;
	/* 半荘の初期化と配牌を行なう */
	tableinit(gameStat);
	/* 配牌終了時の処理 */
	for (PlayerID i = 0; i < Players; i++) {
		gameStat->Player[i].HandStat = handUpright;
		lipai(gameStat, i);
	}
	gameStat->CurrentPlayer.Active = gameStat->GameRound % Players; // 最初に親から捨牌を行なう
	info(_T("配牌を完了しました。"));
	gameStat->TurnRound = 1; // 配牌が終わったら1巡目
	/* 摸打ループ */
	EndType roundEndType = Continuing;
	do {
		roundEndType = doTableTurn(gameStat);
	} while ((roundEndType == DrawRinshan) || (roundEndType == Continuing));
	int OrigHonba = gameStat->Honba, OrigTurn = gameStat->GameRound;
	endround::endround(gameStat, roundEndType, OrigTurn, OrigHonba);
	Sleep(5000);
	// 半荘終了判定
	/* return */ endround::nextRound(gameStat, roundEndType, OrigTurn);
	return false; // 仮置き
}

namespace {

DWORD titlescreen() {
	mihajong_graphic::Transit(mihajong_graphic::sceneTitle);
	return mihajong_graphic::ui::WaitUI();
}

}

/* ゲーム開始処理 */
void startgame(GameTypeID gameType) {
	while (true) {
		/* 変数類の初期化 */
		GameTable* gameStat = initializeGameTable(gameType);
		/* TODO: 画面初期化 vanish */
		info(_T("ゲーム情報を初期化しました。"));
		mihajong_socket::server::rotation_reset();
		sound::Play(sound::IDs::musTitle); // タイトル曲を流す
		unsigned ClientNumber = 0u;
		start:
		switch (titlescreen()) { // タイトル画面
		case 4:
			mihajong_graphic::Transit(mihajong_graphic::sceneConfig);
			mihajong_graphic::ui::WaitUI();
			goto start;
		case 6:
			return;
		}
		auto PositionArray = SeatShuffler::shuffle(ClientNumber); // 親決めの処理
		gameinit(&GameStat, gameType, ""/* TODO: 通信対戦の時は接続先サーバーIPにすること */, PositionArray, ClientNumber); // 半荘の初期化処理

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
