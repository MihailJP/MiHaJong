#pragma once

#include <tchar.h>
#include <functional>
#include "scenes/scene_id.h"
#include "scenes/proto.h"
#include "scenes/fps.h"
#include "input.h"
#include "../common/mutex.h"
#include "directx.h"

namespace mihajong_graphic {

class ScreenManipulator {
private:
	HWND hWnd;
	RenderingSysPtr pd3d; // Direct3D
	DevicePtr pDevice; // Direct3D�f�o�C�X
	Scene* myScene; // �V�[���Ǘ��p�̃N���X
	FPSIndicator* myFPSIndicator; // FPS�v�Z�E�\��
	UINT64 lastRedrawTime;
	bool redrawFlag; // ��ʂ̍ĕ`������邩�ǂ����̃t���O
	void InitDevice(bool fullscreen); // Direct3D �I�u�W�F�N�g������
	void inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f);
	MHJMutex CS_SceneAccess; // �V�[���A�N�Z�X�̃N���e�B�J���Z�N�V����
public:
	void inputProc(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(); // ��ʂ̍ĕ`��
	ScreenManipulator(HWND windowHandle, bool fullscreen);
	~ScreenManipulator();
	HWND getHWnd() {return hWnd;}
	DevicePtr getDevice() {return pDevice;}
	void inputProc(input::InputManipulator* iManip);
	void transit(sceneID scene);
	void subscene(unsigned int subsceneID);
};

}
