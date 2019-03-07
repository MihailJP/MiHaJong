#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::GariReconst {
private:
	GameTableScreen* caller;
	ShowTile* TileTexture;
private: /**** 抜き北・花牌 ****/
	constexpr unsigned int GariPosH() {return TableSize - 200;}
	constexpr unsigned int GariPosV() {return DeckPosV - 80;}
	void RenderFlower(PlayerID playerID);
	void RenderNorth(PlayerID playerID);
public:
	void Render();
	explicit GariReconst(GameTableScreen* parent);
	GariReconst(const GariReconst&) = delete; // Delete unexpected copy constructor
	GariReconst& operator= (const GariReconst&) = delete; // Delete unexpected assign operator
	~GariReconst();
};

}
