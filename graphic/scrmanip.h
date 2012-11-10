#ifndef GRAPHIC_SCRMANIP_H
#define GRAPHIC_SCRMANIP_H

#include <d3dx9.h>
#include <dxerr.h>
#include <tchar.h>

class ScreenManipulator {
private:
	HWND hWnd;
	LPDIRECT3D9 pd3d;
	LPDIRECT3DDEVICE9 pDevice;
	void InitDevice(); // Direct3D オブジェクト初期化
public:
	void Render(); // 画面の再描画
	ScreenManipulator(HWND windowHandle);
	~ScreenManipulator();
};

#endif
