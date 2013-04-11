#include "agari.h"

#include <functional>
#include <regex>
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "func.h"
#include "haifu.h"
#include "envtbl.h"
#include "endround.h"
#include "chat.h"
#include "ruletbl.h"
#include "tileutil.h"
#include <cassert>

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
		const PlayerID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : (PlayerID)Mode;
		const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
		if (gameStat->playerwind(AgariPlayer) == sEast) { // �e�̘a����
			if (TsumoAgari) { // �c���A�K��(��̐l����l����)
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
				}
			} else { // �����A�K��(��̐l�ƐU�荞�񂾐l�Őܔ�)
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if (cnt == AgariPlayer) {
						agariPoint = agaricalc(AgariPointRaw, 4, 0, 0);
						deltacalcplus(AgariPointRaw, PointDelta, 4, cnt);
					}
					if (isPao(gameStat, AgariPlayer, cnt))
						deltacalcminus(AgariPointRaw, PointDelta, 4, cnt);
				}
			} else { // �����A�K��(��̐l�ƐU�荞�񂾐l�Őܔ�)
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PlayerID AgariPlayer)
	{
		if (gameStat->playerwind(AgariPlayer) == sEast) { // �e�̘a����
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 6, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, 6, cnt);
				}
				if (cnt == gameStat->CurrentPlayer.Furikomi)
					deltacalcminus(AgariPointRaw, PointDelta, 6, cnt);
			}
		} else { // �q�̘a����
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PlayerID AgariPlayer)
	{
		if (gameStat->chkGameType(Yonma) || (gameStat->chkGameType(Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// �l�������[��
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
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
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) { // ��_������������
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (gameStat->playerwind(cnt) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else { // �O���Ńc�����肵�Ȃ����[��
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 3, 3, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 3, cnt);
				} else if (gameStat->playerwind(cnt) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 3, cnt);
			}
		}
	}

	void calcAgariPoints_Tsumo_NonDealer( // �ʏ펞�F�q�̃c��
		const GameTable* gameStat, LNum& agariPoint, const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, PlayerID AgariPlayer)
	{
		if (gameStat->chkGameType(Yonma) || (gameStat->chkGameType(Sanma4) && RuleData::chkRule("tsumo_payment", "same_as_yonma"))) {
			// �l�������[��
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
				}
				else if (gameStat->playerwind(cnt) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
				else
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "fifty_fifty")) {
			// �O���F�e�q�֌W�Ȃ��ܔ����[��
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 2, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
				}
				else if (gameStat->playerwind(cnt) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_omitted") || RuleData::chkRule("tsumo_payment", "add_1000")) {
			// �O���F���̌��胋�[���A��_�������[��
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) {
						agariPoint += 2000;
						PointDelta[cnt] += 2000;
					}
				} else if (gameStat->playerwind(cnt) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				} else if (gameStat->playerwind(cnt) != sNorth) {
					deltacalcminus(AgariPointRaw, PointDelta, 1, cnt);
					if (RuleData::chkRule("tsumo_payment", "add_1000")) PointDelta[cnt] -= 1000;
				}
			}
		} else if (RuleData::chkRule("tsumo_payment", "adjusted_payment")) {
			// �O���F�ێ�胋�[��
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, Rat(8, 3), Rat(4, 3), 1, 0, 0);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
				}
				else if (gameStat->playerwind(cnt) == sEast)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(8, 3), cnt);
				else if (gameStat->playerwind(cnt) != sNorth)
					deltacalcminus(AgariPointRaw, PointDelta, Rat(4, 3), cnt);
			}
		} else if (RuleData::chkRule("tsumo_payment", "north_segment_halved")) {
			// �O���F�k�Ƃ̕���ܔ�
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == AgariPlayer) {
					agariPoint = agaricalc(AgariPointRaw, 2, 1, 1, Rat(1, 2), 2);
					deltacalcplus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, 1, cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
					deltacalcplus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (gameStat->playerwind(cnt) == sEast) {
					deltacalcminus(AgariPointRaw, PointDelta, 2, cnt);
					deltacalcminus(AgariPointRaw, PointDelta, Rat(1, 2), cnt);
				} else if (gameStat->playerwind(cnt) != sNorth) {
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
	const PlayerID AgariPlayer = (Mode == CAP_normal) ? gameStat->CurrentPlayer.Agari : (PlayerID)Mode;
	const bool TsumoAgari = (Mode == CAP_normal) ? gameStat->TsumoAgariFlag : true;
	if (isPaoAgari(gameStat, AgariPlayer)) // ��K�p��
		calcAgariPoints_Pao(gameStat, agariPoint, AgariPointRaw, PointDelta, Mode);
	else if ((TsumoAgari) && (gameStat->playerwind(AgariPlayer) == sEast)) // �ʏ펞�F�e�̃c���A�K��
		calcAgariPoints_Tsumo_Dealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	else if (TsumoAgari) // �ʏ펞�F�q�̃c���A�K��
		calcAgariPoints_Tsumo_NonDealer(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	else // �ʏ펞�F�����A�K��
		calcAgariPoints_Ron(gameStat, agariPoint, AgariPointRaw, PointDelta, AgariPlayer);
	return;
}

// -------------------------------------------------------------------------

namespace {
	void forEachAgariPlayers(std::function<bool (int&)> f) {
		for (int cnt = 0; cnt < Players - 1; ++cnt) {
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
		if (((yakuInfo.CoreHan <= (gameStat->ShibariFlag ? 1 : 0)) && (yakuInfo.CoreSemiMangan <= 0)) || // ����𖞂����Ă��Ȃ����A
			(!gameStat->TsumoAgariFlag && ( // �u�����̎���
			machiInfo.FuritenFlag || // �t���e����
			gameStat->statOfAgari().DoujunFuriten)) || // �����t���e���̎��v�A��������
			(RuleData::chkRule("riichi_shibari", "yes") && (!gameStat->statOfAgari().RichiFlag.RichiFlag))) // ���[�`����𖞂����Ă��Ȃ��Ȃ��
			RoundEndType = Chonbo; // �`�����{�ɂ���
	}

	void verifyAgari(GameTable* gameStat, EndType& RoundEndType) {
		if (!gameStat->TsumoAgariFlag) {
			gameStat->statOfAgari().Tsumohai().tile = gameStat->CurrentDiscard.tile;
			gameStat->statOfAgari().Tsumohai().red  = gameStat->CurrentDiscard.red;
		}
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
		MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
		// ����𖞂����Ȃ����A�U���̂Ƃ��͍��a
		chonboIfShibariUnmet(gameStat, yakuInfo, machiInfo, RoundEndType);
	}

	enum OptionBool {oFalse, oTrue, oNull,};

	OptionBool procSecondaryRon(GameTable* gameStat, EndType& RoundEndType, int& cnt) {
		RoundEndType = Agari;
		gameStat->CurrentPlayer.Agari = (gameStat->CurrentPlayer.Agari + 1) % Players;
		if (gameStat->CurrentPlayer.Agari == gameStat->CurrentPlayer.Furikomi) return oFalse; // ����������_�Ŕ�����
		if (gameStat->statOfAgari().DeclarationFlag.Ron) { // �������Ă����
			verifyAgari(gameStat, RoundEndType);
		} else {
			--cnt; return oTrue;
		}
		return oNull;
	}

	bool isSomeoneDobon(const GameTable* gameStat) {
		bool flag = false;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isDobon(gameStat, i)) flag = true;
		return flag;
	}

	void calcDobonDelta(const GameTable* gameStat, PlayerID AgariPlayerPriority, int penalty) {
		endround::transfer::resetDelta();
		for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (isDobon(gameStat, cnt)) {
				endround::transfer::addDelta(cnt, -penalty);
				endround::transfer::addDelta(AgariPlayerPriority, penalty);
			}
		}
	}


	void dobonPenalty(GameTable* gameStat, PlayerID AgariPlayerPriority) {
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
	PlayerID FirstAgariPlayer = gameStat->CurrentPlayer.Agari;
	int OyaAgari = -1;
	ResultDesc = _T("");
	tmpUraFlag = false;
	tmpAliceFlag = false;
	PlayerID AgariPlayerPriority = -1;
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
			if (gameStat->statOfAgari().AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				RoundEndType = Chonbo; // �a����������̏������냍���E��c���Ƃ��Ĕ����Ƃ���
		}
		if (RoundEndType == Agari) {
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
			endround_agariproc(gameStat, ResultDesc, AgariPlayerPriority, origDoraPointer, yakuInfo, tmpUraFlag, tmpAliceFlag, OyaAgari);
		}
		/**************/
		/* ���a������ */
		/**************/
		if (RoundEndType == Chonbo)
			endround_chonboproc(gameStat, ResultDesc);

		if (gameStat->TsumoAgariFlag) return false; /* �c���a����̎��͏I�� */
		if ((!RuleData::chkRule("getting_deposit", "riichidori")) || gameStat->statOfAgari().RichiFlag.RichiFlag)
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
		gameStat->CurrentPlayer.Agari = FirstAgariPlayer + ((OyaAgari == FirstAgariPlayer) ? 1 : 0) % Players;

	dobonPenalty(gameStat, AgariPlayerPriority);
	return;
}

// -------------------------------------------------------------------------

namespace {
	void deltawareme(PlayerID agariTmpPlayer, PlayerID agariTmpWareme) {
		LNum subtrahend = endround::transfer::getDelta()[agariTmpWareme];
		endround::transfer::addDelta(agariTmpPlayer, -subtrahend);
		return;
	}

	void calculateWareme(const GameTable* gameStat) {
		if (gameStat->WaremePlayer == gameStat->CurrentPlayer.Agari) {
			endround::transfer::doubleDelta(); // ����ڂ̐l���a�������Ƃ�
		} else { // ����ڂ̐l�ȊO���a�������Ƃ�
			deltawareme(gameStat->CurrentPlayer.Agari, gameStat->WaremePlayer);
			endround::transfer::doubleDelta(gameStat->WaremePlayer);
		}
		return;
	}

	void calculateDoukasen(const GameTable* gameStat) {
		if (gameStat->DoukasenPlayer == gameStat->CurrentPlayer.Agari) {
			endround::transfer::doubleDelta(); // ���ΐ��̐l���a�������Ƃ�
		} else { // ���ΐ��̐l�ȊO���a�������Ƃ�
			deltawareme(gameStat->CurrentPlayer.Agari, gameStat->DoukasenPlayer);
			endround::transfer::doubleDelta(gameStat->DoukasenPlayer);
		}
		return;
	}

	void calculateWaremeDelta(const GameTable* gameStat) { // ����ڂƂ�
		if (RuleData::chkRuleApplied("wareme"))
			calculateWareme(gameStat); // ����ڃ��[��
		if ((RuleData::chkRule("wareme", "greater_wareme")) && (gameStat->Dice[0].Number == gameStat->Dice[1].Number))
			calculateWareme(gameStat); // �T�C�R�����]���ڂ̎��͂���ɔ{
		if (RuleData::chkRuleApplied("doukasen"))
			calculateDoukasen(gameStat); // ���ΐ����[��
	}

	void calculateTsumibouDelta(const GameTable* gameStat) { // �ςݖ_�̌v�Z
		int tsumiboh_rate = 0;
		std::smatch matchDat;
		std::string tsumiboh_rate_str(RuleData::chkRule("tsumiboh_rate"));
		// MEMORANDUM: �����Ώە������in-situ�ō���Ă̓_�� �K�������ɓn���O�ɃI�u�W�F�N�g������Ă�������
		if (std::regex_match(tsumiboh_rate_str, matchDat, std::regex("counter_(\\d+)")))
			tsumiboh_rate = std::atoi(matchDat[1].str().c_str()) / (ACTUAL_PLAYERS - 1);
		endround::transfer::resetDelta();
		if (gameStat->TsumoAgariFlag) {
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == gameStat->CurrentPlayer.Agari)
					endround::transfer::addDelta(cnt, gameStat->Honba *   (ACTUAL_PLAYERS - 1)  * tsumiboh_rate);
				else
					endround::transfer::addDelta(cnt, gameStat->Honba * (-(ACTUAL_PLAYERS - 1)) * tsumiboh_rate);
			}
		} else {
			for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (cnt == gameStat->CurrentPlayer.Agari)
					endround::transfer::addDelta(cnt, gameStat->Honba *   (ACTUAL_PLAYERS - 1)  * tsumiboh_rate);
				else if (cnt == gameStat->CurrentPlayer.Furikomi)
					endround::transfer::addDelta(cnt, gameStat->Honba * (-(ACTUAL_PLAYERS - 1)) * tsumiboh_rate);
			}
		}
	}

	void chipTransfer(GameTable* gameStat, unsigned subscene, int ChipAmount) {
		if ((ChipAmount <= 0) || (!RuleData::chkRuleApplied("chip"))) return;
		endround::transfer::resetDelta();
		std::string limithand_bonus(RuleData::chkRule("limithand_bonus"));
		if ((!gameStat->TsumoAgariFlag) && (!std::regex_match(limithand_bonus, std::regex("chip_\\d+_each")))) {
			endround::transfer::addDelta(gameStat->CurrentPlayer.Furikomi, -ChipAmount);
			endround::transfer::addDelta(gameStat->CurrentPlayer.Agari   ,  ChipAmount);
		} else {
			for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
				endround::transfer::addDelta(i, -ChipAmount);
			endround::transfer::addDelta(gameStat->CurrentPlayer.Agari, ChipAmount * ACTUAL_PLAYERS);
		}
		endround::transfer::transferChip(gameStat, subscene, 1500);
		return;
	}

	int getChipAmount(const GameTable* gameStat, const YakuResult* yakuInfo) {
		if (!RuleData::chkRuleApplied("chip")) return 0; // �`�b�v�����̃��[���Ȃ炷���߂�
		int ChipAmount /* �`�b�v�̗� */ = 0;
		/* ���h���A�A���X�A�ꔭ�j�V */
		if (gameStat->statOfAgari().MenzenFlag) {
			ChipAmount = yakuInfo->UraDoraQuantity + yakuInfo->AliceDora; // ���h���ƃA���X�h��
			if ((gameStat->statOfAgari().RichiFlag.IppatsuFlag) &&
				(RuleData::chkRuleApplied("riichi_ippatsu")))
				++ChipAmount; // �ꔭ�j�V
		}
		/* �ԃh���j�V */
		if (RuleData::chkRule("redtile_chip", "menzen_only") || RuleData::chkRule("redtile_chip", "menzen_only_non_dora")) { // ��O�̂ݐԃh���`�b�v���t�����[��
			if (gameStat->statOfAgari().MenzenFlag)
				ChipAmount += yakuInfo->AkaDoraQuantity;
		} else if (RuleData::chkRule("redtile_chip", "riichi_only") || RuleData::chkRule("redtile_chip", "riichi_only_non_dora")) { // ���[�`���̂ݐԃh���`�b�v���t�����[��
			if (gameStat->statOfAgari().RichiFlag.RichiFlag)
				ChipAmount += yakuInfo->AkaDoraQuantity;
		} else if (RuleData::chkRule("redtile_chip", "always") || RuleData::chkRule("redtile_chip", "always_but_non_dora")) { // ���Ă��Ă��ԃh���`�b�v���t�����[��
			ChipAmount += yakuInfo->AkaDoraQuantity;
		}
		/* �h���j�V */
		if (RuleData::chkRule("blue_chip", "menzen_only")) { // ��O�̂ݐԃh���`�b�v���t�����[��
			if (gameStat->statOfAgari().MenzenFlag)
				ChipAmount += yakuInfo->AoDoraQuantity;
		} else if (RuleData::chkRule("blue_chip", "riichi_only")) { // ���[�`���̂ݐԃh���`�b�v���t�����[��
			if (gameStat->statOfAgari().RichiFlag.RichiFlag)
				ChipAmount += yakuInfo->AoDoraQuantity;
		} else if (RuleData::chkRule("blue_chip", "always")) { // ���Ă��Ă��ԃh���`�b�v���t�����[��
			ChipAmount += yakuInfo->AoDoraQuantity;
		}
		/* ���v��Ԃ� */
		return ChipAmount;
	}

	void agariscrproc(const GameTable* gameStat, const YakuResult* yakuInfo,
		const LNum* agariPointArray, int& ChipAmount, const CodeConv::tstring& ResultDesc, bool& tmpUraFlag)
	{
		sound::util::bgmstop();
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		ChipAmount = getChipAmount(gameStat, yakuInfo);
		mihajong_graphic::YakuResult::setYakuStat(yakuInfo, static_cast<LargeNum>(*agariPointArray), ChipAmount);
		tmpUraFlag = gameStat->statOfAgari().MenzenFlag && gameStat->statOfAgari().RichiFlag.RichiFlag && (!RuleData::chkRule("uradora", "no"));
		if (tmpUraFlag)
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneAgariUradora);
		else
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneAgari);
		(void)mihajong_graphic::ui::WaitUI();
	}

}

