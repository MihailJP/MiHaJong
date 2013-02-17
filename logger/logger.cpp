#include "logger.h"
#include "../common/mutex.h"

/* �ȈՃ��M���O�c�[�� */

//#define SYNCMODE

using namespace std;

static Logger* logger = NULL;
static DWORD threadID;
static HANDLE hThread;
static MHJMutex cs;
static HANDLE hEvent;
static ostream* logStream = &cout;
static ofstream logFile;
#ifdef SYNCMODE
static HANDLE hWriteFinishEvent;
#endif

static deque<LogMsg> msgQueue;

DWORD Logger::LoggerThread(LPVOID lp) { // ���M���O���s�p�X���b�h
	while (1) {
		/* �ҋ@ */
		WaitForSingleObject(hEvent, INFINITE);
		while (1) {
			/* �L���[����pop���� */
			LogMsg currentLogMsg = TraceMsg(_T(""));
			bool finished = cs.syncDo<bool>([&currentLogMsg]() -> bool {
				if (msgQueue.empty()) { // �L���[���󂾂����甲����
#ifdef SYNCMODE
					SetEvent(hWriteFinishEvent);
#endif
					return true;
				}
				currentLogMsg = LogMsg(msgQueue[0]);
				msgQueue.pop_front();
				return false;
			});
			if (finished) break; // �L���[���󂾂����甲����
			/* ���b�Z�[�W�������o�� */
			*logStream << EncodeStr(currentLogMsg.toString()) << endl;
		}
	}
}

void Logger::enqueue(LogMsg msgdat) { // �L���[��push����
	cs.syncDo<void>([&msgdat]() -> void {
		msgQueue.push_back(LogMsg(msgdat));
	});
	SetEvent(hEvent);
#ifdef SYNCMODE
	WaitForSingleObject(hWriteFinishEvent, INFINITE);
#endif
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
#ifdef SYNCMODE
	hWriteFinishEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
	hThread = CreateThread(NULL, 0, Logger::LoggerThread, (LPVOID)logger,
		CREATE_SUSPENDED, &threadID);
	ResumeThread(hThread);
}

/* ���b�Z�[�W���L���[�ɓo�^ */

__declspec(dllexport) void trace(LPCTSTR msg) { logger->enqueue(TraceMsg(msg)); }
__declspec(dllexport) void debug(LPCTSTR msg) { logger->enqueue(DebugMsg(msg)); }
__declspec(dllexport) void info(LPCTSTR msg) { logger->enqueue(InfoMsg(msg)); }
__declspec(dllexport) void warn(LPCTSTR msg) { logger->enqueue(WarnMsg(msg)); }
__declspec(dllexport) void error(LPCTSTR msg) { logger->enqueue(ErrorMsg(msg)); }
__declspec(dllexport) void fatal(LPCTSTR msg) { logger->enqueue(FatalMsg(msg)); }

}
