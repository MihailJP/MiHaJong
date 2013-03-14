#include "snddata.h"

#if defined(MIDI_SUPPORT) && defined(_WIN32)
#include "GuruGuruSMF/GuruGuruSMF4_Cpp.h"
#include "../common/strcode.h"


sound::MidiData::MidiData(unsigned ID, const std::string& filename, bool looped) {
	using namespace GuruGuruSmf;
	if (GGS->AddListFromFileA(filename.c_str(), LoadOption::Buffered, ID) != GgsError::NoError)
		throw CodeConv::tstring(_T("GGS->AddListFromFileA���s�I�I"));
	loopFlag = looped; myID = ID;
}

/* �Đ� */
void sound::MidiData::Play() {
	using namespace GuruGuruSmf;
	if (GGS->Play((loopFlag ? PlayOption::Loop : 0) | PlayOption::SkipBeginning, myID, 0, 0, 0))
		throw CodeConv::tstring(_T("GGS->Play���s�I�I"));
}

/* ��~ */
void sound::MidiData::Stop() {
	GGS->Stop(0);
}

/* �f�X�g���N�^ */
sound::MidiData::~MidiData() {
	GGS->DeleteListItem(myID);
}

#endif
