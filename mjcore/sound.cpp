#include "sound.h"

#include "../common/bgmid.h"
#include "../common/strcode.h"
#include "func.h"
#include <vector>

namespace {
	enum bgmMode {None, Vorbis, Midi,};
	std::vector<bgmMode> BGM_Mode;
}

/* BGM�ǂݍ��� */
void sound::util::bgmload(unsigned ID, LPCSTR filename, bool looped) {
	if (BGM_Mode.size() <= ID) BGM_Mode.resize(ID + 1, None);
	CodeConv::tostringstream o;
	std::string oggfile = std::string("bgm\\") + std::string(filename) + std::string(".ogg");
	std::string midifile = std::string("bgm\\") + std::string(filename) + std::string(".mid");
	if (exist(oggfile.c_str())) {
		if (LoadVorbis(ID, oggfile.c_str(), looped ? 1 : 0) == 0) {
			o << _T("BGM�t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T(".ogg] �̓ǂݍ��݂��������܂����B");
			info(o.str().c_str());
			BGM_Mode[ID] = Vorbis;
		} else {
			o << _T("BGM�t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T(".ogg] �̓ǂݍ��݂Ɏ��s���܂����B");
			warn(o.str().c_str());
		}
	}
	else {
		o << _T("BGM�t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T(".ogg] �͂���܂���B");
		if (LoadMidi(ID, midifile.c_str(), looped ? 1 : 0) == 0) {
			o << _T("BGM�t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T(".mid] ��ǂݍ��݂܂����B");
			info(o.str().c_str());
			BGM_Mode[ID] = Midi;
		} else {
			o << _T("BGM�t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T(".mid] �̓ǂݍ��݂Ɏ��s���܂����B");
			warn(o.str().c_str());
		}
	}
	return;
}

/* �T�E���h�ǂݍ��� */
void sound::util::soundload(unsigned ID, LPCSTR filename, bool looped) {
	CodeConv::tostringstream o;
	if (sound::LoadWave(ID, filename, looped ? 1 : 0) == 0) {
		o << _T("�����t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T("] ��ǂݍ��݂܂����B");
		info(o.str().c_str());
	} else {
		o << _T("�����t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T("] �̓ǂݍ��݂Ɏ��s���܂����B");
		warn(o.str().c_str());
	}
	return;
}

/* BGM��~ */
void sound::util::bgmstop() {
	info(_T("BGM�Đ����~���܂��B"));
	for (unsigned i = IDs::BgmStart; i <= IDs::BgmEnd; i++)
		if (BGM_Mode[i] != None)
			Stop(i);
	return;
}

/* �a�f�l�Đ� */
void sound::util::bgmplay(unsigned ID) {
	CodeConv::tostringstream o;
	bgmstop();
	if (sound::Play(ID) == 0) {
		o << _T("BGM [") << ID << _T("] �Ԃ��Đ����܂��B");
		info(o.str().c_str());
	} else {
		o << _T("BGM [") << ID << _T("] �Ԃ��Đ��ł��܂���ł����B");
		warn(o.str().c_str());
	}
	return;
}
