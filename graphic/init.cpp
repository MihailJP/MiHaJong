#include "init.h"
#include "event.h"
#if defined(_WIN32) && !defined(WITH_DIRECTX)
#include <gdiplus.h>
using namespace Gdiplus;
#endif
#ifndef _WIN32
#include <iostream>
#endif
#include <mutex>
#include <condition_variable>
#include "except.h"

namespace mihajong_graphic {

#ifdef _WIN32
HINSTANCE GraphicDLL = nullptr;
#endif /*_WIN32*/
MainWindow* myMainWindow = nullptr;

namespace {
	std::mutex initMutex;
	std::condition_variable condVar;
	bool initialized = false;
}

#if defined(_WIN32) && !defined(WITH_DIRECTX)
GdiplusStartupInput gdiplusInput;
ULONG_PTR gdiplusToken;
#endif

#ifdef _WIN32
EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon, HWND* hwndPtr, unsigned width, unsigned height, ScreenMode::ScreenMode scrMode, unsigned monitor)
#else /*_WIN32*/
EXPORT bool InitWindow(void* hInstance, int nCmdShow, LPCTSTR icon, Window* hwndPtr, unsigned width, unsigned height, ScreenMode::ScreenMode scrMode, unsigned monitor)
#endif /*_WIN32*/
{
	/* ウィンドウの初期化 */
	std::unique_lock<std::mutex> lock(initMutex);
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	GdiplusStartup(&gdiplusToken, &gdiplusInput, nullptr);
#endif
	try {
		myMainWindow = new MainWindow(hInstance, nCmdShow, icon, width, height, scrMode, monitor);
		ui::UIEvent = new ui::UI_Event();
		ui::cancellableWait = new ui::CancellableWait();
		ui::clickEvent = new ui::ClickEvent();
		if (hwndPtr) *hwndPtr = myMainWindow->gethwnd();
	}
#ifdef _WIN32
	catch (const GraphicModuleError& e) {
		MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
		lock.unlock(), condVar.notify_all();
		return FALSE;
	}
	initialized = true, lock.unlock(), condVar.notify_all();
	return TRUE;
#else /*_WIN32*/
	catch (const GraphicModuleError& e) {
		std::cerr << e.what() << std::endl;
		lock.unlock(), condVar.notify_all();
		return false;
	}
	initialized = true, lock.unlock(), condVar.notify_all();
	return true;
#endif /*_WIN32*/
}

EXPORT void RefreshWindow() {
	myMainWindow->Render();
}

EXPORT void WaitForWindowInit() {
	std::unique_lock<std::mutex> lock(initMutex);
	condVar.wait(lock, [] {return initialized;});
}

#ifdef _WIN32
EXPORT BOOL Transit(sceneID scene) try {
	if (!myMainWindow) throw UninitializedObject("ウィンドウが初期化されていません");
	myMainWindow->transit(scene);
	return TRUE;
}
catch (const GraphicModuleError& e) {
	MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
	return FALSE;
}

EXPORT BOOL Subscene(unsigned int subsceneID) try {
	if (!myMainWindow) throw UninitializedObject("ウィンドウが初期化されていません");
	myMainWindow->subscene(subsceneID);
	return TRUE;
}
catch (const GraphicModuleError& e) {
	MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
	return FALSE;
}
#else /*_WIN32*/
EXPORT bool Transit(sceneID scene) try {
	if (!myMainWindow) throw UninitializedObject("ウィンドウが初期化されていません");
	myMainWindow->transit(scene);
	return true;
}
catch (const GraphicModuleError& e) {
	return false;
}

EXPORT bool Subscene(unsigned int subsceneID) try {
	if (!myMainWindow) throw UninitializedObject("ウィンドウが初期化されていません");
	myMainWindow->subscene(subsceneID);
	return true;
}
catch (const GraphicModuleError& e) {
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
	return MainWindow::WinProc(myMainWindow);
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
