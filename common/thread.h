#pragma once

#ifdef WITH_BOOST_THREAD
#include <boost/thread.hpp>
#define THREADLIB boost
#else /*WITH_BOOST_THREAD*/
#include <thread>
#define THREADLIB std
#endif /*WITH_BOOST_THREAD*/
