#pragma once

#ifdef WITH_BOOST_THREAD
#include <boost/thread.hpp>
#define CONDVAR boost
#else /*WITH_BOOST_THREAD*/
#include <condition_variable>
#define CONDVAR std
#endif /*WITH_BOOST_THREAD*/
