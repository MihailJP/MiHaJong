#include "titling.h"
#include "../../gametbl.h"
#include "../../event.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneTitling::TableSubsceneTitling(DevicePtr device) : TableSubscene(device) {
	myTextRenderer = new HugeTextRenderer(device);
}

TableSubsceneTitling::~TableSubsceneTitling() {
	delete myTextRenderer;
}

void TableSubsceneTitling::FadeinStr(const std::wstring& str) {
	const uint64_t Zeit = myTimer.elapsed(), Anfang = 0, Ende = 500000;
	const unsigned len = myTextRenderer->strWidthByCols(str);
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if ((Zeit >= Anfang) && (Zeit < Ende)) {
		myTextRenderer->NewText(0, CodeConv::EnsureTStr(str), TableSize / 2 - (56 * (len > 8 ? 8 :len)), TableSize / 2 - 192,
			1.0f, (len > 8 ? 8.0f / (float)len : 1.0f), D3DCOLOR_ARGB((int)((float)(Zeit - Anfang) / (float)(Ende - Anfang) * 255.0f), 255, 255, 255));
	} else if (Zeit >= Ende) {
		myTextRenderer->NewText(0, CodeConv::EnsureTStr(str), TableSize / 2 - (56 * (len > 8 ? 8 :len)), TableSize / 2 - 192,
			1.0f, (len > 8 ? 8.0f / (float)len : 1.0f));
	}
#else
	/* TODO: OpenGL‚ÅÄŽÀ‘• */
#endif
}

void TableSubsceneTitling::skipEvent() {
	ui::cancellableWait->set(1);
}

// -------------------------------------------------------------------------

TableSubsceneMsg::TableSubsceneMsg(DevicePtr device, const std::wstring& str) : TableSubsceneTitling(device) {
	myString = str;
}
TableSubsceneMsg::TableSubsceneMsg(DevicePtr device, const std::string& str) : TableSubsceneTitling(device) {
	myString = CodeConv::ANSItoWIDE(str);
}

TableSubsceneMsg::~TableSubsceneMsg() {
}

void TableSubsceneMsg::Render() {
	FadeinStr(myString);
	myTextRenderer->Render();
}

// -------------------------------------------------------------------------

TableSubsceneTitlingHonba::TableSubsceneTitlingHonba(DevicePtr device) : TableSubsceneTitling(device) {
}
TableSubsceneTitlingHonba::~TableSubsceneTitlingHonba() {
}

const std::wstring TableSubsceneTitlingHonba::Numeral = L"0‚P‚Q‚R‚S‚T‚U‚V‚W‚X";

void TableSubsceneTitlingHonba::Render() {
	std::wstringstream o;
	const int honba = GameStatus::gameStat()->Honba;
	if (honba < 10) o << Numeral.substr(honba, 1);
	else o << honba;
	o << L"–{ê";
	FadeinStr(o.str());
	myTextRenderer->Render();
}

// -------------------------------------------------------------------------

}
