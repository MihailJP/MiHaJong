#include "editbox.h"

namespace mihajong_graphic {

EditBox::EditBox(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned width, unsigned height) {
	myHWnd = hwnd; myDevice = device;
	myRegion = std::make_tuple(X, Y, width, height);
	myTextRenderer = new SmallTextRenderer(device);
	maxStr = 0u;
}

EditBox::~EditBox() {
	delete myTextRenderer;
}

void EditBox::Render() {
	int X, Y; std::tie(X, Y, std::ignore, std::ignore) = myRegion;
	IMStat imStat(myHWnd);
	CodeConv::tstring convStr(imStat.getGCSCompStr());
	std::vector<BYTE> charInfo(imStat.getGCSCompAttr());
	unsigned TextID = 0u, cols = 0u, startcol = 0u, startchr = 0u; BYTE tmpInfo;
	for (unsigned i = 0u; i <= convStr.size(); i++) {
		if (convStr.empty()) break;
		if ((i == convStr.size()) || ((i > 0) && (i < charInfo.size()) && (tmpInfo != charInfo[i]))) {
			D3DCOLOR color;
			switch (tmpInfo) {
			case ATTR_INPUT:
				color = 0xff336699; break;
			case ATTR_CONVERTED:
				color = 0xff993366; break;
			case ATTR_TARGET_CONVERTED:
				color = 0xff996633; break;
			case ATTR_TARGET_NOTCONVERTED:
				color = 0xff669933; break;
			default:
				color = 0xff666666; break;
			}
			myTextRenderer->NewText(TextID++, convStr.substr(startchr, i - startchr), X + startcol * 9, Y, 1.0f, 1.0f, color);
			startcol = cols; startchr = i;
			if (i == convStr.size()) break;
		}
		if (i < charInfo.size()) tmpInfo = charInfo[i];
		else tmpInfo = -1;
#ifdef UNICODE
		if (((convStr[i] >= L' ') && (convStr[i] <= L'~')) || ((convStr[i] >= L'\uff61') && (convStr[i] <= L'\uff9f')))
			cols += 1;
		else cols += 2;
#else
		cols += 1;
#endif
	}
	CodeConv::tstring resultStr(imStat.getGCSResultStr());
	myTextRenderer->NewText(TextID++, resultStr, X, Y + 20, 1.0f, 1.0f, 0xff009900);
	for (unsigned i = TextID; i < maxStr; i++)
		myTextRenderer->DelText(i);
	maxStr = TextID;
	myTextRenderer->Render();
}

// -------------------------------------------------------------------------

EditBox::IMStat::IMStat(HWND hWnd) {
	hwnd = hWnd;
	hIMC = ImmGetContext(hwnd);
}

EditBox::IMStat::~IMStat() {
	ImmReleaseContext(hwnd, hIMC);
}

bool EditBox::IMStat::isOpened() {
	return (bool)ImmGetOpenStatus(hIMC);
}

std::tuple<DWORD, DWORD> EditBox::IMStat::getConvStat() {
	DWORD fdwConversion, fdwSentence;
	ImmGetConversionStatus(hIMC, &fdwConversion, &fdwSentence);
	return std::make_tuple(fdwConversion, fdwSentence);
}

std::vector<BYTE> EditBox::IMStat::getCompositionString(DWORD InfoType) {
	std::vector<BYTE> data;
	DWORD bufSize = (DWORD)ImmGetCompositionString(hIMC, InfoType, nullptr, 0);
	if (bufSize) {
		data.resize(bufSize);
		ImmGetCompositionString(hIMC, InfoType, &data[0], bufSize);
	}
	return data;
}

CodeConv::tstring EditBox::IMStat::getGCSCompStr() {
	std::vector<BYTE> data(getCompositionString(GCS_COMPSTR));
	for (int i = 0; i < sizeof(TCHAR); i++)
		data.push_back(0);
	return CodeConv::tstring(reinterpret_cast<LPTSTR>(&(data[0])));
}

std::vector<BYTE> EditBox::IMStat::getGCSCompAttr() {
	return getCompositionString(GCS_COMPATTR);
}

CodeConv::tstring EditBox::IMStat::getGCSResultStr() {
	std::vector<BYTE> data(getCompositionString(GCS_RESULTSTR));
	for (int i = 0; i < sizeof(TCHAR); i++)
		data.push_back(0);
	return CodeConv::tstring(reinterpret_cast<LPTSTR>(&(data[0])));
}

}
