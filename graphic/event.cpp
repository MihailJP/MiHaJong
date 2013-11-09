#include "event.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#ifndef _WIN32
#include <ctime>
#include <unistd.h>
#include <sched.h>
#endif /*_WIN32*/

namespace mihajong_graphic {
namespace ui {

UI_Event* UIEvent = nullptr;
CancellableWait* cancellableWait = nullptr;

Event::Event(bool initialStat, bool automatic) {
#ifdef _WIN32
	myEvent = CreateEvent(nullptr, !automatic, initialStat, nullptr);
	if (!myEvent) throw _T("イベントオブジェクトの初期化に失敗しました");
#else /*_WIN32*/
	isSignaled = initialStat;
	autoResetFlag = automatic;
	waitingThreads = 0;
	myEvent = PTHREAD_COND_INITIALIZER;
#endif /*_WIN32*/
}

Event::~Event() {
#ifdef _WIN32
	CloseHandle(myEvent);
#else /*_WIN32*/
	myEventMutex.syncDo<void> ([this] {
		pthread_cond_broadcast(&myEvent); // 先に全スレッドを再開させなければならない
		pthread_cond_destroy(&myEvent);
	});
#endif /*_WIN32*/
}

void Event::set() {
#ifdef _WIN32
	SetEvent(myEvent);
#else /*_WIN32*/
	myEventMutex.syncDo<void> ([this] {
		isSignaled = true;
		pthread_cond_broadcast(&myEvent);
	});
	sched_yield();
	myEventMutex.syncDo<void> ([this] {
		if (autoResetFlag && waitingThreads)
			isSignaled = false;
		waitingThreads = 0;
	});
#endif /*_WIN32*/
}

void Event::reset() {
#ifdef _WIN32
	ResetEvent(myEvent);
#else /*_WIN32*/
	myEventMutex.syncDo<void> ([this] {
		isSignaled = false;
	});
#endif /*_WIN32*/
}

#ifdef _WIN32
DWORD Event::wait(DWORD timeout) {
	DWORD result = WaitForSingleObject(myEvent, timeout);
	ResetEvent(myEvent);
	return result;
}
#else /*_WIN32*/
uint32_t Event::wait(int32_t timeout) {
	bool result;
	if (timeout == 0x7fffffff) {
		myEventMutex.syncDo<void> ([this] {
			while (!isSignaled)
				pthread_cond_wait(&myEvent, myEventMutex.getMutex());
		});
		sched_yield();
		result = true;
	} else {
		timespec destTime;
		clock_gettime(CLOCK_REALTIME, &destTime); // 絶対指定なので現在時刻を取得しなければならない
		destTime.tv_sec += timeout / 1000; // 引数の単位はミリ秒
		destTime.tv_nsec += (timeout % 1000) * 1000000;
		if (destTime.tv_nsec >= 1000000000) { // 繰り上がりの処理
			destTime.tv_sec += 1; destTime.tv_nsec -= 1000000000;
		}
		int res = myEventMutex.syncDo<int> ([this, &destTime] {
			int r = 0;
			while ((!isSignaled) && (r == 0))
				r = pthread_cond_timedwait(&myEvent, myEventMutex.getMutex(), &destTime);
			return r;
		});
		sched_yield();
		result = (res == 0);
	}
	myEventMutex.syncDo<void> ([this] {
		isSignaled = false;
	});
	return result ? 1 : 0;
}
#endif /*_WIN32*/

#ifdef _WIN32
void UI_Event::set(DWORD retval) {
#else /*_WIN32*/
void UI_Event::set(uint32_t retval) {
#endif /*_WIN32*/
	retValue = retval;
	Event::set();
}

#ifdef _WIN32
DWORD UI_Event::wait() {
#else /*_WIN32*/
uint32_t UI_Event::wait() {
#endif /*_WIN32*/
	Event::wait();
	return retValue;
}

#ifdef _WIN32
void CancellableWait::set(DWORD retval) {
#else /*_WIN32*/
void CancellableWait::set(uint32_t retval) {
#endif /*_WIN32*/
	retValue = retval;
	Event::set();
}

#ifdef _WIN32
DWORD CancellableWait::wait(DWORD timeout) {
	DWORD result = Event::wait(timeout);
	if (result == WAIT_OBJECT_0) {
#else /*_WIN32*/
uint32_t CancellableWait::wait(int32_t timeout) {
	bool result = Event::wait(timeout);
	if (result) {
#endif /*_WIN32*/
		sound::Play(sound::IDs::sndClick);
		return retValue;
	} else {
		return 0xffffffff;
	}
}

#ifdef _WIN32
EXPORT DWORD WaitUI() {
#else /*_WIN32*/
EXPORT uint32_t WaitUI() {
#endif /*_WIN32*/
	if (UIEvent) return UIEvent->wait();
	else return 0xcccccccc;
}

#ifdef _WIN32
EXPORT DWORD WaitUIWithTimeout(DWORD timeout) {
#else /*_WIN32*/
EXPORT uint32_t WaitUIWithTimeout(int32_t timeout) {
#endif /*_WIN32*/
	if (cancellableWait) return cancellableWait->wait(timeout);
	else return 0xcccccccc;
}

}
}
