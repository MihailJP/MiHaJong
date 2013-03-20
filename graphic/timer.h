#pragma once

#include <cstdint>

namespace mihajong_graphic {

typedef uint64_t TimerMicrosec;

class Timer {
private:
	TimerMicrosec startTime;
public:
	TimerMicrosec currTime();
	TimerMicrosec elapsed();
	void skipTo(TimerMicrosec microseconds);
	Timer();
};

}
