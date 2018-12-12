#include "gameloop.h"

#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
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
#include "result.h"
#include "ruletbl.h"
#include "remote.h"
#include "chat.h"
#include "../common/chrono.h"
#include "../common/sleep.h"

/* 半荘の進行 */
EndType doTableTurn(GameTable* const gameStat) {
	{
		CodeConv::tostringstream o;
		o << _T("プレイヤー [") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("] のツモ番です。");
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
	threadYield();
	threadYield();
	EndType RoundEndType = procdahai(gameStat, DiscardTileIndex);
	if (RoundEndType != Continuing)
		return RoundEndType;
	threadSleep(80);
	/* 栄和の処理 */
	RoundEndType = ronhuproc(gameStat); // 栄和の処理
	if (RoundEndType != Continuing) return RoundEndType;
	threadYield();
	/* 途中流局の判定 */
	EndType round_abort_type = endround::checkroundabort(gameStat);
	if (round_abort_type != Continuing) return round_abort_type;
	/* 捨牌をポン、または大明槓する場合の処理 */
	if (executeFuuro(gameStat, DiscardTileIndex))
		return Continuing; /* 鳴きがあった場合、鳴いたプレーヤーに順番を移して戻る */
	/* ウェイトを入れる */
	threadSleep(100);
	/* 次のプレイヤーが牌を自摸る */
	tsumoproc(gameStat);
	// 打牌へ戻る
	return Continuing;
}

/* 半荘の進行 */
bool doTableRound(GameTable* const gameStat, int& OrigTurn, int& OrigHonba) {
	{
		CodeConv::tostringstream o;
		o << _T("局番号 [") << gameStat->GameRound << _T("] を開始しました。");
		info(o.str().c_str());
	}
	/* チャットの同期 */
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
		mihajong_socket::puts(SOCK_CHAT,
#if defined(_WIN32)
			_T("8\r\n"));
#else
			_T("8\n"));
#endif
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
	volatile EndType roundEndType = Continuing;
	do {
		do {
			roundEndType = doTableTurn(gameStat);
		} while ((roundEndType == DrawRinshan) || (roundEndType == Continuing));
		OrigHonba = gameStat->Honba; OrigTurn = gameStat->GameRound;
		endround::endround(gameStat, roundEndType, OrigTurn, OrigHonba);
#ifdef GUOBIAO
		if (roundEndType == Chonbo) {
			roundEndType = Continuing;
			tsumoproc(gameStat);
		}
#endif /* GUOBIAO */
	}
#ifdef GUOBIAO
	while ((roundEndType == Chonbo) || (roundEndType == Continuing));
#else /* GUOBIAO */
	while (false);
#endif /* GUOBIAO */
	mihajong_graphic::ui::WaitUIWithTimeout(5000);
	// 半荘終了判定
	return endround::nextRound(gameStat, roundEndType, OrigTurn);
}

namespace {

#ifdef _WIN32
DWORD titlescreen() {
#else /*_WIN32*/
uint32_t titlescreen() {
#endif /*_WIN32*/
	mihajong_graphic::Transit(mihajong_graphic::sceneTitle);
	return mihajong_graphic::ui::WaitUI();
}

}

/* ゲーム開始処理 */
void startgame(GameTypeID gameType) {
	while (true) {
		/* 変数類の初期化 */
		GameTable* gameStat = initializeGameTable(gameType);
		info(_T("ゲーム情報を初期化しました。"));
		mihajong_socket::server::rotation_reset();
		RuleData::applyPreference(); // 環境設定を反映
		sound::Play(sound::IDs::musTitle); // タイトル曲を流す
		unsigned ClientNumber = 0u;
	start:
		std::string serverAddr;
		const unsigned short gamePort =
			gameStat->chkGameType(Sanma) ? 50010 :
			gameStat->chkGameType(Sanma4) ? 50030 :
			gameStat->chkGameType(SanmaS) ? 50060 : 50000;
		switch (titlescreen()) { // タイトル画面
		case 1:
			EnvTable::Instantiate()->GameMode = EnvTable::Standalone;
			EnvTable::Instantiate()->PlayerDat[0].PlayerName =
				CodeConv::tstring(_T("[A]")) + CodeConv::EnsureTStr(RuleData::chkPreference("name"));
			EnvTable::Instantiate()->PlayerDat[1].PlayerName = _T("[b]COM1");
			EnvTable::Instantiate()->PlayerDat[2].PlayerName = _T("[c]COM2");
			EnvTable::Instantiate()->PlayerDat[3].PlayerName = _T("[d]COM3");
			serverAddr = "";
			break;
		case 2:
			RemoteConnection::startServer(serverAddr, gamePort);
			break;
		case 3:
			RemoteConnection::startClient(serverAddr, ClientNumber, gamePort);
			if (EnvTable::Instantiate()->GameMode == EnvTable::Standalone)
				goto start; // 接続失敗の時は戻る
			break;
		case 4:
			mihajong_graphic::Transit(mihajong_graphic::sceneConfig);
			mihajong_graphic::ui::WaitUI();
			goto start;
		case 5:
			mihajong_graphic::Transit(mihajong_graphic::sceneSetting);
			mihajong_graphic::ui::WaitUI();
			RuleData::applyPreference(); // 環境設定を反映
			goto start;
		case 6:
			return;
		}
		auto PositionArray = SeatShuffler::shuffle(ClientNumber); // 親決めの処理
		gameinit(&GameStat, gameType, serverAddr, PositionArray, ClientNumber); // 半荘の初期化処理

		/* 半荘の進行 */
		bool endFlag = false; int OrigTurn = 0, OrigHonba = 0;
		do {
			endFlag = doTableRound(gameStat, OrigTurn, OrigHonba);
		} while (!endFlag);
		// 半荘終了時
		gameResult(gameStat, OrigTurn, OrigHonba);
	}
}
