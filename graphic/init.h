#pragma once

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else /*_WIN32*/
#include "../common/strcode.h"
#include <X11/Xlib.h>
#endif /*_WIN32*/
#include "exports.h"
#include "scenes/scene_id.h"
#include "../common/scrmode.h"

#include "window.h"
#if defined(_WIN32) && defined(GRAPHIC_EXPORTS)
namespace mihajong_graphic {
	extern HINSTANCE GraphicDLL;
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
#endif

namespace mihajong_graphic {

#ifdef _WIN32
EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon, HWND* hwndPtr,
	unsigned width, unsigned height, Screen_Mode::ScreenMode scrMode, unsigned monitor);
EXPORT void RefreshWindow();
EXPORT void WaitForWindowInit();
EXPORT BOOL Transit(SceneID scene);
EXPORT BOOL Subscene(SubSceneID subsceneID);
EXPORT void CleanupWindow();
#else /*_WIN32*/
EXPORT bool InitWindow(void* hInstance, int nCmdShow, LPCTSTR icon, Window* hwndPtr,
	unsigned width, unsigned height, Screen_Mode::ScreenMode scrMode, unsigned monitor);
EXPORT void RefreshWindow();
EXPORT void WaitForWindowInit();
EXPORT bool Transit(SceneID scene);
EXPORT bool Subscene(SubSceneID subsceneID);
EXPORT void CleanupWindow();
EXPORT bool Event();
#endif /*_WIN32*/

}
