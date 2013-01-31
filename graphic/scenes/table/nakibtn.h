#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::ButtonReconst {
private:
	GameTableScreen* caller;
public:
	void Render();
	explicit ButtonReconst(GameTableScreen* parent);
	~ButtonReconst();
};

}
