#include "astro.h"
#include <math.h>

EXPORT double sun_ecliptic_longitude (double jday)
{ /* Calculate ecliptic longitude of the sun */
	double T, O, M, L;
	T = (jday - 2451545.0) / 36525.0;
	O = deg2rad(282.9400);
	M = deg2rad(357.5256 + 35999.049 * T);
	L = O + M + (6191.2 * T + 6893.0 * sin(M) + 72.0 * sin(2.0 * M))
		/ 1296000.0 * 2.0 * M_PI;
	return fmod(rad2deg(L), 360.0);
}

EXPORT void sun_ecliptic_longitude_hsp (double* l, double jday)
{
	*l = sun_ecliptic_longitude(jday);
	return;
}
