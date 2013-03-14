lib_LTLIBRARIES = libmihajong-sound.la
libmihajong_sound_la_SOURCES = \
	logger.cpp midi.cpp sound.cpp soundobj.cpp vorbis.cpp wave.cpp \
	logger.h resource.h snddata.h sound.h soundobj.h \
	ver.rc
if MIDI_SUPPORT
libmihajong_sound_la_SOURCES += \
	GuruGuruSMF/GuruGuruSMF4_Cpp_Static.cpp \
	GuruGuruSMF/GuruGuruSMF4_Cpp.h
endif
libmihajong_sound_la_LDFLAGS = -no-undefined
libmihajong_sound_la_LIBADD = ver.o
libmihajong_sound_la_CXXFLAGS = -DSOUND_EXPORTS --input-charset=cp932
if VORBIS_SUPPORT
libmihajong_sound_la_CXXFLAGS += -DVORBIS_SUPPORT
endif

.rc.o:
	windres $^ -o $@ -l 0411
%.o: %.rc
	windres $^ -o $@ -l 0411
