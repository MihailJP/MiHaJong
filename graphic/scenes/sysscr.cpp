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

void SystemScreen::skipto(unsigned frames) {
	FILETIME currTime; GetSystemTimeAsFileTime(&currTime);
	int64_t ct = ((int64_t)currTime.dwHighDateTime << 32) | currTime.dwLowDateTime;
	ct -= frames * timePerFrame;
	startTime.dwHighDateTime = ct >> 32; startTime.dwLowDateTime = ct & 0xffffffff;
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
