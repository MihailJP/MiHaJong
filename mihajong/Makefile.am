bin_PROGRAMS = mihajong
mihajong_SOURCES = main.cpp main.h resource.h mihajong.rc
mihajong_CXXFLAGS = --input-charset=cp932
mihajong_LDADD = \
	../graphic/libmihajong-graphic.la \
	../mjcore/libmihajong.la
EXTRA_DIST = \
	mihajong.vcxproj mihajong.vcxproj.filters mihajong.vcxproj.user \
	icons/mihajong.ico icons/mihasanm.ico icons/mihassnm.ico \
	icons/mihaysnm.ico

.rc.o:
	windres $^ -o $@ -l 0411
%.o: %.rc
	windres $^ -o $@ -l 0411
