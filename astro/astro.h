#ifndef ASTRO_H
#define ASTRO_H

#ifdef ASTRO_EXPORTS
#pragma once
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#include <Windows.h>

/* julian.c */
EXPORT double systime_to_julian(const LPSYSTEMTIME);
EXPORT void time_to_julian_hsp(double*, int, int, int, int, int, int, int);
#endif
