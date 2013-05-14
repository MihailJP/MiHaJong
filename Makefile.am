AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = astro socket sound graphic mjcore mihajong
EXTRA_DIST = \
	auxtools/auxtools.vcxproj auxtools/auxtools.vcxproj.filters \
	auxtools/auxtools.vcxproj.user auxtools/compress.py auxtools/compress.rb \
	auxtools/Makefile.nmake auxtools/rulesyms.rb \
	common/*.h \
	data \
	dist/dist.vcxproj dist/dist.vcxproj.filters dist/dist.vcxproj.user \
	dist/Makefile.nmake \
	logger/logger.cpp logger/logger.h logger/logger.vcxproj \
	logger/logger.vcxproj.filters logger/logger.vcxproj.user \
	logger/resource.h logger/ver.rc \
	mentzdat \
	mihajong.sln
dist_doc_DATA = \
	README \
	doc/addyaku.md \
	doc/history.txt doc/license.txt doc/playing.txt doc/readme.txt \
	doc/script.txt

aidir = $(pkgdatadir)/ai
dist_ai_DATA = data/ai/default.lua

haifudatdir = $(pkgdatadir)/haifu
dist_haifudat_DATA = \
	data/haifu/haifu.css data/haifu/haifu.dtd data/haifu/haifu.xsd \
	data/haifu/haifu.xsl
