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
					for (unsigned i = 0; i < 2; ++i) {
						mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Kuikae);
						mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
						mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::AgariHouki);
						mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
					}
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
					mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
					mihajong_graphic::GameStatus::updateGameStat(gameStat);
					/* �a��������͈ȍ~�����c���؂�A�����s�������ƂȂ�܂� */
					gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki = true;
					/* TODO: �ڐA���邩������Ȃ����p�~���邩������Ȃ� if (GetWatchModeFlag(GameEnv) == 0) {statmes "�a��������F�����c���؂肳��܂�"}*/
					/* TODO: ����ɂ��Ċm�F���邱�� vanish2@ */
				} else if (RuleData::chkRule("kuikae", "chombo") || RuleData::chkRule("kuikae", "chombo_if_in_kind")) {
					/* �����ɍ��a�Ƃ���ݒ� */
					logKuikae(gameStat, true);
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Chonbo);
					mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
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
			for (unsigned i = 0; i < 2; ++i) {
				if (tmptilecnt > (NUM_OF_TILES_IN_HAND - 1))
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Tahai);
				else
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Shouhai);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
				mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::AgariHouki);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
			}
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
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

// -------------------------------------------------------------------------

namespace {
	std::array<bool, PLAYERS> chkNagashiMangan(const GameTable* gameStat, EndType& RoundEndType) { /* �������т̔��� */
		std::array<bool, PLAYERS> NagashiManganFlag = {false,};
		if (RoundEndType == Ryuukyoku) {
			for (unsigned i = 0; i < ACTUAL_PLAYERS; ++i) {
				if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, i, gameStat->GameRound) == sNorth))
					continue; // �l�l�O���̏ꍇ�k�Ƃ͖���
				if (RuleData::chkRuleApplied("nagashi_mangan") && isNagashiMangan(gameStat, i)) {
					NagashiManganFlag[i] = true; RoundEndType = NagashiMangan;
				}
			}
		}
		return NagashiManganFlag;
	}

	void ryuukyokuScreen(unsigned soundNum, CodeConv::tstring* ResultDesc, unsigned subsceneCode, unsigned waittime = 3000) {
		using namespace mihajong_graphic;
		using namespace CodeConv;
		if (ResultDesc) {
			sound::Play(soundNum);
			chat::appendchat((_T("*** ") + (*ResultDesc) + _T("\n")).c_str());
			Subscene(subsceneCode);
			Sleep(3000);
		}
		for (PLAYER_ID i = 0; i < PLAYERS; ++i)
			calltext::setCall(i, calltext::None);
		sound::Play(sound::IDs::sndPingju);
		sound::Play(sound::IDs::musRyuukyoku);
		Subscene(tblSubsceneRyuukyoku);
		Sleep(waittime);
	}

	unsigned checkTenpai(GameTable* gameStat, CodeConv::tstring& ResultDesc, unsigned OrigTurn) {
		unsigned TenpaiCnt = 0;
		for (PLAYER_ID i = 0; i < PLAYERS; ++i) {
			if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, i, OrigTurn)))
				continue; // �l�l�O���̖k�Ƃ͖���
			if (isTenpai(gameStat, i)) { // ���v�̎�
				++TenpaiCnt; gameStat->Player[i].HandStat = handExposed;
			} else if (gameStat->Player[i].RichiFlag.RichiFlag) { // ���a(�s��)���[�`�̎�
				gameStat->Player[i].HandStat = handExposed;
			} else { // �s���̎��͕�����
				gameStat->Player[i].HandStat = handHidden;
			}
		}
		CodeConv::tstring TenpaiCountTxt;
		switch (TenpaiCnt) {
			case 0: TenpaiCountTxt = _T("�S���s��"); break;
			case 1: TenpaiCountTxt = _T("�P�l���v"); break;
			case 2: TenpaiCountTxt = _T("�Q�l���v"); break;
			case 3: TenpaiCountTxt = _T("�R�l���v"); break;
			case 4: TenpaiCountTxt = _T("�S�l���v"); break;
		}
		ResultDesc = _T("�r�v���ǁA") + TenpaiCountTxt;
		/* TODO: ���ꂢ��Ȃ����m�F statmes "���� "+TenpaiCountTxt */
		chat::appendchat((_T("*** ") + TenpaiCountTxt + _T("�ł�\n")).c_str());
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneChkTenpai);
		Sleep(5000);
		return TenpaiCnt;
	}

	void ryuukyokuProc(GameTable* gameStat, bool RenchanFlag) {
		/* TODO: ���̕Ӎčl�̂���
		repeat NUM_OF_PLAYERS
			setCall cnt, ""
		loop
		setCenterTitle ""
		if (RenchanFlag) {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "�A��"
		} else {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "�e����"
		}
		snd_play SND_PAGE
		redrscreen
		redraw 1
		*/
		++(gameStat->Honba);
		if (!RenchanFlag) ++(gameStat->GameRound);
		gameStat->AgariChain = 0; gameStat->LastAgariPlayer = -1;
		return;
	}
}

