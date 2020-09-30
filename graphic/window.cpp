#include "window.h"
#include "../common/version.h"
#include "extchar.h"
#include "loadtex.h"
#ifndef _WIN32
#include <X11/Xutil.h>
#include <iostream>
#endif /*_WIN32*/
#include <cstdlib>
#include "except.h"

namespace mihajong_graphic {

constexpr LPCTSTR MainWindow::myWindowClassName = _T("mihajong_main");
constexpr LPCTSTR MainWindow::WindowCaption = _T("MiHaJong ver. ") _T(MIHAJONG_VER);
unsigned& MainWindow::WindowWidth = Geometry::WindowWidth;
unsigned& MainWindow::WindowHeight = Geometry::WindowHeight;
extern MainWindow* myMainWindow;

#ifdef _WIN32
LRESULT MainWindow::keyev(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_LEFT:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_LEFT, lParam);
		break;
	case VK_RIGHT:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_RIGHT, lParam);
		break;
	case VK_RETURN:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_ENTER, lParam);
		break;
	case VK_ESCAPE:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_ESCAPE, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK MainWindow::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { // ウィンドウプロシージャ
	switch (message) {
	case WM_DESTROY: // ウィンドウを閉じた時
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		return keyev(hWnd, message, wParam, lParam);
	case WM_CHAR:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(wParam, lParam);
		break;
	case WM_INPUTLANGCHANGE: case WM_IME_SETCONTEXT: case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION: case WM_IME_ENDCOMPOSITION: case WM_IME_NOTIFY:
		if (myMainWindow) myMainWindow->myScreenManipulator->IMEvent(message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MainWindow::initWindowClass(HINSTANCE hThisInst, LPCTSTR icon) { // ウィンドウクラスの初期化
	WNDCLASSEX myWindowClass;
	
	myWindowClass.hInstance = hThisInst;
	myWindowClass.lpszClassName = myWindowClassName;
	myWindowClass.lpfnWndProc = WinProc;
#ifdef WITH_DIRECTX
	myWindowClass.style = 0;
#else
	myWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
#endif
	myWindowClass.cbSize = sizeof(WNDCLASSEX);
	myWindowClass.hIcon = static_cast<HICON>(LoadImage(hThisInst, icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
	myWindowClass.hIconSm = static_cast<HICON>(LoadImage(hThisInst, icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
	myWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	myWindowClass.lpszMenuName = nullptr;
	myWindowClass.cbClsExtra = 0;
	myWindowClass.cbWndExtra = 0;
	myWindowClass.hbrBackground = nullptr;
	
	if (!RegisterClassEx(&myWindowClass))
		throw WindowInitializationError("ウィンドウクラスの登録に失敗しました");
	else return;
}

struct RectList {
	int cnt = 0;
	LPRECT rect = nullptr;
};

BOOL CALLBACK monitorInfoCallback(HMONITOR, HDC, LPRECT monRect, LPARAM lParam) {
	auto rect = reinterpret_cast<RectList*>(lParam);
	rect->rect[rect->cnt].top    = monRect->top;
	rect->rect[rect->cnt].bottom = monRect->bottom;
	rect->rect[rect->cnt].left   = monRect->left;
	rect->rect[rect->cnt].right  = monRect->right;
	++(rect->cnt);
	return TRUE;
}

void MainWindow::initWindow(HINSTANCE hThisInst, int nWinMode, ScreenMode::ScreenMode scrMode, unsigned monitor) {
	RECT WindowRect;
	WindowRect.left = 0; WindowRect.right = WindowWidth;
	WindowRect.top = 0; WindowRect.bottom = WindowHeight;
	DWORD ExStyle = 0;
	DWORD Style;
	if (scrMode != ScreenMode::scrModeFullscreen) {
		int n = GetSystemMetrics(SM_CMONITORS);
		RectList rectList;
		rectList.cnt = 0;
		rectList.rect = new RECT[n]{};
		EnumDisplayMonitors(nullptr, nullptr, monitorInfoCallback, reinterpret_cast<LPARAM>(&rectList));
		if (scrMode == ScreenMode::scrModeBorderless) {
			auto& dispRect = rectList.rect[monitor - 1];
			WindowRect.left = dispRect.left;
			WindowRect.right = dispRect.right;
			WindowRect.top = dispRect.top;
			WindowRect.bottom = dispRect.bottom;
		} else {
			auto& dispRect = rectList.rect[monitor - 1];
			const int paddingX = (dispRect.right - dispRect.left - static_cast<int>(WindowWidth)) / 2;
			const int paddingY = (dispRect.bottom - dispRect.top - static_cast<int>(WindowHeight)) / 2;
			WindowRect.left = dispRect.left + paddingX;
			WindowRect.right = dispRect.right - paddingX;
			WindowRect.top = dispRect.top + paddingY;
			WindowRect.bottom = dispRect.bottom - paddingY;
		}
		delete[] rectList.rect, rectList.rect = nullptr;
	}
#ifndef WITH_DIRECTX
	if (scrMode == ScreenMode::scrModeFullscreen) {
		Style = WS_POPUP;
	} else {
#endif
		if (scrMode == ScreenMode::scrModeBorderless) {
			Style = WS_POPUP;
		} else {
			Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
			AdjustWindowRectEx(&WindowRect, Style, FALSE, ExStyle);
		}
#ifndef WITH_DIRECTX
	}
#endif
	hWnd = CreateWindowEx(
		ExStyle, myWindowClassName, WindowCaption, Style,
#ifdef WITH_DIRECTX
		scrMode == ScreenMode::scrModeBorderless ? WindowRect.left : CW_USEDEFAULT,
		scrMode == ScreenMode::scrModeBorderless ? WindowRect.top : CW_USEDEFAULT,
#else
		scrMode == ScreenMode::scrModeBorderless ? WindowRect.left : ScreenMode::scrModeFullscreen ? 0 : CW_USEDEFAULT,
		scrMode == ScreenMode::scrModeBorderless ? WindowRect.top : ScreenMode::scrModeFullscreen ? 0 : CW_USEDEFAULT,
#endif
		WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
		nullptr, nullptr, hThisInst, nullptr);
	if (!hWnd) throw WindowInitializationError("ウィンドウの生成に失敗しました");
	ShowWindow(hWnd, nWinMode);
	UpdateWindow(hWnd);

#ifndef WITH_DIRECTX
	/* フルスクリーンにする処理(WinAPI) */
	/* DirectXの場合はDirectX側の設定でできるが、OpenGLの場合はWinAPIで設定が必要(GLUTは使用しない) */
	if (scrMode == ScreenMode::scrModeFullscreen) {
		DEVMODE dMode; ZeroMemory(&dMode, sizeof dMode);
		dMode.dmSize = sizeof dMode;
		dMode.dmPelsWidth = WindowWidth;
		dMode.dmPelsHeight = WindowHeight;
		dMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		if (FAILED(ChangeDisplaySettings(&dMode, CDS_FULLSCREEN)))
			throw WindowInitializationError("フルスクリーンにできませんでした");
	}
#endif
	return;
}
#else /*_WIN32*/
bool MainWindow::WinProc(MainWindow* mainWindow) { // ウィンドウプロシージャ
	if (XPending(mainWindow->disp)) { // イベントあり
		XEvent event;
		XNextEvent(mainWindow->disp, &event); // イベント待機
		
		switch (event.type) {
		case ClientMessage:
			if (event.xclient.data.l[0] == mainWindow->wmDelMsg) { // ウィンドウを閉じた時
				XFlush(mainWindow->disp);
				return false;
			}
			break;
		case MotionNotify: // マウスポインタの移動
		case ButtonPress: // マウスボタンを押した時
		case ButtonRelease: // マウスボタンを離した時
			mainWindow->myScreenManipulator->mouseInputProc(&event);
			break;
		case KeyPress: // キーを押した時
		case KeyRelease: // キーを離した時
			mainWindow->myScreenManipulator->kbdInputProc(&event);
			break;
		}
		return true;
	} else { // イベントなし
		mainWindow->Render();
		return true;
	}
}

void MainWindow::initWindow(void* hThisInst, int nWinMode, ScreenMode::ScreenMode scrMode, unsigned monitor) {
	XInitThreads();
	disp = XOpenDisplay(nullptr); // 接続先ディスプレイは DISPLAY で指定
	if (disp == nullptr) throw WindowInitializationError("ディスプレイに接続出来ません");
	int screen = DefaultScreen(disp);
	hWnd = XCreateSimpleWindow(
		disp,
		RootWindow(disp, screen),
		0, 0,
		WindowWidth, WindowHeight,
		1, BlackPixel(disp, screen),
		WhitePixel(disp, screen));
	
	std::string wName(CodeConv::toANSI(WindowCaption));
	XStoreName(disp, hWnd, wName.c_str()); // ウィンドウキャプションを設定

	XSizeHints hints;
	hints.flags = PMinSize|PMaxSize; // ウィンドウサイズを固定
	hints.min_width = hints.max_width = WindowWidth;
	hints.min_height = hints.max_height = WindowHeight;
	XSetWMNormalHints(disp, hWnd, &hints);
	
	XSelectInput(disp, hWnd,
		PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
		KeyPressMask | KeyReleaseMask
		);

	wmDelMsg = XInternAtom(disp, "WM_DELETE_WINDOW", False);
	XMapWindow(disp, hWnd);
	XSetWMProtocols(disp, hWnd, &wmDelMsg, 1);
	XFlush(disp);
	return;
}
#endif /*_WIN32*/

#ifdef _WIN32
MainWindow::MainWindow(HINSTANCE hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height,ScreenMode::ScreenMode scrMode, unsigned monitor) {
	Geometry::WindowWidth = width; Geometry::WindowHeight = height;
	initWindowClass(hThisInst, icon);
	initWindow(hThisInst, nWinMode, scrMode, monitor);
	myScreenManipulator = new ScreenManipulator(hWnd, scrMode == ScreenMode::scrModeFullscreen);
	myInputManipulator = new input::InputManipulator(hWnd, scrMode == ScreenMode::scrModeFullscreen);
#ifndef WITH_DIRECTX
	myScreenManipulator->Render();
	myScreenManipulator->preloadTextures();
#endif /*WITH_DIRECTX*/
}
#else /*_WIN32*/
MainWindow::MainWindow(void* hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height, ScreenMode::ScreenMode scrMode, unsigned monitor) {
	Geometry::WindowWidth = width; Geometry::WindowHeight = height;
	initWindow(hThisInst, nWinMode, scrMode, monitor);
	myScreenManipulator = new ScreenManipulator(disp, hWnd, scrMode == ScreenMode::scrModeFullscreen);
	myScreenManipulator->Render();
	myScreenManipulator->preloadTextures();
}
#endif /*_WIN32*/

MainWindow::~MainWindow() {
	delete myScreenManipulator; myScreenManipulator = nullptr;
	UnloadAllTextures();
#ifndef _WIN32
	XDestroyWindow(disp, hWnd);
	XCloseDisplay(disp);
	std::exit(0);
#endif /*_WIN32*/
}

void MainWindow::Render() { // ウィンドウの再描画
	if (myScreenManipulator) {
		myScreenManipulator->Render();
#ifdef _WIN32
		// Windowsのみ。Linuxでは別の箇所で入力イベントを扱うので不要
		ValidateRect(hWnd, nullptr);
		if (myInputManipulator)
			myScreenManipulator->inputProc(myInputManipulator);
#endif /*_WIN32*/
	}
	return;
}

void MainWindow::transit(sceneID scene) { // シーン切り替え
	if (myScreenManipulator)
		myScreenManipulator->transit(scene);
}

void MainWindow::subscene(unsigned int subsceneID) { // サブシーン切り替え
	if (myScreenManipulator)
		myScreenManipulator->subscene(subsceneID);
}

}
