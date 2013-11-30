# This file checks which function to use for thread sleeping
# Must run AX_BOOST_BASE first

AC_DEFUN([MH_TEST_SLEEP], [
	CPPFLAGS_TMP=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
	LDFLAGS_TMP=$LDFLAGS
	LDFLAGS="$LDFLAGS_TMP -lboost_system -lboost_thread -lboost_chrono"
# boost::this_thread::sleep_for (for newer version)
	AC_MSG_CHECKING([for boost::this_thread::sleep_for])
	AC_RUN_IFELSE(
		[ AC_LANG_SOURCE([[
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
int main() {
	boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	return 0;
}
]])
		],
		[AC_MSG_RESULT([yes])
		 CPPFLAGS=$CPPFLAGS_TMP
		 LDFLAGS=$LDFLAGS_TMP
		 sleep_func=sleep_for],
		[AC_MSG_RESULT([no])
# boost::this_thread::sleep (for older version)
		 AC_MSG_CHECKING([for boost::this_thread::sleep])
		 LDFLAGS="$LDFLAGS_TMP -lboost_system -lboost_thread -lboost_date_time"
		 AC_RUN_IFELSE(
			[ AC_LANG_SOURCE([[
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
int main() {
	boost::this_thread::sleep(boost::posix_time::milliseconds(500));
	return 0;
}
]])
			],
			[AC_MSG_RESULT([yes])
			 CPPFLAGS=$CPPFLAGS_TMP
			 LDFLAGS=$LDFLAGS_TMP
			 sleep_func=sleep],
			[AC_MSG_RESULT([no])
			 CPPFLAGS=$CPPFLAGS_TMP
			 LDFLAGS=$LDFLAGS_TMP
			 AC_MSG_ERROR([no functions available for thread sleeping])
			]
		 )
		]
	)
	]
)
