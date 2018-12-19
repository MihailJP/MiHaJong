#include "astro.h"
#include <math.h>
#include <stdio.h>

/***************************************************************************
 *
 *  Ported from Perl module Astro::Sunrise
 *
 *  License:
 *  Here is the copyright information provided by Paul Schlyer:
 *  
 *  Written as DAYLEN.C, 1989-08-16
 *  
 *  Modified to SUNRISET.C, 1992-12-01
 *  
 *  (c) Paul Schlyter, 1989, 1992
 *  
 *  Released to the public domain by Paul Schlyter, December 1992
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in
 *  the Software without restriction, including without limitation the rights to
 *  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *  of the Software, and to permit persons to whom the Software is furnished to do
 *  so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 *  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  See http://search.cpan.org/~rkhill/Astro-Sunrise-0.91/Sunrise.pm
 *
 ***************************************************************************/

#define INV360 (1.0/360.0)
#define sind(deg) sin(deg2rad(deg))
#define cosd(deg) cos(deg2rad(deg))
#define tand(deg) tan(deg2rad(deg))
#define asind(val) rad2deg(asin(val))
#define acosd(val) rad2deg(acos(val))
#define atand(val) rad2deg(atan(val))
#define atan2d(y,x) rad2deg(atan2(y,x))

#define upper_limb 1

double revolution (double);
double rev180 (double);
struct RISE_SET_HOUR convert_hour (double, double, double, int);

struct SUN_RISE_SET;
struct SUN_RISE_SET sun_rise_set (double, double, double, double);

/**************************************************************************/
double GMST0 (double d)
{
	/*
	 *
	 * FUNCTIONAL SEQUENCE for GMST0 
	 *
	 * _GIVEN
	 * Day number
	 *
	 * _THEN
	 *
	 * computes GMST0, the Greenwich Mean Sidereal Time  
	 * at 0h UT (i.e. the sidereal time at the Greenwhich meridian at  
	 * 0h UT).  GMST is then the sidereal time at Greenwich at any     
	 * time of the day..
	 * 
	 *
	 * _RETURN
	 *
	 * Sidtime
	 */
	return revolution((180.0 + 356.0470 + 282.9404) +
		(0.9856002585 + 4.70935e-5) * d);
}

struct SUNPOS
{
	double Solar_distance, True_solar_longitude;
};
struct SUNPOS sunpos (double d)
{
	/*
	 *
	 * FUNCTIONAL SEQUENCE for sunpos
	 *
	 * _GIVEN
	 *  day number
	 *
	 * _THEN
	 *
	 * Computes the Sun's ecliptic longitude and distance
	 * at an instant given in d, number of days since
	 * 2000 Jan 0.0. 
	 * 
	 *
	 * _RETURN
	 *
	 * ecliptic longitude and distance
	 * ie. $True_solar_longitude, $Solar_distance
	 */

	struct SUNPOS ans;
	double Mean_anomaly_of_sun, Mean_longitude_of_perihelion, Eccentricity_of_Earth_orbit,
		Eccentric_anomaly, x, y, Solar_distance, True_anomaly, True_solar_longitude;

	/* Compute mean elements */
	Mean_anomaly_of_sun = revolution( 356.0470 + 0.9856002585 * d );
	Mean_longitude_of_perihelion = 282.9404 + 4.70935E-5 * d;
	Eccentricity_of_Earth_orbit  = 0.016709 - 1.151E-9 * d;

	/* Compute true longitude and radius vector */
	Eccentric_anomaly =
		Mean_anomaly_of_sun + rad2deg(Eccentricity_of_Earth_orbit) *
		sind(Mean_anomaly_of_sun) *
		( 1.0 + Eccentricity_of_Earth_orbit * cosd(Mean_anomaly_of_sun) );
	
	x = cosd(Eccentric_anomaly) - Eccentricity_of_Earth_orbit;
	
	y =
		sqrt( 1.0 - Eccentricity_of_Earth_orbit * Eccentricity_of_Earth_orbit )
		* sind(Eccentric_anomaly);
	
	Solar_distance = hypot( x, y );    /* Solar distance */
	True_anomaly = atan2d( y, x );             /* True anomaly */
	
	True_solar_longitude =
		True_anomaly + Mean_longitude_of_perihelion;    /* True solar longitude */
	
	if ( True_solar_longitude >= 360.0 )
		True_solar_longitude -= 360.0;    /* Make it 0..360 degrees */
	
	ans.Solar_distance = Solar_distance;
	ans.True_solar_longitude = True_solar_longitude;
	return ans;
}

struct SUN_RA_DEC
{
	double RA, dec;
};
struct SUN_RA_DEC sun_RA_dec (double d)
{
	/*
	 *
	 *
	 * FUNCTIONAL SEQUENCE for sun_RA_dec 
	 *
	 * _GIVEN
	 * day number, $r and $lon (from sunpos) 
	 *
	 * _THEN
	 *
	 * compute RA and dec
	 * 
	 *
	 * _RETURN
	 *
	 * Sun's Right Ascension (RA) and Declination (dec)
	 * 
	 */
	struct SUN_RA_DEC ans;
	struct SUNPOS r_lon;
	double* r = &r_lon.Solar_distance;
	double* lon = &r_lon.True_solar_longitude;
	double x, y, obl_ecl, z, RA, dec;
	
	/* Compute Sun's ecliptical coordinates */
	r_lon = sunpos(d);
	
	/* Compute ecliptic rectangular coordinates (z=0) */
	x = *r * cosd(*lon);
	y = *r * sind(*lon);
	
