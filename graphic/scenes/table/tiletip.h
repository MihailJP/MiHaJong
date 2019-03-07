#pragma once

#include "../table.h"
#include "../../text.h"
#include "../../showtile.h"

namespace mihajong_graphic {

class GameTableScreen::TileTipReconst {
private:
	GameTableScreen* caller;
	TextRenderer* myTextRenderer;
	ShowTile* myTileRenderer;
	static constexpr int TipX = (TableSize - ShowTile::VertTileWidth * 13) / 2;
	static constexpr int TipY = TableSize - DeckPosV + 60;
public:
	explicit TileTipReconst(GameTableScreen* parent);
	TileTipReconst(const TileTipReconst&) = delete; // Delete unexpected copy constructor
	TileTipReconst& operator= (const TileTipReconst&) = delete; // Delete unexpected assign operator
	~TileTipReconst();
	void reconstruct();
	void Render();
};

}
