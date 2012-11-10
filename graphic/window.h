#ifndef GRAPHIC_WINDOW_H
#define GRAPHIC_WINDOW_H

#include <Windows.h>
#include <tchar.h>

class MainWindow {
private:
	HWND hWnd;
	static const LPTSTR myWindowClassName, WindowCaption;
	static const unsigned WindowWidth, WindowHeight;
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ウィンドウプロシージャ
	void initWindowClass(HINSTANCE hThisInst); // ウィンドウクラスの初期化
	void initWindow(HINSTANCE hThisInst, int nWinMode); // ウィンドウの生成
public:
	MainWindow(HINSTANCE hThisInst, int nWinMode);
};

#endif
