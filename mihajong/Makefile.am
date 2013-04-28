bin_PROGRAMS = mihajong mihasanm mihaysnm mihassnm

libraries = \
	../graphic/libmihajong-graphic.la \
	../mjcore/libmihajong.la
common_sources = main.cpp main.h resource.h

mihajong_SOURCES = ${common_sources} mihajong.cpp mihajong.rc
mihajong_CXXFLAGS = --input-charset=cp932
mihajong_LDADD = ${libraries}

mihasanm_SOURCES = ${common_sources} mihasanm.cpp mihasanm.rc
mihasanm_CXXFLAGS = --input-charset=cp932
mihasanm_LDADD = ${libraries}

mihaysnm_SOURCES = ${common_sources} mihaysnm.cpp mihaysnm.rc
mihaysnm_CXXFLAGS = --input-charset=cp932
mihaysnm_LDADD = ${libraries}

mihassnm_SOURCES = ${common_sources} mihassnm.cpp mihassnm.rc
mihassnm_CXXFLAGS = --input-charset=cp932
mihassnm_LDADD = ${libraries}

EXTRA_DIST = \
	mihajong.vcxproj mihajong.vcxproj.filters mihajong.vcxproj.user \
	mihasanm.vcxproj mihasanm.vcxproj.filters mihasanm.vcxproj.user \
	mihaysnm.vcxproj mihaysnm.vcxproj.filters mihaysnm.vcxproj.user \
	mihassnm.vcxproj mihassnm.vcxproj.filters mihassnm.vcxproj.user \
	icons/mihajong.ico icons/mihasanm.ico icons/mihassnm.ico \
	icons/mihaysnm.ico

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
