#pragma once

#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#else /*_WIN32*/
#include <ctime>
#endif /*_WIN32*/

typedef uint64_t TimerMicrosec;

inline TimerMicrosec getCurrentTime() {
#ifdef _WIN32
	FILETIME t; GetSystemTimeAsFileTime(&t);
	return ((static_cast<unsigned long long>(t.dwHighDateTime) << 32) | t.dwLowDateTime) / 10;
#else /*_WIN32*/
	timespec t; clock_gettime(CLOCK_REALTIME, &t);
	return static_cast<unsigned long long>(t.tv_sec) * 1000000ull + static_cast<unsigned long long>(t.tv_nsec) / 1000ull;
#endif /*_WIN32*/
}
