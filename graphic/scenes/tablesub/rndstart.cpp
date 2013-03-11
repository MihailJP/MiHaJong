#include "rndstart.h"
#include "../../rule.h"
#include "../../gametbl.h"
#include "../../../common/strcode.h"

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
	const uint64_t Zeit = myTimer.elapsed();
	if ((Zeit >= Anfang) && (Zeit < Ende)) {
		float size = std::pow(3.0f - ((float)(Zeit - Anfang) / (float)(Ende - Anfang) * 2.0f), 2);
		myTextRenderer->NewText(ID, CodeConv::EnsureTStr(str),
			TableSize / 2 - (224 * size) / 2 + xOffset, TableSize / 2 - 192 + 112 - (224 * size) / 2,
			size, 1.0f, D3DCOLOR_ARGB((int)((float)(Zeit - Anfang) / (float)(Ende - Anfang) * 255.0f), 255, 255, 255));
	} else if (Zeit >= Ende) {
		myTextRenderer->NewText(ID, CodeConv::EnsureTStr(str), TableSize / 2 - 112 + xOffset, TableSize / 2 - 192);
	} else {
		myTextRenderer->DelText(ID);
	}
}

void TableSubsceneBeginning::Render() {
	const int roundnum = GameStatus::gameStat()->GameRound;
	const int roundcode = GameStatus::gameStat()->LoopRound * 16 + GameStatus::gameStat()->GameRound;
	int64_t timeOffset = (roundcode >= GameStatus::gameStat()->GameLength) ? 1000000 : 0;

	if (myTimer.elapsed() < timeOffset) {
		if (roundcode > GameStatus::gameStat()->GameLength) {
			ZoomChar(0, L"延", -256, 0, 250000);
			ZoomChar(1, L"長",    0, 0, 250000);
			ZoomChar(2, L"戦",  256, 0, 250000);
		} else if (roundcode == 0) {
			ZoomChar(0, L"一", -336, 0, 250000);
			ZoomChar(1, L"局", -112, 0, 250000);
			ZoomChar(2, L"精",  112, 0, 250000);
			ZoomChar(3, L"算",  336, 0, 250000);
		} else {
			ZoomChar(0, L"オ", -336, 0, 250000);
			ZoomChar(1, L"ー", -112, 0, 250000);
			ZoomChar(2, L"ラ",  112, 0, 250000);
			ZoomChar(3, L"ス",  336, 0, 250000);
		}
	} else {
		if (rules::chkRule("game_length", "twice_east_game") || rules::chkRule("game_length", "east_only_game")) {
			// 東場のみのルール
			ZoomChar(    0, WindName.substr(0                 , 1), -256,      0 + timeOffset, 250000 + timeOffset);
			if (roundnum < 10) {
				ZoomChar(1, Numeral.substr( roundnum          , 1),    0, 250000 + timeOffset, 500000 + timeOffset);
			} else {
				std::wstringstream o; o << (roundnum + 1);
				ZoomChar(1, o.str()                               ,    0, 250000 + timeOffset, 500000 + timeOffset);
			}
		} else {
			// 一般のルール
			ZoomChar(    0, WindName.substr(roundnum / Players, 1), -256,      0 + timeOffset, 250000 + timeOffset);
			ZoomChar(    1, Numeral.substr( roundnum % Players, 1),    0, 250000 + timeOffset, 500000 + timeOffset);
		}
		ZoomChar(        2, L"局"                                 ,  256, 500000 + timeOffset, 750000 + timeOffset);
		myTextRenderer->DelText(3);
	}
	myTextRenderer->Render();
}

}
