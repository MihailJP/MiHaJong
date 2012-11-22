#ifndef SOUND_DLL_LOGGER_H
#define SOUND_DLL_LOGGER_H

#include <Windows.h>
#include <string>

#ifndef DLL
#ifdef SOUND_EXPORTS
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif /*SOCKET_EXPORTS*/
#endif /*DLL*/

namespace sound {
namespace logger {

#ifdef SOUND_EXPORTS
typedef int (__cdecl *CHRPPROC)(const char* a);
extern HMODULE logger;
extern CHRPPROC fpInitLogger, fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC;
#endif

DLL void setLoggerLib (HMODULE lib);

#ifdef SOUND_EXPORTS
std::string posPrefix(const char* file, int line, std::string msg);
std::string posPrefix(const char* file, int line, char* msg);

void trace_msg(const char* msg);
void debug_msg(const char* msg);
void info_msg(const char* msg);
void warn_msg(const char* msg);
void error_msg(const char* msg);
void fatal_msg(const char* msg);
#endif

}
}

#ifdef SOUND_EXPORTS
#define trace(msg) sound::logger::trace_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define debug(msg) sound::logger::debug_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define info(msg) sound::logger::info_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define warn(msg) sound::logger::warn_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define error(msg) sound::logger::error_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define fatal(msg) sound::logger::fatal_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#endif

#endif
