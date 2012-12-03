#ifndef GRAPHIC_SCENES_SPLASH_H
#define GRAPHIC_SCENES_SPLASH_H

#include "proto.h"

namespace mihajong_graphic {

class SplashScreen : public Scene {
public:
	SplashScreen(ScreenManipulator* const manipulator);
	~SplashScreen();
	void Render();
};

}
#endif
