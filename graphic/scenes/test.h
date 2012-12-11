#pragma once

#include "proto.h"
#include "../editbox.h"

namespace mihajong_graphic {

class TestScene : public Scene {
private:
	EditBox* myEditBox;
public:
	TestScene(ScreenManipulator* const manipulator);
	~TestScene();
	void Render();
	void KeyboardInput(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
};

}
