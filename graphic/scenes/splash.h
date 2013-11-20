#pragma once

#include "proto.h"

namespace mihajong_graphic {

class SplashScreen : public Scene {
public:
	SplashScreen(ScreenManipulator* const manipulator);
	SplashScreen(const SplashScreen&) = delete; // Delete unexpected copy constructor
	SplashScreen& operator= (const SplashScreen&) = delete; // Delete unexpected assign operator
	~SplashScreen();
	void Render();
};

}
