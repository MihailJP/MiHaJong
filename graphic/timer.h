#pragma once

#include "../common/clock.h"

namespace mihajong_graphic {

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
