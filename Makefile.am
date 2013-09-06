AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = astro socket sound graphic mjcore mjcoregb mihajong
EXTRA_DIST = \
	auxtools/auxtools.vcxproj auxtools/auxtools.vcxproj.filters \
	auxtools/auxtools.vcxproj.user auxtools/compress.py auxtools/compress.rb \
	auxtools/Makefile.nmake auxtools/md2html.py auxtools/rulesyms.rb \
	common/*.h \
	data \
	dist/dist.vcxproj dist/dist.vcxproj.filters dist/dist.vcxproj.user \
	dist/Makefile.nmake \
	doc/Makefile \
	logger/logger.cpp logger/logger.h logger/logger.vcxproj \
	logger/logger.vcxproj.filters logger/logger.vcxproj.user \
	logger/resource.h logger/ver.rc \
	mentzdat \
	mihajong.sln
dist_doc_DATA = \
	README doc/manual.css \
	doc/addyaku.md doc/history.md doc/license.md doc/playing.md \
	doc/readme.md doc/rulesyms.md doc/script.md \
	doc/addyaku.html doc/history.html doc/license.html doc/playing.html \
	doc/readme.html doc/rulesyms.html doc/script.html

SUFFIXES = .md .html
.md.html:
	cd $(@D)&&make $(@F)
doc/rulesyms.md:
	cd $(@D)&&make $(@F)

aidir = $(pkgdatadir)/ai
dist_ai_DATA = data/ai/default.lua

gbaidir = $(pkgdatadir)/gbai
dist_gbai_DATA = data/gbai/default.lua

haifudatdir = $(pkgdatadir)/haifu
dist_haifudat_DATA = \
	data/haifu/haifu.css data/haifu/haifu.dtd data/haifu/haifu.xsd \
	data/haifu/haifu.xsl