void endround::endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba) {
	EndType RoundEndType = roundEndType;
	{
		CodeConv::tostringstream o;
		o << _T("�ǂ��I�� �I���R�[�h [") << (int)RoundEndType << _T(']');
	}
	/* TODO: ���̕ӊm�F
	statmes ""
	vanish2@ */
	/* �������т̔��� */
	auto NagashiManganFlag = chkNagashiMangan(gameStat, RoundEndType);
	/************/
	/* �a������ */
	/************/
	if ((RoundEndType == Agari) || (RoundEndType == Chonbo)) {
		/* TODO: �a���莞�̏��� agariproc RoundEndType, GameStat, GameEnv, tmpUraFlag, tmpAliceFlag, ResultDesc */
	}
	CodeConv::tstring ResultDesc; bool RenchanFlag = false;
	switch (RoundEndType) {
	/**************/
	/* �r�v���ǎ� */
	/**************/
	case Ryuukyoku:
		/* TODO: ���ꑽ������Ȃ� statmes "���ǂł�" */
		ResultDesc = _T("�r�v����");
		chat::appendchat((_T("*** ") + ResultDesc + _T("\n")).c_str());
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);
		checkTenpai(gameStat, ResultDesc, OrigTurn);
#if 0 /* ������ */
		dim PointDelta, NUM_OF_PLAYERS, NUM_OF_DIGIT_GROUPS
		repeat NUM_OF_ACTUAL_PLAYERS
#ifdef SANMA4
			if (playerWind(cnt, getRound(GameStat)) == PLAYER_NORTH) {
				continue // �k�Ƃ͖���
			}
#endif
#ifdef ALLSANMA
			if (isTenpai(GameStat, GameEnv, cnt)) {
				if (TenpaiCnt == 1) {PointDelta(cnt) += 30}
				if (TenpaiCnt == 2) {PointDelta(cnt) += 15}
			} else {
				if (TenpaiCnt == 1) {PointDelta(cnt) -= 15}
				if (TenpaiCnt == 2) {PointDelta(cnt) -= 30}
			}
#else
			if (isTenpai(GameStat, GameEnv, cnt)) {
				if (TenpaiCnt == 1) {PointDelta(cnt) += 30}
				if (TenpaiCnt == 2) {PointDelta(cnt) += 15}
				if (TenpaiCnt == 3) {PointDelta(cnt) += 10}
			} else {
				if (TenpaiCnt == 1) {PointDelta(cnt) -= 10}
				if (TenpaiCnt == 2) {PointDelta(cnt) -= 15}
				if (TenpaiCnt == 3) {PointDelta(cnt) -= 30}
			}
#endif
		loop
		if ((TenpaiCnt > 0)&&(TenpaiCnt < NUM_OF_ACTUAL_PLAYERS)) {
			setCenterTitle "�s������"
			putdelta PointDelta
			redraw 1: await 2500
			pointcalc GameStat, PointDelta
		}
		
		repeat NUM_OF_ACTUAL_PLAYERS
			// ���a�����i�s�������j�̎҂������ꍇ
			if ((isTenpai(GameStat, GameEnv, cnt) == 0)&&(getRichiFlag(GameStat, RICHI_FLAG, cnt))) {
				transferChonboPenalty GameStat, cnt
				await 500
			}
		loop
#endif
		
		if (RuleData::chkRule("round_continuation", "renchan_if_ready")) {
			if (isTenpai(gameStat, gameStat->GameRound % PLAYERS)) RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_always")) {
			RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_ready_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % PLAYERS) ||
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_mahjong_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % PLAYERS) &&
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		}
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* �a�������� */
	/**************/
	case Agari: /* ������ */
