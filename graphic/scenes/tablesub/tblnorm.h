#pragma once

#include "tablesub.h"

namespace mihajong_graphic {

class TableSubsceneNormal : public TableSubscene {
public:
	TableSubsceneNormal(DevicePtr device);
	~TableSubsceneNormal();
	void Render();
};

}
