#pragma once

#ifdef _WIN32
#include <windows.h>
#include <dinput.h>
#endif /*_WIN32*/
#include "../../directx.h"
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
	TableSubscene(const TableSubscene&) = delete; // Delete unexpected copy constructor
	TableSubscene& operator= (const TableSubscene&) = delete; // Delete unexpected assign operator
	virtual ~TableSubscene() {}
	virtual void Render() = 0;
	static const std::int32_t NoTimeout = 0x7fffffff;
	virtual std::int32_t timeout() {return NoTimeout;} // タイムアウト判定用
	virtual void skipEvent() {} // ボタンを押してスキップした場合
};

}
