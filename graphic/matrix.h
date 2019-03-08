#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "directx.h"

namespace mihajong_graphic {

TransformMatrix getMatrix(
	float baseX         = 0.0f, // 処理中心のX座標
	float baseY         = 0.0f, // 処理中心のY座標
	float scaleX        = 1.0f, // X方向の倍率
	float scaleY        = 1.0f, // Y方向の倍率
	float rotateAngle   = 0.0f, // 回転 (rad)
	float rotateOffsetX = 0.0f, // 回転中心のオフセット
	float rotateOffsetY = 0.0f, // 回転中心のオフセット
	float scaleAfterX   = 1.0f,
	float scaleAfterY   = 1.0f
);

}
