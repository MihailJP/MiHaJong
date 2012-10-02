#include "logging.h"

namespace logger {

	using std::string;

	static HMODULE logger = NULL;
	static CHRPPROC fpInitLogger, fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC = NULL;

	__declspec(dllexport) int initLogger() {
		HMODULE lg = NULL;
		lg = LoadLibrary((LPCSTR)"logger.dll");
		if (lg == NULL) return (-1);

		fpInitLogger = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"initLogger");
		if (lg == NULL) return (-2);

		if (!(fpTraceC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"trace"))) return (-3);
		if (!(fpDebugC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"debug"))) return (-3);
		if (!(fpInfoC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"info"))) return (-3);
		if (!(fpWarnC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"warn"))) return (-3);
		if (!(fpErrorC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"error"))) return (-3);
		if (!(fpFatalC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"fatal"))) return (-3);
#ifdef MJCORE_EXPORTS
		(*fpInitLogger)("debug.log");
#endif
		logger = lg;
		return 0;
	}

	string posPrefix(const char* file, int line, string msg) {
		std::ostringstream o;
		o << "("
			<< ( (string(file).rfind("\\")) == string::npos ?
			file : file + (string(file).rfind("\\")) + 1 )
			<< ":" << line << ") " << msg << std::flush;
		return string(o.str());
	}
	string posPrefix(const char* file, int line, char* msg) {
		std::ostringstream o;
		o << "("
			<< ( (string(file).rfind("\\")) == string::npos ?
			file : file + (string(file).rfind("\\")) + 1 )
			<< ":" << line << ") " << msg << std::flush;
		return string(o.str());
	}

	__declspec(dllexport) void trace_msg(const char* msg) { if (logger) (*fpTraceC)(msg); }
	__declspec(dllexport) void debug_msg(const char* msg) { if (logger) (*fpDebugC)(msg); }
	__declspec(dllexport) void info_msg(const char* msg) { if (logger) (*fpInfoC)(msg); }
	__declspec(dllexport) void warn_msg(const char* msg) { if (logger) (*fpWarnC)(msg); }
	__declspec(dllexport) void error_msg(const char* msg) { if (logger) (*fpErrorC)(msg); }
	__declspec(dllexport) void fatal_msg(const char* msg) { if (logger) (*fpFatalC)(msg); }

}
