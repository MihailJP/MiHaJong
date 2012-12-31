#include "endround.h"

// �H���ς�����p�� gameStat->AgariSpecialStat �ԍ�
#define AGARI_KUIKAE 999

namespace { // ���������Ɏg���֐�
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
		o << _T("�򂢕ς������o���܂����B") <<
			(chonbo ? _T("�`�����{") : _T("�a�������")) << _T("�Ƃ��Ĉ����܂��B�Ŕv [") <<
			(int)gameStat->CurrentDiscard.tile << _T("] �����v [") <<
			(int)gameStat->PreviousMeld.Discard << _T("]");
		info(o.str().c_str());
		sound::Play(sound::IDs::sndCuohu);
	}
	bool chkKuikae(GameTable* gameStat) { // �򂢑ւ��̏ꍇ�̏���
		if (((gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Discard) || // �����̐H���ς��ɂȂ��Ă���ꍇ��
			(gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Stepped)) && // �ؐH���ς��ɂȂ��Ă���ꍇ��
			(!gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki)) { // �܂��A�K�������ɂȂ��Ă��Ȃ��Ȃ�
				if (RuleData::chkRule("kuikae", "agari_houki") || RuleData::chkRule("kuikae", "agari_houki_if_in_kind")) {
					/* �a��������Ƃ���ݒ� */
					logKuikae(gameStat, false);
					/* TODO: �����̈ڐA
					repeat 2
						setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "���"
						redrscreen: await 1500
						setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "�a������"
						redrscreen: await 1500
					loop
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), ""
					*/
					mihajong_graphic::GameStatus::updateGameStat(gameStat);
					/* �a��������͈ȍ~�����c���؂�A�����s�������ƂȂ�܂� */
					gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki = true;
					/* TODO: �ڐA���邩������Ȃ����p�~���邩������Ȃ� if (GetWatchModeFlag(GameEnv) == 0) {statmes "�a��������F�����c���؂肳��܂�"}*/
					/* TODO: ����ɂ��Ċm�F���邱�� vanish2@ */
				} else if (RuleData::chkRule("kuikae", "chombo") || RuleData::chkRule("kuikae", "chombo_if_in_kind")) {
					/* �����ɍ��a�Ƃ���ݒ� */
					logKuikae(gameStat, true);
					/* TODO: �������ڐA
					RoundEndType = ENDKYOKU_CHONBO: setTsumoAgariFlag GameStat, AGARI_KUIKAE
					setCurrentPlayer GameStat, CURRENTPLAYER_AGARI, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "���a"
					redrscreen: await 1500
					*/
					/* �ǂ�ł��؂�A���є����̎x�����ɐi�� */
					return true;
				}
		}
		return false;
	}

	void chkTahai(GameTable* gameStat) {
		/* ���v�⏭�v�����Ă��Ȃ����̃`�F�b�N */
		unsigned tmptilecnt = 0;
		for (unsigned i = 0; i < NUM_OF_TILES_IN_HAND; ++i)
			if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[i].tile != NoTile)
				++tmptilecnt;
		tmptilecnt += gameStat->Player[gameStat->CurrentPlayer.Active].MeldPointer * 3;
		if ((tmptilecnt != (NUM_OF_TILES_IN_HAND - 1)) && (!gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki)) {
			// ���v�⏭�v�̏ꍇ�̏���(�ʏ�N���邱�Ƃ͂Ȃ��͂������c)
			if (tmptilecnt > (NUM_OF_TILES_IN_HAND - 1))
				warn(_T("���v�����o���܂����B�a��������Ƃ��Ĉ����܂����A�̑ł̏����ŕs�������N���Ă����\��������܂��B"));
			else
				warn(_T("���v�����o���܂����B�a��������Ƃ��Ĉ����܂����A�̑ł̏����ŕs�������N���Ă����\��������܂��B"));
			sound::Play(sound::IDs::sndCuohu);
			/* TODO: �����̈ڐA
			repeat 2
				if (tmptilecnt > (NUM_OF_TILES_IN_HAND-1)) {
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "���v"
				} else {
					setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "���v"
				}
				redrscreen: await 1500
				setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "�a������"
				redrscreen: await 1500
			loop
			setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), ""
			*/
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki = true;
			/* TODO: �ڐA���邩������Ȃ����p�~���邩������Ȃ� if (GetWatchModeFlag(GameEnv) == 0) {statmes "�a��������F�����c���؂肳��܂�"} */
			/* TODO: ����ɂ��Ċm�F���邱�� vanish2@ */
		}
		return;
	}
}

EndType endround::checkroundabort(GameTable* gameStat) { // �ǏI�������̔���
	/* �l�J�ȂȂ痬�� */
	if (RuleData::chkRuleApplied("four_kong_ryuukyoku") && (gameStat->KangNum == 4) &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->NumberOfQuads < 4;})))
		return Suukaikan;
	// �l�l�����E�O�l�����̔���
	if (RuleData::chkRuleApplied("four_riichi_ryuukyoku") &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->RichiFlag.RichiFlag;})))
		return SuuchaRiichi;
	// �l���A�ŁE�O���A�ł̔���
	if (RuleData::chkRuleApplied("four_wind_ryuukyoku") &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->DiscardPointer == 1;})) &&
		(chksifeng(gameStat)))
		return SuufonRenda;

	for (PLAYER_ID i = 0; i < PLAYERS; ++i) gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile; // �o�O�h�~�̂���
	if (chkKuikae(gameStat)) { // �򂢑ւ��̏ꍇ�̏���
		gameStat->AgariSpecialStat = AGARI_KUIKAE;
		return Chonbo;
	}
	/* ���v�⏭�v�����Ă��Ȃ����̃`�F�b�N */
	chkTahai(gameStat);
	/* �r�v�̏ꍇ�����ŏI������(�͒�v�͋h�|���ł��Ȃ��������͂ł���) */
	if (gameStat->TilePointer == (gameStat->RinshanPointer - (gameStat->DeadTiles - 1)))
		return Ryuukyoku;
	/* �������Ȃ������ꍇ */
	return Continuing;
}