#if 0
		repeat NUM_OF_PLAYERS
			setCall cnt, ""
		loop
		setCenterTitle ""
		if ((playerwind(getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getRound(GameStat)) == PLAYER_EAST)&&(chkRule("round_continuation", "renchan_never") == 0)) {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "�A��"
		} else {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "�e����"
		}
		snd_play SND_PAGE
		redrscreen
		redraw 1
		
		if ((playerwind(getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getRound(GameStat)) == PLAYER_EAST)&&(chkRule("round_continuation", "renchan_never") == 0)) {
			incHonba GameStat
		} else {
			setHonba GameStat, 0: incRound GameStat
		}
		setDeposit GameStat, 0
		// ���A���������A�J�E���^�����Z�b�g
		if (getAgariChain(GameStat) == 8) {
			setAgariChain GameStat, 0
		}
#endif
		break;
	/**************/
	/* ���a������ */
	/**************/
	case Chonbo: /* ������ */
		/* TODO: ���� setAgariChain GameStat, 0: setLastAgariPlayer GameStat, -1 */
		break;
	/**************/
	/* ��헬�ǎ� */
	/**************/
	case KyuushuKyuuhai: /* ������ */
		if      (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sEast ) % PLAYERS))
			ResultDesc = _T("���Ƃ̋���v");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sSouth) % PLAYERS))
			ResultDesc = _T("��Ƃ̋���v");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sWest ) % PLAYERS))
			ResultDesc = _T("���Ƃ̋���v");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sNorth) % PLAYERS))
			ResultDesc = _T("�k�Ƃ̋���v");
		else ResultDesc = _T("����v"); // �������Ď��s����Ȃ��͂�
		chat::appendchat((_T("*** ") + ResultDesc + _T("\n")).c_str());
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);

		if (RuleData::chkRule("nine_terminals", "next_dealer") == 0)
			RenchanFlag = ((!RuleData::chkRule("nine_terminals", "renchan_if_dealer_kyuushu")) || (gameStat->CurrentPlayer.Active == (gameStat->GameRound % PLAYERS)));
		else
			RenchanFlag = false;
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* �l�ȗ��ǎ� */
	/**************/
	case Suukaikan:
		/* TODO: ����̐�����m�F statmes "����(�l�J��)" */
		ResultDesc = _T("�l�J��");
		ryuukyokuScreen(sound::IDs::voxSikang, &ResultDesc, mihajong_graphic::tblSubsceneSikang);
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_kong_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* �O�Ƙa�̎� */
	/**************/
	case TripleRon:
		Sleep(1300);
		/* TODO: ������̏��������߂Ă�������
		statmes "����(��Ƙa)"
		statmes "����(�O�Ƙa)"
		*/
		ResultDesc = chkGameType(gameStat, AllSanma) ? _T("��Ƙa") : _T("�O�Ƙa");
		ryuukyokuScreen(sound::IDs::voxSanjiahu, &ResultDesc, mihajong_graphic::tblSubsceneTripleRon);

		if (RuleData::chkRule("triple_mahjong", "renchan_if_nondealer_furikomi"))
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi != (gameStat->GameRound % PLAYERS));
		else if (RuleData::chkRule("triple_mahjong", "renchan_if_north_furikomi") || RuleData::chkRule("triple_mahjong", "renchan_if_west_furikomi"))
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi == ((gameStat->GameRound + (chkGameType(gameStat, AllSanma) ? sWest : sNorth)) % PLAYERS));
		else if (RuleData::chkRule("triple_mahjong", "same_dealer"))
			RenchanFlag = true;
		else if (RuleData::chkRule("triple_mahjong", "next_dealer"))
			RenchanFlag = false;
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* �l�����ǎ� */
	/**************/
	case SuufonRenda:
		/* TODO: ������̐i�ނ����߂Ă����Ă�������
		statmes "����(�O���A��)"
		statmes "����(�l���A��)"
		*/
		ResultDesc = chkGameType(gameStat, AllSanma) ? _T("�O���A��") : _T("�l���A��");
		ryuukyokuScreen(sound::IDs::voxSifeng, &ResultDesc, mihajong_graphic::tblSubsceneSifeng);
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_wind_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* �l�l������ */
	/**************/
	case SuuchaRiichi:
		/* TODO: ����̏������܂��ۗ���
		statmes "����(�O�Ɨ���)"
		statmes "����(�l�Ɨ���)"
		*/
		ResultDesc = chkGameType(gameStat, AllSanma) ? _T("�O�Ɨ���") : _T("�l�Ɨ���");
		ryuukyokuScreen(sound::IDs::voxSifeng, &ResultDesc, mihajong_graphic::tblSubsceneFourRiichi, 1500u);
		/* TODO: ���v���ǂ����m�F���鏈�����ڐA����
		TenpaiCnt = 0
		repeat NUM_OF_PLAYERS
#ifdef SANMA4
			if (playerWind(targetPlayer, getRound(GameStat)) == PLAYER_NORTH) {continue}
#endif
			setHandStat GameStat, cnt, 1
			if (isTenpai(GameStat, GameEnv, cnt)) {
				setCall cnt, "���v"
			} else {
				setCall cnt, "���a"
			}
		loop
		redrscreen: await 5000

		repeat NUM_OF_ACTUAL_PLAYERS
#ifdef SANMA4
			if (playerWind(targetPlayer, getRound(GameStat)) == PLAYER_NORTH) {continue}
#endif
			// ���a�����i�s�������j�̎҂������ꍇ
			if (isTenpai(GameStat, GameEnv, cnt) == 0) {
				transferChonboPenalty GameStat, cnt
				await 500
			}
		loop
		*/
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_riichi_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* �������ю� */
	/**************/
	case NagashiMangan: /* ������ */
