#pragma once

#include "tablesub.h"

namespace mihajong_graphic {

class TableSubsceneNormal : public TableSubscene {
public:
	TableSubsceneNormal(LPDIRECT3DDEVICE9 device);
	~TableSubsceneNormal();
	void Render();
};

}
