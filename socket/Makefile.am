lib_LTLIBRARIES = libmihajong-socket.la
libmihajong_socket_la_SOURCES = \
	class.cpp client.cpp logger.cpp server.cpp socket.cpp \
	class.h client.h except.h logger.h protocol.h resource.h server.h socket.h
libmihajong_socket_la_LDFLAGS = -no-undefined -release $(VERSION)
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
libmihajong_socket_la_CXXFLAGS += -DUNICODE -D_UNICODE
endif
if WITH_BOOST_CHRONO
libmihajong_socket_la_CXXFLAGS += -DWITH_BOOST_CHRONO $(BOOST_CHRONO_LIB)
endif
if WITH_BOOST_DATE_TIME
libmihajong_socket_la_CXXFLAGS += -DWITH_BOOST_DATE_TIME $(BOOST_DATE_TIME_LIB)
endif
if WITH_BOOST_THREAD
libmihajong_socket_la_CXXFLAGS += -DWITH_BOOST_THREAD $(BOOST_THREAD_LIB)
endif
if OLDER_BOOST_SLEEP
libmihajong_socket_la_CXXFLAGS += -DOLDER_BOOST_SLEEP
endif

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
