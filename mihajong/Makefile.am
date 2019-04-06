bin_PROGRAMS = mihajong mihasanm mihaysnm mihassnm mihastsm mihagbmj

libraries = ../graphic/libmihajong-graphic.la
richilib = ../mjcore/libmihajong.la
guobiaolib = ../mjcoregb/libmihagbmj.la
common_sources = main.cpp main.h resource.h

mihajong_SOURCES = ${common_sources} mihajong.cpp
mihajong_CXXFLAGS =
mihajong_LDADD = ${libraries} ${richilib}

mihasanm_SOURCES = ${common_sources} mihasanm.cpp
mihasanm_CXXFLAGS =
mihasanm_LDADD = ${libraries} ${richilib}

mihaysnm_SOURCES = ${common_sources} mihaysnm.cpp
mihaysnm_CXXFLAGS =
mihaysnm_LDADD = ${libraries} ${richilib}

mihassnm_SOURCES = ${common_sources} mihassnm.cpp
mihassnm_CXXFLAGS =
mihassnm_LDADD = ${libraries} ${richilib}

mihagbmj_SOURCES = ${common_sources} mihagbmj.cpp
mihagbmj_CXXFLAGS =
mihagbmj_LDADD = ${libraries} ${guobiaolib}

mihastsm_SOURCES = ${common_sources} mihastsm.cpp
mihastsm_CXXFLAGS =
mihastsm_LDADD = ${libraries} ${richilib}

EXTRA_DIST = \
	mihajong.vcxproj mihajong.vcxproj.filters \
	mihasanm.vcxproj mihasanm.vcxproj.filters \
	mihaysnm.vcxproj mihaysnm.vcxproj.filters \
	mihassnm.vcxproj mihassnm.vcxproj.filters \
	mihastsm.vcxproj mihastsm.vcxproj.filters \
	mihagbmj.vcxproj mihagbmj.vcxproj.filters \
	icons/mihajong.ico icons/mihasanm.ico icons/mihassnm.ico \
	icons/mihaysnm.ico icons/mihagbmj.ico

if WITH_WINAPI
mihajong_SOURCES += mihajong.rc
mihasanm_SOURCES += mihasanm.rc
mihaysnm_SOURCES += mihaysnm.rc
mihassnm_SOURCES += mihassnm.rc
mihastsm_SOURCES += mihastsm.rc
mihagbmj_SOURCES += mihagbmj.rc
if WITH_DIRECTX
mihajong_CXXFLAGS += -DWITH_DIRECTX
mihasanm_CXXFLAGS += -DWITH_DIRECTX
mihaysnm_CXXFLAGS += -DWITH_DIRECTX
mihassnm_CXXFLAGS += -DWITH_DIRECTX
mihastsm_CXXFLAGS += -DWITH_DIRECTX
mihagbmj_CXXFLAGS += -DWITH_DIRECTX
endif
else
EXTRA_DIST += mihajong.rc mihasanm.rc mihaysnm.rc mihassnm.rc
mihajong_CXXFLAGS += -DUNICODE -D_UNICODE
mihasanm_CXXFLAGS += -DUNICODE -D_UNICODE
mihaysnm_CXXFLAGS += -DUNICODE -D_UNICODE
mihassnm_CXXFLAGS += -DUNICODE -D_UNICODE
mihastsm_CXXFLAGS += -DUNICODE -D_UNICODE
mihagbmj_CXXFLAGS += -DUNICODE -D_UNICODE
endif

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
