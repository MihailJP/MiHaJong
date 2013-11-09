#pragma once

#include "../table.h"
#include "../../loadtex.h"
#include "../../geometry.h"

namespace mihajong_graphic {

class GameTableScreen::Clock {
private:
	GameTableScreen* parent;
	TexturePtr myTexture;
	void setClockMatrix(TransformMatrix* matrix, float angle = 0.0f);
private:
	static const int clockPosX = 460;
	static const int clockPosY = 460;
	static const unsigned clockDiameter = 512u;
	static const unsigned scaledClockDiameter = 160u;
private:
	void renderMoon();
	void renderShadow();
	void renderPanel();
	void renderHour();
	void renderMinute();
	void renderPin();
	void renderFrame();
public:
	Clock(GameTableScreen* caller);
	Clock(const Clock&) = delete; // Delete unexpected copy constructor
	Clock& operator= (const Clock&) = delete; // Delete unexpected assign operator
	~Clock();
	void Render();
};

}
