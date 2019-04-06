#pragma once

#include <thread>
#include <chrono>

/* Utilities */
inline void threadSleep(unsigned int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
inline void threadYield() {
	std::this_thread::yield();
}
