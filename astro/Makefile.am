lib_LTLIBRARIES = libmihajong-astro.la
libmihajong_astro_la_SOURCES = \
	ecllong.c julian.c mphase.c sunrise.c \
	astro.h resource.h \
	ver.rc
libmihajong_astro_la_LDFLAGS = -no-undefined
libmihajong_astro_la_LIBADD = ver.o
libmihajong_astro_la_CFLAGS = -DASTRO_EXPORTS --input-charset=cp932
EXTRA_DIST = \
	astro.vcxproj astro.vcxproj.filters astro.vcxproj.user

.rc.o:
	windres $^ -o $@ -l 0411
%.o: %.rc
	windres $^ -o $@ -l 0411
