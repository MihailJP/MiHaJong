bin_PROGRAMS = mihajong
mihajong_SOURCES = main.cpp main.h resource.h mihajong.rc
mihajong_CXXFLAGS = --input-charset=cp932
mihajong_LDADD = \
	../graphic/libmihajong-graphic.la \
	../mjcore/libmihajong.la

.rc.o:
	windres $^ -o $@ -l 0411
%.o: %.rc
	windres $^ -o $@ -l 0411
