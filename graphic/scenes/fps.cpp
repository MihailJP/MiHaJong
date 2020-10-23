#include "fps.h"
#include "../scrmanip.h"
#include "../text.h"
#if !defined(_MSC_VER)
#include <stdio.h>
#endif
#include "../../common/safec.h"

namespace mihajong_graphic {

FPSIndicator::FPSIndicator(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LastRecalcTime = 0;
	currentFPS = 0.0f;
	textRenderer = new TextRenderer(caller->getDevice());
	fpsstr[0] = 0;
}

FPSIndicator::~FPSIndicator() {
	if (textRenderer) delete textRenderer;
}

void FPSIndicator::Render() {
	{ // 時刻を取得
		uint64_t tempus = myTimer.currTime();
		RedrawTime.push_back(tempus);
		while ((tempus - RedrawTime.front()) > 1000000) RedrawTime.pop_front();
	}
	/* 計算する */
	if ((RedrawTime.back() - LastRecalcTime) >= 500000) {
		LastRecalcTime = RedrawTime.back();
		if ((RedrawTime.size() < 3) || (RedrawTime.back() == RedrawTime.front())) {
			currentFPS = 0;
#if defined(_MSC_VER) || defined(USE_LIBSAFEC) || (!defined(_UNICODE) && defined(HAVE_SPRINTF_S)) || (defined(_UNICODE) && defined(HAVE_SWPRINTF_S))
			_stprintf_s(fpsstr, fpsstr_size, _T("--.-- FPS"));
#else
			_sntprintf(fpsstr, fpsstr_size, _T("--.-- FPS"));
#endif
		} else {
			currentFPS = 1000000.0f / (static_cast<float>(RedrawTime.back() - RedrawTime.front()) / static_cast<float>(RedrawTime.size() - 1));
#if defined(_MSC_VER) || defined(USE_LIBSAFEC) || (!defined(_UNICODE) && defined(HAVE_SPRINTF_S)) || (defined(_UNICODE) && defined(HAVE_SWPRINTF_S))
			_stprintf_s(fpsstr, fpsstr_size, _T("%5.2f FPS"), currentFPS);
#else
			_sntprintf(fpsstr, fpsstr_size, _T("%5.2f FPS"), currentFPS);
#endif
		}
	}
	/* 表示する */
	textRenderer->NewText(0, fpsstr, 0, 0, 0.75f, 1.333333f);
	textRenderer->Render();
}

}
