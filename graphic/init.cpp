#include "init.h"
#include "event.h"
#if defined(_WIN32) && !defined(WITH_DIRECTX)
#include <gdiplus.h>
using namespace Gdiplus;
#endif
#ifndef _WIN32
#include <iostream>
#endif

namespace mihajong_graphic {

#ifdef _WIN32
HINSTANCE GraphicDLL = nullptr;
#endif /*_WIN32*/
MainWindow* myMainWindow = nullptr;

#if defined(_WIN32) && !defined(WITH_DIRECTX)
GdiplusStartupInput gdiplusInput;
ULONG_PTR gdiplusToken;
#endif

#ifdef _WIN32
EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon, HWND* hwndPtr, unsigned width, unsigned height, bool fullscreen) {
#else /*_WIN32*/
EXPORT bool InitWindow(void* hInstance, int nCmdShow, LPCTSTR icon, Window* hwndPtr, unsigned width, unsigned height, bool fullscreen) {
#endif /*_WIN32*/
	/* ウィンドウの初期化 */
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	GdiplusStartup(&gdiplusToken, &gdiplusInput, nullptr);
#endif
	try {
		myMainWindow = new MainWindow(hInstance, nCmdShow, icon, width, height, fullscreen);
		ui::UIEvent = new ui::UI_Event();
		ui::cancellableWait = new ui::CancellableWait();
		if (hwndPtr) *hwndPtr = myMainWindow->gethwnd();
	}
#ifdef _WIN32
	catch (LPTSTR e) {
		MessageBox(nullptr, e, _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
#else /*_WIN32*/
	catch (LPTSTR e) {
		std::cerr << e << std::endl;
		return false;
	}
	return true;
#endif /*_WIN32*/
}

EXPORT void RefreshWindow() {
	myMainWindow->Render();
}

#ifdef _WIN32
EXPORT BOOL Transit(sceneID scene) try {
	if (!myMainWindow) throw _T("ウィンドウが初期化されていません");
	myMainWindow->transit(scene);
	return TRUE;
}
catch (LPTSTR e) {
	MessageBox(nullptr, e, _T("Error"), MB_OK | MB_ICONERROR);
	return FALSE;
}

EXPORT BOOL Subscene(unsigned int subsceneID) try {
	if (!myMainWindow) throw _T("ウィンドウが初期化されていません");
	myMainWindow->subscene(subsceneID);
	return TRUE;
}
catch (LPTSTR e) {
	MessageBox(nullptr, e, _T("Error"), MB_OK | MB_ICONERROR);
	return FALSE;
}
#else /*_WIN32*/
EXPORT bool Transit(sceneID scene) try {
	if (!myMainWindow) throw _T("ウィンドウが初期化されていません");
	myMainWindow->transit(scene);
	return true;
}
catch (LPTSTR e) {
	return false;
}

EXPORT bool Subscene(unsigned int subsceneID) try {
	if (!myMainWindow) throw _T("ウィンドウが初期化されていません");
	myMainWindow->subscene(subsceneID);
	return true;
}
catch (LPTSTR e) {
	return false;
}
#endif /*_WIN32*/

EXPORT void CleanupWindow() {
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	GdiplusShutdown(gdiplusToken);
#endif
	delete myMainWindow;
	delete ui::UIEvent;
	delete ui::cancellableWait;
}

#ifndef _WIN32
EXPORT bool Event() {
	return MainWindow::WinProc(myMainWindow); // TODO: 仮置き
}
#endif /*_WIN32*/

}

#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		mihajong_graphic::GraphicDLL = hinstDLL;
		break;
	case DLL_PROCESS_DETACH:
		mihajong_graphic::GraphicDLL = nullptr;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
#endif /*_WIN32*/
