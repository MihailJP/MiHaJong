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
	default:
		assert(false); // This may not occur.
	}
}

unsigned SystemScreen::strwidth(const std::wstring& str) {
	unsigned cols = 0u; // 桁数(日本語は2桁)
	for (auto k = str.begin(); k != str.end(); ++k) {
		if (*k <= _T('\x7f')) cols += 1;
		else cols += 2;
	}
	return cols;
}
unsigned SystemScreen::strwidth(const std::string& str) {
	return str.size();
}

// -------------------------------------------------------------------------

}
