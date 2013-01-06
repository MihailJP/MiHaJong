#pragma once

#include "call.h"

namespace mihajong_graphic {
	
class TableSubsceneCheckTenpai : public TableSubsceneCallFadeProto {
public:
	TableSubsceneCheckTenpai(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCheckTenpai();
	void Render();
};

}
