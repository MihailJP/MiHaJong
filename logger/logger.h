#pragma once

#include <sys/timeb.h>
#include <iomanip>
#include "../common/strcode.h"

using namespace std; using namespace CodeConv;

enum class LogLevel {trace, debug, info, warn, error, fatal, off};

class LogMsg {
private:
	LogLevel logLevel;
	tstring messageTxt;
	_timeb issueTime;
	LogMsg() : logLevel(LogLevel::off) { memset(&issueTime, 0, sizeof issueTime); }
protected:
	LogMsg( LogLevel ll, const tstring& txt ) {
		this->logLevel = ll;
		this->messageTxt = tstring(txt);
		_ftime_s(&this->issueTime);
	}
	LogMsg( LogLevel ll, LPCTSTR txt ) {
		this->logLevel = ll;
		this->messageTxt = tstring(txt);
		_ftime_s(&this->issueTime);
	}
public:
	LogLevel msgLevel() {return logLevel;}
	bool isWorthOutputting(LogLevel ll) {return (logLevel >= ll);}
	tstring msg() {return tstring(messageTxt);}
	tstring toString() {
		tm tempus; localtime_s(&tempus, &(this->issueTime.time));
		tostringstream o;
		// 日付
		o << setw(4) << setfill(_T('0')) << (tempus.tm_year + 1900);
		o << _T("-") << setw(2) << setfill(_T('0')) << (tempus.tm_mon + 1);
		o << _T("-") << setw(2) << setfill(_T('0')) << tempus.tm_mday;
		o << _T(" ") << setw(2) << setfill(_T('0')) << tempus.tm_hour;
		o << _T(":") << setw(2) << setfill(_T('0')) << tempus.tm_min;
		o << _T(":") << setw(2) << setfill(_T('0')) << tempus.tm_sec;
		o << _T(".") << setw(3) << setfill(_T('0')) << this->issueTime.millitm;
		// ログレベル
		switch (this->logLevel) {
		case LogLevel::trace:
			o << _T(" TRACE "); break;
		case LogLevel::debug:
			o << _T(" DEBUG "); break;
		case LogLevel::info:
			o << _T(" INFO  "); break;
		case LogLevel::warn:
			o << _T(" WARN  "); break;
		case LogLevel::error:
			o << _T(" ERROR "); break;
		case LogLevel::fatal:
			o << _T(" FATAL "); break;
		default:
			o << _T(" ????? "); break;
		}
		// ログメッセージ
		o << this->messageTxt.c_str() << flush;
		return tstring(o.str());
	}
};
class TraceMsg : public LogMsg {
public:
	TraceMsg( const tstring& txt ) : LogMsg(LogLevel::trace, txt) {}
	TraceMsg( LPCTSTR txt ) : LogMsg(LogLevel::trace, txt) {}
};
class DebugMsg : public LogMsg {
public:
	DebugMsg( const tstring& txt ) : LogMsg(LogLevel::debug, txt) {}
	DebugMsg( LPCTSTR txt ) : LogMsg(LogLevel::debug, txt) {}
};
class InfoMsg : public LogMsg {
public:
	InfoMsg( const tstring& txt ) : LogMsg(LogLevel::info, txt) {}
	InfoMsg( LPCTSTR txt ) : LogMsg(LogLevel::info, txt) {}
};
class WarnMsg : public LogMsg {
public:
	WarnMsg( const tstring& txt ) : LogMsg(LogLevel::warn, txt) {}
	WarnMsg( LPCTSTR txt ) : LogMsg(LogLevel::warn, txt) {}
};
class ErrorMsg : public LogMsg {
public:
	ErrorMsg( const tstring& txt ) : LogMsg(LogLevel::error, txt) {}
	ErrorMsg( LPCTSTR txt ) : LogMsg(LogLevel::error, txt) {}
};
class FatalMsg : public LogMsg {
public:
	FatalMsg( const tstring& txt ) : LogMsg(LogLevel::fatal, txt) {}
	FatalMsg( LPCTSTR txt ) : LogMsg(LogLevel::fatal, txt) {}
};

class Logger {
public:
	static DWORD WINAPI LoggerThread(LPVOID lp);
	static void enqueue(LogMsg msgdat);
private:
	DWORD WINAPI ExecThread();
};
