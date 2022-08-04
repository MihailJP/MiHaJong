#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(mihajong, 2.4.0, mihailjp@gmail.com)
AC_LIBTOOL_WIN32_DLL
AC_DISABLE_STATIC
lt_cv_deplibs_check_method='pass_all'
LT_INIT
LT_LANG([C++])
LT_LANG([Windows Resource])
AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip subdir-objects])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([astro/astro.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_DEFINE_UNQUOTED([PKGDATADIR], ["$pkgdatadir"], [Package data directory.])

# Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_14([noext])
AC_PROG_CC
AX_PROG_LUA([5.1])

# Checks for Win32
AC_CHECK_HEADER([windows.h], [using_winapi=yes], [using_winapi=no])
AM_CONDITIONAL(WITH_WINAPI, test x$using_winapi != xno)

# Check for OSX
AC_MSG_CHECKING([for Mac])
AS_IF([test "x$host_vendor" = xapple], [
	AC_MSG_RESULT([yes])
	AS_IF([test "x$host_cpu" = xaarch64], [
		AC_MSG_ERROR([Apple silicon Mac is not supported.])
	], [
		AC_MSG_WARN([Will not run on Big Sur or newer versions of macOS.])
		AC_MSG_NOTICE([Please make sure openal-soft and mesa are installed with Homebrew.])
		AC_MSG_NOTICE([The framework shipped with Mac will not be used.])
		CPPFLAGS="$CPPFLAGS -I/usr/local/opt/openal-soft/include"
		LDFLAGS="$LDFLAGS -L/usr/local/opt/openal-soft/lib"
	])
], [AC_MSG_RESULT([no])])

# Resource editor
AS_IF([test "x$using_winapi" != xno],
	[MH_CHECK_WINDRES]
)

# DirectX option
AC_ARG_WITH([directx],
	[AS_HELP_STRING([--without-directx],
		[use OpenAL and OpenGL in lieu of DirectX (default on non-Windows systems)])],
	[],
	[with_directx=$using_winapi]
)
AS_IF([test "x$using_winapi" = xno],
	[AS_IF([test "x$with_directx" != xno],
		[AC_MSG_WARN(--with-directx is specified on non-Windows system)
		 with_directx=no]
	)]
)
AM_CONDITIONAL(WITH_DIRECTX, test x$with_directx != xno)

# Checks for libraries.
AX_LUA_LIBS([], [AC_MSG_ERROR([liblua is not installed.])])

AS_IF([test "x$using_winapi" != xno],
	[AC_CHECK_LIB([ws2_32], [main], [], AC_MSG_ERROR([ws2_32 is unavailable]))
	 AC_CHECK_LIB([ole32], [main], [], AC_MSG_ERROR([ole32 is unavailable]))
	 AC_CHECK_LIB([uuid], [main], [], AC_MSG_ERROR([uuid is unavailable]))
	 AS_IF([test "x$with_directx" != xno],
		[AC_CHECK_LIB([dsound], [main], [], AC_MSG_ERROR([dsound is unavailable]))
		 AC_CHECK_LIB([d3d9], [main], [], AC_MSG_ERROR([d3d9 is unavailable]))
		 AC_CHECK_LIB([d3dx9d], [main], [], AC_MSG_ERROR([d3dx9d is unavailable]))
		 AC_CHECK_LIB([dxerr9], [main], [], AC_MSG_ERROR([dxerr9 is unavailable]))]
	 )
	 AC_CHECK_LIB([dxguid], [main], [], AC_MSG_ERROR([dxguid is unavailable]))
	 AC_CHECK_LIB([dinput8], [main], [], AC_MSG_ERROR([dinput8 is unavailable]))
	 AC_CHECK_LIB([imm32], [main], [], AC_MSG_ERROR([imm32 is unavailable]))
	 AC_CHECK_LIB([imagehlp], [main], [], AC_MSG_ERROR([imagehlp is unavailable]))]
)
AS_IF([test "x$with_directx" = xno],
	[AS_IF([test "x$using_winapi" != xno],
		[AC_CHECK_LIB([OpenAL32], [main], [], AC_MSG_ERROR([OpenAL32 is unavailable]))
		 AC_CHECK_LIB([gdiplus], [main], [], AC_MSG_ERROR([gdiplus is unavailable]))
		 AC_CHECK_LIB([gdi32], [main], [], AC_MSG_ERROR([gdi32 is unavailable]))
		 AC_CHECK_LIB([opengl32], [main], [], AC_MSG_ERROR([opengl32 is unavailable]))],
		[AX_PTHREAD([], AC_MSG_ERROR([pthread is unavailable]))
		 AC_CHECK_LIB([openal], [main], [], AC_MSG_ERROR([openal is unavailable]))
		 AC_CHECK_LIB([png], [main], [], AC_MSG_ERROR([libpng is unavailable]))]
	 )]
)


# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h stddef.h stdlib.h string.h sys/timeb.h])
AX_LUA_HEADERS([], [AC_MSG_ERROR([liblua headers unavailable.])])
AS_IF([test "x$using_winapi" != xno],
	[AC_CHECK_HEADER([winsock2.h], [], AC_MSG_ERROR([winSock2.h is unavailable]))
	 AS_IF([test "x$with_directx" != xno],
		[AC_CHECK_HEADER([dsound.h], [], AC_MSG_ERROR([dsound.h is unavailable]))
		 MH_CHECK_DIRECT3D]
	 )
	 AC_CHECK_HEADER([dinput.h], [], AC_MSG_ERROR([dinput.h is unavailable]))
	 AC_CHECK_HEADER([imm.h], [], AC_MSG_ERROR([imm.h is unavailable]), [[
#include <windef.h>
#include <wingdi.h>
]]
	 )
	 AC_CHECK_HEADER([imagehlp.h], [], AC_MSG_ERROR([imagehlp.h is unavailable]), [[
#include <windef.h>
#include <stdarg.h>
#include <winbase.h>
#include <winver.h>
]]
	 )]
)
AS_IF([test "x$with_directx" = xno],
	[AC_CHECK_HEADER([AL/al.h], [], AC_MSG_ERROR([AL/al.h is unavailable]))
	 AC_CHECK_HEADER([AL/alc.h], [], AC_MSG_ERROR([AL/alc.h is unavailable]))
	 AS_IF([test "x$using_winapi" != xno],
		[AC_CHECK_HEADER([gdiplus.h], [], AC_MSG_ERROR([gdiplus.h is unavailable]), [[
#include <propidl.h>
]]
		 )
		 AC_CHECK_HEADER([wingdi.h], [], AC_MSG_ERROR([wingdi.h is unavailable]), [[
#include <windef.h>
]]
		 )],
		[AC_CHECK_HEADER([unistd.h], [], AC_MSG_ERROR([unistd.h is unavailable]))
		 AC_CHECK_HEADER([sys/socket.h], [], AC_MSG_ERROR([sys/socket.h is unavailable]))
		 AC_CHECK_HEADER([arpa/inet.h], [], AC_MSG_ERROR([arpa/inet.h is unavailable]))
		 AC_CHECK_HEADER([netinet/in.h], [], AC_MSG_ERROR([netinet/in.h is unavailable]))
		 AC_CHECK_HEADER([netdb.h], [], AC_MSG_ERROR([netdb.h is unavailable]))
		 AC_CHECK_HEADER([fcntl.h], [], AC_MSG_ERROR([fcntl.h is unavailable]))
		 AC_CHECK_HEADER([png.h], [], AC_MSG_ERROR([png.h is unavailable]))
		 AC_CHECK_HEADER([sys/stat.h], [], AC_MSG_ERROR([sys/stat.h is unavailable]))
		 AC_CHECK_HEADER([dirent.h], [], AC_MSG_ERROR([dirent.h is unavailable]))
		 AC_CHECK_HEADER([sched.h], [], AC_MSG_ERROR([sched.h is unavailable]))]
	 )]
)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
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
AS_IF([test "x$using_winapi" = xno],
	[AC_TYPE_PID_T]
)
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MKTIME
AS_IF([test "x$using_winapi" = xno], [
	AC_PATH_X
	AS_IF([test "x$no_x" = xyes], [AC_MSG_ERROR([X is missing])])
	AS_IF([test "x$x_includes" != x],	[CPPFLAGS="$CPPFLAGS -I$x_includes"])
	AS_IF([test "x$x_libraries" != x],	[LDFLAGS="$LDFLAGS -L$x_libraries"])
	AC_CHECK_LIB([X11], [main], [], AC_MSG_ERROR([X11 is unavailable]))
	AC_CHECK_LIB([GL], [main], [], AC_MSG_ERROR([GL is unavailable]))
	AC_CHECK_HEADER([X11/Xlib.h], [], AC_MSG_ERROR([X11/Xlib.h is unavailable]))
	AC_CHECK_HEADER([X11/Xutil.h], [], AC_MSG_ERROR([X11/Xutil.h is unavailable]))
	AC_CHECK_HEADER([X11/keysym.h], [], AC_MSG_ERROR([X11/keysym.h is unavailable]))
])
AS_IF([test "x$with_directx" = xno], [
	AC_CHECK_HEADER([GL/gl.h], [], AC_MSG_ERROR([GL/gl.h is unavailable]))
	AC_CHECK_HEADER([GL/glx.h], [], AC_MSG_ERROR([GL/glx.h is unavailable]))
])

AC_ARG_ENABLE([large_pics],
	[AS_HELP_STRING([--enable-large-pics],
		[use larger graphic data for 4K displays [default=no]])],
	[],
	[enable_large_pics=no]
)
AM_CONDITIONAL(WITH_4K_PICS, test x$enable_large_pics != xno)


AC_CHECK_FUNCS([clock_gettime floor getaddrinfo getenv_s gmtime_s localtime_s memcpy_s memmove memset mkdir pow setlocale socket sprintf_s swprintf_s sqrt strcat_s strcpy_s strstr wcscat_s wcscpy_s])


AC_ARG_ENABLE([midi],
	[AS_HELP_STRING([--enable-midi],
		[enable MIDI support as background music (Windows only) [default=no]])],
	[],
	[enable_midi=no]
)
AM_CONDITIONAL(MIDI_SUPPORT, test x$enable_midi != xno)

MH_OGG_VORBIS([], [
	AC_MSG_WARN([libogg/libvorbis library not available: disabled])
])

MH_LIBSAFEC([], [])

AC_PATH_PROG([RUBY], [ruby])
AX_PROG_RUBY_VERSION([1.9], [], [AC_MSG_ERROR([Ruby is unavailable or too old])])
AM_PATH_PYTHON([2.6], [], [AC_MSG_ERROR([Python is unavailable or too old])])
MH_CHECK_RUBY_MODULE([pathname], [], [
	AC_MSG_ERROR([Ruby module 'pathname' is unavailable. Check the installation.])
])
MH_CHECK_RUBY_MODULE([csv], [], [
	AC_MSG_ERROR([Ruby module 'csv' is unavailable. Check the installation.])
])
MH_CHECK_RUBY_MODULE([digest/sha2], [], [
	AC_MSG_ERROR([Ruby module 'digest/sha2' is unavailable. Check the installation.])
])
MH_CHECK_RUBY_MODULE([lzma], [], [
	MH_CHECK_PYTHON_MODULE([lzma], [], [
		MH_CHECK_PYTHON_MODULE([pylzma], [], [
			AC_MSG_ERROR([LZMA module not available])
		])
	])
])
MH_CHECK_PYTHON_MODULE([markdown], [], [
	AC_MSG_ERROR([Python module 'markdown' is unavailable])
])
MH_CHECK_PYTHON_MODULE([pygments], [], [
	AC_MSG_ERROR([Python module 'pygments' is unavailable])
])

AC_CONFIG_FILES([Makefile
                 astro/Makefile
                 graphic/Makefile
                 mihajong/Makefile
                 mjcore/Makefile
                 mjcoregb/Makefile
                 socket/Makefile
                 sound/Makefile])
AC_OUTPUT
