#pragma once

#ifdef _WIN32
#ifndef WINDOWS_LEAN_AND_MEAN
#define WINDOWS_LEAN_AND_MEAN
#endif

#ifndef WINDOWS_EXTRA_LEAN
#define WINDOWS_EXTRA_LEAN
#endif

#include <windows.h>
#endif /*_WIN32*/
#include "../common/strcode.h"

#ifdef _WIN32
namespace logger {
	typedef int (__cdecl *CHRPPROC)(LPCSTR a);
	typedef int (__cdecl *TCHRPPROC)(LPCTSTR a);

	int initLogger();

	CodeConv::tstring posPrefix(const char* file, int line, CodeConv::tstring msg);
	CodeConv::tstring posPrefix(const char* file, int line, LPCTSTR msg);

	void trace_msg(LPCTSTR msg);
	void debug_msg(LPCTSTR msg);
	void info_msg(LPCTSTR msg);
	void warn_msg(LPCTSTR msg);
	void error_msg(LPCTSTR msg);
	void fatal_msg(LPCTSTR msg);
}

#define trace(msg) ::logger::trace_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define debug(msg) ::logger::debug_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define info(msg) ::logger::info_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define warn(msg) ::logger::warn_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define error(msg) ::logger::error_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define fatal(msg) ::logger::fatal_msg(::logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())

#else /*_WIN32*/

#define trace(msg) /* Unsupported */
#define debug(msg) /* Unsupported */
#define info(msg) /* Unsupported */
#define warn(msg) /* Unsupported */
#define error(msg) /* Unsupported */
#define fatal(msg) /* Unsupported */

#endif /*_WIN32*/
