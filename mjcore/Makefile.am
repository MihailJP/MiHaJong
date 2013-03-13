lib_LTLIBRARIES = libmihajong.la
libmihajong_la_SOURCES = \
	agari.cpp chat.cpp decomp.cpp discard.cpp dllmain.cpp endround.cpp \
	envtbl.cpp func.cpp fuuro.cpp gameloop.cpp gametbl.cpp haifu.cpp init.cpp \
	largenum.cpp logging.cpp main.cpp mouda.cpp prepare.cpp random.cpp \
	remote.cpp shanten.cpp ruletbl.cpp sound.cpp tileutil.cpp \
	ai/aiscript.cpp ai/functbl.cpp ai/filesel.cpp ai/gftbl.cpp ai/riskchk.cpp \
	lzma/Alloc.c lzma/LzFind.c lzma/LzFindMt.c lzma/LzmaDec.c lzma/LzmaLib.c \
	lzma/Sha256.c lzma/Threads.c \
	reader/csv2arry.cpp reader/ini2map.cpp reader/readrsrc.cpp \
	yaku/catalog.cpp yaku/suittbl.cpp yaku/yaku.cpp yaku/ykclass.cpp \
	yaku/yk_tools.cpp \
	yaku/catalog/context.cpp yaku/catalog/dora.cpp yaku/catalog/irreg.cpp \
	yaku/catalog/misc.cpp yaku/catalog/pinhu.cpp yaku/catalog/post.cpp \
	yaku/catalog/quad.cpp yaku/catalog/sequence.cpp yaku/catalog/suit.cpp \
	yaku/catalog/triplet1.cpp yaku/catalog/triplet2.cpp \
	yaku/catalog/triplet3.cpp yaku/catalog/yaojiu.cpp \
	yaku/catalog/shisanbd.cpp \
	agari.h chat.h decomp.h discard.h dllmain.h endround.h endtype.h envtbl.h \
	except.h func.h fuuro.h gameloop.h gametbl.h haifu.h init.h largenum.h \
	logging.h main.h mjexport.h mouda.h prepare.h random.h rational.h \
	remote.h shanten.h resource.h ruletbl.h socknum.h sound.h tilecode.h \
	tileutil.h \
	ai/aiscript.h ai/class.h ai/functbl.h ai/filesel.h \
	lzma/Alloc.h lzma/LzFind.h lzma/LzFindMt.h lzma/LzHash.h lzma/LzmaDec.h \
	lzma/LzmaLib.h lzma/RotateDefs.h lzma/Sha256.h lzma/Threads.h lzma/Types.h \
	reader/csv2arry.h reader/ini2map.h reader/readrsrc.h \
	yaku/catalog.h yaku/yaku.h yaku/yvalue.h yaku/ykclass.h yaku/yk_tools.h \
	mjcore.rc
nodist_libmihajong_la_SOURCES = \
	data/confitem_csv.cpp data/confitem_ini.cpp \
	data/confitem_csv.h data/confitem_ini.h
BUILT_SOURCES = $(nodist_libmihajong_la_SOURCES)
libmihajong_la_LDFLAGS = -no-undefined
libmihajong_la_CXXFLAGS = -DMJCORE_EXPORTS --input-charset=cp932
libmihajong_la_LIBADD = \
	$(LUA_LIB) mjcore.o \
	../sound/libmihajong-sound.la \
	../graphic/libmihajong-graphic.la \
	../socket/libmihajong-socket.la \
	../astro/libmihajong-astro.la

CLEANFILES = $(libmihajong_la_BUILT_SOURCES) \
	data/confitem.csv.lzma data/confitem.ini.lzma

.rc.o:
	windres $^ -o $@ -l 0411
%.o: %.rc
	windres $^ -o $@ -l 0411

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
