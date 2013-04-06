# This file checks for windres (for MinGW only)

AC_DEFUN([MH_CHECK_WINDRES], [
	AC_CHECK_TOOL([WINDRES], [windres], [no])
	if test "x$WINDRES" = "xno"; then
		AC_MSG_ERROR([cannot find windres])
	fi
	AC_SUBST(WINDRES)]
)
