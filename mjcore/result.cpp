#include "result.h"
#include "regex.h"
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "sound.h"
#include "func.h"
#include "largenum.h"
#include "rankval.h"
#include "ruletbl.h"
#include "haifu.h"

namespace {

	/* �Ō�܂Ŏc���Ă��������_�_�̓g�b�v��� */
	void withdrawDepoScore(GameTable* gameStat) {
		const PlayerRankList rank(calcRank(gameStat));
		for (PlayerID i = 0; i < Players; ++i) {
			if (rank[i] == 1) {
				gameStat->Player[i].PlayerScore += gameStat->Deposit * 1000;
				gameStat->Deposit = 0;
			}
		}
		return;
	}

	/* 1000�Ŋ����Ċۂ߂� */
	LNum roundScore(LNum val) {
#ifdef GUOBIAO
		return val;
#else /* GUOBIAO */
		/* TODO: �����Ɋۂߏ��������� */
		return val / 1000;
#endif /* GUOBIAO */
	}

	/* �E�}�v�Z */
	void calcUma(const GameTable* gameStat, InfoByPlayer<LNum>& score) {
#ifndef GUOBIAO
		const PlayerRankList rank(calcRank(gameStat));
		int plusCount = 0;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isAboveBase(gameStat, i))
				++plusCount;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			score[i] += RankVal::Instantiate()->getRankVal(gameStat,
			RuleData::chkRule("point_basis"), plusCount, rank[i]) * 1000;
#endif /* GUOBIAO */
	}

	/* �I�J�v�Z */
	void calcOka(const GameTable* gameStat, InfoByPlayer<LNum>& score) {
		const PlayerRankList rank(calcRank(gameStat));
		LNum sumOfScore = 0; // �s�����܂��͉ߏ蕪
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			sumOfScore += score[i];
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) {
			if ((rank[i] == 1) && (sumOfScore <= (LNum)0)) // �g�b�v��
				score[i] -= sumOfScore;
			if ((rank[i] == ACTUAL_PLAYERS) && (sumOfScore > (LNum)0)) // �}�C�i�X�I�J
				score[i] -= sumOfScore;
		}
	}

	/* �`�b�v���[�g */
	unsigned int chipRate() {
#ifdef GUOBIAO
		return 0;
#else /* GUOBIAO */
		if (RuleData::chkRule("chip", "no")) return 0; // �`�b�v�Ȃ����[��
		const std::string chipRule(RuleData::chkRule("chip"));
		REGEX::smatch matchDat;
		if (REGEX::regex_match(chipRule, matchDat, REGEX::regex("chip_rate_(\\d+)"))) { // �`�b�v���[�g
			return atoi(matchDat[1].str().c_str()); // ���[���ݒ蕶���񂩂琮���𒊏o
		} else { // �ُ�f�[�^�H
			CodeConv::tostringstream o;
			o << _T("�F���ł��Ȃ��`�b�v���[�g�ݒ�ł� [") <<
				CodeConv::EnsureTStr(chipRule) << _T(']');
			warn(o.str().c_str());
			return 0;
		}
#endif /* GUOBIAO */
	}

	/* ���_�v�Z */
	void calcScore(const GameTable* gameStat) {
		InfoByPlayer<LNum> playerScore;
		for (PlayerID i = 0; i < Players; ++i) // �_������
			playerScore[i] = gameStat->Player[i].PlayerScore - (LargeNum)BasePoint();
		for (PlayerID i = 0; i < Players; ++i) // �`�b�v�𔽉f
			playerScore[i] += gameStat->Player[i].playerChip * (signed)chipRate();
		calcUma(gameStat, playerScore); // �E�}�����Z����
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) // �ۂߏ���
			playerScore[i] = roundScore(playerScore[i]);
		calcOka(gameStat, playerScore); // �I�J�����Z����
		mihajong_graphic::setFinalScore( // �\�������pDLL�ɓn��
			playerScore[0], playerScore[1], playerScore[2], playerScore[3]);
	}

	/* �Ă��� */
	void yakitori(GameTable* gameStat) {
#ifndef GUOBIAO
		const PlayerRankList rank(calcRank(gameStat));
		const std::string yakitoriRule(RuleData::chkRule("yakitori"));
		const PlayerID winner = [rank]() -> PlayerID {
			for (PlayerID i = 0; i < Players; ++i) // Who is the winner?
				if (rank[i] == 1)                  // Are you the winner?
					return i;                      // This is the winner!
			return -1;                             // Nobody won: this shouldn't be occur...
		} ();
		REGEX::smatch matchDat;
		if (REGEX::regex_match(yakitoriRule, matchDat, REGEX::regex("(\\d+)pts"))) { // �_�_�Ŏx�����ꍇ
			int yakitoriVal = atoi(matchDat[1].str().c_str()); // ���[���ݒ蕶���񂩂琮���𒊏o
			for (PlayerID i = 0; i < Players; ++i) {
				if (gameStat->Player[i].YakitoriFlag) {
					gameStat->Player[i].PlayerScore -= yakitoriVal;
					gameStat->Player[winner].PlayerScore += yakitoriVal;
				}
			}
		} else if (REGEX::regex_match(yakitoriRule, matchDat, REGEX::regex("chip(\\d+)"))) { // �`�b�v�Ŏx�����ꍇ
			int yakitoriVal = atoi(matchDat[1].str().c_str()); // ���[���ݒ蕶���񂩂琮���𒊏o
			for (PlayerID i = 0; i < Players; ++i) {
				if (gameStat->Player[i].YakitoriFlag) {
					gameStat->Player[i].playerChip -= yakitoriVal;
					gameStat->Player[winner].playerChip += yakitoriVal;
				}
			}
		}
#endif /* GUOBIAO */
	}

}

void gameResult(GameTable* gameStat, int origTurn, int origHonba) {
	sound::util::bgmstop();
	withdrawDepoScore(gameStat); // �����_�_�̏���
	yakitori(gameStat); // �Ă����̏���
	mihajong_graphic::GameStatus::updateGameStat(gameStat); // ���f������
	calcScore(gameStat);
	/*  */
	mihajong_graphic::Transit(mihajong_graphic::sceneResult);
	haifu::haifusave(gameStat); // �v����ۑ�����
	(void)mihajong_graphic::ui::WaitUI(); // ���͑҂�
}
