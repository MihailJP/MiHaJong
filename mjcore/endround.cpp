#include "endround.h"

#include "ruletbl.h"
#include "func.h"
#include "logging.h"
#include <functional>
#include <array>
#include <tuple>
#include <cassert>
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "../graphic/graphic.h"
#include "tileutil.h"
#include "haifu.h"
#include "chat.h"
#include "agari.h"
#include "envtbl.h"

namespace { // ���������Ɏg���֐�
	bool all_player(const GameTable* gameStat, std::function<bool (const PlayerTable*)> f) {
		bool flag = true;
		if (chkGameType(gameStat, Sanma4)) {
			for (PlayerID i = 0; i < Players; ++i)
				flag = flag && ((playerwind(gameStat, i, gameStat->GameRound) == sNorth) || f(&(gameStat->Player[i])));
		} else if (chkGameType(gameStat, SanmaT)) {
			for (PlayerID i = 0; i < 3; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		} else {
			for (PlayerID i = 0; i < Players; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		}
		return flag;
	}
	bool chksifeng(const GameTable* gameStat) {
		bool flag = false;
		std::array<TileCode, 4> winds = {EastWind, SouthWind, WestWind, NorthWind,};
		for (auto k = winds.begin(); k != winds.end(); ++k) {
			bool tmpflag = true;
			if (chkGameType(gameStat, Sanma4)) {
				for (PlayerID i = 0; i < Players; ++i)
					tmpflag = tmpflag && ((playerwind(gameStat, i, gameStat->GameRound) == sNorth) || (gameStat->Player[i].Discard[1].tcode.tile == *k));
			} else if (chkGameType(gameStat, SanmaT)) {
				for (PlayerID i = 0; i < 3; ++i)
					tmpflag = tmpflag && (gameStat->Player[i].Discard[1].tcode.tile == *k);
			} else {
				for (PlayerID i = 0; i < Players; ++i)
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
			(!gameStat->statOfActive().AgariHouki)) { // �܂��A�K�������ɂȂ��Ă��Ȃ��Ȃ�
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
					gameStat->statOfActive().AgariHouki = true;
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
		for (unsigned i = 0; i < NumOfTilesInHand; ++i)
			if (gameStat->statOfActive().Hand[i].tile != NoTile)
				++tmptilecnt;
		tmptilecnt += gameStat->statOfActive().MeldPointer * 3;
		if ((tmptilecnt != (NumOfTilesInHand - 1)) && (!gameStat->statOfActive().AgariHouki)) {
			// ���v�⏭�v�̏ꍇ�̏���(�ʏ�N���邱�Ƃ͂Ȃ��͂������c)
			if (tmptilecnt > (NumOfTilesInHand - 1))
				warn(_T("���v�����o���܂����B�a��������Ƃ��Ĉ����܂����A�̑ł̏����ŕs�������N���Ă����\��������܂��B"));
			else
				warn(_T("���v�����o���܂����B�a��������Ƃ��Ĉ����܂����A�̑ł̏����ŕs�������N���Ă����\��������܂��B"));
			sound::Play(sound::IDs::sndCuohu);
			for (unsigned i = 0; i < 2; ++i) {
				if (tmptilecnt > (NumOfTilesInHand - 1))
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
			gameStat->statOfActive().AgariHouki = true;
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

	for (PlayerID i = 0; i < Players; ++i) gameStat->Player[i].Tsumohai().tile = NoTile; // �o�O�h�~�̂���
	if (chkKuikae(gameStat)) { // �򂢑ւ��̏ꍇ�̏���
		gameStat->AgariSpecialStat = agariKuikae;
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
	std::array<bool, Players> chkNagashiMangan(const GameTable* gameStat, EndType& RoundEndType) { /* �������т̔��� */
		std::array<bool, Players> NagashiManganFlag = {false,};
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

	void ryuukyokuScreen(unsigned soundNum, CodeConv::tstring* ResultDesc, unsigned subsceneCode, unsigned waittime = 3000, unsigned bgmNum = sound::IDs::musRyuukyoku) {
		using namespace mihajong_graphic;
		using namespace CodeConv;
		if (ResultDesc) {
			sound::Play(soundNum);
			writeChat(*ResultDesc);
			Subscene(subsceneCode);
			Sleep(3000);
		}
		for (PlayerID i = 0; i < Players; ++i)
			calltext::setCall(i, calltext::None);
		sound::Play(sound::IDs::sndPingju);
		sound::util::bgmplay(bgmNum);
		Subscene(tblSubsceneRyuukyoku);
		Sleep(waittime);
	}

	unsigned checkTenpai(GameTable* gameStat, CodeConv::tstring& ResultDesc, unsigned OrigTurn) {
		unsigned TenpaiCnt = 0;
		for (PlayerID i = 0; i < Players; ++i) {
			if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, i, OrigTurn) == sNorth))
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
		writeChat(ResultDesc + _T("�ł�"));
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneChkTenpai);
		Sleep(5000);
		return TenpaiCnt;
	}

	void transferNotenBappu(GameTable* gameStat, unsigned OrigTurn, unsigned TenpaiCnt) {
		using namespace endround::transfer;
		resetDelta();
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) {
			if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, i, OrigTurn) == sNorth))
				continue; // �l�l�O���̖k�Ƃ͖���
			if (chkGameType(gameStat, AllSanma)) { // �O���̏ꍇ
				if ((TenpaiCnt > 0) && (TenpaiCnt < 3)) {
					if (isTenpai(gameStat, i))
						addDelta(i, 3000 / TenpaiCnt);
					else addDelta(i, -3000 / (signed)(3 - TenpaiCnt));
				}
			} else { // �l���̏ꍇ
				if ((TenpaiCnt > 0) && (TenpaiCnt < 4)) {
					if (isTenpai(gameStat, i))
						addDelta(i, 3000 / TenpaiCnt);
					else addDelta(i, -3000 / (signed)(4 - TenpaiCnt));
				}
			}
		}
		if ((TenpaiCnt > 0) && (TenpaiCnt < ACTUAL_PLAYERS))
			transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValNotenBappu, 2500);
	}

	void showRenchanFlag(GameTable* gameStat, bool RenchanFlag) {
		using namespace mihajong_graphic::calltext;
		for (PlayerID cnt = 0; cnt < Players; ++cnt)
			setCall(cnt, None);
		if (RenchanFlag)
			setCall(gameStat->GameRound % Players, Renchan);
		else
			setCall(gameStat->GameRound % Players, Oyanagare);
		sound::Play(sound::IDs::sndPage);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade);
		return;
	}

