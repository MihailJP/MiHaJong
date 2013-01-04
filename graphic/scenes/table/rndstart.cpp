#include "rndstart.h"
#include "../../rule.h"
#include "../../../mjcore/strcode.h"

namespace mihajong_graphic {

TableSubsceneBeginning::TableSubsceneBeginning(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myTextRenderer = new HugeTextRenderer(device);
}

TableSubsceneBeginning::~TableSubsceneBeginning() {
	delete myTextRenderer;
}

const std::wstring TableSubsceneBeginning::WindName = L"“Œ“ì¼–k”’á¢’†";
const std::wstring TableSubsceneBeginning::Numeral = L"ˆê“ñŽOŽlŒÜ˜ZŽµ”ª‹ã\";

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
	ZoomChar(0, WindName.substr(0, 1), -256,       0, 2500000);
	ZoomChar(1, Numeral.substr(0, 1),     0, 2500000, 5000000);
	ZoomChar(2, L"‹Ç"                ,  256, 5000000, 7500000);
	myTextRenderer->Render();
}

}
