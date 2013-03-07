#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(mihajong, 2.0.0, mihailjp@gmail.com)
LT_INIT
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([astro/astro.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext])
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([float.h stddef.h stdlib.h string.h sys/timeb.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor gethostbyname memmove memset pow socket sqrt strstr])

AC_ARG_ENABLE([midi],
	[AS_HELP_STRING([--enable-midi],
		[enable MIDI support as background music (Windows only) [default=no]])],
	[],
	[enable_midi=no]
)
AM_CONDITIONAL(MIDI_SUPPORT, test x$enable_midi != xno)

AC_CONFIG_FILES([Makefile
                 astro/Makefile
                 graphic/Makefile
                 mihajong/Makefile
                 mjcore/Makefile
                 socket/Makefile
                 sound/Makefile])
AC_OUTPUT
