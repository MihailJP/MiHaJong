# This file checks for a Ruby module

#
# MH_CHECK_RUBY_MODULE([MODULE-NAME], [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
AC_DEFUN([MH_CHECK_RUBY_MODULE], [
	AC_MSG_CHECKING([for Ruby module $1])
	AS_IF([echo "require '$1'" | ruby 2>/dev/null], [
		AC_MSG_RESULT([yes])
		$2
	], [
		AC_MSG_RESULT([no])
		$3
	])
])
