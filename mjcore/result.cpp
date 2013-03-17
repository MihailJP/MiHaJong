#include "result.h"
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
		PlayerRankList rank(calcRank(gameStat));
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
		/* TODO: �����Ɋۂߏ��������� */
		return val / 1000;
	}

	/* �E�}�v�Z */
	void calcUma(const GameTable* gameStat, InfoByPlayer<LNum>& score) {
		PlayerRankList rank(calcRank(gameStat));
		int plusCount = 0;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			if (isAboveBase(gameStat, i))
				++plusCount;
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i)
			score[i] += RankVal::Instantiate()->getRankVal(gameStat,
			RuleData::chkRule("point_basis"), plusCount, rank[i]) * 1000;
	}

	/* �I�J�v�Z */
	void calcOka(const GameTable* gameStat, InfoByPlayer<LNum>& score) {
		PlayerRankList rank(calcRank(gameStat));
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

	/* ���_�v�Z */
	void calcScore(const GameTable* gameStat) {
		InfoByPlayer<LNum> playerScore;
		for (PlayerID i = 0; i < Players; ++i)
			playerScore[i] = gameStat->Player[i].PlayerScore - BasePoint();
		calcUma(gameStat, playerScore); // �E�}�����Z����
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; ++i) // �ۂߏ���
			playerScore[i] = roundScore(playerScore[i]);
		/* TODO: �`�b�v�̔��f */
		calcOka(gameStat, playerScore); // �I�J�����Z����
		mihajong_graphic::setFinalScore( // �\�������pDLL�ɓn��
			playerScore[0], playerScore[1], playerScore[2], playerScore[3]);
	}
}

void gameResult(GameTable* gameStat, int origTurn, int origHonba) {
	sound::util::bgmstop(); /* TODO: BGM�I�� */
	withdrawDepoScore(gameStat); // �����_�_�̏���
	mihajong_graphic::GameStatus::updateGameStat(gameStat); // ���f������
	calcScore(gameStat);
	/*  */
	mihajong_graphic::Transit(mihajong_graphic::sceneResult);
	haifu::haifusave(gameStat); // �v����ۑ�����
	/* TODO: �I�����̏��� */
	/* TODO: ���͑҂�
	buttonPressed = 0
	repeat
		redrscreen
		await 1000
		if (buttonPressed) {break}
	loop
	*/
	Sleep(INFINITE);
}
