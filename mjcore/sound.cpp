﻿#include "sound.h"

#include "../common/bgmid.h"
#include "../common/strcode.h"
#include "func.h"
#include "ruletbl.h"
#include <vector>
#include <regex>
#include "envtbl.h"

#ifdef None
#undef None
#endif

#ifdef _WIN32
#define BGMDIR "bgm\\"
#else /* _WIN32 */
#define BGMDIR "bgm/"
#endif /* _WIN32 */

namespace {
	enum class BgmMode {none, vorbis, midi,};
	std::vector<BgmMode> BGM_Mode;
}

/* BGM読み込み */
void sound::util::bgmload(unsigned ID, const char* filename, bool looped) {
	if (BGM_Mode.size() <= static_cast<std::size_t>(ID)) BGM_Mode.resize(static_cast<std::size_t>(ID) + 1, BgmMode::none);
	CodeConv::tostringstream o;
	std::string oggfile = std::string(BGMDIR) + std::string(filename) + std::string(".ogg");
	std::string midifile = std::string(BGMDIR) + std::string(filename) + std::string(".mid");
#ifdef PKGDATADIR
	oggfile = std::string(PKGDATADIR) + std::string("/") + oggfile;
	midifile = std::string(PKGDATADIR) + std::string("/") + midifile;
#endif /* PKGDATADIR */
	if (exist(oggfile.c_str())) {
		if (LoadVorbis(ID, oggfile.c_str(), looped ? 1 : 0) == 0) {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".ogg] の読み込みを完了しました。");
			info(o.str().c_str());
			BGM_Mode[ID] = BgmMode::vorbis;
		} else {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".ogg] の読み込みに失敗しました。");
			warn(o.str().c_str());
		}
	}
	else {
		o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".ogg] はありません。");
		if (LoadMidi(ID, midifile.c_str(), looped ? 1 : 0) == 0) {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".mid] を読み込みました。");
			info(o.str().c_str());
			BGM_Mode[ID] = BgmMode::midi;
		} else {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".mid] の読み込みに失敗しました。");
			warn(o.str().c_str());
		}
	}
	return;
}

/* サウンド読み込み */
void sound::util::soundload(unsigned ID, const char* filename, bool looped) {
	CodeConv::tostringstream o;
#ifdef PKGDATADIR
	const std::string fName = std::string(PKGDATADIR) + std::string("/") + std::string(filename);
#else /* PKGDATADIR */
	const std::string fName(filename);
#endif /* PKGDATADIR */
	if (sound::LoadWave(ID, fName.c_str(), looped ? 1 : 0) == 0) {
		o << _T("音声ファイル [") << CodeConv::EnsureTStr(fName) << _T("] を読み込みました。");
		info(o.str().c_str());
	} else {
		o << _T("音声ファイル [") << CodeConv::EnsureTStr(fName) << _T("] の読み込みに失敗しました。");
		warn(o.str().c_str());
	}
	return;
}

/* BGM停止 */
void sound::util::bgmstop() {
	if (EnvTable::Instantiate()->WatchModeFlag) return; // デモ画面では無視
	info(_T("BGM再生を停止します。"));
	for (unsigned i = IDs::BgmStart; i <= IDs::BgmEnd; i++)
		if (BGM_Mode[i] != BgmMode::none)
			Stop(i);
	return;
}

/* ＢＧＭ再生 */
void sound::util::bgmplay(unsigned ID) {
	if (EnvTable::Instantiate()->WatchModeFlag) return; // デモ画面では無視
	CodeConv::tostringstream o;
	bgmstop();
	if (sound::Play(ID) == 0) {
		o << _T("BGM [") << ID << _T("] 番を再生します。");
		info(o.str().c_str());
	} else {
		o << _T("BGM [") << ID << _T("] 番を再生できませんでした。");
		warn(o.str().c_str());
	}
	return;
}

/* 音量設定を反映 */
void sound::util::setvolume() {
	using namespace sound;
	{
		CodeConv::tostringstream o;
		o << _T("BGM音量 [") << RuleData::confFile.bgmVolume() << _T("] 音声ファイル音量 [" << RuleData::confFile.soundVolume() << _T("]"));
		info(o.str().c_str());
	}
	for (unsigned i = IDs::BgmStart; i <= IDs::BgmEnd; i++)
		if (BGM_Mode[i] != BgmMode::none)
			SetVolume(i, static_cast<double>(RuleData::confFile.bgmVolume()) / 100.0);
	for (unsigned i = IDs::SndStart; i <= IDs::SndEnd; i++)
		SetVolume(i, static_cast<double>(RuleData::confFile.soundVolume()) / 100.0);
	for (unsigned i = IDs::VoxStart; i <= IDs::VoxEnd; i++)
		SetVolume(i, static_cast<double>(RuleData::confFile.soundVolume()) / 100.0);
}
