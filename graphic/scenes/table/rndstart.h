#pragma once

#include "tablesub.h"
#include "../../gametbl.h"
#include "../../text.h"

namespace mihajong_graphic {

class TableSubsceneBeginning : public TableSubscene {
protected:
	HugeTextRenderer* myTextRenderer;
public:
	TableSubsceneBeginning(LPDIRECT3DDEVICE9 device);
	~TableSubsceneBeginning();
	void Render();
};

}
