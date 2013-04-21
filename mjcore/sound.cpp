#include "sound.h"

#include "../common/bgmid.h"
#include "../common/strcode.h"
#include "func.h"
#include "ruletbl.h"
#include <vector>
#include <regex>

#ifdef None
#undef None
#endif

namespace {
	enum bgmMode {None, Vorbis, Midi,};
	std::vector<bgmMode> BGM_Mode;
}

/* BGM�ǂݍ��� */
void sound::util::bgmload(unsigned ID, const char* filename, bool looped) {
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
void sound::util::soundload(unsigned ID, const char* filename, bool looped) {
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

/* ���ʐݒ�𔽉f */
void sound::util::setvolume() {
	using namespace sound;
	auto getvolume = [] (std::string ruleTag) -> double {
		const std::string chipRule(RuleData::chkPreference(ruleTag));
		std::smatch matchDat; int volperc = 100;
		if (std::regex_match(chipRule, matchDat, std::regex("vol_(\\d+)")))
			volperc = atoi(matchDat[1].str().c_str()); // ���[���ݒ蕶���񂩂琮���𒊏o
		return (double)volperc / 100.0;
	};
	for (unsigned i = IDs::BgmStart; i <= IDs::BgmEnd; i++)
		if (BGM_Mode[i] != None)
			SetVolume(i, getvolume("bgmvolume"));
	for (unsigned i = IDs::SndStart; i <= IDs::SndEnd; i++)
		SetVolume(i, getvolume("sndvolume"));
	for (unsigned i = IDs::VoxStart; i <= IDs::VoxEnd; i++)
		SetVolume(i, getvolume("sndvolume"));
}
