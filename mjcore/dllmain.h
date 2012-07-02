#ifndef DLLMAIN_H
#define DLLMAIN_H

#define WINDOWS_LEAN_AND_MEAN
#define WINDOWS_EXTRA_MEAN
#include <Windows.h>
#include <cstdlib>
#include <exception>
#include <sstream>
#include <typeinfo>
#include "logging.h"

extern HINSTANCE dllInst;
void MJCore_Terminate_Handler();
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

#endif
