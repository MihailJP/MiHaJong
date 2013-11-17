#pragma once

#ifdef WITH_BOOST_CHRONO
#include <boost/chrono.hpp>
#define CHRONO boost
#else /*WITH_BOOST_CHRONO*/
#include <chrono>
#define CHRONO std
#endif /*WITH_BOOST_CHRONO*/
