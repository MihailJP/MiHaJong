#pragma once

#define WINDOWS_LEAN_AND_MEAN
#define WINDOWS_EXTRA_MEAN
#include <windows.h>
#include "except.h"

extern HINSTANCE dllInst;
extern ErrorInfo errorInfo;
extern const ULONG_PTR errorInfoPtr[1];

void translateException(unsigned int code, _EXCEPTION_POINTERS* ep);
void StackTraceToArray();
void traceLog(CONTEXT* ex, int*  addrList, int addrListSize);
LONG CALLBACK MJCore_Exception_Filter(_EXCEPTION_POINTERS *ex);
//void MJCore_Terminate_Handler();
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
