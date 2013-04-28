#include "event.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"

namespace mihajong_graphic {
namespace ui {

UI_Event* UIEvent = nullptr;
CancellableWait* cancellableWait = nullptr;

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

void Event::reset() {
	ResetEvent(myEvent);
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

void CancellableWait::set(DWORD retval) {
	retValue = retval;
	Event::set();
}

DWORD CancellableWait::wait(DWORD timeout) {
	DWORD result = Event::wait(timeout);
	if (result == WAIT_OBJECT_0) {
		sound::Play(sound::IDs::sndClick);
		return retValue;
	} else {
		return 0xffffffff;
	}
}

EXPORT DWORD WaitUI() {
	if (UIEvent) return UIEvent->wait();
	else return 0xcccccccc;
}

EXPORT DWORD WaitUIWithTimeout(DWORD timeout) {
	if (cancellableWait) return cancellableWait->wait(timeout);
	else return 0xcccccccc;
}

}
}
