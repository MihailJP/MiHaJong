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
#include "../common/scrmode.h"

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
	ScreenManipulator* myScreenManipulator; // DirectX オブジェクト
#ifdef _WIN32
	input::InputManipulator* myInputManipulator; // DirectInput
	// Linuxでは別の箇所で入力イベントを処理するため不要
#endif /*_WIN32*/
	static const LPCTSTR myWindowClassName, WindowCaption;
	static unsigned& WindowWidth, & WindowHeight;
#ifdef _WIN32
	static LRESULT keyev(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ウィンドウプロシージャ
	void initWindowClass(HINSTANCE hThisInst, LPCTSTR icon); // ウィンドウクラスの初期化
	void initWindow(HINSTANCE hThisInst, int nWinMode, ScreenMode::ScreenMode scrMode, unsigned monitor); // ウィンドウの生成
#else /*_WIN32*/
	void initWindow(void* hThisInst, int nWinMode, ScreenMode::ScreenMode scrMode, unsigned monitor); // ウィンドウの生成
public:
	static bool WinProc(MainWindow* mainWindow); // ウィンドウプロシージャ
#endif /*_WIN32*/
public:
#ifdef _WIN32
	MainWindow(HINSTANCE hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height, ScreenMode::ScreenMode scrMode, unsigned monitor);
#else /*_WIN32*/
	MainWindow(void* hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height, ScreenMode::ScreenMode scrMode, unsigned monitor);
#endif /*_WIN32*/
	MainWindow(const MainWindow&) = delete; // Delete unexpected copy constructor
	MainWindow& operator= (const MainWindow&) = delete; // Delete unexpected assign operator
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
