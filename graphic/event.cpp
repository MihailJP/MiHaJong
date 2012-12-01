#include "event.h"

namespace mihajong_graphic {
namespace ui {

UI_Event* UIEvent = nullptr;

Event::Event(bool initialStat, bool automatic) {
	myEvent = CreateEvent(nullptr, !automatic, initialStat, nullptr);
	if (!myEvent) throw _T("イベントオブジェクトの初期化に失敗しました");
}

Event::~Event() {
	CloseHandle(myEvent);
}

void Event::set() {
	SetEvent(myEvent);
}

DWORD Event::wait(DWORD timeout) {
	DWORD result = WaitForSingleObject(myEvent, timeout);
	ResetEvent(myEvent);
	return result;
}

void UI_Event::set(DWORD retval) {
	retValue = retval;
	Event::set();
}

DWORD UI_Event::wait() {
	Event::wait();
	return retValue;
}

EXPORT DWORD WaitUI() {
	if (UIEvent) return UIEvent->wait();
	else return 0xcccccccc;
}

}
}
