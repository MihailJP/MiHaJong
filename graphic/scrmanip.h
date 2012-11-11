#ifndef GRAPHIC_SCRMANIP_H
#define GRAPHIC_SCRMANIP_H

#include <d3dx9.h>
#include <dxerr.h>
#include <tchar.h>
#include "scenes/scenes.h"

class ScreenManipulator {
private:
	HWND hWnd;
	LPDIRECT3D9 pd3d; // Direct3D
	LPDIRECT3DDEVICE9 pDevice; // Direct3Dデバイス
	Scene* myScene; // シーン管理用のクラス
	void InitDevice(); // Direct3D オブジェクト初期化
public:
	void Render(); // 画面の再描画
	ScreenManipulator(HWND windowHandle);
	~ScreenManipulator();
};

#endif
