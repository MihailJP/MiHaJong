#pragma once

#include "thread.h"
#include "chrono.h"

/* Utilities */
inline void threadSleep(unsigned int milliseconds) {
	THREADLIB::this_thread::SLEEP_FOR(CHRONO::DURATION::milliseconds(milliseconds));
}
inline void threadYield() {
	THREADLIB::this_thread::yield();
}