#if 0
		statmes "���ǂł�"
		snd_play SND_PINGJU
		setCenterTitle "����"
		if (NagashiManganFlag(getPlayer(GameStat)) == 1) {agariBgmSet = 0} else {agariBgmSet = 1}
		if (GetWatchModeFlag(GameEnv) == 1) {agariBgmSet = 0}
		if (chkRule("nagashi_mangan", "yakuman")) {
			switch agariBgmSet
				case 0: bgmplay MUS_AGARI_SELF_3: swbreak
				case 1: bgmplay MUS_AGARI_FURIKOMI_3: swbreak
			swend
		} else {
			switch agariBgmSet
				case 0: bgmplay MUS_AGARI_SELF_2: swbreak
				case 1: bgmplay MUS_AGARI_FURIKOMI_2: swbreak
			swend
		}
		redrscreen: await 1500
		statmes "�������т��������܂���"
		dim PointDelta, NUM_OF_PLAYERS, NUM_OF_DIGIT_GROUPS
		ResultDesc = ""
		repeat NUM_OF_ACTUAL_PLAYERS
			if (isNagashiMangan(GameStat, GameEnv, cnt)) {
				setCall cnt, "��������"
				switch playerWind(cnt, getRound(GameStat))
					case PLAYER_EAST:
						if (ResultDesc != "") {ResultDesc += "�A"}
						ResultDesc += "����"
					swbreak
					case PLAYER_SOUTH:
						if (ResultDesc != "") {ResultDesc += "�A"}
						ResultDesc += "���"
					swbreak
					case PLAYER_WEST:
						if (ResultDesc != "") {ResultDesc += "�A"}
						ResultDesc += "����"
					swbreak
#ifndef SANMAT
					case PLAYER_NORTH:
						if (ResultDesc != "") {ResultDesc += "�A"}
						ResultDesc += "�k��"
					swbreak
#endif
				swend
				dim AgariPointRaw, NUM_OF_DIGIT_GROUPS
				if (chkRule("nagashi_mangan", "mangan")) {
					AgariPointRaw(0) = 2000
				} else: if (chkRule("nagashi_mangan", "haneman")) {
					AgariPointRaw(0) = 3000
				} else: if (chkRule("nagashi_mangan", "baiman")) {
					AgariPointRaw(0) = 4000
				} else: if (chkRule("nagashi_mangan", "yakuman")) {
					AgariPointRaw(0) = 8000
				}
				calcAgariPoints GameStat, agariPointArray, AgariPointRaw, PointDelta, cnt
			}
		loop
		ResultDesc += "�̗�������"
		chatappend "*** "+ResultDesc+"\n"
		snd_play SND_PAGE
		redrscreen
		redraw 1: await 1500
		
		setCenterTitle "��������"
		putdelta PointDelta
		redraw 1: await 1500
		pointcalc GameStat, PointDelta
		
		ryuukyokuProc GameStat, 1
