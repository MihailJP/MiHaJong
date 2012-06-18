#include "logger.h"

/* �ȈՃ��M���O�c�[�� */

using namespace std;

static Logger* logger = NULL;
static DWORD threadID;
static HANDLE hThread;
static CRITICAL_SECTION cs;
static HANDLE hEvent;
static ostream* logStream = &cout;
static ofstream logFile;

static deque<LogMsg> msgQueue;

DWORD Logger::LoggerThread(LPVOID lp) { // ���M���O���s�p�X���b�h
	while (1) {
		/* �ҋ@ */
		WaitForSingleObject(hEvent, INFINITE);
		/* �L���[����pop���� */
		EnterCriticalSection(&cs);
		LogMsg currentLogMsg = LogMsg(msgQueue[0]);
		msgQueue.pop_front();
		LeaveCriticalSection(&cs);
		/* �����i�������j */
		*logStream << currentLogMsg.toString() << endl;
	}
}

void Logger::enqueue(LogMsg msgdat) { // �L���[��push����
	EnterCriticalSection(&cs);
	msgQueue.push_back(LogMsg(msgdat));
	LeaveCriticalSection(&cs);
	SetEvent(hEvent);
}

extern "C" {

/* ���������� */

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
	InitializeCriticalSection(&cs);
	hThread = CreateThread(NULL, 0, Logger::LoggerThread, (LPVOID)logger,
		CREATE_SUSPENDED, &threadID);
	ResumeThread(hThread);
}

/* ���b�Z�[�W���L���[�ɓo�^ */

__declspec(dllexport) void trace(const char* msg) { logger->enqueue(TraceMsg(msg)); }
__declspec(dllexport) void debug(const char* msg) { logger->enqueue(DebugMsg(msg)); }
__declspec(dllexport) void info(const char* msg) { logger->enqueue(InfoMsg(msg)); }
__declspec(dllexport) void warn(const char* msg) { logger->enqueue(WarnMsg(msg)); }
__declspec(dllexport) void error(const char* msg) { logger->enqueue(ErrorMsg(msg)); }
__declspec(dllexport) void fatal(const char* msg) { logger->enqueue(FatalMsg(msg)); }

}
