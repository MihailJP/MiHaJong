#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <dinput.h>
#include "../../geometry.h"
#include <cstdint>
#include "../../timer.h"

namespace mihajong_graphic {

class TableSubscene {
protected:
	static const unsigned int TableSize = Geometry::BaseSize;
	LPDIRECT3DDEVICE9 myDevice;
	Timer myTimer;
public:
	TableSubscene(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubscene() {}
	virtual void Render() = 0;
	static const std::int32_t NoTimeout = 0x7fffffff;
	virtual std::int32_t timeout() {return NoTimeout;} // �^�C���A�E�g����p
	virtual void skipEvent() {} // �{�^���������ăX�L�b�v�����ꍇ
};

}
