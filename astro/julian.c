#include "astro.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <time.h>
#include <sys/time.h>
#endif /* _WIN32 */

double time_to_julian(int year, int month, int day, int hour, int min, int sec, int msec)
{ /* Calculate Julian day from given date and time */
	return
		floor(365.25 * (double)year - (month < 3 ? 1.0 : 0.0)) +
		floor((double)(year - (month < 3 ? 1.0 : 0.0)) / 400.0) -
		floor((double)(year - (month < 3 ? 1.0 : 0.0)) / 100.0) +
		floor(30.59 * (double)(month < 3 ? month + 10 : month - 2)) +
		(double)day - 678912.0 + 2400000.5 +
		(double)hour / 24.0 +
		(double)min / 24.0 / 60.0 +
		(double)sec / 24.0 / 3600.0 +
		(double)msec / 24.0 / 3600000.0;
}

#ifdef _WIN32
EXPORT double systime_to_julian(const LPSYSTEMTIME sysTime)
{ /* Calculate Julian day from given SYSTEMTIME struct */
	SYSTEMTIME time_utc;

	if (sysTime == NULL) GetSystemTime(&time_utc);
	else memcpy(&time_utc, sysTime, sizeof(SYSTEMTIME));

	return time_to_julian(
		time_utc.wYear, time_utc.wMonth, time_utc.wDay,
		time_utc.wHour, time_utc.wMinute, time_utc.wSecond,
		time_utc.wMilliseconds);
}
#else /* _WIN32 */
EXPORT double systime_to_julian(const struct timespec* sysTime)
{ /* Calculate Julian day from given SYSTEMTIME struct */
	struct timespec target_time;
	struct tm time_utc;

	if (sysTime == NULL) clock_gettime(CLOCK_REALTIME, &target_time);
	else memcpy(&target_time, sysTime, sizeof(struct timespec));

	memcpy(&time_utc, gmtime(&target_time.tv_sec), sizeof(struct tm));

	return time_to_julian(
		time_utc.tm_year + 1900, time_utc.tm_mon + 1, time_utc.tm_mday,
		time_utc.tm_hour, time_utc.tm_min, time_utc.tm_sec,
		target_time.tv_nsec / 1000000);
}
#endif /* _WIN32 */
