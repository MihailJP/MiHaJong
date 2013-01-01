#pragma once

#include "table.h"

namespace mihajong_graphic {

class GameTableCall : public GameTableScreen {
public:
	GameTableCall(ScreenManipulator* const manipulator);
	virtual ~GameTableCall();
	virtual void Render();
};

}
