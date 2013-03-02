#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::GariReconst {
private:
	GameTableScreen* caller;
	ShowTile* TileTexture;
private: /**** �����k�E�Ԕv ****/
	unsigned int GariPosH() {return TableSize - 200;}
	unsigned int GariPosV() {return DeckPosV - 80;}
	void RenderFlower(PlayerID playerID);
public:
	void Render();
	explicit GariReconst(GameTableScreen* parent);
	~GariReconst();
};

}
