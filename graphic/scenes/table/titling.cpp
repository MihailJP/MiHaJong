#include "titling.h"
#include "../../gametbl.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneTitling::TableSubsceneTitling(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myTextRenderer = new HugeTextRenderer(device);
}

TableSubsceneTitling::~TableSubsceneTitling() {
	delete myTextRenderer;
}

void TableSubsceneTitling::FadeinStr(const std::wstring& str) {
	const uint64_t Zeit = currTime() - startTime, Anfang = 0, Ende = 5000000;
	unsigned len = 0;
	for (auto k = str.begin(); k != str.end(); ++k)
		if (*k <= L'\x7f') len += 1;
		else len += 2;
	if ((Zeit >= Anfang) && (Zeit < Ende)) {
		myTextRenderer->NewText(0, CodeConv::EnsureTStr(str), TableSize / 2 - (56 * len), TableSize / 2 - 192,
			1.0f, 1.0f, D3DCOLOR_ARGB((int)((float)(Zeit - Anfang) / (float)(Ende - Anfang) * 255.0f), 255, 255, 255));
	} else if (Zeit >= Ende) {
		myTextRenderer->NewText(0, CodeConv::EnsureTStr(str), TableSize / 2 - (56 * len), TableSize / 2 - 192);
	}
}


// -------------------------------------------------------------------------

TableSubsceneMsg::TableSubsceneMsg(LPDIRECT3DDEVICE9 device, const std::wstring& str) : TableSubsceneTitling(device) {
	myString = str;
}
TableSubsceneMsg::TableSubsceneMsg(LPDIRECT3DDEVICE9 device, const std::string& str) : TableSubsceneTitling(device) {
	myString = CodeConv::ANSItoWIDE(str);
}

TableSubsceneMsg::~TableSubsceneMsg() {
}

void TableSubsceneMsg::Render() {
	FadeinStr(myString);
	myTextRenderer->Render();
}

// -------------------------------------------------------------------------

TableSubsceneTitlingHonba::TableSubsceneTitlingHonba(LPDIRECT3DDEVICE9 device) : TableSubsceneTitling(device) {
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
