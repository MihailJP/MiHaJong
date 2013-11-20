lib_LTLIBRARIES = libmihajong-astro.la
libmihajong_astro_la_SOURCES = \
	ecllong.c julian.c mphase.c sunrise.c \
	astro.h resource.h
libmihajong_astro_la_LDFLAGS = -no-undefined -release $(VERSION)
if WITH_WINAPI
libmihajong_astro_la_LIBADD = ver.o
endif
libmihajong_astro_la_CFLAGS = -DASTRO_EXPORTS
EXTRA_DIST = \
	astro.vcxproj astro.vcxproj.filters astro.vcxproj.user
if WITH_WINAPI
libmihajong_astro_la_SOURCES += ver.rc
else
EXTRA_DIST += ver.rc
libmihajong_astro_la_CFLAGS += -DUNICODE -D_UNICODE
endif

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
