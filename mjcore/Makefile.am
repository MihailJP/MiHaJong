lib_LTLIBRARIES = libmihajong.la
libmihajong_la_SOURCES = \
	agari.cpp decomp.cpp discard.cpp dllmain.cpp endround.cpp \
	envtbl.cpp func.cpp fuuro.cpp gameloop.cpp gametbl.cpp haifu.cpp init.cpp \
	logging.cpp main.cpp mouda.cpp prepare.cpp random.cpp \
	rankval.cpp remote.cpp result.cpp shanten.cpp ruletbl.cpp sound.cpp \
	tileutil.cpp \
	ai/aiscript.cpp ai/functbl.cpp ai/filesel.cpp ai/gftbl.cpp ai/riskchk.cpp \
	conffile/conffile.cpp \
	reader/csv2arry.cpp reader/ini2map.cpp reader/readrsrc.cpp \
	yaku/catalog.cpp yaku/suittbl.cpp yaku/yaku.cpp yaku/ykclass.cpp \
	yaku/yk_tools.cpp \
	yaku/catalog/context.cpp yaku/catalog/dora.cpp yaku/catalog/irreg.cpp \
	yaku/catalog/misc.cpp yaku/catalog/pinhu.cpp yaku/catalog/post.cpp \
	yaku/catalog/quad.cpp yaku/catalog/sequence.cpp yaku/catalog/suit.cpp \
	yaku/catalog/triplet1.cpp yaku/catalog/triplet2.cpp \
	yaku/catalog/triplet3.cpp yaku/catalog/yaojiu.cpp \
	yaku/catalog/shisanbd.cpp \
	agari.h confitms.h decomp.h discard.h dllmain.h endround.h \
	endtype.h envtbl.h except.h func.h fuuro.h gameloop.h gametbl.h haifu.h \
	init.h logging.h main.h mjexport.h mjimport.h mouda.h prepare.h \
	random.h rankval.h rational.h remote.h result.h shanten.h \
	resource.h ruletbl.h socknum.h sound.h tilecode.h tileutil.h \
	ai/aiscript.h ai/class.h ai/functbl.h ai/filesel.h \
	conffile/conffile.h \
	lzma/Alloc.c lzma/LzFind.c lzma/LzmaDec.c lzma/LzmaLib.c lzma/Sha256.c \
	lzma/Alloc.h lzma/LzFind.h lzma/LzHash.h lzma/LzmaDec.h lzma/LzmaLib.h \
	lzma/RotateDefs.h lzma/Sha256.h lzma/Types.h \
	reader/csv2arry.h reader/ini2map.h reader/readrsrc.h \
	yaku/catalog.h yaku/yaku.h yaku/ykclass.h yaku/yk_tools.h
nodist_libmihajong_la_SOURCES = \
	data/confitem_csv.cpp data/confitem_ini.cpp data/rankval_csv.cpp \
	data/confitem_csv.h data/confitem_ini.h data/rankval_csv.h
BUILT_SOURCES = $(nodist_libmihajong_la_SOURCES)
libmihajong_la_LDFLAGS = -no-undefined -release $(VERSION)
libmihajong_la_CXXFLAGS = $(LUA_INCLUDE) -DMJCORE_EXPORTS -Wno-switch
if WITH_WINAPI
if WITH_DIRECTX
libmihajong_la_CXXFLAGS += -DWITH_DIRECTX
endif
else
libmihajong_la_CXXFLAGS += -DPKGDATADIR='"$(pkgdatadir)"' -DUNICODE -D_UNICODE
endif
if WITH_4K_PICS
libmihajong_la_CXXFLAGS += -DWITH_4K_PICS
endif
libmihajong_la_LIBADD = \
	$(LUA_LIB) \
	../sound/libmihajong-sound.la \
	../graphic/libmihajong-graphic.la \
	../socket/libmihajong-socket.la \
	../astro/libmihajong-astro.la
if WITH_WINAPI
libmihajong_la_LIBADD += mjcore.o
endif
dist_pkgdata_DATA = \
	data/mentz.dat.lzma
nodist_pkgdata_DATA = \
	data/confitem.csv.lzma data/confitem.ini.lzma data/rankval.csv.lzma
EXTRA_DIST = \
	mjcore.vcxproj mjcore.vcxproj.filters \
	lua/README \
	data/confitem.csv data/confitem.ini \
	data/rankval.csv
if WITH_WINAPI
libmihajong_la_SOURCES += \
	lzma/LzFindMt.c lzma/Threads.c lzma/LzFindMt.h lzma/Threads.h \
	mjcore.rc
EXTRA_DIST += \
	filenum.cpp filenum.h
else
EXTRA_DIST += \
	lzma/LzFindMt.c lzma/Threads.c lzma/LzFindMt.h lzma/Threads.h \
	mjcore.rc
libmihajong_la_SOURCES += \
	filenum.cpp filenum.h
endif

CLEANFILES = $(libmihajong_la_BUILT_SOURCES) \
	data/confitem.csv.lzma data/confitem.ini.lzma data/rankval.csv.lzma

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411

data/confitem.csv.lzma: data/confitem.csv
	../auxtools/compress.rb data/confitem.csv
data/confitem_csv.h: data/confitem.csv.lzma
	true
data/confitem_csv.cpp: data/confitem.csv.lzma
	true

data/confitem.ini.lzma: data/confitem.ini
	../auxtools/compress.rb data/confitem.ini
data/confitem_ini.h: data/confitem.ini.lzma
	true
data/confitem_ini.cpp: data/confitem.ini.lzma
	true

data/rankval.csv.lzma: data/rankval.csv
	../auxtools/compress.rb data/rankval.csv
data/rankval_csv.h: data/rankval.csv.lzma
	true
data/rankval_csv.cpp: data/rankval.csv.lzma
	true
