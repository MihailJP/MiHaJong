#include "tehai.h"
#include "../../utils.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* è”v‚ğ•\¦‚·‚é */
void GameTableScreen::TehaiReconst::Reconstruct(const GameTable* gameStat, PLAYER_ID targetPlayer) {
	int tilePos;
	/* è”v */
	switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
	case sOpposite: /* ‘Î–Ê‚Ìè”v */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(i,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosH + ShowTile::VertTileWidth * (HandLength - (tilePos++)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				HandPosV, UpsideDown, Obverse);
			else TileTexture->DelTile(i);
		break;
	case sLeft: /* ã‰Æ‚Ìè”v */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(i + NUM_OF_TILES_IN_HAND,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosV,
				HandPosH + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Clockwise, Obverse);
			else TileTexture->DelTile(i + NUM_OF_TILES_IN_HAND);
		break;
	case sRight: /* ‰º‰Æ‚Ìè”v */
		tilePos = 0;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				++tilePos;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile((NUM_OF_TILES_IN_HAND - 1 - i) + NUM_OF_TILES_IN_HAND * 2,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				TableSize - HandPosV,
				HandPosH + ShowTile::VertTileWidth * (HandLength - (--tilePos)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Withershins, Obverse);
			else TileTexture->DelTile((NUM_OF_TILES_IN_HAND - 1 - i) + NUM_OF_TILES_IN_HAND * 2);
		break;
	case sSelf: /* ©•ª‚Ìè”v */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i) {
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile) {
				const int tileX = HandPosH + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0);
				const int tileY = TableSize - HandPosV;
				TileTexture->NewTile(i + NUM_OF_TILES_IN_HAND * 3,
					gameStat->Player.val[targetPlayer].Hand[i].tile,
					gameStat->Player.val[targetPlayer].Hand[i].red,
					tileX, tileY, Portrait, Obverse,
					(tileCursor == i) ? 0xffff9999 : 0xffffffff);
				const Region nullRegion = {0, 0, -1, -1};
				if (caller->regions.size() <= i) caller->regions.resize(i + 1, nullRegion);
				const Region newRegion = {
					tileX - ShowTile::VertTileWidth / 2, tileY - ShowTile::VertTileHeight / 2,
					tileX + ShowTile::VertTileWidth / 2, tileY + ShowTile::VertTileHeight / 2,
				};
				caller->regions[i] = newRegion;
			} else {
				TileTexture->DelTile(i + NUM_OF_TILES_IN_HAND * 3);
				if (caller->regions.size() <= i) caller->regions.resize(i + 1);
				const Region nullRegion = {0, 0, -1, -1};
				caller->regions[i] = nullRegion;
			}
		}
		break;
	}
}

void GameTableScreen::TehaiReconst::Render() {
	TileTexture->Render();
}

GameTableScreen::TehaiReconst::TehaiReconst(GameTableScreen* parent) {
	caller = parent;
	TileTexture = new ShowTile(parent->caller->getDevice());
	tileCursor = tileCursorOff;
}

GameTableScreen::TehaiReconst::~TehaiReconst() {
	delete TileTexture;
}

}
