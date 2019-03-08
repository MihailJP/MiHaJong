lib_LTLIBRARIES = libmihagbmj.la
libmihagbmj_la_SOURCES = \
	../mjcore/agari.cpp ../mjcore/decomp.cpp ../mjcore/discard.cpp ../mjcore/dllmain.cpp ../mjcore/endround.cpp \
	../mjcore/envtbl.cpp ../mjcore/func.cpp ../mjcore/fuuro.cpp ../mjcore/gameloop.cpp ../mjcore/gametbl.cpp ../mjcore/haifu.cpp ../mjcore/init.cpp \
	../mjcore/logging.cpp ../mjcore/main.cpp ../mjcore/mouda.cpp ../mjcore/prepare.cpp ../mjcore/random.cpp \
	../mjcore/rankval.cpp ../mjcore/remote.cpp ../mjcore/result.cpp ../mjcore/shanten.cpp ../mjcore/ruletbl.cpp ../mjcore/sound.cpp \
	../mjcore/tileutil.cpp \
	../mjcore/ai/aiscript.cpp ../mjcore/ai/functbl.cpp ../mjcore/ai/filesel.cpp ../mjcore/ai/gftbl.cpp ../mjcore/ai/riskchk.cpp \
	../mjcore/conffile/conffile.cpp \
	../mjcore/reader/csv2arry.cpp ../mjcore/reader/ini2map.cpp ../mjcore/reader/readrsrc.cpp \
	../mjcore/yaku/catalog.cpp ../mjcore/yaku/suittbl.cpp ../mjcore/yaku/yaku.cpp ../mjcore/yaku/ykclass.cpp \
	../mjcore/yaku/yk_tools.cpp \
	../mjcore/yaku/catalog/context.cpp ../mjcore/yaku/catalog/dora.cpp ../mjcore/yaku/catalog/irreg.cpp \
	../mjcore/yaku/catalog/misc.cpp ../mjcore/yaku/catalog/pinhu.cpp ../mjcore/yaku/catalog/post.cpp \
	../mjcore/yaku/catalog/quad.cpp ../mjcore/yaku/catalog/sequence.cpp ../mjcore/yaku/catalog/suit.cpp \
	../mjcore/yaku/catalog/triplet1.cpp ../mjcore/yaku/catalog/triplet2.cpp \
	../mjcore/yaku/catalog/triplet3.cpp ../mjcore/yaku/catalog/yaojiu.cpp \
	../mjcore/yaku/catalog/shisanbd.cpp \
	../mjcore/agari.h ../mjcore/confitms.h ../mjcore/decomp.h ../mjcore/discard.h ../mjcore/dllmain.h ../mjcore/endround.h \
	../mjcore/endtype.h ../mjcore/envtbl.h ../mjcore/except.h ../mjcore/func.h ../mjcore/fuuro.h ../mjcore/gameloop.h ../mjcore/gametbl.h ../mjcore/haifu.h \
	../mjcore/init.h ../mjcore/logging.h ../mjcore/main.h ../mjcore/mjexport.h ../mjcore/mjimport.h ../mjcore/mouda.h ../mjcore/prepare.h \
	../mjcore/random.h ../mjcore/rankval.h ../mjcore/rational.h ../mjcore/regex.h ../mjcore/remote.h ../mjcore/result.h ../mjcore/shanten.h \
	../mjcore/resource.h ../mjcore/ruletbl.h ../mjcore/socknum.h ../mjcore/sound.h ../mjcore/tilecode.h ../mjcore/tileutil.h \
	../mjcore/ai/aiscript.h ../mjcore/ai/class.h ../mjcore/ai/functbl.h ../mjcore/ai/filesel.h \
	../mjcore/conffile/conffile.h \
	../mjcore/lzma/Alloc.c ../mjcore/lzma/LzFind.c ../mjcore/lzma/LzmaDec.c ../mjcore/lzma/LzmaLib.c ../mjcore/lzma/Sha256.c \
	../mjcore/lzma/Alloc.h ../mjcore/lzma/LzFind.h ../mjcore/lzma/LzHash.h ../mjcore/lzma/LzmaDec.h ../mjcore/lzma/LzmaLib.h \
	../mjcore/lzma/RotateDefs.h ../mjcore/lzma/Sha256.h ../mjcore/lzma/Types.h \
	../mjcore/reader/csv2arry.h ../mjcore/reader/ini2map.h ../mjcore/reader/readrsrc.h \
	../mjcore/yaku/catalog.h ../mjcore/yaku/yaku.h ../mjcore/yaku/yvalue.h ../mjcore/yaku/ykclass.h ../mjcore/yaku/yk_tools.h
