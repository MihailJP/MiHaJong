#ifndef FUNC_H
#define FUNC_H

#include <array>
#include <cstdint>
#include "gametbl.h"

typedef std::array<uint8_t, TILE_CODE_MAXIMUM> tileCountTbl;

class TileCount { // Žè”v‚É‚ ‚éŽí—Þ•Ê‚Ì”v‚Ì”
public:
	tileCountTbl count;
	TileCount() {
		this->count.fill(0);
	}
};

TileCount countTilesInHand(GameTable gameStat, PLAYER_ID playerID);

#endif
