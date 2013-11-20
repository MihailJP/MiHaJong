#pragma once

#include "proto.h"
#include <deque>
#include <cstdint>
#include "../timer.h"
#ifndef _WIN32
#include "../../common/strcode.h"
#endif /*_WIN32*/

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
	FPSIndicator(const FPSIndicator&) = delete; // Delete unexpected copy constructor
	FPSIndicator& operator= (const FPSIndicator&) = delete; // Delete unexpected assign operator
	~FPSIndicator();
	void Render();
};

}
