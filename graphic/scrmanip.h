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
	void InitDevice(); // Direct3D �I�u�W�F�N�g������
public:
	void Render(); // ��ʂ̍ĕ`��
	ScreenManipulator(HWND windowHandle);
	~ScreenManipulator();
};

#endif
