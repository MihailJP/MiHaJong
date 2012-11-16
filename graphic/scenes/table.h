#ifndef GRAPHIC_SCENES_TABLE_H
#define GRAPHIC_SCENES_TABLE_H

#include "game.h"

class GameTableScreen : public TableProtoScene {
private:
	static const unsigned int TableSize = 768;
	static const unsigned int TileThickness = ShowTile::HoriTileHeight - ShowTile::VertTileWidth;
	static const unsigned int DeckChainLength = 17;
	static const unsigned int DeckPosH = (TableSize - ShowTile::VertTileWidth * (DeckChainLength - 1)) / 2;
	static const unsigned int DeckPosV = 125;
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // ‘ì‚Ì˜g
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // —…ŽÑ’n
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

#endif
