#pragma once

#include "proto.h"

namespace mihajong_graphic {

class SplashScreen : public Scene {
public:
	SplashScreen(ScreenManipulator* const manipulator);
	~SplashScreen();
	void Render();
};

}
