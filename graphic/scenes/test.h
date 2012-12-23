#pragma once

#include "proto.h"
#include "../logwnd.h"

namespace mihajong_graphic {

class TestScene : public Scene {
private:
	logwnd::LogWindow* myLogWindow;
public:
	TestScene(ScreenManipulator* const manipulator);
	~TestScene();
	void Render();
};

}
