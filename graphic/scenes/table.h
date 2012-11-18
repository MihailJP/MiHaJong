#ifndef GRAPHIC_SCENES_TABLE_H
#define GRAPHIC_SCENES_TABLE_H

#include "game.h"
#include "../geometry.h"

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
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // —…ŽÑ’n
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

#endif
