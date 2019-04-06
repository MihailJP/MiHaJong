#include "splash.h"
#include "../scrmanip.h"
#include "../loadtex.h"
#include "../sprite.h"
#include "../resource.h"
#include "../geometry.h"

namespace mihajong_graphic {

TexturePtr SplashScreen::tBackground = (TexturePtr)0;

void SplashScreen::LoadTexture(DevicePtr device) {
	if (!tBackground)
		mihajong_graphic::LoadTexture(device, &tBackground, MAKEINTRESOURCE(IDB_PNG_SPLASH_SCREEN));
}
void SplashScreen::DisposeTexture() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (tBackground) tBackground->Release();
#endif
	tBackground = (TexturePtr)0;
}

SplashScreen::SplashScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LoadTexture(caller->getDevice());
#ifdef WITH_DIRECTX
	myMatrix = getMatrix();
#else /* WITH_DIRECTX */
	myMatrix = getMatrix(
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f / Geometry::WindowScale(),
		1.0f / Geometry::WindowScale()
	);
#endif /* WITH_DIRECTX */
}

SplashScreen::~SplashScreen() {
}

void SplashScreen::Render() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
	constexpr RECT rect = {0, 0, 1920, 1080};
	const auto xPos = static_cast<int>((static_cast<float>(Geometry::WindowWidth) * Geometry::WindowScale() - 1080.0f) / 2.0f);
#else
	const auto width = static_cast<int>(static_cast<float>(Geometry::WindowWidth) / Geometry::WindowScale());
	const RECT rect = {(1920 - width) / 2, 0, (1920 - width) / 2 + width, 1080};
	constexpr int xPos = 0;
#endif
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBackground, xPos, 0,
		Geometry::WindowWidth, Geometry::WindowHeight, 0xffffffff, &rect, 0, 0, &myMatrix);
}

}
