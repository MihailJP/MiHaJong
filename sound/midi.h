#pragma once

#include <cstring>
#include "../common/strcode.h"
#include "audioobj.h"

namespace sound {

	/* MIDIデータ用オブジェクト */
	class MidiData : public AudioData {
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	private:
		int myID;
		bool loopFlag;
	public:
		explicit MidiData(unsigned ID, const std::string& filename, bool looped = false);
		MidiData(const MidiData&) = delete; // Delete unexpected copy constructor
		MidiData& operator= (const MidiData&) = delete; // Delete unexpected assign operator
		void Play() override;
		void Stop() override;
		virtual void setVolume(double volume) override;
		virtual ~MidiData();
#else
	public:
		explicit MidiData(unsigned ID, const std::string& filename, bool looped = false) {
			throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));
		}
		MidiData(const MidiData&) = delete; // Delete unexpected copy constructor
		MidiData& operator= (const MidiData&) = delete; // Delete unexpected assign operator
		void Play() override {throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));}
		void Stop() override {throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));}
		virtual void setVolume(double volume) override {
			throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));
		}
#endif
	};

}
