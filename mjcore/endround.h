#pragma once

#include "endtype.h"
#include "gametbl.h"
#include "largenum.h"

namespace endround {
	EndType checkroundabort(GameTable* gameStat); // ‹ÇI—¹ğŒ‚Ì”»’è
	void endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba);
	void transferChonboPenalty(GameTable* gameStat, PlayerID targetPlayer);
	bool nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn); // Ÿ‚Ì‹Ç‚Ö(I—¹‚·‚éê‡‚Ítrue)

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
