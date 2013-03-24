lib_LTLIBRARIES = libmihajong-graphic.la
libmihajong_graphic_la_SOURCES = \
	button.cpp calltext.cpp chrwidth.cpp editbox.cpp event.cpp finscore.cpp \
	font.cpp gametbl.cpp geometry.cpp init.cpp input.cpp loadtex.cpp \
	logwnd.cpp rule.cpp scrmanip.cpp showtile.cpp sprite.cpp text.cpp \
	timer.cpp utils.cpp window.cpp yakustat.cpp \
	scenes/fps.cpp scenes/game.cpp scenes/proto.cpp scenes/result.cpp \
	scenes/ruleconf.cpp scenes/splash.cpp scenes/sysscr.cpp scenes/table.cpp \
	scenes/title.cpp \
	scenes/tablesub/agariscr.cpp scenes/tablesub/call.cpp \
	scenes/tablesub/callval.cpp scenes/tablesub/chktnpai.cpp \
	scenes/tablesub/player.cpp scenes/tablesub/rndstart.cpp \
	scenes/tablesub/tablesub.cpp scenes/tablesub/titling.cpp \
	scenes/table/chicha.cpp scenes/table/gari.cpp scenes/table/nakibtn.cpp \
	scenes/table/nakihai.cpp scenes/table/nakihai0.cpp \
	scenes/table/richibou.cpp scenes/table/showdice.cpp \
	scenes/table/sutehai.cpp scenes/table/tehai.cpp scenes/table/tehai0.cpp \
	scenes/table/yamahai.cpp \
	button.h calltext.h chrwidth.h directx.h editbox.h event.h exports.h \
	extchar.h finscore.h font.h gametbl.h geometry.h init.h input.h loadtex.h \
	logwnd.h pi.h resource.h rule.h scrmanip.h showtile.h sprite.h text.h \
	timer.h utils.h window.h yakustat.h \
	scenes/fps.h scenes/game.h scenes/proto.h scenes/result.h \
	scenes/ruleconf.h scenes/scenes.h scenes/splash.h scenes/scene_id.h \
	scenes/sysscr.h scenes/table.h scenes/title.h \
	scenes/tablesub/agariscr.h scenes/tablesub/call.h \
	scenes/tablesub/callval.h scenes/tablesub/chktnpai.h \
	scenes/tablesub/player.h scenes/tablesub/rndstart.h \
	scenes/tablesub/tablesub.h scenes/tablesub/tblnorm.h \
	scenes/tablesub/tblsubs.h scenes/tablesub/titling.h scenes/table/chicha.h \
	scenes/table/color.h scenes/table/gari.h scenes/table/nakibtn.h \
	scenes/table/nakihai.h scenes/table/nakihai0.h scenes/table/naki_id.h \
	scenes/table/richibou.h scenes/table/showdice.h scenes/table/sutehai.h \
	scenes/table/tehai.h scenes/table/tehai0.h scenes/table/yamahai.h \
	graphic.rc
libmihajong_graphic_la_LDFLAGS = -no-undefined
libmihajong_graphic_la_CXXFLAGS = -DGRAPHIC_EXPORTS --input-charset=cp932
libmihajong_graphic_la_LIBADD = graphic.o ../sound/libmihajong-sound.la
dist_data_DATA = \
	data/agariwnd.png data/baize.png data/button.png data/call.png \
	data/cdigits.chr data/cdigits.png data/chiicha.png data/dice.png \
	data/editbox.png data/font.chr data/font.png data/frame.png \
	data/largechr.chr data/largechr.png data/score.png data/sdigits.chr \
	data/sdigits.png data/sidebar.png data/smallchr.chr data/smallchr.png \
	data/tenbou.png data/tiles.png data/title.png
EXTRA_DIST = \
	graphic.vcxproj graphic.vcxproj.filters graphic.vcxproj.user

.rc.o:
	windres $^ -o $@ -l 0411
%.o: %.rc
	windres $^ -o $@ -l 0411
