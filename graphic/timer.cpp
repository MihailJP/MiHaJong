#include "timer.h"

namespace mihajong_graphic {

TimerMicrosec Timer::currTime() {
	return getCurrentTime();
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
