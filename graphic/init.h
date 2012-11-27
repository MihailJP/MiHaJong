#ifndef GRAPHIC_INIT_H
#define GRAPHIC_INIT_H

#include <Windows.h>
#include <tchar.h>
#include "exports.h"

#ifdef GRAPHIC_EXPORTS
#include "window.h"
extern HINSTANCE GraphicDLL;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
#endif

EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon);
EXPORT void RefreshWindow();
EXPORT void CleanupWindow();

#endif
