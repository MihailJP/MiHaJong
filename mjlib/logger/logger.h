#ifndef LOGGER_H
#define LOGGER_H

#define WINDOWS_LEAN_AND_MEAN
#define WINDOWS_EXTRA_LEAN
#include <Windows.h>
#include <process.h>
#include <string>
#include <deque>
#include <sys/timeb.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

enum LogLevel {Trace, Debug, Info, Warn, Error, Fatal, Off};

class LogMsg {
private:
	LogLevel logLevel;
	LogMsg() {}
	string messageTxt;
	_timeb issueTime;
protected:
	LogMsg( LogLevel ll, const string& txt ) {
		this->logLevel = ll;
		this->messageTxt = string(txt);
		_ftime_s(&this->issueTime);
	}
	LogMsg( LogLevel ll, const char* txt ) {
		this->logLevel = ll;
		this->messageTxt = string(txt);
		_ftime_s(&this->issueTime);
	}
public:
	LogLevel msgLevel() {return logLevel;}
	bool isWorthOutputting(LogLevel ll) {return (logLevel >= ll);}
	string msg() {return string(messageTxt);}
	string toString() {
		tm tempus; _localtime64_s(&tempus, &(this->issueTime.time));
		ostringstream o;
		// 日付
		o << setw(4) << setfill('0') << (tempus.tm_year + 1900);
		o << "-" << setw(2) << setfill('0') << (tempus.tm_mon + 1);
		o << "-" << setw(2) << setfill('0') << tempus.tm_mday;
		o << " " << setw(2) << setfill('0') << tempus.tm_hour;
		o << ":" << setw(2) << setfill('0') << tempus.tm_min;
		o << ":" << setw(2) << setfill('0') << tempus.tm_sec;
		o << "." << setw(3) << setfill('0') << this->issueTime.millitm;
		// ログレベル
		switch (this->logLevel) {
		case Trace:
			o << " TRACE "; break;
		case Debug:
			o << " DEBUG "; break;
		case Info:
			o << " INFO  "; break;
		case Warn:
			o << " WARN  "; break;
		case Error:
			o << " ERROR "; break;
		case Fatal:
			o << " FATAL "; break;
		default:
			o << " ????? "; break;
		}
		// ログメッセージ
		o << this->messageTxt.c_str() << flush;
		return string(o.str());
	}
};
class TraceMsg : public LogMsg {
public:
	TraceMsg( const string& txt ) : LogMsg(Trace, txt) {}
	TraceMsg( const char* txt ) : LogMsg(Trace, txt) {}
};
class DebugMsg : public LogMsg {
public:
	DebugMsg( const string& txt ) : LogMsg(Debug, txt) {}
	DebugMsg( const char* txt ) : LogMsg(Debug, txt) {}
};
class InfoMsg : public LogMsg {
public:
	InfoMsg( const string& txt ) : LogMsg(Info, txt) {}
	InfoMsg( const char* txt ) : LogMsg(Info, txt) {}
};
class WarnMsg : public LogMsg {
public:
	WarnMsg( const string& txt ) : LogMsg(Warn, txt) {}
	WarnMsg( const char* txt ) : LogMsg(Warn, txt) {}
};
class ErrorMsg : public LogMsg {
public:
	ErrorMsg( const string& txt ) : LogMsg(Error, txt) {}
	ErrorMsg( const char* txt ) : LogMsg(Error, txt) {}
};
class FatalMsg : public LogMsg {
public:
	FatalMsg( const string& txt ) : LogMsg(Fatal, txt) {}
	FatalMsg( const char* txt ) : LogMsg(Fatal, txt) {}
};

class Logger {
public:
	static DWORD WINAPI LoggerThread(LPVOID lp);
	static void enqueue(LogMsg msgdat);
private:
	DWORD WINAPI ExecThread();
};

#endif
