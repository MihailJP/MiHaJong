#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::RichibouReconst {
private:
	GameTableScreen* caller;
private:
	static const unsigned int RiichiPosH = TableSize / 2;
	static const unsigned int RiichiPosV = (TableSize / 2) + 86;
	TexturePtr tRichi;  // ÉäÅ[É`ñ_
	void ShowRiichibou(const GameTable* gameStat); // ÉäÅ[É`ñ_ÇÃçƒç\íz
public:
	void Render();
	explicit RichibouReconst(GameTableScreen* parent);
	RichibouReconst(const RichibouReconst&) = delete; // Delete unexpected copy constructor
	RichibouReconst& operator= (const RichibouReconst&) = delete; // Delete unexpected assign operator
	~RichibouReconst();
};

}
