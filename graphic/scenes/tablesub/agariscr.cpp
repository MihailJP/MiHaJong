#include "agariscr.h"
#include "../../loadtex.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../geometry.h"
#include "../../yakustat.h"
#include <cmath>

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::TableSubsceneAgariScreenProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myDevice = device;
	LoadTexture(device, &windowTexture, MAKEINTRESOURCE(IDB_PNG_AGARI_WINDOW), yakuWndWidth, yakuWndHeight);
	yakuData = YakuResult::getYakuStat();
	parseYakuList();
}
TableSubsceneAgariScreenProto::~TableSubsceneAgariScreenProto() {
	windowTexture->Release();
}

void TableSubsceneAgariScreenProto::parseYakuList() {
	typedef std::vector<CodeConv::tstring> TStrList;
	LPCTSTR yakuName = (yakuData.yakumanNameList[0] == _T('\0')) ? yakuData.yakuNameList : yakuData.yakumanNameList;
	LPCTSTR yakuVal  = (yakuData.yakumanValList [0] == _T('\0')) ? yakuData.yakuValList  : yakuData.yakumanValList;
	CodeConv::tstring yakuNameTxt, yakuValTxt;
	auto splitstr = [](LPCTSTR str) -> TStrList { // â¸çsÇ≈ï™äÑ
		TStrList txtlst;
		LPCTSTR ssptr = str; LPCTSTR sptr = ssptr;
		for (; *sptr != _T('\0'); ++sptr) {
			if (*sptr == _T('\n')) { // â¸çsÇ™åªÇÍÇΩÇÁ
				txtlst.push_back(CodeConv::tstring(ssptr, sptr - (*(sptr - 1) == _T('\r') ? 1 : 0)));
				ssptr = sptr;
			}
		}
		CodeConv::tstring laststr(ssptr, sptr - (*(sptr - 1) == _T('\r') ? 1 : 0));
		if (!laststr.empty()) txtlst.push_back(laststr);
		return txtlst;
	};
	TStrList yakuNameList(splitstr(yakuName)), yakuValList(splitstr(yakuVal));
	for (int i = 0; i < min(yakuNameList.size(), yakuValList.size()); ++i)
		yakuList.push_back(std::make_pair(yakuNameList[i], yakuValList[i]));
}

void TableSubsceneAgariScreenProto::renderWindow() {
	const double Zeit = seconds();
	const int yOffset = (Zeit >= 1.0) ? 0 : (int)(pow(1.0 - Zeit, 2) * (double)Geometry::BaseSize);
	SpriteRenderer::instantiate(myDevice)->ShowSprite(
		windowTexture, BaseX, BaseY - yOffset, yakuWndWidth, yakuWndHeight);
}

void TableSubsceneAgariScreenProto::renderYakuName() {
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreen::TableSubsceneAgariScreen(LPDIRECT3DDEVICE9 device) : TableSubsceneAgariScreenProto(device) {
}
TableSubsceneAgariScreen::~TableSubsceneAgariScreen() {
}
void TableSubsceneAgariScreen::Render() {
	renderWindow();
	renderYakuName();
}

// -------------------------------------------------------------------------

}
