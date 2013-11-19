# This file checks which function to use for thread waiting
# Must run AX_BOOST_BASE first

AC_DEFUN([MH_TEST_WAIT], [
	CPPFLAGS_TMP=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
	LDFLAGS_TMP=$LDFLAGS
	LDFLAGS="$LDFLAGS_TMP -lboost_thread -lboost_chrono"
# boost::condition_variable::wait_for (for newer version)
	AC_MSG_CHECKING([for boost::condition_variable::wait_for])
	AC_RUN_IFELSE(
		[ AC_LANG_SOURCE([[
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
int main() {
	boost::mutex mutex;
	boost::unique_lock<boost::mutex> lock(mutex);
	boost::condition_variable cond;
	cond.wait_for(lock, boost::chrono::milliseconds(500));
	return 0;
}
]])
		],
		[AC_MSG_RESULT([yes])
		 CPPFLAGS=$CPPFLAGS_TMP
		 LDFLAGS=$LDFLAGS_TMP
		 wait_func=wait_for],
		[AC_MSG_RESULT([no])
# boost::condition_variable::timed_wait (for older version)
		 AC_MSG_CHECKING([for boost::condition_variable::timed_wait])
		 LDFLAGS="$LDFLAGS_TMP -lboost_thread -lboost_date_time"
		 AC_RUN_IFELSE(
			[ AC_LANG_SOURCE([[
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
int main() {
	boost::mutex mutex;
	boost::unique_lock<boost::mutex> lock(mutex);
	boost::condition_variable cond;
	cond.timed_wait(lock, boost::posix_time::milliseconds(500));
	return 0;
}
]])
			],
			[AC_MSG_RESULT([yes])
			 CPPFLAGS=$CPPFLAGS_TMP
			 LDFLAGS=$LDFLAGS_TMP
			 wait_func=timed_wait],
			[AC_MSG_RESULT([no])
			 CPPFLAGS=$CPPFLAGS_TMP
			 LDFLAGS=$LDFLAGS_TMP
			 AC_MSG_ERROR([no functions available for thread waiting])
			]
		 )
		]
	)
	]
)