	/* Compute obliquity of ecliptic (inclination of Earth's axis) */
	obl_ecl = 23.4393 - 3.563E-7 * d;
	
	/* Convert to equatorial rectangular coordinates - x is unchanged */
	z = y * sind(obl_ecl);
	y = y * cosd(obl_ecl);
	
	/* Convert to spherical coordinates */
	RA  = atan2d( y, x );
	dec = atan2d( z, hypot( x, y ) );
	
	ans.RA = RA; ans.dec = dec;
	return ans;
	
}

int days_since_2000_Jan_0 (int year, int month, int day)
{
	/*
	 *
	 *
	 * FUNCTIONAL SEQUENCE for days_since_2000_Jan_0 
	 *
	 * _GIVEN
	 * year, month, day
	 *
	 * _THEN
	 *
	 * process the year month and day (counted in days)
	 * Day 0.0 is at Jan 1 2000 0.0 UT
	 * Note that ALL divisions here should be INTEGER divisions
	 *
	 * _RETURN
	 *
	 * day number
	 */
	return 367 * year - (7 * (year + ((month + 9) / 12))) / 4 +
		(275 * month) / 9 + day - 730530;
}

double revolution (double x)
{
	/*
	 *
	 * FUNCTIONAL SEQUENCE for revolution
	 *
	 * _GIVEN
	 * any angle
	 *
	 * _THEN
	 *
	 * reduces any angle to within the first revolution 
	 * by subtracting or adding even multiples of 360.0
	 * 
	 *
	 * _RETURN
	 *
	 * the value of the input is >= 0.0 and < 360.0
	 */
	return x - 360.0 * floor(x * INV360);
}

double rev180 (double x)
{
	/*
	 *
	 * FUNCTIONAL SEQUENCE for rev180
	 *
	 * _GIVEN
	 * 
	 * any angle
	 *
	 * _THEN
	 *
	 * Reduce input to within +180..+180 degrees
	 * 
	 *
	 * _RETURN
	 *
	 * angle that was reduced
	 */
	return x - 360.0 * floor(x * INV360 + 0.5);
}

struct SUN_RISE_SET sun_rise_set (double d, double lon, double lat, double altit)
{
	struct SUN_RISE_SET ans;
	struct SUN_RA_DEC srd;
	double* sRA = &srd.RA; double* sdec = &srd.dec;
	double sidtime, tsouth, sradius, cost, t;

	sidtime = revolution( GMST0(d) + 180.0 + lon );
	
	srd = sun_RA_dec(d);
	tsouth  = 12.0 - rev180( sidtime - *sRA ) / 15.0;
	sradius = 0.2666 / *sRA;

	if (upper_limb)
		altit -= sradius;

	/* Compute the diurnal arc that the Sun traverses to reach 
	   the specified altitude altit: */

	cost =
		( sind(altit) - sind(lat) * sind(*sdec) ) /
		( cosd(lat) * cosd(*sdec) );

	if ( cost >= 1.0 )
		/* Sun never rises!! */
		t = 0.0;    /* Sun always below altit */
	else if ( cost <= -1.0 )
		/* Sun never sets!! */
		t = 12.0;   /* Sun always above altit */
	else
		t = acosd(cost) / 15.0;    /* The diurnal arc, hours */

	/* Store rise and set times - in hours UT */

	ans.hour_rise_ut = tsouth - t;
	ans.hour_set_ut  = tsouth + t;
	return ans;
}

struct RISE_SET_HOUR convert_hour (double hour_rise_ut, double hour_set_ut, double TZ, int isdst)
{
	/*
	 *
	 * FUNCTIONAL SEQUENCE for convert_hour 
	 *
	 * _GIVEN
	 * Hour_rise, Hour_set, Time zone offset, DST setting
	 * hours are in UT
	 *
	 * _THEN
	 *
	 * convert to local time
	 * 
	 *
	 * _RETURN
	 *
	 * hour:min rise and set 
	 */
	struct RISE_SET_HOUR ans;
	double rise_local, set_local;

	rise_local = hour_rise_ut + TZ;
	set_local = hour_set_ut + TZ;
	if (isdst) {
		rise_local +=1;
		set_local +=1;
	}

	/* Rise and set should be between 0 and 24; */
	if (rise_local<0)
		rise_local+=24;
	else if (rise_local>=24)
		rise_local -=24;

	if (set_local<0)
		set_local+=24;
	else if (set_local>=24)
		set_local -=24;

	ans.hour_rise =  (int) rise_local;
	ans.hour_set  =  (int)set_local;

	ans.min_rise  = (int)floor((rise_local-ans.hour_rise)*60+0.5);
	ans.min_set   = (int)floor((set_local-ans.hour_set)*60+0.5);

	if (ans.min_rise>=60) {
		ans.min_rise -=60;
		ans.hour_rise+=1;
		if (ans.hour_rise>=24) ans.hour_rise-=24;
	}
	if (ans.min_set>=60) {
		ans.min_set -=60;
		ans.hour_set+=1;
		if (ans.hour_set>=24) ans.hour_set-=24;
	}

	return ans;

}

EXPORT struct RISE_SET_HOUR sunrise (int year, int month, int day, double lon, double lat, double TZ, int isdst, double altit)
{
	struct SUN_RISE_SET h;
	double* h1 = &h.hour_rise_ut; double* h2 = &h.hour_set_ut;
	int d;

	d = (int)((double)days_since_2000_Jan_0( year, month, day ) + 0.5 - lon / 360.0);
	h = sun_rise_set(d, lon, lat,altit/*,15.0*/);
	return convert_hour(*h1,*h2,TZ, isdst);
}
