#ifndef GRAPHIC_SCENES_FPS_H
#define GRAPHIC_SCENES_FPS_H

#include "proto.h"
#include <deque>

class FPSIndicator : public Scene {
private:
	LPD3DXFONT pFont;
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

#endif
