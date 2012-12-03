#ifndef MIHAJONG_MAIN_H
#define MIHAJONG_MAIN_H

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdlib>
#include "../mjcore/mjimport.h"
#include "../graphic/graphic.h"
#include "resource.h"

#if defined(__GNUC__) && defined(_WIN32) && defined(_UNICODE) /* Workaround for MinGW */
#define MINGW_UNICODE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
#endif

#endif
