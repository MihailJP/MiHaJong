#pragma once

#include "sysscr.h"

namespace mihajong_graphic {

class ResultScreen : public SystemScreen {
public:
	ResultScreen(ScreenManipulator* const manipulator);
	~ResultScreen();
	void Render();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
};

}
