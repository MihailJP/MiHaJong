#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::TileTipReconst {
private:
	GameTableScreen* caller;
public:
	explicit TileTipReconst(GameTableScreen* parent);
	~TileTipReconst();
};

}
