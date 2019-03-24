#pragma once

#include <cstdint>
#include "../astro/astro.h"

#ifndef _WIN32
#include <ctime>
#ifndef HAVE_LOCALTIME_S
#define localtime_s localtime_r
#endif /* HAVE_LOCALTIME_S */
#ifndef HAVE_GMTIME_S
#define gmtime_s gmtime_r
#endif /* HAVE_LOCALTIME_S */
#endif /*_WIN32*/

namespace DateTime {

enum Month : uint_fast8_t {
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December,
};
enum DayOfWeek : uint_fast8_t {
	Sunday = 0,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
};
struct Time {
	uint_fast16_t year;
	Month month;
	uint_fast8_t day;
	DayOfWeek dayOfWeek;
	uint_fast8_t hour, minute, second;
	uint_fast16_t millisecond;
};

inline int timeZone() { // 時差を分単位で取得する（日本は540）
#ifdef _WIN32
	TIME_ZONE_INFORMATION tz; GetTimeZoneInformation(&tz);
	return -tz.Bias;
#else /*_WIN32*/
	const time_t t1 = 86400; // GNU Cはそうではないが、time_tがunsignedの処理系を見たことがあるので86400とする
	tm tmDat;
	gmtime_s(&t1, &tmDat); // 協定世界時を算出
	const time_t t2 = mktime(&tmDat); // わざと地方時と解釈することで時差を求める
	return (t1 - t2) / 60; // 分単位で時差を返す。日本時間だったら540となる
#endif /*_WIN32*/
}

inline Time localTime() { // 地方時を取得する
#ifdef _WIN32
	SYSTEMTIME tempus; GetLocalTime(&tempus);
	const Time currTime = {
		static_cast<uint_fast16_t>(tempus.wYear),
		static_cast<Month>(tempus.wMonth),
		static_cast<uint_fast8_t>(tempus.wDay),
		static_cast<DayOfWeek>(tempus.wDayOfWeek),
		static_cast<uint_fast8_t>(tempus.wHour),
		static_cast<uint_fast8_t>(tempus.wMinute),
		static_cast<uint_fast8_t>(tempus.wSecond),
		static_cast<uint_fast16_t>(tempus.wMilliseconds),
	};
#else /*_WIN32*/
	timespec timeVal; clock_gettime(CLOCK_REALTIME, &timeVal);
	tm tempus;
	localtime_s(&timeVal.tv_sec, &tempus);
	const Time currTime = {
		static_cast<uint_fast16_t>(tempus.tm_year + 1900),
		static_cast<Month>(tempus.tm_mon + 1),
		static_cast<uint_fast8_t>(tempus.tm_mday),
		static_cast<DayOfWeek>(tempus.tm_wday),
		static_cast<uint_fast8_t>(tempus.tm_hour),
		static_cast<uint_fast8_t>(tempus.tm_min),
		static_cast<uint_fast8_t>(tempus.tm_sec),
		static_cast<uint_fast16_t>(timeVal.tv_nsec / 1'000'000),
	};
#endif /*_WIN32*/
	return currTime;
}

inline double getJulian(const Time& timeDat) {
#ifdef _WIN32
	const SYSTEMTIME timeVal = {
		timeDat.year, timeDat.month, timeDat.dayOfWeek, timeDat.day,
		timeDat.hour, timeDat.minute, timeDat.second, timeDat.millisecond,
	};
#else /*_WIN32*/
	const int month = static_cast<int>(timeDat.month) - 1;
	constexpr int startDay[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	const bool leap = (timeDat.year % 400 == 0) || ((timeDat.year % 100 != 0) && (timeDat.year % 4 == 0));
	tm timeStruct = {
		timeDat.second, timeDat.minute, timeDat.hour, timeDat.day,
		static_cast<int>(timeDat.month) - 1, static_cast<int>(timeDat.year) - 1900,
		timeDat.dayOfWeek,
		startDay[month] + timeDat.day + (leap && (month >= 2) ? 1 : 0), -1,
	};
	const timespec timeVal = {
		mktime(&timeStruct),
		static_cast<long>(timeDat.millisecond) * 1'000'000L
	};
#endif /*_WIN32*/
	return systime_to_julian(&timeVal);
}

} /* namespace */
