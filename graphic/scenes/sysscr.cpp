#include "sysscr.h"

#include <cassert>
#include "../gametbl.h"

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

}