/* �a���������̏��� */
void endround::agari::endround_agariproc(GameTable* gameStat, CodeConv::tstring& ResultDesc, PlayerID& AgariPlayerPriority,
	std::uint16_t origDoraPointer, const yaku::YAKUSTAT& yakuInfo, bool& tmpUraFlag, bool& tmpAliceFlag, int& OyaAgari)
{
	LNum AgariPointRaw = yakuInfo.AgariPoints;
	if (AgariPlayerPriority == -1) AgariPlayerPriority = gameStat->CurrentPlayer.Agari;
	if (!ResultDesc.empty()) ResultDesc += _T("\n");
	CodeConv::tstring tmpResultDesc;
	switch (gameStat->playerwind(gameStat->CurrentPlayer.Agari)) {
		case sEast:  tmpResultDesc += _T("����"); break;
		case sSouth: tmpResultDesc += _T("���"); break;
		case sWest:  tmpResultDesc += _T("����"); break;
		case sNorth: tmpResultDesc += _T("�k��"); break;
	}
	if (gameStat->TsumoAgariFlag) {
		tmpResultDesc += _T("�̃c���a����");
	} else {
		switch (gameStat->playerwind(gameStat->CurrentPlayer.Furikomi)) {
			case sEast:  tmpResultDesc += _T("�����Ƃ��烍���a����"); break;
			case sSouth: tmpResultDesc += _T("����Ƃ��烍���a����"); break;
			case sWest:  tmpResultDesc += _T("�����Ƃ��烍���a����"); break;
			case sNorth: tmpResultDesc += _T("���k�Ƃ��烍���a����"); break;
		}
	}
	ResultDesc += tmpResultDesc;
	writeChat(tmpResultDesc);
	/* TODO: �����p�~���邩�̌��� statmes tmpResultDesc */
	mihajong_graphic::ui::WaitUIWithTimeout(1500);
	std::uint16_t tmpDoraPointer = origDoraPointer;
	const int AlicePointer = tmpDoraPointer - yakuInfo.AliceDora * 2 - 2;

	if (gameStat->statOfAgari().MenzenFlag && RuleData::chkRuleApplied("alice")) { // �߂����Ă�������
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneAlice); // �\��
		while (gameStat->DoraPointer > AlicePointer) {
			gameStat->DoraPointer -= 2;
			sound::Play(sound::IDs::sndMekuri);
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			mihajong_graphic::ui::WaitUIWithTimeout(1200);
		}
		gameStat->DoraPointer = tmpDoraPointer;
		tmpAliceFlag = true;
	}

	sound::util::bgmstop();

	transfer::resetDelta();
	if (gameStat->playerwind(gameStat->CurrentPlayer.Agari) == sEast)
		OyaAgari = gameStat->CurrentPlayer.Agari; // �e�̘a����
	LNum agariPoint;
	calcAgariPoints(gameStat, agariPoint, AgariPointRaw, transfer::getDelta(), -1);
	assert(agariPoint > (LNum)0);
	calculateWaremeDelta(gameStat);
	int ChipAmount;
	agariscrproc(gameStat, &yakuInfo, &agariPoint, ChipAmount, ResultDesc, tmpUraFlag); /* �a����� */
	gameStat->statOfAgari().YakitoriFlag = false; // �Ă����t���O�����낷
	/*if (gameStat->statOfAgari().MenzenFlag && RuleData::chkRuleApplied("alice"))
		gameStat->DoraPointer = AlicePointer;*/
	transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValAgariten, 1500);
	
	if ((gameStat->Honba > 0) && RuleData::chkRuleApplied("tsumiboh_rate")) {
		calculateTsumibouDelta(gameStat);
		// ����ڂŐςݖ_���Q�{�ɂȂ�
		calculateWaremeDelta(gameStat);
		/* ��̏ꍇ */
		if (isPaoAgari(gameStat, gameStat->CurrentPlayer.Agari)) {
			PlayerID PaoPlayer = getPaoPlayer(gameStat, gameStat->CurrentPlayer.Agari);
			if (gameStat->TsumoAgariFlag) {
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if ((PaoPlayer != cnt) && (gameStat->CurrentPlayer.Agari != cnt)) {
						transfer::addDelta(gameStat->CurrentPlayer.Agari, transfer::getDelta()[cnt]);
						transfer::addDelta(cnt, -transfer::getDelta()[cnt]);
					}
				}
			} else {
				for (PlayerID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
					if ((PaoPlayer != cnt) && (gameStat->CurrentPlayer.Furikomi != cnt) && (gameStat->CurrentPlayer.Agari != cnt)) {
						transfer::addDelta(PaoPlayer, transfer::getDelta()[cnt]);
						transfer::addDelta(cnt, -transfer::getDelta()[cnt]);
					}
				}
			}
		}
		transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValTsumibou, 1500);
	}
	
	if (gameStat->Deposit) { // �����_�_����ɂ���ꍇ�ŁA�����𖞂����Ă���Ȃ�
		if ((!RuleData::chkRule("getting_deposit", "riichidori")) || gameStat->statOfAgari().RichiFlag.RichiFlag) {
			transfer::resetDelta();
			transfer::addDelta(gameStat->CurrentPlayer.Agari, gameStat->Deposit * 1000);
			transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValKyoutaku, 1500);
		}
	}
	
	chipTransfer(gameStat, mihajong_graphic::tblSubsceneCallValChip, ChipAmount);
	
	ChipAmount = 0;
	if ((yakuInfo.CoreSemiMangan + yakuInfo.BonusSemiMangan) >= 8) { // �𖞏j�V
		std::smatch vals;
		std::string limithand_bonus(RuleData::chkRule("limithand_bonus"));
		if (std::regex_match(limithand_bonus, vals, std::regex("chip(\\d+)")))
			ChipAmount = std::atoi(vals[1].str().c_str());
		else if (std::regex_match(limithand_bonus, vals, std::regex("chip_tsumo(\\d+)each_ron(\\d)")))
			ChipAmount = std::atoi(vals[gameStat->TsumoAgariFlag ? 1 : 2].str().c_str());
		else if (std::regex_match(limithand_bonus, vals, std::regex("chip_(\\d+)_each")))
			ChipAmount = std::atoi(vals[1].str().c_str());
	}
	chipTransfer(gameStat, mihajong_graphic::tblSubsceneCallValYakuman, ChipAmount);

	/* �l�n�H���k�Ƃ̕��e�������ꍇ */
	if (!gameStat->chkGameType(SanmaT)) {
		if (gameStat->playerwind(gameStat->CurrentPlayer.Furikomi) == sNorth) {
			if (std::regex_search(yakuInfo.yakuNameList, std::basic_regex<TCHAR>(_T("(^|\\r?\\n)�l�n�H(\r?\n|$)")))) {
				transfer::resetDelta();
				for (PlayerID i = 0; i < Players; ++i)
					transfer::addDelta(i, 1000);
				transfer::addDelta(gameStat->CurrentPlayer.Agari, -3000);
				transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValKitamakura, 1500);
			}
		}
	}
	return;
}

