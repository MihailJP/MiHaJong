#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::RichibouReconst {
private:
	GameTableScreen* caller;
private:
	static constexpr unsigned int RiichiPosH = TableSize / 2;
	static constexpr unsigned int RiichiPosV = (TableSize / 2) + 86;
	TexturePtr tRichi;  // リーチ棒
	void ShowRiichibou(const GameTable* gameStat); // リーチ棒の再構築
public:
	void Render();
	explicit RichibouReconst(GameTableScreen* parent);
	RichibouReconst(const RichibouReconst&) = delete; // Delete unexpected copy constructor
	RichibouReconst& operator= (const RichibouReconst&) = delete; // Delete unexpected assign operator
	~RichibouReconst();
};

}
