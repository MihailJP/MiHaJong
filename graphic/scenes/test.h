#pragma once

#include "proto.h"

namespace mihajong_graphic {

class TestScene : public Scene {
public:
	TestScene(ScreenManipulator* const manipulator);
	~TestScene();
	void Render();
};

}
