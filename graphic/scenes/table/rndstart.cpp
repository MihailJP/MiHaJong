#include "rndstart.h"
#include "../../rule.h"
#include "../../gametbl.h"
#include "../../../mjcore/strcode.h"

namespace mihajong_graphic {

TableSubsceneBeginning::TableSubsceneBeginning(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myTextRenderer = new HugeTextRenderer(device);
}

TableSubsceneBeginning::~TableSubsceneBeginning() {
	delete myTextRenderer;
}

const std::wstring TableSubsceneBeginning::WindName = L"東南西北白發中";
const std::wstring TableSubsceneBeginning::Numeral = L"一二三四五六七八九十";

void TableSubsceneBeginning::ZoomChar(unsigned ID, const std::wstring& str, int xOffset, uint64_t Anfang, uint64_t Ende) {
	const uint64_t Zeit = currTime() - startTime;
	if ((Zeit >= Anfang) && (Zeit < Ende)) {
		float size = std::pow(3.0f - ((float)(Zeit - Anfang) / (float)(Ende - Anfang) * 2.0f), 2);
		myTextRenderer->NewText(ID, CodeConv::EnsureTStr(str),
			TableSize / 2 - (224 * size) / 2 + xOffset, TableSize / 2 - 192 + 112 - (224 * size) / 2,
			size, 1.0f, D3DCOLOR_ARGB((int)((float)(Zeit - Anfang) / (float)(Ende - Anfang) * 255.0f), 255, 255, 255));
	} else if (Zeit >= Ende) {
		myTextRenderer->NewText(ID, CodeConv::EnsureTStr(str), TableSize / 2 - 112 + xOffset, TableSize / 2 - 192);
	}
}

void TableSubsceneBeginning::Render() {
	const int roundnum = GameStatus::gameStat()->GameRound;
	if (rules::chkRule("game_length", "twice_east_game") || rules::chkRule("game_length", "east_only_game")) {
		// 東場のみのルール
		ZoomChar(    0, WindName.substr(0                 ,                      1), -256,       0, 2500000);
		if (roundnum < 10) {
			ZoomChar(1, Numeral.substr( roundnum          , roundnum           + 1),    0, 2500000, 5000000);
		} else {
			std::wstringstream o; o << (roundnum + 1);
			ZoomChar(1, o.str()                                                    ,    0, 2500000, 5000000);
		}
	} else {
		// 東場のみのルール
		ZoomChar(    0, WindName.substr(roundnum / PLAYERS, roundnum / PLAYERS + 1), -256,       0, 2500000);
		ZoomChar(    1, Numeral.substr( roundnum % PLAYERS, roundnum % PLAYERS + 1),    0, 2500000, 5000000);
	}
	ZoomChar(        2, _T("局")                                                   ,  256, 5000000, 7500000);
	myTextRenderer->Render();
}

}
