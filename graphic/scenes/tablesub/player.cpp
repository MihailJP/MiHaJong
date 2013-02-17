#include "player.h"

#include "../../../common/strcode.h"
#include <iomanip>
#include "../../../sound/sound.h"
#include "../../../mjcore/bgmid.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubscenePlayerProto::TableSubscenePlayerProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myTextRenderer = new TextRenderer(device);
}

TableSubscenePlayerProto::~TableSubscenePlayerProto() {
	delete myTextRenderer;
}

std::int32_t TableSubscenePlayerProto::timeout() {
	return timeout_val - (int)((double)(currTime() - startTime) / ((double)timeResolution / 1000.0));
}

void TableSubscenePlayerProto::showTimeout() {
	static double prevTime = (double)timeout() / 1000.0;
	double timeLeft = (double)timeout() / 1000.0;
	CodeConv::tostringstream(o); o << std::setw(4) << std::fixed << std::setprecision(1) << timeLeft;
	myTextRenderer->NewText(0, o.str().c_str(), timeX, timeY);
	myTextRenderer->Render();
	for (int i = 0; i <= 5; ++i)
		if ((prevTime > (double)i) && (timeLeft <= (double)i))
			sound::Play(sound::IDs::sndClock); // カウントダウンの音を鳴らす
	prevTime = timeLeft;
}

// -------------------------------------------------------------------------

TableSubscenePlayerDahai::TableSubscenePlayerDahai(LPDIRECT3DDEVICE9 device) : TableSubscenePlayerProto(device) {
	timeout_val = 15000;
}

TableSubscenePlayerDahai::~TableSubscenePlayerDahai() {
}

void TableSubscenePlayerDahai::Render() {
	showTimeout();
}

// -------------------------------------------------------------------------

TableSubscenePlayerNaki::TableSubscenePlayerNaki(LPDIRECT3DDEVICE9 device) : TableSubscenePlayerProto(device) {
	timeout_val = 10000;
}

TableSubscenePlayerNaki::~TableSubscenePlayerNaki() {
}

void TableSubscenePlayerNaki::Render() {
	showTimeout();
}

// -------------------------------------------------------------------------

}
