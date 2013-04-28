#include "splash.h"
#include "../scrmanip.h"

namespace mihajong_graphic {

SplashScreen::SplashScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
}

SplashScreen::~SplashScreen() {
}

void SplashScreen::Render() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
#endif
}

}
