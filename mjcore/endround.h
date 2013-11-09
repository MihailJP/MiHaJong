#pragma once

#include "endtype.h"
#include "gametbl.h"
#include "largenum.h"

namespace endround {
	EndType checkroundabort(GameTable* gameStat); // 局終了条件の判定
	void endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba);
	void transferChonboPenalty(GameTable* gameStat, PlayerID targetPlayer);
	bool nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn); // 次の局へ(終了する場合はtrue)

	namespace transfer {
		InfoByPlayer<LNum>& getDelta();
		void resetDelta();
		void addDelta(PlayerID player, const LNum& deltaVal);
		void negateDelta();
		void doubleDelta(PlayerID player);
		void doubleDelta();
		void transferPoints(GameTable* gameStat, unsigned subscene, unsigned wait);
		void transferChip(GameTable* gameStat, unsigned subscene, unsigned wait);
	}
}
