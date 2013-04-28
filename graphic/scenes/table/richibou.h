#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::RichibouReconst {
private:
	GameTableScreen* caller;
private:
	static const unsigned int RiichiPosH = TableSize / 2;
	static const unsigned int RiichiPosV = (TableSize / 2) + 86;
	TexturePtr tRichi;  // リーチ棒
	void ShowRiichibou(const GameTable* gameStat); // リーチ棒の再構築
public:
	void Render();
	explicit RichibouReconst(GameTableScreen* parent);
	~RichibouReconst();
};

}
