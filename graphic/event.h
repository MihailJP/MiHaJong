#pragma once

#include "exports.h"
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else /*_WIN32*/
#include "../common/strcode.h"
#include <pthread.h>
#include "../common/mutex.h"
#endif /*_WIN32*/

namespace mihajong_graphic {
namespace ui {
	
#ifdef GRAPHIC_EXPORTS
class Event { // イベントの基底クラス
protected:
#ifdef _WIN32
	HANDLE myEvent;
#else /*_WIN32*/
	pthread_cond_t myEvent;
	MHJMutex myEventMutex;
	bool isSignaled, autoResetFlag;
	unsigned waitingThreads;
#endif /*_WIN32*/
public:
	Event(bool initialStat = false, bool automatic = false);
	Event(const Event&) = delete; // Delete unexpected copy constructor
	Event& operator= (const Event&) = delete; // Delete unexpected assign operator
	virtual ~Event() = 0;
	virtual void set();
	void reset();
#ifdef _WIN32
	virtual DWORD wait(DWORD timeout = INFINITE);
#else /*_WIN32*/
	virtual uint32_t wait(int32_t timeout = 0x7fffffff);
#endif /*_WIN32*/
};

class UI_Event : public Event { // UIの入力が完了したかどうかを表すイベント
private:
#ifdef _WIN32
	DWORD retValue;
#else /*_WIN32*/
	uint32_t retValue;
#endif /*_WIN32*/
public:
	UI_Event() : Event(false, false) {}
	UI_Event(const UI_Event&) = delete; // Delete unexpected copy constructor
	UI_Event& operator= (const UI_Event&) = delete; // Delete unexpected assign operator
	~UI_Event() {}
#ifdef _WIN32
	void set(DWORD retval);
	DWORD wait();
#else /*_WIN32*/
	void set(uint32_t retval);
	uint32_t wait();
#endif /*_WIN32*/
};

class CancellableWait : public Event { // UIの入力が完了したかどうかを表すイベント
private:
#ifdef _WIN32
	DWORD retValue;
#else /*_WIN32*/
	uint32_t retValue;
#endif /*_WIN32*/
public:
	CancellableWait() : Event(false, false) {}
	CancellableWait(const CancellableWait&) = delete; // Delete unexpected copy constructor
	CancellableWait& operator= (const CancellableWait&) = delete; // Delete unexpected assign operator
	~CancellableWait() {}
#ifdef _WIN32
	void set(DWORD retval);
	DWORD wait(DWORD timeout);
#else /*_WIN32*/
	void set(uint32_t retval);
	uint32_t wait(int32_t timeout);
#endif /*_WIN32*/
};

extern UI_Event* UIEvent;
extern CancellableWait* cancellableWait;
#endif

#ifdef _WIN32
EXPORT DWORD WaitUI();
EXPORT DWORD WaitUIWithTimeout(DWORD timeout);
#else /*_WIN32*/
EXPORT uint32_t WaitUI();
EXPORT uint32_t WaitUIWithTimeout(int32_t timeout);
#endif /*_WIN32*/

}
}
