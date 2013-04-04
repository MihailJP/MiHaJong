#pragma once

#include "exports.h"
#include <Windows.h>
#include <tchar.h>

namespace mihajong_graphic {
namespace ui {
	
#ifdef GRAPHIC_EXPORTS
class Event { // �C�x���g�̊��N���X
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

class UI_Event : public Event { // UI�̓��͂������������ǂ�����\���C�x���g
private:
	UI_Event(const UI_Event&) {}
	DWORD retValue;
public:
	UI_Event() : Event(false, false) {}
	~UI_Event() {}
	void set(DWORD retval);
	DWORD wait();
};

class CancellableWait : public Event { // UI�̓��͂������������ǂ�����\���C�x���g
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
