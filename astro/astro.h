#ifndef ASTRO_H
#define ASTRO_H

#ifdef ASTRO_EXPORTS
#pragma once
#define EXPORT __declspec(dllexport)

#ifndef deg2rad
#include <math.h>
#define deg2rad(deg) ((deg) * M_PI / 180.0)
#endif /* deg2rad */

#ifndef rad2deg
#include <math.h>
#define rad2deg(deg) ((deg) * 180.0 / M_PI)
#endif /* rad2deg */

#else /* ASTRO_EXPORTS */
#define EXPORT __declspec(dllimport)
#endif /* ASTRO_EXPORTS */

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif /* _MSC_VER */

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	/* julian.c */
	EXPORT double systime_to_julian(const LPSYSTEMTIME);
	EXPORT void time_to_julian_hsp(double*, int, int, int, int, int, int, int);

	/* ecllong.c */
	EXPORT double sun_ecliptic_longitude (double jday);
	EXPORT void sun_ecliptic_longitude_hsp (double* l, double jday);

#ifdef __cplusplus
}
#endif

#endif
