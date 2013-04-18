#pragma once

#include "proto.h"
#include <deque>
#include <cstdint>
#include "../timer.h"

namespace mihajong_graphic {

class TextRenderer;

class FPSIndicator : public Scene {
private:
	TextRenderer* textRenderer;
	std::deque<uint64_t> RedrawTime;
	Timer myTimer;
	TimerMicrosec LastRecalcTime;
	float currentFPS;
	static const unsigned int fpsstr_size = 64;
	TCHAR fpsstr[fpsstr_size];
public:
	FPSIndicator(ScreenManipulator* const manipulator);
	~FPSIndicator();
	void Render();
};

}
