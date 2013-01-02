#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>

namespace mihajong_graphic {

class TableSubscene {
protected:
	LPDIRECT3DDEVICE9 myDevice;
public:
	TableSubscene(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubscene() {}
	virtual void Render() = 0;
};

}
