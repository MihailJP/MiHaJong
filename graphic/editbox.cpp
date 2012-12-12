#include "editbox.h"
#include <cmath>
#include "extchar.h"

namespace mihajong_graphic {

EditBox::EditBox(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned width, unsigned height) {
	myHWnd = hwnd; myDevice = device;
	myRegion = std::make_tuple(X, Y, width, height);
	myTextRenderer = new SmallTextRenderer(device);
	D3DXCreateLine(device, &cursorLine);
	maxStr = 0u; cursorPos = 0u;
	cursorBlinkStart = currTime();
}

EditBox::~EditBox() {
	if (cursorLine) cursorLine->Release();
	if (myTextRenderer) delete myTextRenderer;
}

bool EditBox::isFullWidth(wchar_t chr) {
	return !(((chr >= L' ') && (chr <= L'~')) || ((chr >= L'\uff61') && (chr <= L'\uff9f')));
}
bool EditBox::isFullWidth(char) {
	return false;
}

unsigned long long EditBox::currTime() {
	FILETIME t; GetSystemTimeAsFileTime(&t);
	return ((unsigned long long)t.dwHighDateTime << 32) | t.dwLowDateTime;
}

void EditBox::renderNormalText(unsigned start, unsigned end, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol) {
	const unsigned startcol = cols;
	for (unsigned i = start; i < end; i++) {
		if (i == cursorPos) cursorcol = cols;
		cols += isFullWidth(myText[i]) ? 2 : 1;
	}
	myTextRenderer->NewText(TextID++, myText.substr(start, end - start), X + startcol * 9, Y, 1.0f, 1.0f, 0xff333333);
}

void EditBox::renderIMText(IMStat& imStat, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol) {
	unsigned startcol = 0u, startchr = 0u; BYTE tmpInfo;
	CodeConv::tstring convStr(imStat.getGCSCompStr());
	std::vector<BYTE> charInfo(imStat.getGCSCompAttr());
	int imCursor(imStat.getGCSCursorPos());
	startcol = cols;
	if (!convStr.empty()) cursorcol = -1;
	for (unsigned i = 0u; i <= convStr.size(); i++) {
		if (convStr.empty()) break;
		if (i == imCursor) cursorcol = cols;
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
}

void EditBox::renderIMCandidates(IMStat& imStat, int X, int Y, unsigned& TextID) {
	unsigned candidateNum, pageStart, pageSize;
	std::vector<CodeConv::tstring> candidates;
	std::tie(candidateNum, candidates, pageStart, pageSize) = imStat.getCandidateList();
	for (unsigned i = pageStart; (i < candidates.size()) && (i < (pageStart + pageSize)); i++) {
		CodeConv::tostringstream o;
		o << (i - pageStart + 1) << ". " << candidates[i];
		myTextRenderer->NewText(TextID++, o.str(), X, Y + 20 * (i - pageStart + 1), 1.0f, 1.0f,
			(candidateNum == i) ? 0xffff6600 : 0xff999999);
	}
}

void EditBox::renderCursor(IMStat& imStat, int X, int Y, signed& cursorcol) {
	D3DXVECTOR2 vec[] = {
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * 9), Geometry::WindowScale() * Y),
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * 9), Geometry::WindowScale() * (Y + 18)),
	};
	cursorLine->SetWidth(2);
	cursorLine->Begin();
	cursorLine->Draw(vec, 2,
		(imStat.isOpened() ? 0xffcc0000 : 0xff000066) |
		((int)(sin((double)(currTime() - cursorBlinkStart) / 2000000.0) * 96.0 + 120.0) << 8)
		);
	cursorLine->End();
}

void EditBox::Render() {
	/* Textbox */
	int X, Y; std::tie(X, Y, std::ignore, std::ignore) = myRegion;
	unsigned TextID = 0u, cols = 0u; signed cursorcol = -1;
	IMStat imStat(myHWnd);
	renderNormalText(0u, cursorPos, X, Y, TextID, cols, cursorcol);
	renderIMText(imStat, X, Y, TextID, cols, cursorcol);
	renderNormalText(cursorPos, myText.size(), X, Y, TextID, cols, cursorcol);
	if (cursorcol == -1) cursorcol = cols;

	/* Candidate words */
	renderIMCandidates(imStat, X, Y, TextID);

	/* Commit */
	for (unsigned i = TextID; i < maxStr; i++)
		myTextRenderer->DelText(i);
	maxStr = TextID;
	myTextRenderer->Render();

	/* Cursor */
	renderCursor(imStat, X, Y, cursorcol);
}

bool EditBox::isLeadingByte(wchar_t chr) {
	return false;
}
bool EditBox::isLeadingByte(char chr) {
	if (GetACP() == 932)
		return ((chr >= 0x81) && (chr <= 0x9f)) || ((chr >= 0xe0) && (chr <= 0xfc));
	else return false;
}

void EditBox::KeyboardInput(WPARAM wParam, LPARAM lParam) {
	if (wParam == CHARDAT_CURSOR_LEFT) { // Cursor key
		if (cursorPos > 0) --cursorPos;
		try {
			if (isLeadingByte(myText.at(cursorPos))) --cursorPos;
		} catch (std::out_of_range&) {}
	} else if (wParam == CHARDAT_CURSOR_RIGHT) { // Cursor key
		try {
			if (isLeadingByte(myText.at(cursorPos))) ++cursorPos;
			if (cursorPos < myText.size()) ++cursorPos;
		} catch (std::out_of_range&) {}
	} else {
		WCHAR Letter[2] = {(WCHAR)wParam, 0};
		if (Letter[0] >= L' ') { // Ordinary
			CodeConv::tstring currentInput = CodeConv::EnsureTStr(std::wstring(Letter));
			myText = myText.substr(0, cursorPos) + currentInput + myText.substr(cursorPos, myText.size());
			++cursorPos;
		} else if ((Letter[0] == L'\b') && (!myText.empty())) { // Backspace
			if (cursorPos > 0) {
				try {
					myText = myText.substr(0, cursorPos - 1) + myText.substr(cursorPos, myText.size());
					--cursorPos;
					if (isLeadingByte(myText.at(cursorPos))) {
						myText = myText.substr(0, cursorPos - 1) + myText.substr(cursorPos, myText.size());
						--cursorPos;
					}
				} catch (std::out_of_range&) {}
			}
		}
	}
}

void EditBox::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	if ((message == WM_IME_COMPOSITION) && (lParam & GCS_RESULTSTR)) {
		IMStat imStat(myHWnd);
		CodeConv::tstring resultStr(imStat.getGCSResultStr());
		myText = myText.substr(0, cursorPos) + resultStr + myText.substr(cursorPos, myText.size());
		cursorPos += resultStr.size();
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
	if ((bufSize == IMM_ERROR_NODATA) || (bufSize == IMM_ERROR_GENERAL)) {
		return std::vector<BYTE>();
	} else if (bufSize) {
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

int EditBox::IMStat::getGCSCursorPos() {
	return ImmGetCompositionString(hIMC, GCS_CURSORPOS, nullptr, 0);
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
