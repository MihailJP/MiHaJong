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
