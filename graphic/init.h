#ifndef GRAPHIC_INIT_H
#define GRAPHIC_INIT_H

#include <Windows.h>
#include <tchar.h>

#ifdef GRAPHIC_EXPORTS
#include "window.h"
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon);
EXPORT void RefreshWindow();
EXPORT void CleanupWindow();

#endif
