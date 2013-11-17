#pragma once

#ifdef WITH_BOOST_THREAD
#include <boost/thread.hpp>
#define THREADLIB boost
#ifdef OLDER_BOOST_SLEEP
#define SLEEP_FOR sleep
#else /*OLDER_BOOST_SLEEP*/
#define SLEEP_FOR sleep_for
#endif /*OLDER_BOOST_SLEEP*/
#else /*WITH_BOOST_THREAD*/
#include <thread>
#define THREADLIB std
#define SLEEP_FOR sleep_for
#endif /*WITH_BOOST_THREAD*/
