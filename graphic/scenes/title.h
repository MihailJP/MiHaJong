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
	unsigned menuCursor;
public:
	TitleScreen(ScreenManipulator* const manipulator);
	~TitleScreen();
	void Render();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
};

class TitleScreen::TitleSprite {
private:
	static LPDIRECT3DTEXTURE9 texture;
	RECT rect;
	int width, height;
	LPDIRECT3DDEVICE9 myDevice;
public:
	static void LoadTexture(LPDIRECT3DDEVICE9 device);
	static void DisposeTexture();
	TitleSprite(LPDIRECT3DDEVICE9 device, int X, int Y, int Width, int Height);
	~TitleSprite();
	void show(int X, int Y, float scale = 1.0f, uint8_t opacity = 0xff);
};

}
