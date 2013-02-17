#include "logger.h"
#include <Windows.h>
#include <process.h>
#include <string>
#include <deque>
#include <sstream>
#include <iostream>
#include <fstream>

/* 簡易ロギングツール */

//#define SYNCMODE

using namespace std;

static Logger* logger = NULL;
static DWORD threadID;
static HANDLE hThread;
static CRITICAL_SECTION cs;
static HANDLE hEvent;
static ostream* logStream = &cout;
static ofstream logFile;
#ifdef SYNCMODE
static HANDLE hWriteFinishEvent;
#endif

static deque<LogMsg> msgQueue;

DWORD Logger::LoggerThread(LPVOID lp) { // ロギング実行用スレッド
	while (1) {
		/* 待機 */
		WaitForSingleObject(hEvent, INFINITE);
		while (1) {
			/* キューからpopする */
			EnterCriticalSection(&cs);
			if (msgQueue.empty()) { // キューが空だったら抜ける
				LeaveCriticalSection(&cs); // ちゃんと返してあげましょうね
#ifdef SYNCMODE
				SetEvent(hWriteFinishEvent);
#endif
				break;
			}
			LogMsg currentLogMsg = LogMsg(msgQueue[0]);
			msgQueue.pop_front();
			LeaveCriticalSection(&cs);
			/* メッセージを書き出す */
			*logStream << EncodeStr(currentLogMsg.toString()) << endl;
		}
	}
}

void Logger::enqueue(LogMsg msgdat) { // キューにpushする
	EnterCriticalSection(&cs);
	msgQueue.push_back(LogMsg(msgdat));
	LeaveCriticalSection(&cs);
	SetEvent(hEvent);
#ifdef SYNCMODE
	WaitForSingleObject(hWriteFinishEvent, INFINITE);
#endif
}

extern "C" {

/* 初期化処理 */

__declspec(dllexport) void initLogger(const char* fname) {
	logger = new Logger();
	if (fname) {
		logFile.open(fname, ios_base::app);
		logStream = &logFile;
	}
	else {
		logStream = &cout;
	}
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#ifdef SYNCMODE
	hWriteFinishEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
	InitializeCriticalSection(&cs);
	hThread = CreateThread(NULL, 0, Logger::LoggerThread, (LPVOID)logger,
		CREATE_SUSPENDED, &threadID);
	ResumeThread(hThread);
}

/* メッセージをキューに登録 */

__declspec(dllexport) void trace(LPCTSTR msg) { logger->enqueue(TraceMsg(msg)); }
__declspec(dllexport) void debug(LPCTSTR msg) { logger->enqueue(DebugMsg(msg)); }
__declspec(dllexport) void info(LPCTSTR msg) { logger->enqueue(InfoMsg(msg)); }
__declspec(dllexport) void warn(LPCTSTR msg) { logger->enqueue(WarnMsg(msg)); }
__declspec(dllexport) void error(LPCTSTR msg) { logger->enqueue(ErrorMsg(msg)); }
__declspec(dllexport) void fatal(LPCTSTR msg) { logger->enqueue(FatalMsg(msg)); }

}
