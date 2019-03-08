lib_LTLIBRARIES = libmihajong-graphic.la
libmihajong_graphic_la_SOURCES = \
	calltext.cpp chrwidth.cpp event.cpp finscore.cpp font.cpp gametbl.cpp \
	geometry.cpp init.cpp input.cpp loadtex.cpp matrix.cpp rule.cpp
	scrmanip.cpp  showtile.cpp sprite.cpp text.cpp timer.cpp utils.cpp \
	window.cpp yakustat.cpp \
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
	calltext.h chrwidth.h directx.h event.h exports.h extchar.h finscore.h \
	font.h gametbl.h geometry.h graphic.h init.h input.h loadtex.h matrix.h \
	pi.h resource.h rule.h scrmanip.h showtile.h sprite.h text.h timer.h \
	utils.h  window.h yakustat.h \
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
libmihajong_graphic_la_CXXFLAGS = -DGRAPHIC_EXPORTS
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
if WITH_BOOST_CHRONO
libmihajong_graphic_la_CXXFLAGS += -DWITH_BOOST_CHRONO $(BOOST_CHRONO_LIB)
endif
if WITH_BOOST_DATE_TIME
libmihajong_graphic_la_CXXFLAGS += -DWITH_BOOST_DATE_TIME $(BOOST_DATE_TIME_LIB)
endif
if WITH_BOOST_THREAD
libmihajong_graphic_la_CXXFLAGS += -DWITH_BOOST_THREAD $(BOOST_THREAD_LIB)
endif
if OLDER_BOOST_SLEEP
libmihajong_graphic_la_CXXFLAGS += -DOLDER_BOOST_SLEEP
endif
if OLDER_BOOST_WAIT
libmihajong_graphic_la_CXXFLAGS += -DOLDER_BOOST_WAIT
endif
dist_pkgdata_DATA = \
	data/agariwnd.png data/baize.png data/button.png data/call.png \
	data/cdigits.chr data/cdigits.png data/checkbox.png data/chiicha.png \
	data/clock.png \
	data/dice.png data/font.chr data/font.png data/frame.png \
	data/largechr.chr data/largechr.png data/score.png data/sdigits.chr \
	data/sdigits.png data/sidebar.png data/smallchr.chr data/smallchr.png \
	data/tenbou.png data/tiles.png data/tiles_bk.png data/title.png
EXTRA_DIST = \
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
