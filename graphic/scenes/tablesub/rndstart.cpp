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

const std::wstring TableSubsceneBeginning::WindName = L"���쐼�k��ᢒ�";
const std::wstring TableSubsceneBeginning::Numeral = L"���O�l�ܘZ������\";

void TableSubsceneBeginning::ZoomChar(unsigned ID, const std::wstring& str, int xOffset, uint64_t Anfang, uint64_t Ende) {
	const uint64_t Zeit = currTime() - startTime;
	if ((Zeit >= Anfang) && (Zeit < Ende)) {
		float size = pow(3.0f - ((float)(Zeit - Anfang) / (float)(Ende - Anfang) * 2.0f), 2);
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
	int64_t timeOffset = (roundcode >= GameStatus::gameStat()->GameLength) ? 10000000 : 0;

	if (currTime() - startTime < timeOffset) {
		if (roundcode > GameStatus::gameStat()->GameLength) {
			ZoomChar(0, _T("��"), -256, 0, 2500000);
			ZoomChar(1, _T("��"),    0, 0, 2500000);
			ZoomChar(2, _T("��"),  256, 0, 2500000);
		} else if (roundcode == 0) {
			ZoomChar(0, _T("��"), -336, 0, 2500000);
			ZoomChar(1, _T("��"), -112, 0, 2500000);
			ZoomChar(2, _T("��"),  112, 0, 2500000);
			ZoomChar(3, _T("�Z"),  336, 0, 2500000);
		} else {
			ZoomChar(0, _T("�I"), -336, 0, 2500000);
			ZoomChar(1, _T("�["), -112, 0, 2500000);
			ZoomChar(2, _T("��"),  112, 0, 2500000);
			ZoomChar(3, _T("�X"),  336, 0, 2500000);
		}
	} else {
		if (rules::chkRule("game_length", "twice_east_game") || rules::chkRule("game_length", "east_only_game")) {
			// ����݂̂̃��[��
			ZoomChar(    0, WindName.substr(0                 , 1), -256,       0 + timeOffset, 2500000 + timeOffset);
			if (roundnum < 10) {
				ZoomChar(1, Numeral.substr( roundnum          , 1),    0, 2500000 + timeOffset, 5000000 + timeOffset);
			} else {
				std::wstringstream o; o << (roundnum + 1);
				ZoomChar(1, o.str()                               ,    0, 2500000 + timeOffset, 5000000 + timeOffset);
			}
		} else {
			// ��ʂ̃��[��
			ZoomChar(    0, WindName.substr(roundnum / Players, 1), -256,       0 + timeOffset, 2500000 + timeOffset);
			ZoomChar(    1, Numeral.substr( roundnum % Players, 1),    0, 2500000 + timeOffset, 5000000 + timeOffset);
		}
		ZoomChar(        2, _T("��")                              ,  256, 5000000 + timeOffset, 7500000 + timeOffset);
		myTextRenderer->DelText(3);
	}
	myTextRenderer->Render();
}

}
