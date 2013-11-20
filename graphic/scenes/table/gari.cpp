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
		{flowerFlag->Spring, Spring       }, {flowerFlag->Summer, Summer},
		{flowerFlag->Autumn, Autumn       }, {flowerFlag->Winter, Winter},
		{flowerFlag->Plum,   Plum         }, {flowerFlag->Orchid, Orchid},
		{flowerFlag->Chrys,  Chrysanthemum}, {flowerFlag->Bamboo, Bamboo},
	};
	int numOfTiles = 0, currentTile = 0;
	for (int i = 0; i < 8; ++i)
		if (flowerParam[i].flag)
			++numOfTiles;

	// 表示処理
	switch (playerRelative(playerID, gameStat->PlayerID)) {
	case sOpposite:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(i, flowerParam[i].tileCode, Normal,
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				GariPosV(),
				UpsideDown, Obverse);
			else
				TileTexture->DelTile(i);
		break;
	case sLeft:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(i + 8, flowerParam[i].tileCode, Normal,
				GariPosV(),
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				Clockwise, Obverse);
			else
				TileTexture->DelTile(i + 8);
		break;
	case sRight:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(23 - i, flowerParam[i].tileCode, Normal,
				TableSize - GariPosV(),
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				Withershins, Obverse);
			else
				TileTexture->DelTile(23 - i);
		break;
	case sSelf:
		for (int i = 0; i < 8; ++i)
			if (flowerParam[i].flag)
				TileTexture->NewTile(i + 24, flowerParam[i].tileCode, Normal,
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				TableSize - GariPosV(),
				Portrait, Obverse);
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
	case sOpposite:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(i, NorthWind, Normal,
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				GariPosV(),
				UpsideDown, Obverse);
			else
				TileTexture->DelTile(i);
		break;
	case sLeft:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(i + 4, NorthWind, Normal,
				GariPosV(),
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				Clockwise, Obverse);
			else
				TileTexture->DelTile(i + 4);
		break;
	case sRight:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(11 - i, NorthWind, Normal,
				TableSize - GariPosV(),
				TableSize - (GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++))),
				Withershins, Obverse);
			else
				TileTexture->DelTile(11 - i);
		break;
	case sSelf:
		for (int i = 0; i < 4; ++i)
			if (i < numOfTiles)
				TileTexture->NewTile(i + 12, NorthWind, Normal,
				GariPosH() - ShowTile::VertTileWidth * (numOfTiles - (currentTile++)),
				TableSize - GariPosV(),
				Portrait, Obverse);
			else
				TileTexture->DelTile(i + 12);
		break;
	}
}

void GameTableScreen::GariReconst::Render() {
	for (PlayerID i = 0; i < Players; ++i)
		if (GameStatus::gameStat()->chkGameType(AllSanma))
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
