#pragma once

#ifdef ASTRO_EXPORTS
#pragma once
#define EXPORT __declspec(dllexport)

#ifndef deg2rad
#include <math.h>
#define deg2rad(deg) ((deg) * M_PI / 180.0)
#endif /* deg2rad */

#ifndef rad2deg
#include <math.h>
#define rad2deg(rad) ((rad) * 180.0 / M_PI)
#endif /* rad2deg */

#ifndef fixangle
#include <math.h>
#define fixangle(deg) fmod(deg, 360.0)
#endif /* fixangle */

#else /* ASTRO_EXPORTS */
#define EXPORT __declspec(dllimport)
#endif /* ASTRO_EXPORTS */

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif /* _MSC_VER */

#include <windows.h>

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
	EXPORT double systime_to_julian(const LPSYSTEMTIME);
	EXPORT void time_to_julian_hsp(double*, int, int, int, int, int, int, int);

	/* ecllong.c */
	EXPORT double sun_ecliptic_longitude (double);
	EXPORT void sun_ecliptic_longitude_hsp (double*, double);

	/* mphase.c */
	EXPORT struct MOONPHASE calc_moon_phase (double);
	EXPORT void calc_moon_phase_hsp (double*, double);

	/* sunrise.c */
	EXPORT struct RISE_SET_HOUR sunrise (int, int, int, double, double, double, int, double);
	EXPORT void sunrise_hsp (int*, int, int, int, double, int, double, double, double);

#ifdef __cplusplus
}
#endif
