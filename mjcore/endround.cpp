#include "endround.h"

// 食い変え判定用の gameStat->AgariSpecialStat 番号
#define AGARI_KUIKAE 999

namespace { // 内部処理に使う関数
	bool all_player(const GameTable* gameStat, std::function<bool (const PlayerTable*)> f) {
		bool flag = true;
		if (chkGameType(gameStat, Sanma4)) {
			for (PLAYER_ID i = 0; i < PLAYERS; ++i)
				flag = flag && ((playerwind(gameStat, i, gameStat->GameRound) == sNorth) || f(&(gameStat->Player[i])));
		} else if (chkGameType(gameStat, SanmaT)) {
			for (PLAYER_ID i = 0; i < 3; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		} else {
			for (PLAYER_ID i = 0; i < PLAYERS; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		}
		return flag;
	}
	bool chksifeng(const GameTable* gameStat) {
		bool flag = false;
		std::array<tileCode, 4> winds = {EastWind, SouthWind, WestWind, NorthWind,};
		for (auto k = winds.begin(); k != winds.end(); ++k) {
			bool tmpflag = true;
			if (chkGameType(gameStat, Sanma4)) {
				for (PLAYER_ID i = 0; i < PLAYERS; ++i)
					tmpflag = tmpflag && ((playerwind(gameStat, i, gameStat->GameRound) == sNorth) || (gameStat->Player[i].Discard[1].tcode.tile == *k));
			} else if (chkGameType(gameStat, SanmaT)) {
				for (PLAYER_ID i = 0; i < 3; ++i)
					tmpflag = tmpflag && (gameStat->Player[i].Discard[1].tcode.tile == *k);
			} else {
				for (PLAYER_ID i = 0; i < PLAYERS; ++i)
					tmpflag = tmpflag && (gameStat->Player[i].Discard[1].tcode.tile == *k);
			}
			flag = flag || tmpflag;
		}
		return flag;
	}

	void logKuikae(const GameTable* gameStat, bool chonbo) {
		CodeConv::tostringstream o;
		o << _T("喰い変えを検出しました。") <<
			(chonbo ? _T("チョンボ") : _T("和了り放棄")) << _T("として扱います。打牌 [") <<
			(int)gameStat->CurrentDiscard.tile << _T("] 鳴いた牌 [") <<
			(int)gameStat->PreviousMeld.Discard << _T("]");
		info(o.str().c_str());
		sound::Play(sound::IDs::sndCuohu);
	}
	bool chkKuikae(GameTable* gameStat) { // 喰い替えの場合の処理
		if (((gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Discard) || // 現物の食い変えになっている場合か
			(gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Stepped)) && // 筋食い変えになっている場合で
			(!gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki)) { // まだアガリ放棄になっていないなら
				if (RuleData::chkRule("kuikae", "agari_houki") || RuleData::chkRule("kuikae", "agari_houki_if_in_kind")) {
					/* 和了り放棄とする設定 */
					logKuikae(gameStat, false);
					/* TODO: ここの移植
					repeat 2
						setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "喰替"
						redrscreen: await 1500
						setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "和了放棄"
						redrscreen: await 1500
					loop
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), ""
					*/
					mihajong_graphic::GameStatus::updateGameStat(gameStat);
					/* 和了り放棄は以降強制ツモ切り、強制不聴扱いとなります */
					gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki = true;
					/* TODO: 移植するかもしれないし廃止するかもしれない if (GetWatchModeFlag(GameEnv) == 0) {statmes "和了り放棄：強制ツモ切りされます"}*/
					/* TODO: これについて確認すること vanish2@ */
				} else if (RuleData::chkRule("kuikae", "chombo") || RuleData::chkRule("kuikae", "chombo_if_in_kind")) {
					/* 直ちに錯和とする設定 */
					logKuikae(gameStat, true);
					/* TODO: ここを移植
					RoundEndType = ENDKYOKU_CHONBO: setTsumoAgariFlag GameStat, AGARI_KUIKAE
					setCurrentPlayer GameStat, CURRENTPLAYER_AGARI, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "錯和"
					redrscreen: await 1500
					*/
					/* 局を打ち切り、満貫罰符の支払いに進む */
					return true;
				}
		}
		return false;
	}

	void chkTahai(GameTable* gameStat) {
		/* 多牌や少牌をしていないかのチェック */
		unsigned tmptilecnt = 0;
		for (unsigned i = 0; i < NUM_OF_TILES_IN_HAND; ++i)
			if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[i].tile != NoTile)
				++tmptilecnt;
		tmptilecnt += gameStat->Player[gameStat->CurrentPlayer.Active].MeldPointer * 3;
		if ((tmptilecnt != (NUM_OF_TILES_IN_HAND - 1)) && (!gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki)) {
			// 多牌や少牌の場合の処理(通常起きることはないはずだが…)
			if (tmptilecnt > (NUM_OF_TILES_IN_HAND - 1))
				warn(_T("多牌を検出しました。和了り放棄として扱いますが、摸打の処理で不整合が起きていた可能性があります。"));
			else
				warn(_T("少牌を検出しました。和了り放棄として扱いますが、摸打の処理で不整合が起きていた可能性があります。"));
			sound::Play(sound::IDs::sndCuohu);
			/* TODO: ここの移植
			repeat 2
				if (tmptilecnt > (NUM_OF_TILES_IN_HAND-1)) {
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "多牌"
				} else {
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "少牌"
				}
				redrscreen: await 1500
				setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "和了放棄"
				redrscreen: await 1500
			loop
			setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), ""
			*/
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki = true;
			/* TODO: 移植するかもしれないし廃止するかもしれない if (GetWatchModeFlag(GameEnv) == 0) {statmes "和了り放棄：強制ツモ切りされます"} */
			/* TODO: これについて確認すること vanish2@ */
		}
		return;
	}
}

EndType endround::checkroundabort(GameTable* gameStat) { // 局終了条件の判定
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

	for (PLAYER_ID i = 0; i < PLAYERS; ++i) gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile; // バグ防止のため
	if (chkKuikae(gameStat)) { // 喰い替えの場合の処理
		gameStat->AgariSpecialStat = AGARI_KUIKAE;
		return Chonbo;
	}
	/* 多牌や少牌をしていないかのチェック */
	chkTahai(gameStat);
	/* 荒牌の場合ここで終了する(河底牌は吃ポンできないがロンはできる) */
	if (gameStat->TilePointer == (gameStat->RinshanPointer - (gameStat->DeadTiles - 1)))
		return Ryuukyoku;
	/* 何事もなかった場合 */
	return Continuing;
}
