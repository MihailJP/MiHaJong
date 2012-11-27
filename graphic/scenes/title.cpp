#include "title.h"
#include "../scrmanip.h"
#include "../gametbl.h"
#include <cassert>

TitleScreen::TitleScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	caller = manipulator;
}

TitleScreen::~TitleScreen() {
}

void TitleScreen::Render() {
	// バッファクリア
	switch (GameStatus::gameStat()->gameType) {
	case Yonma:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0);
		break;
	case Sanma:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 0, 128), 1.0f, 0);
		break;
	case Sanma4:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 128, 128), 1.0f, 0);
		break;
	case SanmaS:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(128, 0, 128), 1.0f, 0);
		break;
	default:
		assert(false); // This may not occur.
	}
}
