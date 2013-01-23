#pragma once

#include "endtype.h"
#include "gametbl.h"

namespace endround {
	EndType checkroundabort(GameTable* gameStat); // �ǏI�������̔���
	void endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba);
	void transferChonboPenalty(GameTable* gameStat, PLAYER_ID targetPlayer);
	bool nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn); // ���̋ǂ�(�I������ꍇ��true)

	namespace transfer {
		InfoByPlayer<LNum>& getDelta();
		void resetDelta();
		void addDelta(PLAYER_ID player, const LNum& deltaVal);
		void negateDelta();
		void transferPoints(GameTable* gameStat, unsigned subscene, unsigned wait);
		void transferChip(GameTable* gameStat, unsigned subscene, unsigned wait);
	}
}