	void ryuukyokuProc(GameTable* gameStat, bool RenchanFlag) {
		showRenchanFlag(gameStat, RenchanFlag);
		++(gameStat->Honba);
		if (!RenchanFlag) ++(gameStat->GameRound);
		gameStat->AgariChain = 0; gameStat->LastAgariPlayer = -1;
		return;
	}
}

void endround::endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba) {
	EndType RoundEndType = roundEndType;
	bool tmpUraFlag = false, tmpAliceFlag = false;
	CodeConv::tstring ResultDesc; 
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
	if ((RoundEndType == Agari) || (RoundEndType == Chonbo))
		agari::agariproc(RoundEndType, gameStat, tmpUraFlag, tmpAliceFlag, ResultDesc);
	bool RenchanFlag = false;
	switch (RoundEndType) {
	/**************/
	/* �r�v���ǎ� */
	/**************/
	case Ryuukyoku:
		/* TODO: ���ꑽ������Ȃ� statmes "���ǂł�" */
		ResultDesc = _T("�r�v����");
		writeChat(ResultDesc);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);
		transferNotenBappu(gameStat, OrigTurn,
			checkTenpai(gameStat, ResultDesc, OrigTurn));

		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			// ���a�����i�s�������j�̎҂������ꍇ
			if ((!isTenpai(gameStat, cnt)) && (gameStat->Player[cnt].RichiFlag.RichiFlag)) {
				transferChonboPenalty(gameStat, cnt);
				Sleep(500);
			}
		}
		
		if (RuleData::chkRule("round_continuation", "renchan_if_ready")) {
			if (isTenpai(gameStat, gameStat->GameRound % Players)) RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_always")) {
			RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_ready_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % Players) ||
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_mahjong_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % Players) &&
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		}
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* �a�������� */
	/**************/
	case Agari: {
		const bool RenchanFlag =
			(playerwind(gameStat, gameStat->CurrentPlayer.Agari, gameStat->GameRound) == sEast) &&
			(!RuleData::chkRule("round_continuation", "renchan_never"));
		showRenchanFlag(gameStat, RenchanFlag);
		if (RenchanFlag) {
			++(gameStat->Honba);
		} else {
			++(gameStat->GameRound); gameStat->Honba = 0;
		}
		gameStat->Deposit = 0;
		// ���A���������A�J�E���^�����Z�b�g
		if (gameStat->AgariChain == 8)
			gameStat->AgariChain = 0;
	}
		break;
	/**************/
	/* ���a������ */
	/**************/
	case Chonbo:
		gameStat->AgariChain = 0; gameStat->LastAgariPlayer = -1;
		break;
	/**************/
	/* ��헬�ǎ� */
	/**************/
	case KyuushuKyuuhai:
		if      (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sEast ) % Players))
			ResultDesc = _T("���Ƃ̋���v");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sSouth) % Players))
			ResultDesc = _T("��Ƃ̋���v");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sWest ) % Players))
			ResultDesc = _T("���Ƃ̋���v");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sNorth) % Players))
			ResultDesc = _T("�k�Ƃ̋���v");
		else ResultDesc = _T("����v"); // �������Ď��s����Ȃ��͂�
		writeChat(ResultDesc);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);

		if (RuleData::chkRule("nine_terminals", "next_dealer") == 0)
			RenchanFlag = ((!RuleData::chkRule("nine_terminals", "renchan_if_dealer_kyuushu")) || (gameStat->CurrentPlayer.Active == (gameStat->GameRound % Players)));
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
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi != (gameStat->GameRound % Players));
		else if (RuleData::chkRule("triple_mahjong", "renchan_if_north_furikomi") || RuleData::chkRule("triple_mahjong", "renchan_if_west_furikomi"))
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi == ((gameStat->GameRound + (chkGameType(gameStat, AllSanma) ? sWest : sNorth)) % Players));
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
		checkTenpai(gameStat, ResultDesc, OrigTurn);
		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, cnt, gameStat->GameRound) == sNorth))
				continue;
			// ���a�����i�s�������j�̎҂������ꍇ
			if (!isTenpai(gameStat, cnt)) {
				transferChonboPenalty(gameStat, cnt);
				Sleep(500);
			}
		}
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_riichi_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* �������ю� */
	/**************/
	case NagashiMangan:
	{
		/* TODO: �{���ɂ���Ȃ����m�F statmes "���ǂł�" */
		const bool agariBgmSet = !(NagashiManganFlag[gameStat->PlayerID] || EnvTable::Instantiate()->WatchModeFlag); // �����̗������тȂ�true
		const unsigned bgmNum =
			RuleData::chkRule("nagashi_mangan", "yakuman") ? (agariBgmSet ? sound::IDs::musAgariSelf3 : sound::IDs::musAgariFurikomi3)
			: (agariBgmSet ? sound::IDs::musAgariSelf2 : sound::IDs::musAgariFurikomi2);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u, bgmNum);
		/* TODO: ����͖{���ɂ���Ȃ��̂��ǂ��� statmes "�������т��������܂���" */
		transfer::resetDelta();
		{
			CodeConv::tstring ResultDesc(_T(""));
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (isNagashiMangan(gameStat, cnt)) {
					mihajong_graphic::calltext::setCall(cnt, mihajong_graphic::calltext::NagashiMangan);
					if (!ResultDesc.empty()) ResultDesc += _T("�A");
					switch (playerwind(gameStat, cnt, gameStat->GameRound)) {
						case sEast:  ResultDesc += _T("����"); break;
						case sSouth: ResultDesc += _T("���"); break;
						case sWest:  ResultDesc += _T("����"); break;
						case sNorth: ResultDesc += _T("�k��"); break;
					}
					LNum AgariPointRaw = 0;
					if      (RuleData::chkRule("nagashi_mangan", "mangan"))  AgariPointRaw = 2000;
					else if (RuleData::chkRule("nagashi_mangan", "haneman")) AgariPointRaw = 3000;
					else if (RuleData::chkRule("nagashi_mangan", "baiman"))  AgariPointRaw = 4000;
					else if (RuleData::chkRule("nagashi_mangan", "yakuman")) AgariPointRaw = 8000;
					LNum agariPoint; // �_�~�[�B���̕ϐ��͎g�p���Ȃ��B
					agari::calcAgariPoints(gameStat, agariPoint, AgariPointRaw, transfer::getDelta(), cnt);
				}
			}
			ResultDesc += _T("�̗�������");
			writeChat(ResultDesc);
		}
		sound::Play(sound::IDs::sndPage);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall);
		Sleep(1500);
		transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValNagashiMangan, 1500);
		ryuukyokuProc(gameStat, true);
	}
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

