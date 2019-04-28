lib_LTLIBRARIES = libmihajong-graphic.la
libmihajong_graphic_la_SOURCES = \
	calltext.cpp chrwidth.cpp event.cpp except.cpp finscore.cpp font.cpp \
	gametbl.cpp geometry.cpp init.cpp input.cpp loadtex.cpp matrix.cpp \
	rule.cpp scrmanip.cpp  showtile.cpp sprite.cpp text.cpp timer.cpp \
	utils.cpp window.cpp yakustat.cpp \
	scenes/confmenu.cpp scenes/fps.cpp scenes/game.cpp scenes/proto.cpp \
	scenes/result.cpp scenes/ruleconf.cpp scenes/splash.cpp \
	scenes/sysscr.cpp scenes/table.cpp scenes/title.cpp scenes/waiting.cpp \
	scenes/tablesub/agariscr.cpp scenes/tablesub/call.cpp \
	scenes/tablesub/callval.cpp scenes/tablesub/chktnpai.cpp \
	scenes/tablesub/player.cpp scenes/tablesub/rndstart.cpp \
	scenes/tablesub/tablesub.cpp scenes/tablesub/titling.cpp \
	scenes/table/chicha.cpp scenes/table/clock.cpp scenes/table/gari.cpp \
	scenes/table/nakibtn.cpp scenes/table/nakihai.cpp \
	scenes/table/nakihai0.cpp scenes/table/richibou.cpp \
	scenes/table/showdice.cpp scenes/table/sutehai.cpp scenes/table/tehai.cpp \
	scenes/table/tehai0.cpp scenes/table/tiletip.cpp scenes/table/yamahai.cpp \
	widgets/button.cpp widgets/checkbox.cpp \
	calltext.h chrwidth.h directx.h event.h except.h exports.h extchar.h \
	finscore.h font.h gametbl.h geometry.h graphic.h init.h input.h loadtex.h \
	matrix.h pi.h resource.h rule.h scrmanip.h showtile.h sprite.h text.h \
	timer.h utils.h window.h yakustat.h \
	scenes/confmenu.h scenes/fps.h scenes/game.h scenes/proto.h \
	scenes/result.h scenes/ruleconf.h scenes/scenes.h \
	scenes/splash.h scenes/scene_id.h scenes/sysscr.h scenes/table.h \
	scenes/title.h scenes/waiting.h \
	scenes/tablesub/agariscr.h scenes/tablesub/call.h \
	scenes/tablesub/callval.h scenes/tablesub/chktnpai.h \
	scenes/tablesub/player.h scenes/tablesub/rndstart.h \
	scenes/tablesub/tablesub.h scenes/tablesub/tblnorm.h \
	scenes/tablesub/tblsubs.h scenes/tablesub/titling.h scenes/table/chicha.h \
	scenes/table/clock.h scenes/table/color.h scenes/table/gari.h \
	scenes/table/nakibtn.h scenes/table/nakihai.h scenes/table/nakihai0.h \
	scenes/table/naki_id.h scenes/table/richibou.h scenes/table/showdice.h \
	scenes/table/sutehai.h scenes/table/tehai.h scenes/table/tehai0.h \
	scenes/table/tiletip.h scenes/table/yamahai.h \
	widgets/button.h widgets/checkbox.h
libmihajong_graphic_la_LDFLAGS = -no-undefined -release $(VERSION)
libmihajong_graphic_la_CXXFLAGS = -DGRAPHIC_EXPORTS -Wno-switch
if WITH_DIRECTX
libmihajong_graphic_la_CXXFLAGS += -DWITH_DIRECTX
endif
if WITH_WINAPI
else
libmihajong_graphic_la_CXXFLAGS += -DPKGDATADIR='"$(pkgdatadir)"' -DUNICODE -D_UNICODE
endif
libmihajong_graphic_la_LIBADD = ../sound/libmihajong-sound.la ../astro/libmihajong-astro.la
if WITH_WINAPI
libmihajong_graphic_la_LIBADD += graphic.o
endif
if WITH_4K_PICS
DATA1 = data/2x
DATA2 = data
libmihajong_graphic_la_CXXFLAGS += -DWITH_4K_PICS
else
DATA1 = data
DATA2 = data/2x
endif
graphicdatadir = $(pkgdatadir)/graphic
dist_graphicdata_DATA = \
	data/cdigits.chr data/font.chr data/largechr.chr data/sdigits.chr \
	data/smallchr.chr
dist_graphicdata_DATA += \
	$(DATA1)/agariwnd.png $(DATA1)/baize.png $(DATA1)/button.png \
	$(DATA1)/call.png $(DATA1)/cdigits.png $(DATA1)/checkbox.png \
	$(DATA1)/chiicha.png $(DATA1)/clock.png $(DATA1)/dice.png $(DATA1)/font.png \
	$(DATA1)/frame.png $(DATA1)/largechr.png $(DATA1)/score.png \
	$(DATA1)/sdigits.png $(DATA1)/sidebar.png $(DATA1)/smallchr.png \
	$(DATA1)/tenbou.png $(DATA1)/tiles.png $(DATA1)/tiles_bk.png \
	$(DATA1)/title.png $(DATA1)/splash.png $(DATA1)/titlback.png
EXTRA_DIST = \
	$(DATA2)/agariwnd.png $(DATA2)/baize.png $(DATA2)/button.png \
	$(DATA2)/call.png $(DATA2)/cdigits.png $(DATA2)/checkbox.png \
	$(DATA2)/chiicha.png $(DATA2)/clock.png $(DATA2)/dice.png $(DATA2)/font.png \
	$(DATA2)/frame.png $(DATA2)/largechr.png $(DATA2)/score.png \
	$(DATA2)/sdigits.png $(DATA2)/sidebar.png $(DATA2)/smallchr.png \
	$(DATA2)/tenbou.png $(DATA2)/tiles.png $(DATA2)/tiles_bk.png \
	$(DATA2)/title.png $(DATA2)/splash.png $(DATA2)/titlback.png
EXTRA_DIST += \
	graphic.vcxproj graphic.vcxproj.filters
if WITH_WINAPI
libmihajong_graphic_la_SOURCES += graphic.rc
EXTRA_DIST += filenum.cpp filenum.h keycode.h
else
EXTRA_DIST += graphic.rc
libmihajong_graphic_la_SOURCES += filenum.cpp filenum.h keycode.h
endif

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
