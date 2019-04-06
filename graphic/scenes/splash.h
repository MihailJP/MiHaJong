#pragma once

#include "proto.h"
#include "../matrix.h"

namespace mihajong_graphic {

class SplashScreen : public Scene {
private:
	static TexturePtr tBackground;
	TransformMatrix myMatrix;
public:
	static void LoadTexture(DevicePtr device);
	static void DisposeTexture();
	SplashScreen(ScreenManipulator* const manipulator);
	SplashScreen(const SplashScreen&) = delete; // Delete unexpected copy constructor
	SplashScreen& operator= (const SplashScreen&) = delete; // Delete unexpected assign operator
	~SplashScreen();
	void Render();
};

}
