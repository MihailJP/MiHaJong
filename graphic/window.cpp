#include "window.h"

const LPTSTR MainWindow::myWindowClassName = _T("testwnd");
const LPTSTR MainWindow::WindowCaption = _T("テストウィンドウ");
const unsigned MainWindow::WindowWidth = 1024;
const unsigned MainWindow::WindowHeight = 768;

LRESULT CALLBACK MainWindow::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { // ウィンドウプロシージャ
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MainWindow::initWindowClass(HINSTANCE hThisInst) { // ウィンドウクラスの初期化
	WNDCLASSEX myWindowClass;
	
	myWindowClass.hInstance = hThisInst;
	myWindowClass.lpszClassName = myWindowClassName;
	myWindowClass.lpfnWndProc = WinProc;
	myWindowClass.style = 0;
	myWindowClass.cbSize = sizeof(WNDCLASSEX);
	myWindowClass.hIcon = LoadIcon(hThisInst, IDI_APPLICATION);
	myWindowClass.hIconSm = LoadIcon(hThisInst, IDI_WINLOGO);
	myWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	myWindowClass.lpszMenuName = nullptr;
	myWindowClass.cbClsExtra = 0;
	myWindowClass.cbWndExtra = 0;
	myWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	
	if (!RegisterClassEx(&myWindowClass))
		throw _T("ウィンドウクラスの登録に失敗しました");
	else return;
}

void MainWindow::initWindow(HINSTANCE hThisInst, int nWinMode) {
	RECT WindowRect;
	WindowRect.left = 0; WindowRect.right = WindowWidth;
	WindowRect.top = 0; WindowRect.bottom = WindowHeight;
	DWORD ExStyle = 0;
	DWORD Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRectEx(&WindowRect, Style, FALSE, ExStyle);
	hWnd = CreateWindowEx(
		ExStyle, myWindowClassName, WindowCaption, Style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
		nullptr, nullptr, hThisInst, nullptr);
	if (!hWnd) throw _T("ウィンドウの生成に失敗しました");
	ShowWindow(hWnd, nWinMode);
	UpdateWindow(hWnd);
	return;
}

MainWindow::MainWindow(HINSTANCE hThisInst, int nWinMode) {
	initWindowClass(hThisInst);
	initWindow(hThisInst, nWinMode);
}
