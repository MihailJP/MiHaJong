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
	LPDIRECT3DDEVICE9 pDevice; // Direct3D�f�o�C�X
	Scene* myScene; // �V�[���Ǘ��p�̃N���X
	void InitDevice(); // Direct3D �I�u�W�F�N�g������
public:
	void Render(); // ��ʂ̍ĕ`��
	ScreenManipulator(HWND windowHandle);
	~ScreenManipulator();
};

#endif
