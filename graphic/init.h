#ifndef GRAPHIC_INIT_H
#define GRAPHIC_INIT_H

#include <Windows.h>
#include <tchar.h>
#include "exports.h"
#include "scenes/scene_id.h"

#ifdef GRAPHIC_EXPORTS
#include "window.h"
namespace mihajong_graphic {
	extern HINSTANCE GraphicDLL;
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
#endif

namespace mihajong_graphic {

EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon, HWND* hwndPtr);
EXPORT void RefreshWindow();
EXPORT BOOL Transit(sceneID scene);
EXPORT void CleanupWindow();

}
#endif
