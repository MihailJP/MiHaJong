#pragma once

#include "sysscr.h"
#include <array>
#include <cstdint>
#include "../text.h"
#include "../../common/strcode.h"

namespace mihajong_graphic {

class TitleScreen : public SystemScreen {
private:
	static const unsigned nsTitleLogo = 3;
	class TitleSprite;
	std::array<TitleSprite*, nsTitleLogo> sTitleLogo;
	void zoomingLogo(TitleSprite* sprite, int X, int Y, unsigned startF, unsigned endF);
	void menuLabelSlide(unsigned ID, const CodeConv::tstring& menustr, int X, int Y, unsigned startF, unsigned endF);
	void menuLabels();
	void versionInfo();
	unsigned menuCursor;
public:
	TitleScreen(ScreenManipulator* const manipulator);
	TitleScreen(const TitleScreen&) = delete; // Delete unexpected copy constructor
	TitleScreen& operator= (const TitleScreen&) = delete; // Delete unexpected assign operator
	~TitleScreen();
	void Render();
#ifdef _WIN32
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
#else /*_WIN32*/
	virtual void KeyboardInput(const XEvent* od);
	virtual void MouseInput(const XEvent* od, int X, int Y);
#endif /*_WIN32*/
};

class TitleScreen::TitleSprite {
private:
	static TexturePtr texture;
	RECT rect;
	int width, height;
	DevicePtr myDevice;
public:
	static void LoadTexture(DevicePtr device);
	static void DisposeTexture();
	TitleSprite(DevicePtr device, int X, int Y, int Width, int Height);
	TitleSprite(const TitleSprite&) = delete; // Delete unexpected copy constructor
	TitleSprite& operator= (const TitleSprite&) = delete; // Delete unexpected assign operator
	~TitleSprite();
	void show(int X, int Y, float scale = 1.0f, uint8_t opacity = 0xff);
};

}
