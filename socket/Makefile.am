lib_LTLIBRARIES = libmihajong-socket.la
libmihajong_socket_la_SOURCES = \
	class.cpp client.cpp logger.cpp server.cpp socket.cpp \
	class.h client.h except.h logger.h protocol.h resource.h server.h socket.h
libmihajong_socket_la_LDFLAGS = -no-undefined
if WITH_WINAPI
libmihajong_socket_la_LIBADD = ver.o
endif
libmihajong_socket_la_CXXFLAGS = -DSOCKET_EXPORTS --input-charset=cp932
EXTRA_DIST = \
	socket.vcxproj socket.vcxproj.filters socket.vcxproj.user
if WITH_WINAPI
libmihajong_socket_la_SOURCES += ver.rc
else
EXTRA_DIST += ver.rc
endif

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
