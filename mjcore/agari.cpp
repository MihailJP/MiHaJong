#include "agari.h"

#include <functional>
#include <regex>
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "bgmid.h"
#include "func.h"
#include "yaku/yaku.h"
#include "haifu.h"
#include "envtbl.h"
#include "endround.h"
#include "chat.h"
#include "ruletbl.h"
#include "tileutil.h"

// -------------------------------------------------------------------------

/* �V��Ή��̓_�_���� */
LNum endround::agari::agaricalc(const LNum& AgariPointRaw, int agariBairitsu, int agariBairitsu2, unsigned agariCount) {
	LNum agariPointArray = (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	for (unsigned i = 0; i < agariCount; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu2 + 99) / 100 * 100;
	return agariPointArray;
}
LNum endround::agari::agaricalc(const LNum& AgariPointRaw, rat agariBairitsu, rat agariBairitsu2, int agariCount, rat agariBairitsu3, int agariCount2) {
	LNum agariPointArray = (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	for (unsigned i = 0; i < agariCount; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu2.getNumerator() / agariBairitsu2.getDenominator() + 99) / 100 * 100;
	for (unsigned i = 0; i < agariCount2; ++i)
		agariPointArray += (AgariPointRaw * agariBairitsu3.getNumerator() / agariBairitsu3.getDenominator() + 99) / 100 * 100;
	return agariPointArray;
}

// -------------------------------------------------------------------------

namespace {
	using namespace endround::agari;

	void calcAgariPoints_Pao( // ��K�p��
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int Mode)
	{
		const PLAYER_ID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : (PLAYER_ID)Mode;
		const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
		if (playerwind(AgariPlayer, gameStat->GameRound) == sEast) { // �e�̘a����
			if (TsumoAgari) { // �c���A�K��(��̐l����l����)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
				}
			} else { // �����A�K��(��̐l�ƐU�荞�񂾐l�Őܔ�)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 3, 3, 1);
						deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
						deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
					}
					if (cnt == gameStat->CurrentPlayer.Furikomi)
						deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
				}
			}
		} else {
			// �q�̘a����
			if (TsumoAgari) { // �c���A�K��(��̐l����l����)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 4, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 4, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 4, cnt);
				}
			} else { // �����A�K��(��̐l�ƐU�荞�񂾐l�Őܔ�)
				for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
						deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
						deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					}
					if (cnt == gameStat->CurrentPlayer.Furikomi)
						deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
				}
			}
		}
	}

	void calcAgariPoints_Ron( // �ʏ펞�F����
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PLAYER_ID AgariPlayer)
	{
		if (playerwind(AgariPlayer, gameStat->GameRound) == sEast) { // �e�̘a����
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
				}
				if (cnt == gameStat->CurrentPlayer.Furikomi)
					deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
			}
		} else { // �q�̘a����
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 4, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, 4, cnt);
				}
				if (cnt == gameStat->CurrentPlayer.Furikomi)
					deltacalcminus(AgariPointRaw, PointDelta, 4, cnt);
			}
		}
	}

	void calcAgariPoints_Tsumo_Dealer( // �ʏ펞�F�e�̃c��
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PLAYER_ID AgariPlayer)
	{
		if (chkGameType(gameStat, Yonma) || (chkGameType(gameStat, Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// �l�������[��
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
				}
				else
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_omitted") || RuleData::chkRule("tsumo_payment", "add_1000")) {
			// �O�����̌��胋�[��
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) { // ��_������������
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else { // �O���Ńc�����肵�Ȃ����[��
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 3, 3, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
			}
		}
	}

	void calcAgariPoints_Tsumo_NonDealer( // �ʏ펞�F�q�̃c��
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PLAYER_ID AgariPlayer)
	{
		if (chkGameType(gameStat, Yonma) || (chkGameType(gameStat, Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// �l�������[��
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agaricalc(AgariPointRaw, 2, 1, 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
				}
				else if (playerwind(cnt, gameStat->GameRound) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
				else
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "fifty_fifty")) {
			// �O���F�e�q�֌W�Ȃ��ܔ����[��
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
				}
				else if (playerwind(cnt, gameStat->GameRound) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_omitted") || RuleData::chkRule("tsumo_payment", "add_1000")) {
			// �O���F���̌��胋�[���A��_�������[��
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) {
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (playerwind(cnt, gameStat->GameRound) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else if (RuleData::chkRule("tsumo_payment", "adjusted_payment")) {
			// �O���F�ێ�胋�[��
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, Rat(8, 3), Rat(4, 3), 1, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
				}
				else if (playerwind(cnt, gameStat->GameRound) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
				else if (playerwind(cnt, gameStat->GameRound) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_halved")) {
			// �O���F�k�Ƃ̕���ܔ�
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1, Rat(1, 2), 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (playerwind(cnt, gameStat->GameRound) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcminus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (playerwind(cnt, gameStat->GameRound) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcminus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				}
			}
		}
	}
}

void endround::agari::calcAgariPoints(
	const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw,
	InfoByPlayer<LNum>& PointDelta, int Mode)
{
	const PLAYER_ID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : (PLAYER_ID)Mode;
	const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
	if (isPaoAgari(gameStat, AgariPlayer)) // ��K�p��
		calcAgariPoints_Pao(gameStat, agariPoint, AgariPointRaw, PointDelta, Mode);
	else if ((TsumoAgari) && (playerwind(AgariPlayer, gameStat->GameRound) == sEast)) // �ʏ펞�F�e�̃c���A�K��
		calcAgariPoints_Tsumo_Dealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	else if (TsumoAgari) // �ʏ펞�F�q�̃c���A�K��
		calcAgariPoints_Tsumo_NonDealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer); // PLACEHOLDER
	else // �ʏ펞�F�����A�K��
		calcAgariPoints_Ron(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	return;
}

// -------------------------------------------------------------------------

namespace {
	void forEachAgariPlayers(std::function<bool (int&)> f) {
		for (int cnt = 0; cnt < PLAYERS - 1; ++cnt) {
			if (RuleData::chkRule("multiple_mahjong", "single_mahjong_with_draw") || RuleData::chkRule("multiple_mahjong", "single_mahjong")) {
				if (cnt > 0) break; // ���n�l(�_�u�����Ȃ�)���[���̏ꍇ
			} else if (RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw") || RuleData::chkRule("multiple_mahjong", "dual_mahjong")) {
				if (cnt > 1) break; // �_�u��������E�g�������Ȃ��̏ꍇ
			}
			if (!f(cnt)) break;
		}
	}

	void chonboIfShibariUnmet(const GameTable* gameStat, const yaku::YAKUSTAT& yakuInfo, const MachihaiInfo& machiInfo, EndType& RoundEndType) {
		// ����𖞂����Ȃ����A�U���̂Ƃ�
		if ((yakuInfo.CoreHan <= (gameStat->ShibariFlag ? 1 : 0)) || // ����𖞂����Ă��Ȃ����A
			(!gameStat->TsumoAgariFlag && ( // �u�����̎���
			machiInfo.FuritenFlag || // �t���e����
			gameStat->Player[gameStat->CurrentPlayer.Agari].DoujunFuriten)) || // �����t���e���̎��v�A��������
			(RuleData::chkRule("riichi_shibari", "yes") && (!gameStat->Player[gameStat->CurrentPlayer.Agari].RichiFlag.RichiFlag))) // ���[�`����𖞂����Ă��Ȃ��Ȃ��
			RoundEndType = Chonbo; // �`�����{�ɂ���
	}

	void verifyAgari(GameTable* gameStat, EndType& RoundEndType) {
		if (!gameStat->TsumoAgariFlag) {
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].red  = gameStat->CurrentDiscard.red;
		}
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
		MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
		// ����𖞂����Ȃ����A�U���̂Ƃ��͍��a
		chonboIfShibariUnmet(gameStat, yakuInfo, machiInfo, RoundEndType);
	}

	enum OptionBool {oFalse, oTrue, oNull,};

	OptionBool procSecondaryRon(GameTable* gameStat, EndType& RoundEndType, int& cnt) {
		RoundEndType = Agari;
		gameStat->CurrentPlayer.Agari = (gameStat->CurrentPlayer.Agari + 1) % PLAYERS;
		if (gameStat->CurrentPlayer.Agari == gameStat->CurrentPlayer.Furikomi) return oFalse; // ����������_�Ŕ�����
		if (gameStat->Player[gameStat->CurrentPlayer.Agari].DeclarationFlag.Ron) { // �������Ă����
			verifyAgari(gameStat, RoundEndType);
		} else {
			--cnt; return oTrue;
		}
		return oNull;
	}

	bool isSomeoneDobon(const GameTable* gameStat) {
		bool flag = false;
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isDobon(gameStat, i)) flag = true;
		return flag;
	}

	void calcDobonDelta(const GameTable* gameStat, PLAYER_ID AgariPlayerPriority, int penalty) {
		endround::transfer::resetDelta();
		for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (isDobon(gameStat, cnt)) {
				endround::transfer::addDelta(cnt, -penalty);
				endround::transfer::addDelta(AgariPlayerPriority, penalty);
			}
		}
	}


	void dobonPenalty(GameTable* gameStat, PLAYER_ID AgariPlayerPriority) {
		int penalty = 0;
		std::string penaConf(RuleData::chkRule("penalty_negative"));
		std::smatch matchDat;
		if (std::regex_match(penaConf, matchDat, std::regex("(\\d+)pts"))) { // �_�_�Ő��Z
			penalty = atoi(matchDat[1].str().c_str()); // ���[���ݒ蕶���񂩂琮���𒊏o
			if (isSomeoneDobon(gameStat)) {
				calcDobonDelta(gameStat, AgariPlayerPriority, penalty);
				endround::transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValDobon, 3000);
			}
		} else if (std::regex_match(penaConf, matchDat, std::regex("chip(\\d+)"))) { // �`�b�v�Ő��Z
			if (RuleData::chkRuleApplied("chip")) {
				penalty = atoi(matchDat[1].str().c_str()); // ���[���ݒ蕶���񂩂琮���𒊏o
				if (isSomeoneDobon(gameStat)) {
					calcDobonDelta(gameStat, AgariPlayerPriority, penalty);
					endround::transfer::transferChip(gameStat, mihajong_graphic::tblSubsceneCallValDobon, 1500);
				}
			}
		}
		return;
	}

}

void endround::agari::agariproc(EndType& RoundEndType, GameTable* gameStat, bool& tmpUraFlag, bool& tmpAliceFlag, CodeConv::tstring& ResultDesc) {
	bool tmpagariflag = false;
	PLAYER_ID FirstAgariPlayer = gameStat->CurrentPlayer.Agari;
	int OyaAgari = -1;
	ResultDesc = _T("");
	tmpUraFlag = 0;
	tmpAliceFlag = 0;
	int AgariPlayerPriority = -1;
	std::uint16_t origDoraPointer = gameStat->DoraPointer;

	forEachAgariPlayers([&gameStat, &RoundEndType](int& cnt) -> bool {
		if (cnt > 0) { // �_�u�����p�̏���
			OptionBool result = procSecondaryRon(gameStat, RoundEndType, cnt);
			if (result == oFalse) return false;
			else if (result == oTrue) return true;
		}
		if (gameStat->TsumoAgariFlag) return false;
		return true;
	});

	forEachAgariPlayers([&, FirstAgariPlayer](int& cnt) -> bool {
		if (cnt == 0) {
			gameStat->CurrentPlayer.Agari = FirstAgariPlayer;
			verifyAgari(gameStat, RoundEndType);
		} else if (cnt > 0) {
			if (!gameStat->TsumoAgariFlag) {
				OptionBool result = procSecondaryRon(gameStat, RoundEndType, cnt);
				if (result == oFalse) return false;
				else if (result == oTrue) return true;
			}
		}
		haifu::haifualicedoraupd();
		/**************/
		/* �a�������� */
		/**************/
		if (RoundEndType == Agari) {
			tmpagariflag = true;
			if (gameStat->Player[gameStat->CurrentPlayer.Agari].AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				RoundEndType = Chonbo; // �a����������̏������냍���E��c���Ƃ��Ĕ����Ƃ���
		}
		//if (RoundEndType == Agari)
			/* TODO: �����̈ڐA endround_agariproc GameStat, GameEnv, ResultDesc, AgariPlayerPriority, origDoraPointer, YakuInfo, tmpAliceFlag */
		/**************/
		/* ���a������ */
		/**************/
		if (RoundEndType == Chonbo)
			endround_chonboproc(gameStat, ResultDesc);

		if (gameStat->TsumoAgariFlag) return false; /* �c���a����̎��͏I�� */
		if ((!RuleData::chkRule("getting_deposit", "riichidori")) || gameStat->Player[gameStat->CurrentPlayer.Agari].RichiFlag.RichiFlag)
			gameStat->Deposit = 0;
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		return true;
	});
	RoundEndType = tmpagariflag ? Agari : Chonbo;
	/* �A������p�̃v���C���[�ԍ��ݒ� */
	if (RuleData::chkRule("simultaneous_mahjong", "renchan_if_dealer_mahjong"))
		gameStat->CurrentPlayer.Agari = (OyaAgari == -1) ? FirstAgariPlayer : OyaAgari;
	else if (RuleData::chkRule("simultaneous_mahjong", "renchan_if_dealer_upstream"))
		gameStat->CurrentPlayer.Agari = FirstAgariPlayer;
	else if (RuleData::chkRule("simultaneous_mahjong", "next_dealer"))
		gameStat->CurrentPlayer.Agari = FirstAgariPlayer + ((OyaAgari == FirstAgariPlayer) ? 1 : 0) % PLAYERS;

	dobonPenalty(gameStat, AgariPlayerPriority);
	return;
}

// -------------------------------------------------------------------------

/* ���a�������̏��� */
void endround::agari::endround_chonboproc(GameTable* gameStat, CodeConv::tstring& ResultDesc) {
	if (!ResultDesc.empty()) ResultDesc += _T("\n");
	CodeConv::tstring tmpResultDesc;
	switch (playerwind(gameStat, gameStat->CurrentPlayer.Agari, gameStat->GameRound)) {
		case sEast:  tmpResultDesc += _T("���Ƃ̃`�����{"); break;
		case sSouth: tmpResultDesc += _T("��Ƃ̃`�����{"); break;
		case sWest:  tmpResultDesc += _T("���Ƃ̃`�����{"); break;
		case sNorth: tmpResultDesc += _T("�k�Ƃ̃`�����{"); break;
	}
	ResultDesc += tmpResultDesc + _T("�ł�");
	writeChat(tmpResultDesc);
	/* TODO: �����p�~���邩�̌��� statmes tmpResultDesc */
	Sleep(1500);
	sound::Play(sound::IDs::musRyuukyoku);
	/* �냍���܂��͌�c�� */
	if ((gameStat->AgariSpecialStat == 0) || (gameStat->AgariSpecialStat == 1)) {
		if ((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong"))
			ResultDesc += _T("(�喾�Ȃ̗��v�ł̘a����)"); // �喾�Ȃ̗��J�ԋ֎~���[���̏ꍇ
		else if ((!gameStat->Player[gameStat->CurrentPlayer.Agari].RichiFlag.RichiFlag) && RuleData::chkRuleApplied("riichi_shibari"))
			ResultDesc += _T("(�_�}���ł̘a����)"); // ��������ŗ������ĂȂ��Ȃ���a
		else if (!isTenpai(gameStat, gameStat->CurrentPlayer.Agari))
			ResultDesc += _T("(�������Ă��Ȃ��a����)"); // �a���肪�������Ă��Ȃ��ꍇ���a
		else {
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
			MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
			if (machiInfo.FuritenFlag || gameStat->Player[gameStat->CurrentPlayer.Agari].DoujunFuriten)
				ResultDesc += _T("(�U���ł̃����a����)"); // �U���Ȃ̂Ƀ��������ꍇ�͍��a
			else if (yakuInfo.CoreHan == 0)
				ResultDesc += _T("(���̂Ȃ��a����)"); // �����Ȃ������ꍇ�͍��a
			else if ((yakuInfo.CoreHan == 1) && gameStat->ShibariFlag)
				ResultDesc += _T("(���ʂ����Ȃ��a����)"); // ���ʔ���łP�ʂ����Ȃ��Ȃ���a
			else if (gameStat->Player[gameStat->CurrentPlayer.Agari].AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				ResultDesc += _T("(�a��������K�p��)"); // �a��������Ȃ̂ɘa���낤�Ƃ���
		}
		sound::Play(sound::IDs::sndCuohu);
	}
	/* �냍���A��c���ȊO�̍��a */
	else {
		if (gameStat->AgariSpecialStat == AGARI_KUIKAE)
			ResultDesc += _T("(�򂢑ւ�)"); // �򂢑ւ��������Ƃ�
		sound::Play(sound::IDs::sndPage);
	}
	mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneChonbo);
	Sleep(5000);

	transferChonboPenalty(gameStat, gameStat->CurrentPlayer.Agari);
	return;
}
