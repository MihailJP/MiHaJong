#pragma once

#ifdef WITH_BOOST_THREAD
#include <boost/thread.hpp>
#define MUTEXLIB boost
#else /*WITH_BOOST_THREAD*/
#include <mutex>
#define MUTEXLIB std
#endif /*WITH_BOOST_THREAD*/
