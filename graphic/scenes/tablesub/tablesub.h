#pragma once

#include <windows.h>
#include "../../directx.h"
#include <dinput.h>
#include "../../geometry.h"
#include <cstdint>
#include "../../timer.h"

namespace mihajong_graphic {

class TableSubscene {
protected:
	static const unsigned int TableSize = Geometry::BaseSize;
	DevicePtr myDevice;
	Timer myTimer;
public:
	TableSubscene(DevicePtr device);
	virtual ~TableSubscene() {}
	virtual void Render() = 0;
	static const std::int32_t NoTimeout = 0x7fffffff;
	virtual std::int32_t timeout() {return NoTimeout;} // �^�C���A�E�g����p
	virtual void skipEvent() {} // �{�^���������ăX�L�b�v�����ꍇ
};

}
