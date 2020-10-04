#ifdef _MSC_VER
#define _USE_MATH_DEFINES /* required for M_PI by MS VC++ */
#endif
#include <cmath>

#include "tehai0.h"
#include "../../utils.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* 手牌を表示する */
void ShowTehai::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer,
	std::function<std::tuple<int, int> (SeatRelative)> coordFunc,
	SeatRelative direction, std::function<ArgbColor (int)> colorFunc,
	std::function<void (const int*, const int*, int)> regionFunc)
{
	int tilePos, x, y;
	std::tie(x, y) = coordFunc(direction);
	/* 手牌 */
	TileSide tileStat =
		(gameStat->Player[targetPlayer].HandStat == handUpright) ? TileSide::upright :
		(gameStat->Player[targetPlayer].HandStat == handHidden) ? TileSide::reverse : TileSide::obverse;
	switch (direction) {
	case SeatRelative::opposite: /* 対面の手牌 */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player[targetPlayer].Hand[i])
				TileTexture->NewTile(i,
				gameStat->Player[targetPlayer].Hand[i],
				x + ShowTile::VertTileWidth * (HandLength - (tilePos++)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				y, TileDirection::upsideDown, tileStat);
			else TileTexture->DelTile(i);
		break;
	case SeatRelative::left: /* 上家の手牌 */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player[targetPlayer].Hand[i])
				TileTexture->NewTile(i + NumOfTilesInHand,
				gameStat->Player[targetPlayer].Hand[i],
				x,
				y + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				TileDirection::clockwise, tileStat);
			else TileTexture->DelTile(i + NumOfTilesInHand);
		break;
	case SeatRelative::right: /* 下家の手牌 */
		tilePos = 0;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player[targetPlayer].Hand[i])
				++tilePos;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player[targetPlayer].Hand[i])
				TileTexture->NewTile((NumOfTilesInHand - 1 - i) + NumOfTilesInHand * 2,
				gameStat->Player[targetPlayer].Hand[i],
				x,
				y + ShowTile::VertTileWidth * (HandLength - (--tilePos)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				TileDirection::withershins, tileStat);
			else TileTexture->DelTile((NumOfTilesInHand - 1 - i) + NumOfTilesInHand * 2);
		break;
	case SeatRelative::self: /* 自分の手牌 */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i) {
			if (gameStat->Player[targetPlayer].Hand[i]) {
				const ArgbColor tileColor = colorFunc(i);
				const int tileX = x + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0);
				const int tileY = y;
				TileTexture->NewTile(i + NumOfTilesInHand * 3,
					gameStat->Player[targetPlayer].Hand[i],
					tileX, tileY, TileDirection::portrait, tileStat, tileColor);
				regionFunc(&tileX, &tileY, i);
			} else {
				TileTexture->DelTile(i + NumOfTilesInHand * 3);
				regionFunc(nullptr, nullptr, i);
			}
		}
		break;
	}
}

ShowTehai::ShowTehai(DevicePtr device) {
	myDevice = device;
	TileTexture = new ShowTile(device);
}

ShowTehai::~ShowTehai() {
	delete TileTexture;
}

}
