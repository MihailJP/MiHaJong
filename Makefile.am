AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = astro socket sound graphic mjcore mjcoregb mihajong
EXTRA_DIST = \
	auxtools/auxtools.vcxproj auxtools/auxtools.vcxproj.filters \
	auxtools/compress.py auxtools/compress.rb \
	auxtools/Makefile.nmake auxtools/md2html.py auxtools/rulesyms.rb \
	common/*.h \
	data \
	dist/dist.vcxproj dist/dist.vcxproj.filters dist/Makefile.nmake \
	doc/Makefile \
	logger/logger.cpp logger/logger.h logger/logger.vcxproj \
	logger/logger.vcxproj.filters logger/resource.h logger/ver.rc \
	mentzdat \
	mihajong.sln \
	doc/addyaku.md doc/history.md doc/license.md doc/playing.md \
	doc/readme.md doc/rulesyms.md doc/script.md
dist_doc_DATA = \
	readme.md doc/manual.css doc/pygments.css \
	doc/addyaku.html doc/history.html doc/license.html doc/playing.html \
	doc/readme.html doc/rulesyms.html doc/script.html

SUFFIXES = .md .html
.md.html:
	cd $(@D)&&PYTHON=$(PYTHON) $(MAKE) $(@F)
doc/rulesyms.md:
	cd $(@D)&&PYTHON=$(PYTHON) $(MAKE) $(@F)
doc/pygments.css:
	cd $(@D)&&PYTHON=$(PYTHON) $(MAKE) $(@F)

aidir = $(pkgdatadir)/ai
dist_ai_DATA = data/ai/default.lua

gbaidir = $(pkgdatadir)/gbai
dist_gbai_DATA = data/gbai/default.lua

haifudatdir = $(pkgdatadir)/haifu
dist_haifudat_DATA = \
	data/haifu/haifu.css data/haifu/haifu.dtd data/haifu/haifu.xsd \
	data/haifu/haifu.xsl \
	data/haifu/GL-Antique-Dice.woff \
	data/haifu/GL-MahjongTile-Clr-WithFallback.woff \
	data/haifu/GL-MahjongTile-Subset.woff

sounddatdir = $(pkgdatadir)/sound
dist_sounddat_DATA = \
	data/sound/agari1.wav data/sound/agari2.wav data/sound/bell.wav \
	data/sound/button.wav data/sound/chi.wav data/sound/chonbo.wav \
	data/sound/click.wav data/sound/clock.wav data/sound/countdn.wav \
	data/sound/cursor.wav data/sound/dahai1.wav data/sound/dahai2.wav \
	data/sound/flower.wav data/sound/kan.wav data/sound/kyuushu.wav \
	data/sound/mekuri.wav data/sound/page.wav data/sound/pingju.wav \
	data/sound/pon.wav data/sound/richi.wav data/sound/saikoro.wav \
	data/sound/signal.wav data/sound/tsumo.wav data/sound/type.wav \
	data/sound/yakulst1.wav data/sound/yakulst2.wav
