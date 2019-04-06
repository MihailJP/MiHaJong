lib_LTLIBRARIES = libmihajong-socket.la
libmihajong_socket_la_SOURCES = \
	class.cpp client.cpp logger.cpp server.cpp socket.cpp \
	class.h client.h except.h logger.h protocol.h resource.h server.h socket.h
libmihajong_socket_la_LDFLAGS = -no-undefined -release $(VERSION)
if WITH_WINAPI
libmihajong_socket_la_LIBADD = ver.o
endif
libmihajong_socket_la_CXXFLAGS = -DSOCKET_EXPORTS
EXTRA_DIST = \
	socket.vcxproj socket.vcxproj.filters
if WITH_WINAPI
libmihajong_socket_la_SOURCES += ver.rc
else
EXTRA_DIST += ver.rc
libmihajong_socket_la_CXXFLAGS += -DUNICODE -D_UNICODE
endif

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
