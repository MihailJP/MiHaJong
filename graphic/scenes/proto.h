#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "../input.h"

namespace mihajong_graphic {

class ScreenManipulator;

/* シーン オブジェクトのスーパークラス */
class Scene {
protected:
	ScreenManipulator* caller; // 呼び出し元へのポインタ
public:
	virtual void Render() = 0; // 描画処理
	Scene(ScreenManipulator* const manipulator) {
		caller = manipulator;
	}
	virtual ~Scene() {}
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od) {};
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {};
};

}
