#pragma once

#ifdef _WIN32
#include <tchar.h>
#else /*_WIN32*/
#include "../common/strcode.h"
#endif /*_WIN32*/
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
#ifdef _WIN32
	HWND hWnd;
#else /*_WIN32*/
	/* TODO: ������ */
#endif /*_WIN32*/
	RenderingSysPtr pd3d; // Direct3D
#ifdef _WIN32
	DevicePtr pDevice; // Direct3D�f�o�C�X/OpenGL�f�o�C�X�R���e�L�X�g
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	HGLRC rContext;
#endif
#else /*_WIN32*/
	/* TODO: ������ */
#endif /*_WIN32*/
	Scene* myScene; // �V�[���Ǘ��p�̃N���X
	FPSIndicator* myFPSIndicator; // FPS�v�Z�E�\��
	uint64_t lastRedrawTime;
	bool redrawFlag; // ��ʂ̍ĕ`������邩�ǂ����̃t���O
	void InitDevice(bool fullscreen); // Direct3D �I�u�W�F�N�g������
#ifdef _WIN32
	void inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f);
#else /*_WIN32*/
	/* TODO: ������ */
#endif /*_WIN32*/
	MHJMutex CS_SceneAccess; // �V�[���A�N�Z�X�̃N���e�B�J���Z�N�V����
public:
#ifdef _WIN32
	void inputProc(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
#else /*_WIN32*/
	/* TODO: ������ */
#endif /*_WIN32*/
	void Render(); // ��ʂ̍ĕ`��
#ifdef _WIN32
	ScreenManipulator(HWND windowHandle, bool fullscreen);
#else /*_WIN32*/
	/* TODO: ������ */
#endif /*_WIN32*/
	~ScreenManipulator();
#ifdef _WIN32
	HWND getHWnd() {return hWnd;}
	DevicePtr getDevice() {return pDevice;}
	void inputProc(input::InputManipulator* iManip);
#else /*_WIN32*/
	/* TODO: ������ */
	DevicePtr getDevice() {return nullptr;}
#endif /*_WIN32*/
	void transit(sceneID scene);
	void subscene(unsigned int subsceneID);
};

}
