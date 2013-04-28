#include "astro.h"
#include <math.h>
#include <float.h>

/***************************************************************************
 *
 *  Ported from Perl module Astro::MoonPhase
 *
 *  License:
 *  This program is in the public domain: "Do what thou wilt shall be the
 *  whole of the law".
 *
 *  See http://search.cpan.org/~brett/Astro-MoonPhase/MoonPhase.pm
 *
 ***************************************************************************/

/* Astronomical constants. */
#define Epoch       2444238.5    /* 1980 January 0.0 */

/* Constants defining the Sun's apparent orbit. */
#define Elonge      278.833540   /* ecliptic longitude of the Sun at epoch 1980.0 */
#define Elongp      282.596403   /* ecliptic longitude of the Sun at perigee */
#define Eccent      0.016718     /* eccentricity of Earth's orbit */
#define Sunsmax     1.495985e8   /* semi-major axis of Earth's orbit, km */
#define Sunangsiz   0.533128     /* sun's angular size, degrees, at semi-major axis distance */

/* Elements of the Moon's orbit, epoch 1980.0. */
#define Mmlong      64.975464    /* moon's mean longitude at the epoch */
#define Mmlongp     49.383063    /* mean longitude of the perigee at the epoch */
#define Mlnode      151.950429   /* mean longitude of the node at the epoch */
#define Minc        5.145396     /* inclination of the Moon's orbit */
#define Mecc        0.054900     /* eccentricity of the Moon's orbit */
#define Mangsiz     0.5181       /* moon's angular size at distance a from Earth */
#define Msmax       384401.0     /* semi-major axis of Moon's orbit in km */
#define Mparallax   0.9507       /* parallax at distance a from Earth */
#define Synmonth    29.53058868  /* synodic month (new Moon to new Moon) */

double kepler (double m, double ecc)
{ /* solve the equation of Kepler */
	double delta = 0.0;
	double e, M;

	e = (M = deg2rad(m));
	do {
		delta = 0.0 + e - ecc * sin(e) - M;
		e -= delta / (1.0 - ecc * cos(e));
	} while (fabs(delta) > 1e-6);
	return e;
}

EXPORT struct MOONPHASE calc_moon_phase (double pdate)
{ /* Calculate the moon phase of given day */
	struct MOONPHASE answer;
	double Day, N, M, Ec, Lambdasun, ml, MM, MN, Ev, Ae, A3, MmP,
		mEc, A4, lP, V, lPP, NP, y, x, Lambdamoon, BetaM,
		MoonAge, MoonPhase,
		MoonDist, MoonDFrac, MoonAng, MoonPar,
		F, SunDist, SunAng;


	/* Calculation of the Sun's position. */

	Day = pdate - Epoch; /* date within epoch */
	N = fixangle((360.0 / 365.2422) * Day); /* mean anomaly of the Sun */
	M = fixangle(N + Elonge - Elongp); /* convert from perigee co-ordinates to epoch 1980.0 */
	Ec = kepler(M, Eccent); /* solve equation of Kepler */
	Ec = sqrt((1.0 + Eccent) / (1.0 - Eccent)) * tan(Ec / 2.0);
	Ec = 2.0 * rad2deg(atan(Ec)); /* true anomaly */
	Lambdasun = fixangle(Ec + Elongp); /* Sun's geocentric ecliptic longitude */

	/* Orbital distance factor. */
	F = ((1.0 + Eccent * cos(deg2rad(Ec))) / (1.0 - Eccent * Eccent));
	SunDist = Sunsmax / F; /* distance to Sun in km */
	SunAng = F * Sunangsiz; /* Sun's angular size in degrees */


	/* Calculation of the Moon's position. */

	/* Moon's mean longitude. */
	ml = fixangle(13.1763966 * Day + Mmlong);

	/* Moon's mean anomaly. */
	MM = fixangle(ml - 0.1114041 * Day - Mmlongp);

	/* Moon's ascending node mean longitude. */
	MN = fixangle(Mlnode - 0.0529539 * Day);

	/* Evection. */
	Ev = 1.2739 * sin(deg2rad(2.0 * (ml - Lambdasun) - MM));

	/* Annual equation. */
	Ae = 0.1858 * sin(deg2rad(M));

	/* Correction term. */
	A3 = 0.37 * sin(deg2rad(M));

	/* Corrected anomaly. */
	MmP = MM + Ev - Ae - A3;

	/* Correction for the equation of the centre. */
	mEc = 6.2886 * sin(deg2rad(MmP));

	/* Another correction term. */
	A4 = 0.214 * sin(deg2rad(2.0 * MmP));

	/* Corrected longitude. */
	lP = ml + Ev + mEc - Ae + A4;

	/* Variation. */
	V = 0.6583 * sin(deg2rad(2.0 * (lP - Lambdasun)));

	/* True longitude. */
	lPP = lP + V;

	/* Corrected longitude of the node. */
	NP = MN - 0.16 * sin(deg2rad(M));

	/* Y inclination coordinate. */
	y = sin(deg2rad(lPP - NP)) * cos(deg2rad(Minc));

	/* X inclination coordinate. */
	x = cos(deg2rad(lPP - NP));

	/* Ecliptic longitude. */
	Lambdamoon = rad2deg(atan2(y, x));
	Lambdamoon += NP;

	/* Ecliptic latitude. */
	BetaM = rad2deg(asin(sin(deg2rad(lPP - NP)) * sin(deg2rad(Minc))));

	/* Calculation of the phase of the Moon. */

	/* Age of the Moon in degrees. */
	MoonAge = lPP - Lambdasun;

	/* Phase of the Moon. */
	MoonPhase = (1 - cos(deg2rad(MoonAge))) / 2.0;

	/* Calculate distance of moon from the centre of the Earth. */

	MoonDist = (Msmax * (1.0 - Mecc * Mecc)) / (1.0 + Mecc * cos(deg2rad(MmP + mEc)));

	/* Calculate Moon's angular diameter. */

	MoonDFrac = MoonDist / Msmax;
	MoonAng = Mangsiz / MoonDFrac;

	/* Calculate Moon's parallax. */

	MoonPar = Mparallax / MoonDFrac;

	answer.MoonPhase = MoonPhase;
	answer.MoonAge = Synmonth * (fixangle(MoonAge) / 360.0);
	answer.MoonDist = MoonDist;
	answer.MoonAng = MoonAng;
	answer.SunDist = SunDist;
	answer.SunAng = SunAng;
	answer.MoonIllum = fixangle(MoonAge) / 360.0;
	return answer;
}
