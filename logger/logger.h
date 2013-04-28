#pragma once

#include <sys/timeb.h>
#include <iomanip>
#include "../common/strcode.h"

using namespace std; using namespace CodeConv;

enum LogLevel {Trace, Debug, Info, Warn, Error, Fatal, Off};

class LogMsg {
private:
	LogLevel logLevel;
	LogMsg() {}
	tstring messageTxt;
	_timeb issueTime;
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
		tm tempus; _localtime64_s(&tempus, &(this->issueTime.time));
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
		case Trace:
			o << _T(" TRACE "); break;
		case Debug:
			o << _T(" DEBUG "); break;
		case Info:
			o << _T(" INFO  "); break;
		case Warn:
			o << _T(" WARN  "); break;
		case Error:
			o << _T(" ERROR "); break;
		case Fatal:
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
	TraceMsg( const tstring& txt ) : LogMsg(Trace, txt) {}
	TraceMsg( LPCTSTR txt ) : LogMsg(Trace, txt) {}
};
class DebugMsg : public LogMsg {
public:
	DebugMsg( const tstring& txt ) : LogMsg(Debug, txt) {}
	DebugMsg( LPCTSTR txt ) : LogMsg(Debug, txt) {}
};
class InfoMsg : public LogMsg {
public:
	InfoMsg( const tstring& txt ) : LogMsg(Info, txt) {}
	InfoMsg( LPCTSTR txt ) : LogMsg(Info, txt) {}
};
class WarnMsg : public LogMsg {
public:
	WarnMsg( const tstring& txt ) : LogMsg(Warn, txt) {}
	WarnMsg( LPCTSTR txt ) : LogMsg(Warn, txt) {}
};
class ErrorMsg : public LogMsg {
public:
	ErrorMsg( const tstring& txt ) : LogMsg(Error, txt) {}
	ErrorMsg( LPCTSTR txt ) : LogMsg(Error, txt) {}
};
class FatalMsg : public LogMsg {
public:
	FatalMsg( const tstring& txt ) : LogMsg(Fatal, txt) {}
	FatalMsg( LPCTSTR txt ) : LogMsg(Fatal, txt) {}
};

class Logger {
public:
	static DWORD WINAPI LoggerThread(LPVOID lp);
	static void enqueue(LogMsg msgdat);
private:
	DWORD WINAPI ExecThread();
};