#endif
		break;
	/**************/
	/* �l�ȗ��ǎ� */
	/**************/
	case Uukaikan:
		/* TODO: ���������X�g�����邩�ǂ������߂邱�� statmes "����(�l�J��)" */
		ResultDesc = _T("�l�J��(�T��ڂ̞Ȃł̗���)");
		ryuukyokuScreen(sound::IDs::voxSikang, &ResultDesc, mihajong_graphic::tblSubsceneSikang);
		ryuukyokuProc(gameStat, !RuleData::chkRule("fifth_kong", "next_dealer"));
		break;
	}
	// TODO: ���� haifuwritebuffer GameStat, GameEnv, OrigTurn, OrigHonba, tmpUraFlag, tmpAliceFlag, ResultDesc, RoundEndType
	return;
}

// -------------------------------------------------------------------------

bool endround::nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn) { // ���̋ǂ�(�I������ꍇ��true)
	// �n�R����I��
	if (RuleData::chkRuleApplied("buttobi_border"))
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (isDobon(gameStat, i)) return true;
	// �V�ӏI��
	if (RuleData::chkRuleApplied("teppen"))
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (isTeppen(gameStat, i)) return true;
	// �a����~��
	if (RuleData::chkRuleApplied("agariyame")) {
		if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((OrigTurn + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((RoundEndType == Agari) || (RuleData::chkRule("agariyame", "yes_also_ready")))) {
				PlayerRankList Rank = calcRank(gameStat);
				if ((Rank[gameStat->GameRound % PLAYERS] == 1) &&
					(gameStat->Player[gameStat->GameRound % PLAYERS].PlayerScore >= LargeNum::fromInt(BasePoint())))
					return true;
		}
	}
	// �O���̏ꍇ
	if (chkGameType(gameStat, SanmaT) &&
		((gameStat->GameRound % PLAYERS) == 3))
		++(gameStat->GameRound);
	// ��������ꍇ�c�c
	if (gameStat->GameRound == 4) {
		if (RuleData::chkRule("game_length", "east_north_game")) // ���k�􂵂̂Ƃ�
			gameStat->GameRound = 12;
		else if (RuleData::chkRule("game_length", "east_west_game")) // �����􂵂̂Ƃ�
			gameStat->GameRound = 8;
		else if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game")) // ���X�􂵂̂Ƃ�
			gameStat->GameRound = 16;
	}
	// �ʏ�̔����I�����i�g�b�v���R�O�O�O�O�_�������Ɛ����T�h���f�X�j
	if ((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) > gameStat->GameLength) {
		bool flag = false;
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].PlayerScore >= LargeNum::fromInt(BasePoint()))
				return true;
		// ������Ȃ��ݒ�
		if (RuleData::chkRule("sudden_death_type", "no")) return true;
	}
	// ������̒����ɐ���������ꍇ
	if (RuleData::chkRule("sudden_death_type", "one_extra_round")) {
		if ((gameStat->GameRound == 16) && // ���X�􂵂̂Ƃ�
			(RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game"))) {
				if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) >= (gameStat->GameLength + roundLoopRate())))
					return true;
		} else { // ����ȊO�̂Ƃ�
			if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) >= (gameStat->GameLength + 5)))
				return true;
		}
	}
	// �k��I���̏ꍇ�͋A�蓌��
	if (gameStat->GameRound == roundLoopRate()) {
		if (RuleData::chkRule("sudden_death_type", "no")) { // �����햳���ŏI��
			return true;
		} else { // �Ԃ蓌
			++(gameStat->LoopRound); gameStat->GameRound = 0;
		}
	}
	// �Ă����������[���̏ꍇ
	if (RuleData::chkRuleApplied("yakitori") && RuleData::chkRuleApplied("yakitori_again")) {
		bool flag = true;
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].YakitoriFlag) flag = false;
		if (flag)
			for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
				gameStat->Player[i].YakitoriFlag = true;
	}
	return false;
}

