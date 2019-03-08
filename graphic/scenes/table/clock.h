#pragma once

#include "../table.h"
#include "../../loadtex.h"
#include "../../geometry.h"

namespace mihajong_graphic {

class GameTableScreen::Clock {
private:
	GameTableScreen* parent;
	TexturePtr myTexture;
	TransformMatrix setClockMatrix(float angle = 0.0f);
private:
	static int clockPosX;
	static int clockPosY;
	static constexpr unsigned clockDiameter = 512u;
	static unsigned scaledClockDiameter;
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
