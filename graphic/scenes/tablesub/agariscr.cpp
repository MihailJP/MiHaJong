#include "agariscr.h"
#include "../../loadtex.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../geometry.h"
#include "../../yakustat.h"
#include <cmath>
#include "../../../sound/sound.h"
#include "../../../common/bgmid.h"
#include "../../gametbl.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::TableSubsceneAgariScreenProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myDevice = device;
	LoadTexture(device, &windowTexture, MAKEINTRESOURCE(IDB_PNG_AGARI_WINDOW), yakuWndWidth, yakuWndHeight);
	yakuData = YakuResult::getYakuStat();
	parseYakuList();
	myTextRenderer = new TextRenderer(device);
	agariTehai = new AgariTehai(this);
}
TableSubsceneAgariScreenProto::~TableSubsceneAgariScreenProto() {
	delete agariTehai;
	delete myTextRenderer;
	windowTexture->Release();
}

void TableSubsceneAgariScreenProto::parseYakuList() {
	typedef std::vector<CodeConv::tstring> TStrList;
	LPCTSTR yakuName = (yakuData.yakumanNameList[0] == _T('\0')) ? yakuData.yakuNameList : yakuData.yakumanNameList;
	LPCTSTR yakuVal  = (yakuData.yakumanValList [0] == _T('\0')) ? yakuData.yakuValList  : yakuData.yakumanValList;
	CodeConv::tstring yakuNameTxt, yakuValTxt;
	auto splitstr = [](LPCTSTR str) -> TStrList { // 改行で分割
		TStrList txtlst;
		LPCTSTR ssptr = str; LPCTSTR sptr = ssptr;
		for (; *sptr != _T('\0'); ++sptr) {
			if (*sptr == _T('\n')) { // 改行が現れたら
				txtlst.push_back(CodeConv::tstring(ssptr, sptr - (*(sptr - 1) == _T('\r') ? 1 : 0)));
				ssptr = sptr + 1;
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

bool TableSubsceneAgariScreenProto::renderYakuName(unsigned yakuNum) {
	const double anmTime = 0.5;
	const double interval = 0.75;
	const double Zeit = seconds() - (1.0 + interval * yakuNum);
	const int xOffset = (Zeit >= anmTime) ? 0 : (int)(pow(2.0 * (anmTime - Zeit), 2) * (double)(yakuWndWidth / 5));
	if (Zeit <= 0) {
		myTextRenderer->DelText(yakuNum * 2);
		myTextRenderer->DelText(yakuNum * 2 + 1);
		return false;
	} else {
		// 幅計算
		const float compressRate = [](const CodeConv::tstring& str) -> float {
			const std::wstring tmpstr(
#ifdef _UNICODE
				str
#else
				CodeConv::ANSItoWIDE(str)
#endif
				);
			int cols = 0;
			for (auto k = tmpstr.begin(); k != tmpstr.end(); ++k)
				cols += ((*k > L'\x7f') ? 2 : 1);
			return (cols > 12) ? (8.0f / (float)cols) : 1.0f;
		} (yakuList[yakuNum].first);
		// 表示
		const D3DCOLOR color = (Zeit >= anmTime) ? 0xffffffff : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | 0x00ffffff);
		const int x = BaseX + ((yakuNum % 2 == 0) ? 50 : 390);
		const int y = BaseY + 100;
		myTextRenderer->NewText(yakuNum * 2, yakuList[yakuNum].first,
			x + xOffset, y + (yakuNum / 2) * 50,
			1.0f, compressRate, color);
		myTextRenderer->NewText(yakuNum * 2 + 1, yakuList[yakuNum].second,
			x + 220, y + 8 + (yakuNum / 2) * 50,
			0.75f, 1.0f, color);
		return true;
	}
}

void TableSubsceneAgariScreenProto::renderYakuName() {
	static int maxShown = -1;
	for (unsigned i = 0; i < yakuList.size(); ++i) {
		const bool isShown = renderYakuName(i);
		if (isShown && (maxShown < (signed)i)) {
			maxShown = (signed)i;
			sound::Play(sound::IDs::sndPage);
		}
	}
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreen::TableSubsceneAgariScreen(LPDIRECT3DDEVICE9 device) : TableSubsceneAgariScreenProto(device) {
}
TableSubsceneAgariScreen::~TableSubsceneAgariScreen() {
}
void TableSubsceneAgariScreen::Render() {
	renderWindow();
	renderYakuName();
	myTextRenderer->Render();
	agariTehai->Render();
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::AgariTehai::AgariTehai(TableSubsceneAgariScreenProto* caller) : ShowTehai(caller->myDevice) {
	myCaller = caller;
	reconstFlag = true;
}
TableSubsceneAgariScreenProto::AgariTehai::~AgariTehai() {
}

void TableSubsceneAgariScreenProto::AgariTehai::Reconstruct(const GameTable* gameStat) {
	ShowTehai::Reconstruct(gameStat, gameStat->CurrentPlayer.Agari,
		[this](seatRelative) -> std::tuple<int, int> {
			const double Zeit = myCaller->seconds();
			const int yOffset = (Zeit >= 1.0) ? 0 : (int)(pow(1.0 - Zeit, 2) * (double)Geometry::BaseSize);
			return std::make_tuple(BaseX + 28, BaseY + 48 - yOffset);
		},
		sSelf, [](int){return (D3DCOLOR)0xffffffff;},
		[](const int*, const int*, int){});
}

void TableSubsceneAgariScreenProto::AgariTehai::Render() {
	const double Zeit = myCaller->seconds();
	if (reconstFlag) Reconstruct(GameStatus::gameStat());
	if (Zeit >= 1.0) reconstFlag = false;
	TileTexture->Render();
}

}
