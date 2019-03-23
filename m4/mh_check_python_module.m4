# This file checks for a Python module

#
# MH_CHECK_PYTHON_MODULE([MODULE-NAME], [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
AC_DEFUN([MH_CHECK_PYTHON_MODULE], [
	AC_MSG_CHECKING([for Python module $1])
	AS_IF([echo 'import $1' | python - 2>/dev/null], [
		AC_MSG_RESULT([yes])
		$2
	], [
		AC_MSG_RESULT([no])
		$3
	])
])
