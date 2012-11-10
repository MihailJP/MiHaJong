#ifndef GRAPHIC_SCRMANIP_H
#define GRAPHIC_SCRMANIP_H

#include <d3dx9.h>
#include <dxerr.h>
#include <tchar.h>

class ScreenManipulator {
private:
	HWND hWnd;
	LPDIRECT3DDEVICE9 pDevice;
	HRESULT InitDevice(); // Direct3D オブジェクト初期化
public:
	ScreenManipulator(HWND windowHandle);
};

#endif
