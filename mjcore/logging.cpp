#include "logging.h"

namespace logger {

	using std::string;

	static HMODULE logger = nullptr;
	static CHRPPROC fpInitLogger, fpTraceC, fpDebugC, fpInfoC, fpWarnC, fpErrorC, fpFatalC = nullptr;

	__declspec(dllexport) int initLogger() {
		HMODULE lg = nullptr;
		lg = LoadLibrary((LPCSTR)"logger.dll");
		if (lg == nullptr) return (-1);
		mihajong_socket::logger::setLoggerLib(lg);
		sound::logger::setLoggerLib(lg);

		fpInitLogger = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"initLogger");
		if (lg == nullptr) return (-2);

		if (!(fpTraceC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"trace"))) return (-3);
		if (!(fpDebugC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"debug"))) return (-3);
		if (!(fpInfoC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"info"))) return (-3);
		if (!(fpWarnC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"warn"))) return (-3);
		if (!(fpErrorC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"error"))) return (-3);
		if (!(fpFatalC = (CHRPPROC)GetProcAddress(lg, (LPCSTR)"fatal"))) return (-3);
#ifdef MJCORE_EXPORTS
		std::ostringstream fname;
		fname << "debug" << GetCurrentProcessId() << ".log";
		(*fpInitLogger)(fname.str().c_str());
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
