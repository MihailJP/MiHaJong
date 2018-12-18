#include "player.h"

#include "../../../common/strcode.h"
#include <iomanip>
#include "../../../sound/sound.h"
#include "../../../common/bgmid.h"
#include "../../rule.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

namespace {

int getTimeRule(const char* ruleTag, int dflt_msec) {
	if      (rules::chkRule(ruleTag,  "5sec")) return  5000;
	else if (rules::chkRule(ruleTag,  "6sec")) return  6000;
	else if (rules::chkRule(ruleTag,  "7sec")) return  7000;
	else if (rules::chkRule(ruleTag,  "8sec")) return  8000;
	else if (rules::chkRule(ruleTag,  "9sec")) return  9000;
	else if (rules::chkRule(ruleTag, "10sec")) return 10000;
	else if (rules::chkRule(ruleTag, "11sec")) return 11000;
	else if (rules::chkRule(ruleTag, "12sec")) return 12000;
	else if (rules::chkRule(ruleTag, "13sec")) return 13000;
	else if (rules::chkRule(ruleTag, "14sec")) return 14000;
	else if (rules::chkRule(ruleTag, "15sec")) return 15000;
	else if (rules::chkRule(ruleTag, "16sec")) return 16000;
	else if (rules::chkRule(ruleTag, "17sec")) return 17000;
	else if (rules::chkRule(ruleTag, "18sec")) return 18000;
	else if (rules::chkRule(ruleTag, "19sec")) return 19000;
	else if (rules::chkRule(ruleTag, "20sec")) return 20000;
	else return dflt_msec;
}

}

// -------------------------------------------------------------------------


TableSubscenePlayerProto::TableSubscenePlayerProto(DevicePtr device) : TableSubscene(device) {
	myTextRenderer = new TextRenderer(device);
}

TableSubscenePlayerProto::~TableSubscenePlayerProto() {
	delete myTextRenderer;
}

std::int32_t TableSubscenePlayerProto::timeout() {
	return timeout_val - static_cast<int>(static_cast<double>(myTimer.elapsed()) / (static_cast<double>(timeResolution) / 1000.0));
}

void TableSubscenePlayerProto::showTimeout() {
	static double prevTime = static_cast<double>(timeout()) / 1000.0;
	double timeLeft = static_cast<double>(timeout()) / 1000.0;
	CodeConv::tostringstream(o); o << std::setw(4) << std::fixed << std::setprecision(1) << timeLeft;
	myTextRenderer->NewText(0, o.str().c_str(), timeX, timeY);
	myTextRenderer->Render();
	for (int i = 0; i <= 5; ++i)
		if ((prevTime > static_cast<double>(i)) && (timeLeft <= static_cast<double>(i)))
			sound::Play(sound::IDs::sndClock); // カウントダウンの音を鳴らす
	prevTime = timeLeft;
}

// -------------------------------------------------------------------------

TableSubscenePlayerDahai::TableSubscenePlayerDahai(DevicePtr device) : TableSubscenePlayerProto(device) {
	timeout_val = getTimeRule("dahai_time", 15000);
}

TableSubscenePlayerDahai::~TableSubscenePlayerDahai() {
}

void TableSubscenePlayerDahai::Render() {
	showTimeout();
}

// -------------------------------------------------------------------------

TableSubscenePlayerNakiProto::TableSubscenePlayerNakiProto(DevicePtr device) : TableSubscenePlayerProto(device) {
	timeout_val = getTimeRule("fuuro_time", 10000);
}

TableSubscenePlayerNakiProto::~TableSubscenePlayerNakiProto() {
}

// -------------------------------------------------------------------------

TableSubscenePlayerNaki::TableSubscenePlayerNaki(DevicePtr device) : TableSubscenePlayerNakiProto(device) {
}

TableSubscenePlayerNaki::~TableSubscenePlayerNaki() {
}

void TableSubscenePlayerNaki::Render() {
	showTimeout();
}

// -------------------------------------------------------------------------

TableSubscenePlayerNakiChankan::TableSubscenePlayerNakiChankan(DevicePtr device) : TableSubscenePlayerNakiProto(device) {
	callScreen = new TableSubsceneCallCut(device);
}

TableSubscenePlayerNakiChankan::~TableSubscenePlayerNakiChankan() {
	delete callScreen;
}

void TableSubscenePlayerNakiChankan::Render() {
	callScreen->Render();
	showTimeout();
}

// -------------------------------------------------------------------------

}
