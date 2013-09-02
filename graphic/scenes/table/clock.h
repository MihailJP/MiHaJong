#pragma once

#include "../table.h"
#include "../game.h"

namespace mihajong_graphic {

class TableProtoScene::Clock {
	TableProtoScene* parent;
public:
	Clock(TableProtoScene* caller);
	~Clock();
	void Render();
};

}
