# This file checks for Ogg Vorbis library

#
# MH_OGG_VORBIS_HEADERS[([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])]
#
AC_DEFUN([MH_OGG_VORBIS_HEADERS], [
	AC_CHECK_HEADER([ogg/ogg.h], $1, $2)
	AC_CHECK_HEADER([vorbis/codec.h], $1, $2)
	AC_CHECK_HEADER([vorbis/vorbisfile.h], $1, $2)
	]
)

#
# MH_OGG_VORBIS_LIBS[([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])]
#
AC_DEFUN([MH_OGG_VORBIS_LIBS], [
	AC_CHECK_LIB([ogg], [ogg_stream_init], $1, $2)
	AC_CHECK_LIB([vorbis], [vorbis_block_init], $1, $2)
	AC_CHECK_LIB([vorbisfile], [ov_open], $1, $2)
	]
)

#
# MH_OGG_VORBIS[]([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])]
#
AC_DEFUN([MH_OGG_VORBIS], [
	AC_ARG_WITH([oggvorbis],
		[AS_HELP_STRING([--without-oggvorbis],
			[disable Ogg Vorbis support as background music])],
		[with_oggvorbis=no],
		[with_oggvorbis=yes]
	)
	AS_IF([test "x$with_oggvorbis" != xno], [
		MH_OGG_VORBIS_LIBS($1, [
			$2
			with_oggvorbis=no
		])
	])
	AS_IF([test "x$with_oggvorbis" != xno], [
		MH_OGG_VORBIS_HEADERS($1, [
			$2
			with_oggvorbis=no
		])
	])
	AM_CONDITIONAL(VORBIS_SUPPORT, test x$with_oggvorbis != xno)
])
