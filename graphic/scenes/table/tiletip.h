#pragma once

#include "../table.h"
#include "../../text.h"

namespace mihajong_graphic {

class GameTableScreen::TileTipReconst {
private:
	GameTableScreen* caller;
	TextRenderer* myTextRenderer;
	static const int TipX = (TableSize - ShowTile::VertTileWidth * 13) / 2;
	static const int TipY = TableSize - DeckPosV + 60;
public:
	explicit TileTipReconst(GameTableScreen* parent);
	~TileTipReconst();
	void reconstruct();
	void Render();
};

}
