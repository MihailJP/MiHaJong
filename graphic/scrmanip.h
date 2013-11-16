#pragma once

#ifdef _WIN32
#include <tchar.h>
#else /*_WIN32*/
#include "../common/strcode.h"
#include <X11/Xlib.h>
#endif /*_WIN32*/
#include <functional>
#include "scenes/scene_id.h"
#include "scenes/proto.h"
#include "scenes/fps.h"
#include "input.h"
#include "directx.h"

namespace mihajong_graphic {

class ScreenManipulator {
private:
#ifdef _WIN32
	HWND hWnd;
#else /*_WIN32*/
	Window hWnd;
#endif /*_WIN32*/
	RenderingSysPtr pd3d; // Direct3D
	DevicePtr pDevice; // Direct3D�f�o�C�X/OpenGL�f�o�C�X�R���e�L�X�g
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	HGLRC rContext;
#elif !defined(_WIN32)
	Display* disp;
#endif
	Scene* myScene; // �V�[���Ǘ��p�̃N���X
	FPSIndicator* myFPSIndicator; // FPS�v�Z�E�\��
	uint64_t lastRedrawTime;
	bool redrawFlag; // ��ʂ̍ĕ`������邩�ǂ����̃t���O
	void InitDevice(bool fullscreen); // Direct3D �I�u�W�F�N�g������
#ifdef _WIN32
	void inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f);
	// Linux�ł͕ʂ̉ӏ��œ��̓C�x���g���������邽�ߕs�v
#endif /*_WIN32*/
	std::recursive_mutex CS_SceneAccess; // �V�[���A�N�Z�X�̃N���e�B�J���Z�N�V����
public:
#ifdef _WIN32
	void inputProc(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
#else /*_WIN32*/
	/* TODO: Linux�ł͓��{����͂������� */
	void kbdInputProc(const XEvent* event);
#endif /*_WIN32*/
	void Render(); // ��ʂ̍ĕ`��
#ifdef _WIN32
	ScreenManipulator(HWND windowHandle, bool fullscreen);
#else /*_WIN32*/
	ScreenManipulator(Display* displayPtr, Window windowHandle, bool fullscreen);
#endif /*_WIN32*/
	ScreenManipulator(const ScreenManipulator&) = delete; // Delete unexpected copy constructor
	ScreenManipulator& operator= (const ScreenManipulator&) = delete; // Delete unexpected assign operator
	~ScreenManipulator();
#ifdef _WIN32
	HWND getHWnd() {return hWnd;}
	DevicePtr getDevice() {return pDevice;}
	void inputProc(input::InputManipulator* iManip);
#else /*_WIN32*/
	Window getHWnd() {return hWnd;}
	DevicePtr getDevice() {return nullptr;}
	void mouseInputProc(const XEvent* event);
#endif /*_WIN32*/
	void transit(sceneID scene);
	void subscene(unsigned int subsceneID);
};

}
