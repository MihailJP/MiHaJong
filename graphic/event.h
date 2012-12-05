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

extern UI_Event* UIEvent;
#endif

EXPORT DWORD WaitUI();

}
}
