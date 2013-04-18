#include "timer.h"
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/

namespace mihajong_graphic {

TimerMicrosec Timer::currTime() {
#ifdef _WIN32
	FILETIME t; GetSystemTimeAsFileTime(&t);
	return (((unsigned long long)t.dwHighDateTime << 32) | t.dwLowDateTime) / 10;
#else /*_WIN32*/
	/* TODO: ‚±‚±‚ÌÄŽÀ‘• */
	return 0ull;
#endif /*_WIN32*/
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
