#include "fps.h"
#include "../scrmanip.h"
#include "../text.h"

namespace mihajong_graphic {

FPSIndicator::FPSIndicator(ScreenManipulator* const manipulator) : Scene(manipulator) {
	textRenderer = new TextRenderer(caller->getDevice());
	fpsstr[0] = 0;
}

FPSIndicator::~FPSIndicator() {
	if (textRenderer) delete textRenderer;
}

void FPSIndicator::Render() {
	{ // Žž‚ðŽæ“¾
		UINT64 tempus = myTimer.currTime();
		RedrawTime.push_back(tempus);
		while ((tempus - RedrawTime.front()) > 1000000) RedrawTime.pop_front();
	}
	/* ŒvŽZ‚·‚é */
	if ((RedrawTime.back() - LastRecalcTime) >= 500000) {
		LastRecalcTime = RedrawTime.back();
		if ((RedrawTime.size() < 3) || (RedrawTime.back() == RedrawTime.front())) {
			currentFPS = 0;
			_stprintf_s(fpsstr, fpsstr_size, _T("--.-- FPS"));
		} else {
			currentFPS = 1000000.0f / ((float)(RedrawTime.back() - RedrawTime.front()) / (float)(RedrawTime.size() - 1));
			_stprintf_s(fpsstr, fpsstr_size, _T("%5.2f FPS"), currentFPS);
		}
	}
	/* •\Ž¦‚·‚é */
	textRenderer->NewText(0, fpsstr, 0, 0, 0.75, 1.333333);
	textRenderer->Render();
}

}
