lib_LTLIBRARIES = libmihajong-sound.la
libmihajong_sound_la_SOURCES = \
	logger.cpp midi.cpp snddata.cpp sound.cpp soundobj.cpp vorbis.cpp wave.cpp \
	audioobj.h logger.h midi.h resource.h snddata.h sound.h soundobj.h vorbis.h wave.h
if WITH_WINAPI
if MIDI_SUPPORT
nodist_libmihajong_sound_la_SOURCES = \
	GuruGuruSMF/GuruGuruSMF4_Cpp_Static.cpp \
	GuruGuruSMF/GuruGuruSMF4_Cpp.h
endif
endif
libmihajong_sound_la_LDFLAGS = -no-undefined -release $(VERSION)
if WITH_WINAPI
libmihajong_sound_la_LIBADD = ver.o
endif
libmihajong_sound_la_CXXFLAGS = -DSOUND_EXPORTS
if VORBIS_SUPPORT
libmihajong_sound_la_CXXFLAGS += -DVORBIS_SUPPORT
endif
if WITH_DIRECTX
libmihajong_sound_la_CXXFLAGS += -DWITH_DIRECTX
endif
EXTRA_DIST = \
	sound.vcxproj sound.vcxproj.filters \
	GuruGuruSMF/README libogg/Readme.txt libvorbis/Readme.txt
if WITH_WINAPI
libmihajong_sound_la_SOURCES += ver.rc
else
EXTRA_DIST += ver.rc
libmihajong_sound_la_CXXFLAGS += -DUNICODE -D_UNICODE
endif

.rc.o:
	$(WINDRES) $^ -o $@ -l 0411
%.o: %.rc
	$(WINDRES) $^ -o $@ -l 0411