void endround::transferChonboPenalty(GameTable* gameStat, PlayerID targetPlayer) {
	transfer::resetDelta();
	LNum AgariPoint = 0, AgariPointRaw = 2000;
	agari::calcAgariPoints(gameStat, AgariPoint, AgariPointRaw, transfer::getDelta(), targetPlayer);
	transfer::negateDelta();
	/* �Ȃ��킴�킴��U�v���X�ŋ��߂ĕ����𔽓]����Ƃ�����肭�ǂ����Ƃ����Ă���̂��Ƃ�����
	   �_�p�l�̌v�Z���ɓV�䔟��(���l�Ƃ��đ傫�����Ɋۂ߂�)�I�ȏ������s���Ă��邽�߁A
	   �������}�C�i�X�ŗ^�����(���ɎO���Ŋێ��ݒ�ɂ��Ă�Ƃ��Ƃ�)�`�����{��������񂪂��邩��ł� */
	transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValChonboBappu, 1500);
	return;
}

// -------------------------------------------------------------------------

bool endround::nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn) { // ���̋ǂ�(�I������ꍇ��true)
	// �n�R����I��
	if (RuleData::chkRuleApplied("buttobi_border"))
		for (PlayerID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (isDobon(gameStat, i)) return true;
	// �V�ӏI��
	if (RuleData::chkRuleApplied("teppen"))
		for (PlayerID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (isTeppen(gameStat, i)) return true;
	// �a����~��
	if (RuleData::chkRuleApplied("agariyame")) {
		if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((OrigTurn + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((RoundEndType == Agari) || (RuleData::chkRule("agariyame", "yes_also_ready")))) {
				PlayerRankList Rank = calcRank(gameStat);
				if ((Rank[gameStat->GameRound % (int)Players] == 1) &&
					(gameStat->Player[gameStat->GameRound % (int)Players].PlayerScore >= (LNum)BasePoint()))
					return true;
		}
	}
	// �O���̏ꍇ
	if (chkGameType(gameStat, SanmaT) &&
		((gameStat->GameRound % Players) == 3))
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
		for (PlayerID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].PlayerScore >= (LNum)BasePoint())
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
		for (PlayerID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].YakitoriFlag) flag = false;
		if (flag)
			for (PlayerID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
				gameStat->Player[i].YakitoriFlag = true;
	}
	return false;
}

// -------------------------------------------------------------------------

namespace {
	InfoByPlayer<LNum> delta;

	std::tuple<bool, signed short> checkExponent(PlayerID player, unsigned group, unsigned digit) {
		if (((LargeNum)delta[player]).digitGroup[group] / (int)std::pow(10.0, (int)digit) != 0) {
			if (digit == 0) {
				assert(group != 0);
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] % 10) * 100 + ((LargeNum)delta[player]).digitGroup[group - 1] / 1000000);
			} else if (digit == 1) {
				assert(group != 0);
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] % 100) * 10 + ((LargeNum)delta[player]).digitGroup[group - 1] / 10000000);
			} else {
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] / ((int)std::pow(10.0, (int)digit - 2))) % 1000);
			}
		}
		else if ((group == 0) && (digit == 2))
			return std::make_tuple(true, ((LargeNum)delta[player]).digitGroup[0]);
		else return std::make_tuple(false, 0);
	}

	void setTransferParam() {
		bool finishFlag = false; signed short mantissa = 0;
		for (PlayerID i = 0; i < Players; ++i) {
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

InfoByPlayer<LNum>& endround::transfer::getDelta() {
	return delta;
}
void endround::transfer::resetDelta() {
	for (PlayerID i = 0; i < Players; ++i)
		delta[i] = 0;
}
void endround::transfer::addDelta(PlayerID player, const LNum& deltaVal) {
	delta[player] += deltaVal;
}
void endround::transfer::negateDelta() {
	for (PlayerID i = 0; i < Players; ++i)
		delta[i] *= -1;
}
void endround::transfer::doubleDelta(PlayerID player) {
	delta[player] *= 2;
}
void endround::transfer::doubleDelta() {
	for (PlayerID i = 0; i < Players; ++i)
		delta[i] *= 2;
}
void endround::transfer::transferPoints(GameTable* gameStat, unsigned subscene, unsigned wait) {
	setTransferParam();
	mihajong_graphic::Subscene(subscene);
	sound::Play(sound::IDs::sndPage);
	Sleep(wait);
	for (PlayerID i = 0; i < Players; ++i)
		gameStat->Player[i].PlayerScore += delta[i];
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
}
void endround::transfer::transferChip(GameTable* gameStat, unsigned subscene, unsigned wait) {
	setTransferParam();
	mihajong_graphic::Subscene(subscene);
	sound::Play(sound::IDs::sndPage);
	Sleep(wait);
	for (PlayerID i = 0; i < Players; ++i)
		gameStat->Player[i].playerChip += delta[i];
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
}