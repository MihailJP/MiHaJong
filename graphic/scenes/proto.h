#ifndef GRAPHIC_SCENES_PROTO_H
#define GRAPHIC_SCENES_PROTO_H

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>

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
};

#endif
