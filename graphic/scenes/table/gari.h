#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::GariReconst {
private:
	GameTableScreen* caller;
//private: /**** ”²‚«–kE‰Ô”v ****/
public:
	void Render();
	explicit GariReconst(GameTableScreen* parent);
	~GariReconst();
};

}
