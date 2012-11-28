#ifndef GRAPHIC_WINDOW_H
#define GRAPHIC_WINDOW_H

#include <Windows.h>
#include <tchar.h>
#include "scrmanip.h"
#include "geometry.h"

namespace mihajong_graphic {

class MainWindow {
private:
	HWND hWnd;
	static ScreenManipulator* myScreenManipulator; // DirectX オブジェクト
	static const LPTSTR myWindowClassName, WindowCaption;
	static unsigned& WindowWidth, & WindowHeight;
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ウィンドウプロシージャ
	void initWindowClass(HINSTANCE hThisInst, LPCTSTR icon); // ウィンドウクラスの初期化
	void initWindow(HINSTANCE hThisInst, int nWinMode); // ウィンドウの生成
public:
	MainWindow(HINSTANCE hThisInst, int nWinMode, LPCTSTR icon);
	~MainWindow();
	void Render();
};

}
#endif
