#include "gari.h"
#include "../../gametbl.h"
#include "../../utils.h"

namespace mihajong_graphic {

using utils::playerRelative;

void GameTableScreen::GariReconst::RenderFlower(PlayerID playerID) {
	const GameTable* const gameStat = GameStatus::gameStat();
	const Flowers* const flowerFlag = &(gameStat->Player[playerID].FlowerFlag);

	// 判定に使うパラメータ
	const struct {const bool& flag; TileCode tileCode;} flowerParam[8] = {
		{flowerFlag->Spring, TileCode::spring       }, {flowerFlag->Summer, TileCode::summer},
		{flowerFlag->Autumn, TileCode::autumn       }, {flowerFlag->Winter, TileCode::winter},
		{flowerFlag->Plum,   TileCode::plum         }, {flowerFlag->Orchid, TileCode::orchid},
		{flowerFlag->Chrys,  TileCode::chrysanthemum}, {flowerFlag->Bamboo, TileCode::bamboo},
	};
	int numOfTiles = 0, currentTile = 0;
	for (int i = 0; i < 8; ++i)
		if (flowerParam[i].flag)
			++numOfTiles;

	// 表示処理
	switch (playerRelative(playerID, gameStat->PlayerID)) {
	case SeatRelative::opposite:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(i, Tile(flowerParam[i].tileCode),
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				GariPosV(),
				TileDirection::upsideDown, TileSide::obverse);
			else
				TileTexture->DelTile(i);
		break;
	case SeatRelative::left:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(i + 8, Tile(flowerParam[i].tileCode),
				GariPosV(),
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				TileDirection::clockwise, TileSide::obverse);
			else
				TileTexture->DelTile(i + 8);
		break;
	case SeatRelative::right:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(23 - i, Tile(flowerParam[i].tileCode),
				TableSize - GariPosV(),
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				TileDirection::withershins, TileSide::obverse);
			else
				TileTexture->DelTile(23 - i);
		break;
	case SeatRelative::self:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(i + 24, Tile(flowerParam[i].tileCode),
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				TableSize - GariPosV(),
				TileDirection::portrait, TileSide::obverse);
			else
				TileTexture->DelTile(i + 24);
		break;
	}
}

void GameTableScreen::GariReconst::RenderNorth(PlayerID playerID) {
	const GameTable* const gameStat = GameStatus::gameStat();

	// 判定に使うパラメータ
	int numOfTiles = gameStat->Player[playerID].NorthFlag, currentTile = 0;

	// 表示処理
	switch (playerRelative(playerID, gameStat->PlayerID)) {
	case SeatRelative::opposite:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(i, Tile(TileCode::northWind),
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				GariPosV(),
				TileDirection::upsideDown, TileSide::obverse);
			else
				TileTexture->DelTile(i);
		break;
	case SeatRelative::left:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(i + 4, Tile(TileCode::northWind),
				GariPosV(),
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				TileDirection::clockwise, TileSide::obverse);
			else
				TileTexture->DelTile(i + 4);
		break;
	case SeatRelative::right:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(11 - i, Tile(TileCode::northWind),
				TableSize - GariPosV(),
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				TileDirection::withershins, TileSide::obverse);
			else
				TileTexture->DelTile(11 - i);
		break;
	case SeatRelative::self:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(i + 12, Tile(TileCode::northWind),
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				TableSize - GariPosV(),
				TileDirection::portrait, TileSide::obverse);
			else
				TileTexture->DelTile(i + 12);
		break;
	}
}

void GameTableScreen::GariReconst::Render() {
	for (PlayerID i = 0; i < Players; ++i)
		if (GameStatus::gameStat()->chkGameType(GameTypeID::allSanma))
			RenderNorth(i);
		else
			RenderFlower(i);
	TileTexture->Render();
}

GameTableScreen::GariReconst::GariReconst(GameTableScreen* parent) {
	caller = parent;
	TileTexture = new ShowTile(parent->caller->getDevice());
}

GameTableScreen::GariReconst::~GariReconst() {
	delete TileTexture;
}

}
