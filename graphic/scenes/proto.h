#pragma once

#include <windows.h>
#include "../directx.h"
#include <vector>
#include "../input.h"

namespace mihajong_graphic {

class ScreenManipulator;

/* シーン オブジェクトのスーパークラス */
class Scene {
protected:
	struct Region {int Left, Top, Right, Bottom;}; // クリック位置判定用
	static const Region NullRegion;
	void setRegion(unsigned regionID, int Left, int Top, int Right, int Bottom);
	void setRegion(unsigned regionID, const Region& region);
	int whichRegion(int X, int Y); // どの領域にマウスがあるかを取得
private:
	std::vector<Region> regions; // クリック位置判定用
protected:
	ScreenManipulator* caller; // 呼び出し元へのポインタ
public:
	virtual void Render() = 0; // 描画処理
	Scene(ScreenManipulator* const manipulator) {
		caller = manipulator;
	}
	virtual ~Scene() {}
	virtual void IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {}
	virtual void KeyboardInput(WPARAM wParam, LPARAM lParam) {}
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od) {}
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {}
	virtual void SetSubscene(unsigned int scene_ID) {}
};

}
