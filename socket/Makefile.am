lib_LTLIBRARIES = libmihajong-socket.la
libmihajong_socket_la_SOURCES = \
	class.cpp client.cpp logger.cpp server.cpp socket.cpp \
	class.h client.h except.h logger.h protocol.h resource.h server.h socket.h \
	ver.rc
libmihajong_socket_la_LDFLAGS = -no-undefined
libmihajong_socket_la_LIBADD = ver.o
libmihajong_socket_la_CXXFLAGS = -DSOCKET_EXPORTS --input-charset=cp932

.rc.o:
	windres $^ -o $@ -l 0411
%.o: %.rc
	windres $^ -o $@ -l 0411
