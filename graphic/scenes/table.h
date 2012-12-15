#pragma once

#include "game.h"
#include "../geometry.h"

namespace mihajong_graphic {

class GameTableScreen : public TableProtoScene {
private:
	static const unsigned int TableSize = Geometry::BaseSize;
	static const unsigned int TileThickness = ShowTile::HoriTileHeight - ShowTile::VertTileWidth;
	static const unsigned int DeckChainLength = 17;
	static const unsigned int DeckPosH = (TableSize - ShowTile::VertTileWidth * (DeckChainLength - 1)) / 2;
	static const unsigned int DeckPosV = (TableSize / 2) - 300;
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // ‘ì‚Ì˜g
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // —…Ñ’n
	void ReconstructYamahai(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // R”v‚ÌÄ\’z
	void ReconstructTehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // è”v‚ÌÄ\’z
	void ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // ƒvƒŒƒCƒ„[ŠÖŒW‚Ì‰æ–Ê‚ÌÄ\’z
	void Reconstruct(const GameTable* gameStat); // ‰æ–Ê‚ÌÄ•`‰æ
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

}
