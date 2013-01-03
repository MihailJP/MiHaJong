#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "../../geometry.h"
#include <cstdint>

namespace mihajong_graphic {

class TableSubscene {
protected:
	static const unsigned int TableSize = Geometry::BaseSize;
	LPDIRECT3DDEVICE9 myDevice;
protected:
	std::uint64_t currTime();
	std::uint64_t startTime;
public:
	TableSubscene(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubscene() {}
	virtual void Render() = 0;
};

}
