#pragma once

#include "../table.h"
#include "../game.h"
#include "../../loadtex.h"
#include "../../geometry.h"

namespace mihajong_graphic {

class TableProtoScene::Clock {
private:
	TableProtoScene* parent;
	TexturePtr myTexture;
	void setClockMatrix(TransformMatrix* matrix);
private:
	static const int clockPosX = Geometry::BaseSize + 24;
	static const int clockPosY = 144;
private:
	void renderMoon();
	void renderShadowL();
public:
	Clock(TableProtoScene* caller);
	~Clock();
	void Render();
};

}
