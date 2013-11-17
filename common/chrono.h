#pragma once

/* C++11 */
#ifdef WITH_BOOST_CHRONO
#include <boost/chrono.hpp>
#define CHRONO boost
#else /*WITH_BOOST_CHRONO*/
#include <chrono>
#define CHRONO std
#endif /*WITH_BOOST_CHRONO*/
#define DURATION chrono

/* Older Boost API */
#ifdef WITH_BOOST_DATE_TIME
#include <boost/date_time.hpp>
#ifdef OLDER_BOOST_SLEEP
#undef DURATION
#undef CHRONO
#define CHRONO boost
#define DURATION posix_time
#endif /*OLDER_BOOST_SLEEP*/
#endif /*WITH_BOOST_DATE_TIME*/

/* Utilities */
#if defined(THREADLIB) && defined(SLEEP_FOR)
#define SLEEP(msec) THREADLIB::this_thread::SLEEP_FOR(CHRONO::DURATION::milliseconds(msec));
#endif /* defined(THREADLIB) && defined(SLEEP_FOR) */