// -------------------------------------------------------------------------

namespace {
	InfoByPlayer<LargeNum> delta;

	std::tuple<bool, signed short> checkExponent(PLAYER_ID player, unsigned group, unsigned digit) {
		if (delta[player].digitGroup[group] / (int)std::pow(10.0, (int)digit) != 0) {
			if (digit == 0) {
				assert(group != 0);
				return std::make_tuple(true,
					(delta[player].digitGroup[group] % 10) * 100 + delta[player].digitGroup[group - 1] / 1000000);
			} else if (digit == 1) {
				assert(group != 0);
				return std::make_tuple(true,
					(delta[player].digitGroup[group] % 100) * 10 + delta[player].digitGroup[group - 1] / 10000000);
			} else {
				return std::make_tuple(true,
					(delta[player].digitGroup[group] / ((int)std::pow(10.0, (int)digit) - 2)) % 1000);
			}
		}
		else if ((group == 0) && (digit == 2))
			return std::make_tuple(true, delta[player].digitGroup[0]);
		else return std::make_tuple(false, 0);
	}

	void setTransferParam() {
		bool finishFlag = false; signed short mantissa = 0;
		for (PLAYER_ID i = 0; i < PLAYERS; ++i) {
			mihajong_graphic::callvalue::CallValue callVal = {0, 0u};
			for (int j = DIGIT_GROUPS - 1; j >= 0; --j) {
				for (int k = (j == DIGIT_GROUPS - 1) ? 9 : 7; k >= 0; --k) {
					std::tie(finishFlag, mantissa) = checkExponent(i, j, k);
					if (finishFlag) {
						callVal.Exponent = j * 8 + k;
						callVal.Mantissa = mantissa;
						break;
					}
				}
				if (finishFlag) break;
			}
			if (mantissa != 0)
				mihajong_graphic::callvalue::setVal(i, callVal.Mantissa, callVal.Exponent);
			else
				mihajong_graphic::callvalue::setVal(i, 0, 0u);
		}
	}
}
void endround::transfer::resetDelta() {
	for (PLAYER_ID i = 0; i < PLAYERS; ++i)
		delta[i] = LargeNum::fromInt(0);
}
void endround::transfer::addDelta(PLAYER_ID player, LargeNum& deltaVal) {
	delta[player] += deltaVal;
}
void endround::transfer::transferPoints(GameTable* gameStat, unsigned subscene, unsigned wait) {
	setTransferParam();
	mihajong_graphic::Subscene(subscene);
	Sleep(wait);
	for (PLAYER_ID i = 0; i < PLAYERS; ++i)
		gameStat->Player[i].PlayerScore += delta[i];
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
}
