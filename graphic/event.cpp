#include "event.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "../common/thread.h"
#include "../common/chrono.h"
#include "../common/sleep.h"

namespace mihajong_graphic {
namespace ui {

UI_Event* UIEvent = nullptr;
CancellableWait* cancellableWait = nullptr;

Event::Event(bool initialStat, bool automatic) {
	isSignaled = initialStat;
	autoResetFlag = automatic;
	waitingThreads = 0;
}

Event::~Event() {
	isSignaled = true;
	myEvent.notify_all();
}

void Event::set() {
	{ MUTEXLIB::unique_lock<MUTEXLIB::mutex> lock(myEventMutex);
		isSignaled = true;
		myEvent.notify_all();
	}
	threadYield();
	{ MUTEXLIB::unique_lock<MUTEXLIB::mutex> lock(myEventMutex);
		if (autoResetFlag && waitingThreads)
			isSignaled = false;
		waitingThreads = 0;
	}
}

void Event::reset() {
	MUTEXLIB::unique_lock<MUTEXLIB::mutex> lock(myEventMutex);
	isSignaled = false;
}

uint32_t Event::wait(int32_t timeout) {
	bool result;
	if (timeout == Infinite) {
		{ MUTEXLIB::unique_lock<MUTEXLIB::mutex> lock(myEventMutex);
			while (!isSignaled)
				myEvent.wait(lock);
		}
		threadYield();
		result = true;
	} else {
		bool res = [this, timeout] {
			bool r = false;
			MUTEXLIB::unique_lock<MUTEXLIB::mutex> lock(myEventMutex);
			while ((!isSignaled) && (r == false))
#ifdef OLDER_BOOST_WAIT
				r = myEvent.timed_wait(lock, CHRONO::posix_time::milliseconds(timeout)) == false;
#else
				r = myEvent.wait_for(lock, CHRONO::chrono::milliseconds(timeout)) == CONDVAR::cv_status::timeout;
#endif
			return r;
		} ();
		threadYield();
		result = (res == false);
	}
	{ MUTEXLIB::unique_lock<MUTEXLIB::mutex> lock(myEventMutex);
		isSignaled = false;
	}
	return result ? 1 : 0;
}

void UI_Event::set(uint32_t retval) {
	retValue = retval;
	Event::set();
}

uint32_t UI_Event::wait() {
	Event::wait();
	return retValue;
}

void CancellableWait::set(uint32_t retval) {
	retValue = retval;
	Event::set();
}

uint32_t CancellableWait::wait(int32_t timeout) {
	bool result = Event::wait(timeout);
	if (result) {
		sound::Play(sound::IDs::sndClick);
		return retValue;
	} else {
		return 0xffffffff;
	}
}

EXPORT uint32_t WaitUI() {
	if (UIEvent) return UIEvent->wait();
	else return 0xcccccccc;
}

EXPORT uint32_t WaitUIWithTimeout(int32_t timeout) {
	if (cancellableWait) return cancellableWait->wait(timeout);
	else return 0xcccccccc;
}

}
}
