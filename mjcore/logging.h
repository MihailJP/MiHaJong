#ifndef LOGGING_H
#define LOGGING_H

#ifndef WINDOWS_LEAN_AND_MEAN
#define WINDOWS_LEAN_AND_MEAN
#endif

#ifndef WINDOWS_EXTRA_LEAN
#define WINDOWS_EXTRA_LEAN
#endif

#include <Windows.h>
#include <string>
#include <sstream>
#include "../socket/logger.h"

namespace logger {
	typedef int (__cdecl *CHRPPROC)(const char* a);

	extern HMODULE logger;
	extern CHRPPROC fpInitLogger, fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC;

	__declspec(dllexport) int initLogger();

	std::string posPrefix(const char* file, int line, std::string msg);
	std::string posPrefix(const char* file, int line, char* msg);

	__declspec(dllexport) void trace_msg(const char* msg);
	__declspec(dllexport) void debug_msg(const char* msg);
	__declspec(dllexport) void info_msg(const char* msg);
	__declspec(dllexport) void warn_msg(const char* msg);
	__declspec(dllexport) void error_msg(const char* msg);
	__declspec(dllexport) void fatal_msg(const char* msg);
}

#define trace(msg) logger::trace_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define debug(msg) logger::debug_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define info(msg) logger::info_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define warn(msg) logger::warn_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define error(msg) logger::error_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define fatal(msg) logger::fatal_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())

#endif
