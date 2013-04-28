# This file checks for Direct3D headers (for MinGW only)

AC_DEFUN([MH_CHECK_DIRECT3D], [
	AC_CHECK_HEADER([d3d9.h], [], [AC_MSG_ERROR([d3d9.h is unavailable])])
	AC_CHECK_HEADER([d3dx9.h], [], [AC_MSG_ERROR([d3dx9.h is unavailable])],
	                [[
#ifndef DECLARE_INTERFACE_IID_
#define DECLARE_INTERFACE_IID_(i,b,d) DECLARE_INTERFACE_(i,b)
#endif
]]
	 )
	AC_CHECK_HEADER([dxerr9.h], [], [AC_MSG_ERROR([dxerr9.h is unavailable])],
	                [[
#include <windows.h>
]]
	 )]
)
