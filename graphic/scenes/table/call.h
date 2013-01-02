#pragma once

#include "tablesub.h"
#include "../../gametbl.h"
#include <cstdint>

namespace mihajong_graphic {

class TableSubsceneCall : public TableSubscene {
protected:
	std::uint64_t currTime();
	std::uint64_t startTime;
protected:
	LPDIRECT3DTEXTURE9 tCall; LPD3DXSPRITE sCall;
	void ShowCall(PLAYER_ID player, int x, int y);
public:
	TableSubsceneCall(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCall();
	void Render();
};

}
