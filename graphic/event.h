#pragma once

#include "exports.h"
#include "../common/strcode.h"
#include <cstdint>
#include <mutex>
#include "../common/condvar.h"
#include <exception>

namespace mihajong_graphic {
namespace ui {

class DemonstrationTerminated : public std::exception {};

#ifdef GRAPHIC_EXPORTS
class Event { // イベントの基底クラス
protected:
	CONDVAR::condition_variable myEvent;
	std::mutex myEventMutex;
	bool isSignaled, autoResetFlag;
	unsigned waitingThreads;
public:
	static const int32_t Infinite = 0x7fffffff;
	Event(bool initialStat = false, bool automatic = false);
	Event(const Event&) = delete; // Delete unexpected copy constructor
	Event& operator= (const Event&) = delete; // Delete unexpected assign operator
	virtual ~Event() = 0;
	virtual void set();
	void reset();
	virtual uint32_t wait(int32_t timeout = Infinite);
};

class UI_Event : public Event { // UIの入力が完了したかどうかを表すイベント
private:
	uint32_t retValue;
public:
	UI_Event() : Event(false, false), retValue(0) {}
	UI_Event(const UI_Event&) = delete; // Delete unexpected copy constructor
	UI_Event& operator= (const UI_Event&) = delete; // Delete unexpected assign operator
	~UI_Event() {}
	void set(uint32_t retval);
	uint32_t wait();
};

class CancellableWait : public Event { // UIの入力が完了したかどうかを表すイベント
private:
	uint32_t retValue;
public:
	CancellableWait() : Event(false, false), retValue(0) {}
	CancellableWait(const CancellableWait&) = delete; // Delete unexpected copy constructor
	CancellableWait& operator= (const CancellableWait&) = delete; // Delete unexpected assign operator
	~CancellableWait() {}
	void set(uint32_t retval);
	uint32_t wait(int32_t timeout);
};

class ClickEvent : public Event { // UIの入力があったかどうかを表すイベント
public:
	ClickEvent() : Event(false, false) {}
	ClickEvent(const ClickEvent&) = delete; // Delete unexpected copy constructor
	ClickEvent& operator= (const ClickEvent&) = delete; // Delete unexpected assign operator
	~ClickEvent() {}
	uint32_t wait();
};

extern UI_Event* UIEvent;
extern CancellableWait* cancellableWait;
extern ClickEvent* clickEvent;
#endif

EXPORT uint32_t WaitUI();
EXPORT uint32_t WaitUIWithTimeout(int32_t timeout);
EXPORT void CheckIfDemoTerminated();

}
}
