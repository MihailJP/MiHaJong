#pragma once

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else /*_WIN32*/
#include <X11/Xlib.h>
#include "../common/strcode.h"
#endif /*_WIN32*/
#include "scrmanip.h"
#include "geometry.h"
#include "input.h"
#include "scenes/scene_id.h"

namespace mihajong_graphic {

class MainWindow {
private:
#ifdef _WIN32
	HWND hWnd;
#else /*_WIN32*/
	Display* disp;
	Window hWnd;
	Atom wmDelMsg;
#endif /*_WIN32*/
	ScreenManipulator* myScreenManipulator; // DirectX �I�u�W�F�N�g
#ifdef _WIN32
	input::InputManipulator* myInputManipulator; // DirectInput
	// Linux�ł͕ʂ̉ӏ��œ��̓C�x���g���������邽�ߕs�v
#endif /*_WIN32*/
	static const LPCTSTR myWindowClassName, WindowCaption;
	static unsigned& WindowWidth, & WindowHeight;
#ifdef _WIN32
	static LRESULT keyev(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // �E�B���h�E�v���V�[�W��
	void initWindowClass(HINSTANCE hThisInst, LPCTSTR icon); // �E�B���h�E�N���X�̏�����
	void initWindow(HINSTANCE hThisInst, int nWinMode, bool fullscreen); // �E�B���h�E�̐���
#else /*_WIN32*/
	void initWindow(void* hThisInst, int nWinMode, bool fullscreen); // �E�B���h�E�̐���
public:
	static bool WinProc(MainWindow* mainWindow); // �E�B���h�E�v���V�[�W��
#endif /*_WIN32*/
public:
#ifdef _WIN32
	MainWindow(HINSTANCE hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height, bool fullscreen);
#else /*_WIN32*/
	MainWindow(void* hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height, bool fullscreen);
#endif /*_WIN32*/
	~MainWindow();
	void Render();
	void transit(sceneID scene);
	void subscene(unsigned int subsceneID);
#ifdef _WIN32
	HWND gethwnd() {return hWnd;}
#else /*_WIN32*/
	Window gethwnd() {return hWnd;}
#endif /*_WIN32*/
};

}
