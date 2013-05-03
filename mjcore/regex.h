#pragma once

#ifdef WITH_BOOST_REGEX
#include <boost/regex.hpp>
#define REGEX boost
#else /*WITH_BOOST_REGEX*/
#include <regex>
#define REGEX std
#endif /*WITH_BOOST_REGEX*/
