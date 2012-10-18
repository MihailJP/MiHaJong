#ifndef SOCK_LOGGER_H
#define SOCK_LOGGER_H

#include "socket.h"
#include <Windows.h>

namespace mihajong_socket {
namespace logger {

#ifdef SOCKET_EXPORTS
typedef int (__cdecl *CHRPPROC)(const char* a);
extern HMODULE logger;
extern CHRPPROC fpInitLogger, fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC;
#endif

DLL void setLoggerLib (HMODULE lib);

#ifdef SOCKET_EXPORTS
void trace_msg(const char* msg);
void debug_msg(const char* msg);
void info_msg(const char* msg);
void warn_msg(const char* msg);
void error_msg(const char* msg);
void fatal_msg(const char* msg);
#endif

}
}

#ifdef SOCKET_EXPORTS
#define trace(msg) mihajong_socket::logger::trace_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define debug(msg) mihajong_socket::logger::debug_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define info(msg) mihajong_socket::logger::info_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define warn(msg) mihajong_socket::logger::warn_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define error(msg) mihajong_socket::logger::error_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#define fatal(msg) mihajong_socket::logger::fatal_msg(logger::posPrefix(__FILE__, __LINE__, (msg)).c_str())
#endif

#endif
