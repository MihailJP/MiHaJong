#ifdef _MSC_VER
#define _USE_MATH_DEFINES /* required for M_PI by MS VC++ */
#endif
#include <cmath>

#include "tehai0.h"
#include "../../utils.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* éËîvÇï\é¶Ç∑ÇÈ */
void ShowTehai::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer,
	std::function<std::tuple<int, int> (seatRelative)> coordFunc,
	seatRelative direction, std::function<D3DCOLOR (int)> colorFunc,
	std::function<void (const int*, const int*, int)> regionFunc)
{
	int tilePos, x, y;
	std::tie(x, y) = coordFunc(direction);
	/* éËîv */
	switch (direction) {
	case sOpposite: /* ëŒñ ÇÃéËîv */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(i,
				gameStat->Player[targetPlayer].Hand[i].tile,
				gameStat->Player[targetPlayer].Hand[i].red,
				x + ShowTile::VertTileWidth * (HandLength - (tilePos++)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				y, UpsideDown, Obverse);
			else TileTexture->DelTile(i);
		break;
	case sLeft: /* è„â∆ÇÃéËîv */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(i + NumOfTilesInHand,
				gameStat->Player[targetPlayer].Hand[i].tile,
				gameStat->Player[targetPlayer].Hand[i].red,
				x,
				y + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Clockwise, Obverse);
			else TileTexture->DelTile(i + NumOfTilesInHand);
		break;
	case sRight: /* â∫â∆ÇÃéËîv */
		tilePos = 0;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player[targetPlayer].Hand[i].tile != NoTile)
				++tilePos;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile((NumOfTilesInHand - 1 - i) + NumOfTilesInHand * 2,
				gameStat->Player[targetPlayer].Hand[i].tile,
				gameStat->Player[targetPlayer].Hand[i].red,
				x,
				y + ShowTile::VertTileWidth * (HandLength - (--tilePos)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Withershins, Obverse);
			else TileTexture->DelTile((NumOfTilesInHand - 1 - i) + NumOfTilesInHand * 2);
		break;
	case sSelf: /* é©ï™ÇÃéËîv */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i) {
			if (gameStat->Player[targetPlayer].Hand[i].tile != NoTile) {
				const D3DCOLOR tileColor = colorFunc(i);
				const int tileX = x + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0);
				const int tileY = y;
				TileTexture->NewTile(i + NumOfTilesInHand * 3,
					gameStat->Player[targetPlayer].Hand[i].tile,
					gameStat->Player[targetPlayer].Hand[i].red,
					tileX, tileY, Portrait, Obverse, tileColor);
				regionFunc(&tileX, &tileY, i);
			} else {
				TileTexture->DelTile(i + NumOfTilesInHand * 3);
				regionFunc(nullptr, nullptr, i);
			}
		}
		break;
	}
}

ShowTehai::ShowTehai(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	TileTexture = new ShowTile(device);
}

ShowTehai::~ShowTehai() {
	delete TileTexture;
}

}
