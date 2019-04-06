#pragma once

#include "thread.h"
#include <chrono>

/* Utilities */
inline void threadSleep(unsigned int milliseconds) {
	THREADLIB::this_thread::SLEEP_FOR(std::chrono::milliseconds(milliseconds));
}
inline void threadYield() {
	THREADLIB::this_thread::yield();
}
