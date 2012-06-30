#ifndef DLLMAIN_H
#define DLLMAIN_H

#define WINDOWS_LEAN_AND_MEAN
#define WINDOWS_EXTRA_MEAN
#include <Windows.h>

extern HINSTANCE dllInst;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

#endif
