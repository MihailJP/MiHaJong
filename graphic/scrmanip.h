#pragma once

#include <d3dx9.h>
#include <dxerr.h>
#include <tchar.h>
#include <functional>
#include "scenes/scene_id.h"
#include "scenes/proto.h"
#include "scenes/fps.h"
#include "input.h"

namespace mihajong_graphic {

class ScreenManipulator {
private:
	HWND hWnd;
	LPDIRECT3D9 pd3d; // Direct3D
	LPDIRECT3DDEVICE9 pDevice; // Direct3D�f�o�C�X
	Scene* myScene; // �V�[���Ǘ��p�̃N���X
	FPSIndicator* myFPSIndicator; // FPS�v�Z�E�\��
	UINT64 lastRedrawTime;
	bool redrawFlag; // ��ʂ̍ĕ`������邩�ǂ����̃t���O
	void InitDevice(); // Direct3D �I�u�W�F�N�g������
	void inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f);
	CRITICAL_SECTION csRedrawFlag; // �ĕ`��t���O����p�̃N���e�B�J���Z�N�V����
public:
	void Render(); // ��ʂ̍ĕ`��
	ScreenManipulator(HWND windowHandle);
	~ScreenManipulator();
	HWND getHWnd() {return hWnd;}
	LPDIRECT3DDEVICE9 getDevice() {return pDevice;}
	void inputProc(input::InputManipulator* iManip);
	void transit(sceneID scene);
};

}
