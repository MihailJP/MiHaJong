#pragma once

#include "thread.h"
#include "chrono.h"

/* Utilities */
#define SLEEP(msec) THREADLIB::this_thread::SLEEP_FOR(CHRONO::DURATION::milliseconds(msec));
