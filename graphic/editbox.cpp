#include "editbox.h"
#include <iomanip>

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

bool EditBox::isFullWidth(wchar_t chr) {
	return !(((chr >= L' ') && (chr <= L'~')) || ((chr >= L'\uff61') && (chr <= L'\uff9f')));
}
bool EditBox::isFullWidth(char) {
	return false;
}


void EditBox::Render() {
	int X, Y; std::tie(X, Y, std::ignore, std::ignore) = myRegion;
	IMStat imStat(myHWnd);
	CodeConv::tstring convStr(imStat.getGCSCompStr());
	std::vector<BYTE> charInfo(imStat.getGCSCompAttr());
	unsigned TextID = 0u, cols = 0u, startcol = 0u, startchr = 0u; BYTE tmpInfo;
	for (unsigned i = 0u; i < myText.size(); i++)
		cols += isFullWidth(myText[i]) ? 2 : 1;
	myTextRenderer->NewText(TextID++, myText, X, Y, 1.0f, 1.0f, 0xff333333);
	startcol = cols;
	for (unsigned i = 0u; i <= convStr.size(); i++) {
		if (convStr.empty()) break;
		if ((i == convStr.size()) || ((i > 0) && (i < charInfo.size()) && (tmpInfo != charInfo[i]))) {
			D3DCOLOR color;
			switch (tmpInfo) {
			case ATTR_INPUT:
				color = 0xff0000ff; break;
			case ATTR_CONVERTED:
				color = 0xff00cc33; break;
			case ATTR_TARGET_CONVERTED:
				color = 0xffff6600; break;
			case ATTR_TARGET_NOTCONVERTED:
				color = 0xff9900ff; break;
			default:
				color = 0xffff0000; break;
			}
			myTextRenderer->NewText(TextID++, convStr.substr(startchr, i - startchr), X + startcol * 9, Y, 1.0f, 1.0f, color);
			startcol = cols; startchr = i;
			if (i == convStr.size()) break;
		}
		if (i < charInfo.size()) tmpInfo = charInfo[i];
		else tmpInfo = -1;
		cols += isFullWidth(convStr[i]) ? 2 : 1;
	}

	unsigned candidateNum, pageStart, pageSize;
	std::vector<CodeConv::tstring> candidates;
	std::tie(candidateNum, candidates, pageStart, pageSize) = imStat.getCandidateList();
	for (unsigned i = pageStart; (i < candidates.size()) && (i < (pageStart + pageSize)); i++) {
		CodeConv::tostringstream o;
		o << (i - pageStart + 1) << ". " << candidates[i];
		myTextRenderer->NewText(TextID++, o.str(), X, Y + 20 * (i - pageStart + 1), 1.0f, 1.0f,
			(candidateNum == i) ? 0xffff6600 : 0xff999999);
	}

	for (unsigned i = TextID; i < maxStr; i++)
		myTextRenderer->DelText(i);
	maxStr = TextID;
	myTextRenderer->Render();
}

void EditBox::KeyboardInput(WPARAM wParam, LPARAM lParam) {
	HKL hkl = GetKeyboardLayout(0);
	BYTE keyState[256]; GetKeyboardState(keyState);
	WCHAR Letter[16] = {0,};
	int letters = ToUnicodeEx((UINT)wParam, (UINT)((lParam & 0x00ff0000) >> 16), keyState, Letter, 15, 0, hkl);
	if (letters <= 0) return; // No characters
	if (Letter[0] >= L' ') { // Ordinary
		CodeConv::tstring currentInput = CodeConv::EnsureTStr(std::wstring(Letter));
		myText.append(currentInput);
	} else if ((Letter[0] == L'\b') && (!myText.empty())) { // Backspace
		myText.pop_back();
#ifndef UNICODE
		if ((GetACP() == 932) && ((myText.back() >= 0x81) && (myText.back() <= 0x9f)) || ((myText.back() >= 0xe0) && (myText.back() <= 0xfc)))
			myText.pop_back();
#endif
	}
}

void EditBox::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	if ((message == WM_IME_COMPOSITION) && (lParam & GCS_RESULTSTR)) {
		IMStat imStat(myHWnd);
		CodeConv::tstring resultStr(imStat.getGCSResultStr());
		myText.append(resultStr);
	}
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

std::tuple<unsigned, std::vector<CodeConv::tstring>, unsigned, unsigned> EditBox::IMStat::getCandidateList() {
	DWORD BufSize = ImmGetCandidateList(hIMC, 0, nullptr, 0);
	BYTE* buf = new BYTE[BufSize];
	if (ImmGetCandidateList(hIMC, 0, reinterpret_cast<LPCANDIDATELIST>(buf), BufSize)) {
		unsigned selection = reinterpret_cast<LPCANDIDATELIST>(buf)->dwSelection;
		unsigned count = reinterpret_cast<LPCANDIDATELIST>(buf)->dwCount;
		unsigned start = reinterpret_cast<LPCANDIDATELIST>(buf)->dwPageStart;
		unsigned pagesize = reinterpret_cast<LPCANDIDATELIST>(buf)->dwPageSize;
		std::vector<CodeConv::tstring> candidates; candidates.resize(count);
		for (unsigned i = 0; i < count; i++)
			candidates[i] = CodeConv::tstring((LPTSTR)(buf + (reinterpret_cast<LPCANDIDATELIST>(buf)->dwOffset[i])));
		delete[] buf;
		return std::make_tuple(selection, candidates, start, pagesize);
	} else {
		delete[] buf;
		return std::make_tuple(0, std::vector<CodeConv::tstring>(), 0, 0);
	}
}

}
