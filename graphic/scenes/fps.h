#ifndef GRAPHIC_SCENES_FPS_H
#define GRAPHIC_SCENES_FPS_H

#include "proto.h"
#include <deque>

namespace mihajong_graphic {

class TextRenderer;

class FPSIndicator : public Scene {
private:
	TextRenderer* textRenderer;
	std::deque<UINT64> RedrawTime;
	UINT64 LastRecalcTime;
	float currentFPS;
	static const unsigned int fpsstr_size = 64;
	TCHAR fpsstr[fpsstr_size];
public:
	FPSIndicator(ScreenManipulator* const manipulator);
	~FPSIndicator();
	void Render();
};

}
#endif