nodist_libmihagbmj_la_SOURCES = \
	../mjcore/data/confitem_csv.cpp ../mjcore/data/confitem_ini.cpp ../mjcore/data/rankval_csv.cpp \
	../mjcore/data/confitem_csv.h ../mjcore/data/confitem_ini.h ../mjcore/data/rankval_csv.h
BUILT_SOURCES = $(nodist_libmihagbmj_la_SOURCES)
libmihagbmj_la_LDFLAGS = -no-undefined -release $(VERSION)
libmihagbmj_la_CXXFLAGS = -DGUOBIAO -DMJCORE_EXPORTS
if WITH_BOOST_REGEX
libmihagbmj_la_CXXFLAGS += -DWITH_BOOST_REGEX $(BOOST_REGEX_LIB)
endif
if WITH_BOOST_CHRONO
libmihagbmj_la_CXXFLAGS += -DWITH_BOOST_CHRONO $(BOOST_CHRONO_LIB)
endif
if WITH_BOOST_DATE_TIME
libmihagbmj_la_CXXFLAGS += -DWITH_BOOST_DATE_TIME $(BOOST_DATE_TIME_LIB)
endif
if WITH_BOOST_THREAD
libmihagbmj_la_CXXFLAGS += -DWITH_BOOST_THREAD $(BOOST_THREAD_LIB)
endif
if OLDER_BOOST_SLEEP
libmihagbmj_la_CXXFLAGS += -DOLDER_BOOST_SLEEP
endif
if WITH_WINAPI
if WITH_DIRECTX
libmihagbmj_la_CXXFLAGS += -DWITH_DIRECTX
endif
else
libmihagbmj_la_CXXFLAGS += -DPKGDATADIR='"$(pkgdatadir)"' -DUNICODE -D_UNICODE
endif
libmihagbmj_la_LIBADD = \
	$(LUA_LIB) \
	../sound/libmihajong-sound.la \
	../graphic/libmihajong-graphic.la \
	../socket/libmihajong-socket.la \
	../astro/libmihajong-astro.la
if WITH_WINAPI
libmihagbmj_la_LIBADD += mjcoregb.o 
endif
dist_pkgdata_DATA = \
	../mjcore/data/mentz.dat.lzma
nodist_pkgdata_DATA = \
	../mjcore/data/confitem.csv.lzma ../mjcore/data/confitem.ini.lzma ../mjcore/data/rankval.csv.lzma
EXTRA_DIST = \
	mjcoregb.vcxproj mjcoregb.vcxproj.filters \
	../mjcore/lua/README \
	../mjcore/data/confitem.csv ../mjcore/data/confitem.ini ../mjcore/data/rankval.csv
if WITH_WINAPI
libmihagbmj_la_SOURCES += \
	../mjcore/lzma/LzFindMt.c ../mjcore/lzma/Threads.c ../mjcore/lzma/LzFindMt.h ../mjcore/lzma/Threads.h \
	mjcoregb.rc
EXTRA_DIST += \
	../mjcore/filenum.cpp ../mjcore/filenum.h
else
EXTRA_DIST += \
	../mjcore/lzma/LzFindMt.c ../mjcore/lzma/Threads.c ../mjcore/lzma/LzFindMt.h ../mjcore/lzma/Threads.h \
	mjcoregb.rc
libmihagbmj_la_SOURCES += \
	../mjcore/filenum.cpp ../mjcore/filenum.h
endif

CLEANFILES = $(libmihagbmj_la_BUILT_SOURCES) \
	../mjcore/data/confitem.csv.lzma ../mjcore/data/confitem.ini.lzma ../mjcore/data/rankval.csv.lzma

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411

../mjcore/data/confitem.csv.lzma: ../mjcore/data/confitem.csv
	../auxtools/compress.rb ../mjcore/data/confitem.csv
../mjcore/data/confitem_csv.h: ../mjcore/data/confitem.csv.lzma
	true
../mjcore/data/confitem_csv.cpp: ../mjcore/data/confitem.csv.lzma
	true

../mjcore/data/confitem.ini.lzma: ../mjcore/data/confitem.ini
	../auxtools/compress.rb ../mjcore/data/confitem.ini
../mjcore/data/confitem_ini.h: ../mjcore/data/confitem.ini.lzma
	true
../mjcore/data/confitem_ini.cpp: ../mjcore/data/confitem.ini.lzma
	true

../mjcore/data/rankval.csv.lzma: ../mjcore/data/rankval.csv
	../auxtools/compress.rb ../mjcore/data/rankval.csv
../mjcore/data/rankval_csv.h: ../mjcore/data/rankval.csv.lzma
	true
../mjcore/data/rankval_csv.cpp: ../mjcore/data/rankval.csv.lzma
	true
