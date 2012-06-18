#include "func.h"

TileCount countTilesInHand(GameTable gameStat, PLAYER_ID playerID) {
	// ��v�ɑ��݂���v����ޕʂɃJ�E���g����i���ʎq�E�ÞȂ͏����j
	TileCount count = TileCount(); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = tileCode(gameStat.Hand[handTilePage::tlCode][playerID][i])) != tileCode::NoTile)
			count.count[tmpTC]++;
	}
	return TileCount(count);
}
