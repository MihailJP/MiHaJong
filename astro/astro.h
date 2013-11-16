#pragma once

#ifdef ASTRO_EXPORTS
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else /* _WIN32 */
#define EXPORT /**/
#endif /* _WIN32 */

#define PI 3.141592653589793238

#ifndef deg2rad
#define deg2rad(deg) ((deg) * PI / 180.0)
#endif /* deg2rad */

#ifndef rad2deg
#define rad2deg(rad) ((rad) * 180.0 / PI)
#endif /* rad2deg */

#ifndef fixangle
#include <math.h>
#define fixangle(deg) fmod(fmod(deg, 360.0) + 360.0, 360.0)
#endif /* fixangle */

#else /* ASTRO_EXPORTS */
#ifdef _WIN32
#define EXPORT __declspec(dllimport)
#else /* _WIN32 */
#define EXPORT /**/
#endif /* _WIN32 */
#endif /* ASTRO_EXPORTS */

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif /* _MSC_VER */

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <sys/time.h>
#endif /* _WIN32 */

struct MOONPHASE
{
	double MoonPhase; /* the terminator phase angle as a percentage of a full circle (i.e., 0 to 1) */
	double MoonIllum; /* the illuminated fraction of the Moon's disc */
	double MoonAge; /* the Moon's age in days and fraction */
	double MoonDist; /* the distance of the Moon from the centre of the Earth */
	double MoonAng; /* the angular diameter subtended by the Moon as seen by an observer at the centre of the Earth. */
	double SunDist; /* the distance from the Sun in km */
	double SunAng; /* the angular size of Sun in degrees */
};

struct SUN_RISE_SET
{
	double hour_rise_ut, hour_set_ut;
};

struct RISE_SET_HOUR
{
	int hour_rise, min_rise, hour_set, min_set;
};

#ifdef __cplusplus
extern "C" {
#endif

	/* julian.c */
#ifdef _WIN32
	EXPORT double systime_to_julian(const LPSYSTEMTIME);
#else /* _WIN32 */
	EXPORT double systime_to_julian(const struct timespec* sysTime);
#endif /* _WIN32 */

	/* ecllong.c */
	EXPORT double sun_ecliptic_longitude (double);

	/* mphase.c */
	EXPORT struct MOONPHASE calc_moon_phase (double);

	/* sunrise.c */
	EXPORT struct RISE_SET_HOUR sunrise (int, int, int, double, double, double, int, double);

#ifdef __cplusplus
}
#endif