// -------------------------------------------------------------------------

/* ���a�������̏��� */
void endround::agari::endround_chonboproc(GameTable* gameStat, CodeConv::tstring& ResultDesc) {
	if (!ResultDesc.empty()) ResultDesc += _T("\n");
	CodeConv::tstring tmpResultDesc;
	switch (gameStat->playerwind(gameStat->CurrentPlayer.Agari)) {
		case sEast:  tmpResultDesc += _T("���Ƃ̃`�����{"); break;
		case sSouth: tmpResultDesc += _T("��Ƃ̃`�����{"); break;
		case sWest:  tmpResultDesc += _T("���Ƃ̃`�����{"); break;
		case sNorth: tmpResultDesc += _T("�k�Ƃ̃`�����{"); break;
	}
	ResultDesc += tmpResultDesc + _T("�ł�");
	writeChat(tmpResultDesc);
	/* TODO: �����p�~���邩�̌��� statmes tmpResultDesc */
	mihajong_graphic::ui::WaitUIWithTimeout(1500);
	sound::util::bgmplay(sound::IDs::musRyuukyoku);
	/* �냍���܂��͌�c�� */
	if ((gameStat->AgariSpecialStat == 0) || (gameStat->AgariSpecialStat == 1)) {
		if ((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong"))
			ResultDesc += _T("(�喾�Ȃ̗��v�ł̘a����)"); // �喾�Ȃ̗��J�ԋ֎~���[���̏ꍇ
		else if ((!gameStat->statOfAgari().RichiFlag.RichiFlag) && RuleData::chkRuleApplied("riichi_shibari"))
			ResultDesc += _T("(�_�}���ł̘a����)"); // ��������ŗ������ĂȂ��Ȃ���a
		else if (!isTenpai(gameStat, gameStat->CurrentPlayer.Agari))
			ResultDesc += _T("(�������Ă��Ȃ��a����)"); // �a���肪�������Ă��Ȃ��ꍇ���a
		else {
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Agari);
			MachihaiInfo machiInfo = chkFuriten(gameStat, gameStat->CurrentPlayer.Agari);
			if (machiInfo.FuritenFlag || gameStat->statOfAgari().DoujunFuriten)
				ResultDesc += _T("(�U���ł̃����a����)"); // �U���Ȃ̂Ƀ��������ꍇ�͍��a
			else if (yakuInfo.CoreHan == 0)
				ResultDesc += _T("(���̂Ȃ��a����)"); // �����Ȃ������ꍇ�͍��a
			else if ((yakuInfo.CoreHan == 1) && gameStat->ShibariFlag)
				ResultDesc += _T("(���ʂ����Ȃ��a����)"); // ���ʔ���łP�ʂ����Ȃ��Ȃ���a
			else if (gameStat->statOfAgari().AgariHouki || (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Agari].RemotePlayerFlag == -1))
				ResultDesc += _T("(�a��������K�p��)"); // �a��������Ȃ̂ɘa���낤�Ƃ���
		}
		sound::Play(sound::IDs::sndCuohu);
	}
	/* �냍���A��c���ȊO�̍��a */
	else {
		if (gameStat->AgariSpecialStat == agariKuikae)
			ResultDesc += _T("(�򂢑ւ�)"); // �򂢑ւ��������Ƃ�
		sound::Play(sound::IDs::sndPage);
	}
	mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneChonbo);
	mihajong_graphic::ui::WaitUIWithTimeout(5000);

	transferChonboPenalty(gameStat, gameStat->CurrentPlayer.Agari);
	return;
}
