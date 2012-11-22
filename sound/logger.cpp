#include "logger.h"
#include <sstream>

namespace sound {
namespace logger {

HMODULE logger = nullptr;
CHRPPROC fpInitLogger, fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC = nullptr;

DLL void setLoggerLib (HMODULE lib) {
	logger = lib;
	fpTraceC = (CHRPPROC)GetProcAddress(logger, (LPCSTR)"trace");
	fpDebugC = (CHRPPROC)GetProcAddress(logger, (LPCSTR)"debug");
	fpInfoC = (CHRPPROC)GetProcAddress(logger, (LPCSTR)"info");
	fpWarnC = (CHRPPROC)GetProcAddress(logger, (LPCSTR)"warn");
	fpErrorC = (CHRPPROC)GetProcAddress(logger, (LPCSTR)"error");
	fpFatalC = (CHRPPROC)GetProcAddress(logger, (LPCSTR)"fatal");
}

std::string posPrefix(const char* file, int line, std::string msg) {
	std::ostringstream o;
	o << "("
		<< ( (std::string(file).rfind("\\")) == std::string::npos ?
		file : file + (std::string(file).rfind("\\")) + 1 )
		<< ":" << line << ") " << msg << std::flush;
	return std::string(o.str());
}
std::string posPrefix(const char* file, int line, char* msg) {
	return posPrefix(file, line, std::string(msg));
}

void trace_msg(const char* msg) { if (fpTraceC) (*fpTraceC)(msg); }
void debug_msg(const char* msg) { if (fpDebugC) (*fpDebugC)(msg); }
void info_msg(const char* msg) { if (fpInfoC) (*fpInfoC)(msg); }
void warn_msg(const char* msg) { if (fpWarnC) (*fpWarnC)(msg); }
void error_msg(const char* msg) { if (fpErrorC) (*fpErrorC)(msg); }
void fatal_msg(const char* msg) { if (fpFatalC) (*fpFatalC)(msg); }

}
}
