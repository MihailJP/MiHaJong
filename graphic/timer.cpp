#include "timer.h"
#include <windows.h>

namespace mihajong_graphic {

TimerMicrosec Timer::currTime() {
	FILETIME t; GetSystemTimeAsFileTime(&t);
	return (((unsigned long long)t.dwHighDateTime << 32) | t.dwLowDateTime) / 10;
}

TimerMicrosec Timer::elapsed() {
	return currTime() - startTime;
}

void Timer::skipTo(TimerMicrosec microseconds) {
	startTime = currTime() - microseconds;
}

Timer::Timer() {
	startTime = currTime();
}

}
