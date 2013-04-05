#pragma once

#include "exports.h"
#include <Windows.h>
#include <tchar.h>

namespace mihajong_graphic {
namespace ui {
	
#ifdef GRAPHIC_EXPORTS
class Event { // イベントの基底クラス
private:
	Event(const Event&) {}
protected:
	HANDLE myEvent;
public:
	Event(bool initialStat = false, bool automatic = false);
	virtual ~Event() = 0;
	virtual void set();
	void reset();
	virtual DWORD wait(DWORD timeout = INFINITE);
};

class UI_Event : public Event { // UIの入力が完了したかどうかを表すイベント
private:
	UI_Event(const UI_Event&) {}
	DWORD retValue;
public:
	UI_Event() : Event(false, false) {}
	~UI_Event() {}
	void set(DWORD retval);
	DWORD wait();
};

class CancellableWait : public Event { // UIの入力が完了したかどうかを表すイベント
private:
	CancellableWait(const UI_Event&) {}
	DWORD retValue;
public:
	CancellableWait() : Event(false, false) {}
	~CancellableWait() {}
	void set(DWORD retval);
	DWORD wait(DWORD timeout);
};

extern UI_Event* UIEvent;
extern CancellableWait* cancellableWait;
#endif

EXPORT DWORD WaitUI();
EXPORT DWORD WaitUIWithTimeout(DWORD timeout);

}
}
