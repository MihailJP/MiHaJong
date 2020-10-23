# This file checks for libsafec

#
# MH_LIBSAFEC_HEADERS[([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])]
#
AC_DEFUN([MH_LIBSAFEC_HEADERS], [
	AC_CHECK_HEADER([libsafec/safe_str_lib.h], $1, $2,
	[[#ifdef __cplusplus
#include <cstddef>
#define restrict /**/
#else
#include <stddef.h>
#endif
]])
	]
)

#
# MH_LIBSAFEC_LIBS[([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])]
#
AC_DEFUN([MH_LIBSAFEC_LIBS], [
	AC_SEARCH_LIBS([_wcscat_s_chk], [safec \
		safec-3.6 safec-3.5 \
		safec-3.4 safec-3.3 safec-3.2 safec-3.1 safec-3.0],
		$1, $2)
	]
)

#
# MH_LIBSAFEC[]([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])]
#
AC_DEFUN([MH_LIBSAFEC], [
	AC_ARG_WITH([safec],
		[AS_HELP_STRING([--with-safec],
			[use libsafec Safe C Library])],
		[with_safec=yes],
		[with_safec=no]
	)
	AS_IF([test "x$with_safec" != xno], [
		MH_LIBSAFEC_LIBS($1, [
			$2
			with_safec=no
		])
	])
	AS_IF([test "x$with_safec" != xno], [
		MH_LIBSAFEC_HEADERS($1, [
			$2
			with_safec=no
		])
	])
	AM_CONDITIONAL(USE_LIBSAFEC, test x$with_safec != xno)
])
