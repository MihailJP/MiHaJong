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

extern UI_Event* UIEvent;
#endif

EXPORT DWORD WaitUI();

}
}
