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
ClickEvent* clickEvent = nullptr;

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
	{ std::unique_lock<std::mutex> lock(myEventMutex);
		isSignaled = true;
		myEvent.notify_all();
	}
	threadYield();
	{ std::unique_lock<std::mutex> lock(myEventMutex);
		if (autoResetFlag && waitingThreads)
			isSignaled = false;
		waitingThreads = 0;
	}
}

void Event::reset() {
	std::unique_lock<std::mutex> lock(myEventMutex);
	isSignaled = false;
}

uint32_t Event::wait(int32_t timeout) {
	bool result;
	if (timeout == Infinite) {
		{ std::unique_lock<std::mutex> lock(myEventMutex);
			while (!isSignaled)
				myEvent.wait(lock);
		}
		threadYield();
		result = true;
	} else {
		bool res = [this, timeout] {
			bool r = false;
			std::unique_lock<std::mutex> lock(myEventMutex);
			while ((!isSignaled) && (r == false))
				r = myEvent.wait_for(lock, CHRONO::chrono::milliseconds(timeout)) == CONDVAR::cv_status::timeout;
			return r;
		} ();
		threadYield();
		result = (res == false);
	}
	{ std::unique_lock<std::mutex> lock(myEventMutex);
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

uint32_t ClickEvent::wait() {
	bool result = Event::wait(0);
	if (result) {
		sound::Play(sound::IDs::sndClick);
		return 0;
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

EXPORT void CheckIfDemoTerminated() {
	if (clickEvent) {
		if (clickEvent->wait() == 0) {
			if (UIEvent) UIEvent->reset();
			if (cancellableWait) cancellableWait->reset();
			throw DemonstrationTerminated();
		}
	}
}

}
}
