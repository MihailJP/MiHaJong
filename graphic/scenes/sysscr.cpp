#include "sysscr.h"

#include <cassert>
#include "../gametbl.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

SystemScreen::SystemScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	caller = manipulator;
	myTextRenderer = new TextRenderer(caller->getDevice());
	GetSystemTimeAsFileTime(&startTime);
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

uint64_t SystemScreen::elapsed() {
	FILETIME currTime; GetSystemTimeAsFileTime(&currTime);
	uint64_t st = ((uint64_t)startTime.dwHighDateTime << 32) | startTime.dwLowDateTime;
	uint64_t ct = ((uint64_t)currTime.dwHighDateTime << 32) | currTime.dwLowDateTime;
	assert(ct >= st);
	//if ((ct - st) >= 30000000) startTime = currTime; // debug loop
	return ct - st;
}

// -------------------------------------------------------------------------

}
