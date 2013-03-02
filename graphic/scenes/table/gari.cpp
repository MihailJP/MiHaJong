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
	// TODO: ここを実装する
	switch (playerRelative(playerID, gameStat->PlayerID)) {
	case sOpposite:
		break;
	case sLeft:
		break;
	case sRight:
		break;
	case sSelf:
		break;
	}
}

void GameTableScreen::GariReconst::Render() {
	for (PlayerID i = 0; i < Players; ++i)
		RenderFlower(i);
}

GameTableScreen::GariReconst::GariReconst(GameTableScreen* parent) {
	caller = parent;
	TileTexture = new ShowTile(parent->caller->getDevice());
}

GameTableScreen::GariReconst::~GariReconst() {
	delete TileTexture;
}

}
