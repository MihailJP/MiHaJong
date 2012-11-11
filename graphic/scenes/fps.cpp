#include "fps.h"
#include "../scrmanip.h"

FPSIndicator::FPSIndicator(ScreenManipulator* const manipulator) : Scene(manipulator) {
	D3DXCreateFont(caller->getDevice(), 18, 0, FW_REGULAR, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, nullptr, &pFont);
	fpsstr[0] = 0;
}

FPSIndicator::~FPSIndicator() {
	if (pFont) pFont->Release();
}

void FPSIndicator::Render() {
	{ // éûçèÇéÊìæ
		FILETIME sTime; GetSystemTimeAsFileTime(&sTime);
		UINT64 tempus = ((UINT64)sTime.dwHighDateTime << 32) | sTime.dwLowDateTime;
		RedrawTime.push_back(tempus);
		while ((tempus - RedrawTime.front()) > 10000000) RedrawTime.pop_front();
	}
	/* åvéZÇ∑ÇÈ */
	if ((RedrawTime.back() - LastRecalcTime) >= 5000000) {
		LastRecalcTime = RedrawTime.back();
		if ((RedrawTime.size() < 3) || (RedrawTime.back() == RedrawTime.front())) {
			currentFPS = 0;
			_stprintf_s(fpsstr, fpsstr_size, _T("--.-- FPS"));
		} else {
			currentFPS = 10000000.0f / ((float)(RedrawTime.back() - RedrawTime.front()) / (float)(RedrawTime.size() - 1));
			_stprintf_s(fpsstr, fpsstr_size, _T("%5.2f FPS"), currentFPS);
		}
	}
	/* ï\é¶Ç∑ÇÈ */
	RECT rect; SetRect(&rect, 1, 1, 50, 50);
	pFont->DrawText(nullptr, fpsstr, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
	SetRect(&rect, 0, 0, 50, 50);
	pFont->DrawText(nullptr, fpsstr, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
}
