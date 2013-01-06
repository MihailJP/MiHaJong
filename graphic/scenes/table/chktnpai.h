#pragma once

#include "call.h"

namespace mihajong_graphic {
	
class TableSubsceneCheckTenpai : public TableSubsceneCallFadeProto {
private:
	void ShowTenpaiFlag(PLAYER_ID player, int x, int y);
public:
	TableSubsceneCheckTenpai(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCheckTenpai();
	void Render();
};

}
