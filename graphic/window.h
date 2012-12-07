#pragma once

#include <Windows.h>
#include <tchar.h>
#include "scrmanip.h"
#include "geometry.h"
#include "input.h"
#include "scenes/scene_id.h"

namespace mihajong_graphic {

class MainWindow {
private:
	HWND hWnd;
	ScreenManipulator* myScreenManipulator; // DirectX �I�u�W�F�N�g
	input::InputManipulator* myInputManipulator; // DirectInput
	static const LPTSTR myWindowClassName, WindowCaption;
	static unsigned& WindowWidth, & WindowHeight;
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // �E�B���h�E�v���V�[�W��
	void initWindowClass(HINSTANCE hThisInst, LPCTSTR icon); // �E�B���h�E�N���X�̏�����
	void initWindow(HINSTANCE hThisInst, int nWinMode); // �E�B���h�E�̐���
public:
	MainWindow(HINSTANCE hThisInst, int nWinMode, LPCTSTR icon);
	~MainWindow();
	void Render();
	void transit(sceneID scene);
	HWND gethwnd() {return hWnd;}
};

}