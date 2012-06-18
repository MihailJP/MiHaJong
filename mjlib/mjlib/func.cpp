#include "func.h"

TileCount countTilesInHand(GameTable gameStat, PLAYER_ID playerID) {
	// 手牌に存在する牌を種類別にカウントする（鳴き面子・暗槓は除く）
	TileCount count = TileCount(); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = tileCode(gameStat.Hand[handTilePage::tlCode][playerID][i])) != tileCode::NoTile)
			count.count[tmpTC]++;
	}
	return TileCount(count);
}
