#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::GariReconst {
private:
	GameTableScreen* caller;
	ShowTile* TileTexture;
private: /**** î≤Ç´ñkÅEâ‘îv ****/
	unsigned int GariPosH() {return TableSize - 176;}
	unsigned int GariPosV() {return DeckPosV - 200;}
	void RenderFlower(PlayerID playerID);
public:
	void Render();
	explicit GariReconst(GameTableScreen* parent);
	~GariReconst();
};

}
