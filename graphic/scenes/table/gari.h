#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::GariReconst {
private:
	GameTableScreen* caller;
//private: /**** �����k�E�Ԕv ****/
public:
	void Render();
	explicit GariReconst(GameTableScreen* parent);
	~GariReconst();
};

}
