#include "sysscr.h"

#include <cassert>
#include <cmath>
#include "../gametbl.h"
#include "../resource.h"
#include "../sprite.h"
#include "../matrix.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

SystemScreen::SystemScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	caller = manipulator;
	myTextRenderer = new TextRenderer(caller->getDevice());
}

SystemScreen::~SystemScreen() {
	delete myTextRenderer;
}

void SystemScreen::clearWithGameTypeColor() {
	// バッファクリア
#if defined(_WIN32) && defined(WITH_DIRECTX)
	switch (GameStatus::gameStat()->gameType) {
	case Yonma:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 64, 0), 1.0f, 0);
		break;
	case Sanma:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 0, 64), 1.0f, 0);
		break;
	case Sanma4:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 64, 64), 1.0f, 0);
		break;
	case SanmaS:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(64, 0, 64), 1.0f, 0);
		break;
	case SanmaSeto:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(64, 64, 64), 1.0f, 0);
		break;
	case GuobiaoMJ:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(64, 64, 0), 1.0f, 0);
		break;
	default:
		assert(false); // This may not occur.
	}
#else
	switch (GameStatus::gameStat()->gameType) {
	case Yonma:
		glClearColor(0, .25, 0, 1);
		break;
	case Sanma:
		glClearColor(0, 0, .25, 1);
		break;
	case Sanma4:
		glClearColor(0, .25, .25, 1);
		break;
	case SanmaS:
		glClearColor(.25, 0, .25, 1);
		break;
	case SanmaSeto:
		glClearColor(.25, .25, .25, 1);
		break;
	case GuobiaoMJ:
		glClearColor(.25, .25, 0, 1);
		break;
	default:
		assert(false); // This may not occur.
	}
	glClear(GL_COLOR_BUFFER_BIT);
#endif
}

unsigned SystemScreen::strwidth(const std::wstring& str) {
	return myTextRenderer->strWidthByCols(str);
}
unsigned SystemScreen::strwidth(const std::string& str) {
	return myTextRenderer->strWidthByCols(str);
}

// -------------------------------------------------------------------------

#if defined(_WIN32) && defined(WITH_DIRECTX)
TexturePtr SystemScreen::Background::texture = nullptr;
#else
TexturePtr SystemScreen::Background::texture = 0;
#endif
std::map<ScreenManipulator*, Timer> SystemScreen::Background::timers;

void SystemScreen::Background::LoadTexture(DevicePtr device) {
	if (!texture)
		mihajong_graphic::LoadTexture(device, &texture, MAKEINTRESOURCE(IDB_PNG_TITLE_BACKGROUND));
}
void SystemScreen::Background::DisposeTexture() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (texture) texture->Release();
#endif
}

SystemScreen::Background::Background(SystemScreen* const parent) {
	caller = parent;
	myDevice = parent->caller->getDevice();
	LoadTexture(parent->caller->getDevice());
}
SystemScreen::Background::~Background() {
}
RECT SystemScreen::Background::getRect() {
	const int width = static_cast<int>(static_cast<float>(Geometry::WindowWidth) / Geometry::WindowScale());
	const int y = MainPosY;
	const int x = static_cast<int>(((std::sin(static_cast<double>(timer().elapsed() + 8'000'000uLL) / 10'000'000.0) + 1.0) / 2.0) * static_cast<double>(PictureWidth - width));
	const RECT rect = {x, y, x + width, y + Geometry::BaseSize};
	return rect;
}
void SystemScreen::Background::show() {
#ifdef WITH_DIRECTX
	const TransformMatrix matrix(getMatrix());
#else /* WITH_DIRECTX */
	const TransformMatrix matrix(getMatrix(
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f / Geometry::WindowScale(),
		1.0f / Geometry::WindowScale()
	));
#endif /* WITH_DIRECTX */
	const int alpha = (timer().elapsed() >= 1'000'000uLL) ? 255 : (static_cast<int>(timer().elapsed()) * 255) / 1'000'000uLL;
	const RECT rect(getRect());
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, 0, 0,
		Geometry::WindowWidth, Geometry::WindowHeight, (alpha << 24) | 0xffffff, &rect, 0, 0, &matrix);
}
void SystemScreen::Background::skipIntoLoop() {
	timer().skipTo(60u * timePerFrame);
}

// -------------------------------------------------------------------------

SystemScreen::TitleBackground::TitleBackground(SystemScreen* const parent, bool cutIn) : Background(parent) {
	LoadTexture(parent->caller->getDevice());
	if (cutIn)
		timers[parent->caller].skipTo(1'000'000uLL);
	else
		timers[parent->caller].skipTo(0);
}
SystemScreen::TitleBackground::~TitleBackground() {
}
RECT SystemScreen::TitleBackground::getRect() {
	const int width = static_cast<int>(static_cast<float>(Geometry::WindowWidth) / Geometry::WindowScale());
	const int y = (timer().elapsed() >= 1'000'000uLL)
		? static_cast<int>(MainPosY)
		: static_cast<int>((1.0 - std::pow(1.0 - static_cast<double>(timer().elapsed()) / 1'000'000.0, 2)) * static_cast<double>(MainPosY));
	const int x = static_cast<int>(((std::sin(static_cast<double>(std::max(9'000'000uLL, timer().elapsed() + 8'000'000uLL)) / 10'000'000.0) + 1.0) / 2.0) * static_cast<double>(PictureWidth - width));
	const RECT rect = {x, y, x + width, y + static_cast<int>(Geometry::BaseSize)};
	return rect;
}

// -------------------------------------------------------------------------

}
