#ifndef GRAPHIC_EVENT_H
#define GRAPHIC_EVENT_H

#include "exports.h"
#include <Windows.h>
#include <tchar.h>

namespace mihajong_graphic {
namespace ui {
	
#ifdef GRAPHIC_EXPORTS
class UI_Event { // UIの入力が完了したかどうかを表すイベント
private:
	UI_Event(const UI_Event&) {};
protected:
	HANDLE myEvent;
public:
	UI_Event();
	virtual ~UI_Event();
	void set();
	void wait();
};

extern UI_Event* UIEvent;
#endif

EXPORT void WaitUI();

}
}
#endif
