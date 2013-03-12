#pragma once

#ifndef WINDOWS_LEAN_AND_MEAN
#define WINDOWS_LEAN_AND_MEAN
#endif

#ifndef WINDOWS_EXTRA_LEAN
#define WINDOWS_EXTRA_LEAN
#endif

#include <Windows.h>
#include "../common/strcode.h"

namespace logger {
	typedef int (__cdecl *CHRPPROC)(LPCSTR a);
	typedef int (__cdecl *TCHRPPROC)(LPCTSTR a);

	__declspec(dllexport) int initLogger();

	CodeConv::tstring posPrefix(const char* file, int line, CodeConv::tstring msg);
	CodeConv::tstring posPrefix(const char* file, int line, LPCTSTR msg);

	__declspec(dllexport) void trace_msg(LPCTSTR msg);
	__declspec(dllexport) void debug_msg(LPCTSTR msg);
	__declspec(dllexport) void info_msg(LPCTSTR msg);
	__declspec(dllexport) void warn_msg(LPCTSTR msg);
	__declspec(dllexport) void error_msg(LPCTSTR msg);
	__declspec(dllexport) void fatal_msg(LPCTSTR msg);
}

#define trace(msg) ::logger::trace_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define debug(msg) ::logger::debug_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define info(msg) ::logger::info_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define warn(msg) ::logger::warn_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define error(msg) ::logger::error_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define fatal(msg) ::logger::fatal_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
