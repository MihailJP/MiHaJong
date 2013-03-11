#pragma once

#include <Windows.h>
#include "directx.h"
#include <dinput.h>
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
	static const std::int32_t NoTimeout = 0x7fffffff;
	virtual std::int32_t timeout() {return NoTimeout;} // タイムアウト判定用
};

}
