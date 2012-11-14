#include "logger.h"

namespace mihajong_socket {
namespace logger {

HMODULE logger = nullptr;
CHRPPROC fpInitLogger = nullptr;
TCHRPPROC fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC = nullptr;

DLL void setLoggerLib (HMODULE lib) {
	logger = lib;
	fpTraceC = (TCHRPPROC)GetProcAddress(logger, (LPCSTR)"trace");
	fpDebugC = (TCHRPPROC)GetProcAddress(logger, (LPCSTR)"debug");
	fpInfoC = (TCHRPPROC)GetProcAddress(logger, (LPCSTR)"info");
	fpWarnC = (TCHRPPROC)GetProcAddress(logger, (LPCSTR)"warn");
	fpErrorC = (TCHRPPROC)GetProcAddress(logger, (LPCSTR)"error");
	fpFatalC = (TCHRPPROC)GetProcAddress(logger, (LPCSTR)"fatal");
}

CodeConv::tstring posPrefix(LPCTSTR file, int line, CodeConv::tstring msg) {
	CodeConv::tostringstream o;
	o << _T("(")
		<< ( (CodeConv::tstring(file).rfind(_T("\\"))) == CodeConv::tstring::npos ?
		file : file + (CodeConv::tstring(file).rfind(_T("\\"))) + 1 )
		<< _T(":") << line << _T(") ") << msg << std::flush;
	return CodeConv::tstring(o.str());
}
CodeConv::tstring posPrefix(LPCTSTR file, int line, LPCTSTR msg) {
	return posPrefix(file, line, CodeConv::tstring(msg));
}

void trace_msg(LPCTSTR msg) { if (fpTraceC) (*fpTraceC)(msg); }
void debug_msg(LPCTSTR msg) { if (fpDebugC) (*fpDebugC)(msg); }
void info_msg(LPCTSTR msg) { if (fpInfoC) (*fpInfoC)(msg); }
void warn_msg(LPCTSTR msg) { if (fpWarnC) (*fpWarnC)(msg); }
void error_msg(LPCTSTR msg) { if (fpErrorC) (*fpErrorC)(msg); }
void fatal_msg(LPCTSTR msg) { if (fpFatalC) (*fpFatalC)(msg); }

}
}
