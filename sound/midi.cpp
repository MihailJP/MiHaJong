#include "snddata.h"

#if defined(MIDI_SUPPORT) && defined(_WIN32)
#include "GuruGuruSMF/GuruGuruSMF4_Cpp.h"
#include "../common/strcode.h"


sound::MidiData::MidiData(unsigned ID, const std::string& filename, bool looped) {
	using namespace GuruGuruSmf;
	if (GGS->AddListFromFileA(filename.c_str(), LoadOption::Buffered, ID) != GgsError::NoError)
		throw CodeConv::tstring(_T("GGS->AddListFromFileA失敗！！"));
	loopFlag = looped; myID = ID;
}

/* 再生 */
void sound::MidiData::Play() {
	using namespace GuruGuruSmf;
	if (GGS->Play((loopFlag ? PlayOption::Loop : 0) | PlayOption::SkipBeginning, myID, 0, 0, 0))
		throw CodeConv::tstring(_T("GGS->Play失敗！！"));
}

/* 停止 */
void sound::MidiData::Stop() {
	GGS->Stop(0);
}

/* 音量設定 */
void sound::MidiData::setVolume(double volume) {
	int vol;
	if (abs(volume) >= 1.0)
		vol = 0;
	else
		vol = static_cast<int>(-(abs(volume - 1) * 127));
	GGS->SetMasterVolume(vol);
}

/* デストラクタ */
sound::MidiData::~MidiData() {
	GGS->DeleteListItem(myID);
}

#endif
