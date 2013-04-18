#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */
#include <string>
#include "../common/strcode.h"

#ifndef DLL
#if !defined(_WIN32)
#define DLL /* */
#elif defined(SOUND_EXPORTS)
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
#endif /*DLL*/

#ifdef _WIN32
namespace sound {
namespace logger {

#ifdef SOUND_EXPORTS
typedef int (__cdecl *CHRPPROC)(LPCSTR a);
typedef int (__cdecl *TCHRPPROC)(LPCTSTR a);
extern HMODULE logger;
extern CHRPPROC fpInitLogger;
extern TCHRPPROC fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC;
#endif

DLL void setLoggerLib (HMODULE lib);

#ifdef SOUND_EXPORTS
CodeConv::tstring posPrefix(LPCTSTR file, int line, CodeConv::tstring msg);
CodeConv::tstring posPrefix(LPCTSTR file, int line, LPCTSTR msg);

void trace_msg(LPCTSTR msg);
void debug_msg(LPCTSTR msg);
void info_msg(LPCTSTR msg);
void warn_msg(LPCTSTR msg);
void error_msg(LPCTSTR msg);
void fatal_msg(LPCTSTR msg);
#endif

}
}

#ifdef SOUND_EXPORTS
#define trace(msg) sound::logger::trace_msg(logger::posPrefix(_T(__FILE__), __LINE__, (msg)).c_str())
#define debug(msg) sound::logger::debug_msg(logger::posPrefix(_T(__FILE__), __LINE__, (msg)).c_str())
#define info(msg) sound::logger::info_msg(logger::posPrefix(_T(__FILE__), __LINE__, (msg)).c_str())
#define warn(msg) sound::logger::warn_msg(logger::posPrefix(_T(__FILE__), __LINE__, (msg)).c_str())
#define error(msg) sound::logger::error_msg(logger::posPrefix(_T(__FILE__), __LINE__, (msg)).c_str())
#define fatal(msg) sound::logger::fatal_msg(logger::posPrefix(_T(__FILE__), __LINE__, (msg)).c_str())
#endif

#else /* _WIN32 */

#ifdef SOUND_EXPORTS
#define trace(msg) /* Not supported */
#define debug(msg) /* Not supported */
#define info(msg) /* Not supported */
#define warn(msg) /* Not supported */
#define error(msg) /* Not supported */
#define fatal(msg) /* Not supported */
#endif

#endif /* _WIN32 */
