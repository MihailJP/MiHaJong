#include "editbox.h"
#include <cmath>
#include "extchar.h"
#include "resource.h"
#include "loadtex.h"
#include "sprite.h"
#include <cassert>

namespace mihajong_graphic {

EditBox::EditBox(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned width) {
	myHWnd = hwnd; myDevice = device;
	myRegion = std::make_tuple(X, Y, width);
	myTextRenderer = new SmallTextRenderer(device);
	D3DXCreateLine(device, &cursorLine);
	maxStr = 0u; cursorPos = 0u;
	cursorBlinkStart = currTime();
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_TEXTBOX), 88, 56);
	for (int i = 0; i < 3; i++)
		if (FAILED(D3DXCreateSprite(device, &(mySprites[i]))))
			throw _T("スプライトの生成に失敗しました");
}

EditBox::~EditBox() {
	for (auto k = myCSprites.begin(); k != myCSprites.end(); ++k)
		(*k)->Release();
	for (int i = 0; i < 3; i++)
		if (mySprites[i]) mySprites[i]->Release();
	if (myTexture) myTexture->Release();
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

D3DXMATRIX EditBox::getMatrix(int X, int Y, unsigned width) {
	D3DXMATRIX mat, mat1; D3DXMatrixIdentity(&mat); D3DXMatrixIdentity(&mat1);
	D3DXMatrixTranslation(&mat, -X, -Y, 0.0f);
	D3DXMatrixScaling(&mat1, (float)(width * halffontsz) / 77.0f, 1.0f, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixTranslation(&mat1, X, Y, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixScaling(&mat1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	return mat;
}

void EditBox::renderFrame(int X, int Y, unsigned width) {
	RECT rect; rect.left = 0; rect.top = 0; rect.right = 5; rect.bottom = 28;
	SpriteRenderer::ShowSprite(mySprites[0], myTexture, X - 5, Y - 5, 5, 28, 0xffffffff, &rect);
	rect.left = 5; rect.right = 82;
	D3DXMATRIX mat = getMatrix(X, Y, width);
	SpriteRenderer::ShowSprite(mySprites[1], myTexture, X, Y - 5, width * halffontsz, 28, 0xffffffff, &rect, 0, 0, &mat);
	rect.left = 82; rect.right = 87;
	SpriteRenderer::ShowSprite(mySprites[2], myTexture, X + width * halffontsz, Y - 5, 5, 28, 0xffffffff, &rect);
}

void EditBox::renderIMCandidateFrame(int X, int Y, unsigned width, unsigned lines) {
	assert(lines != 0);
	RECT rect;
	if (lines != (myCSprites.size() * 3 + 6)) {
		for (auto k = myCSprites.begin(); k != myCSprites.end(); ++k)
			(*k)->Release();
		myCSprites.resize(lines * 3 + 6);
		for (auto k = myCSprites.begin(); k != myCSprites.end(); ++k)
			if (FAILED(D3DXCreateSprite(myDevice, &(*k))))
				throw _T("スプライトの生成に失敗しました");
	}
	unsigned spriteNum = 0u;
	auto drawLine = [&rect, &spriteNum, X, Y, width, this](int y) -> void {
		rect.left = 0; rect.right = 5;
		SpriteRenderer::ShowSprite(myCSprites[spriteNum++], myTexture, X - 5, Y + y, 5, rect.bottom - rect.top, 0xffffffff, &rect);
		rect.left = 5; rect.right = 82;
		D3DXMATRIX mat = getMatrix(X, Y, width);
		SpriteRenderer::ShowSprite(myCSprites[spriteNum++], myTexture, X, Y + y, width * halffontsz, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &mat);
		rect.left = 82; rect.right = 87;
		SpriteRenderer::ShowSprite(myCSprites[spriteNum++], myTexture, X + width * halffontsz, Y + y, 5, rect.bottom - rect.top, 0xffffffff, &rect);
	};
	rect.top = 28 + 0; rect.bottom = 28 + 5; drawLine(-5);
	rect.top = 28 + 4; rect.bottom = 28 + 24;
	for (unsigned i = 0; i < lines; i++) drawLine(20 * i);
	rect.top = 28 + 24; rect.bottom = 28 + 28; drawLine(20 * lines);
}

void EditBox::renderNormalText(unsigned start, unsigned end, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol) {
	const unsigned startcol = cols;
	for (unsigned i = start; i < end; i++) {
		if (i == cursorPos) cursorcol = cols;
		cols += isFullWidth(myText[i]) ? 2 : 1;
	}
	myTextRenderer->NewText(TextID++, myText.substr(start, end - start), X + startcol * halffontsz, Y, 1.0f, 1.0f, 0xff333333);
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
			myTextRenderer->NewText(TextID++, convStr.substr(startchr, i - startchr), X + startcol * halffontsz, Y, 1.0f, 1.0f, color);
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
	unsigned wndcols = 0;
	for (unsigned i = pageStart; (i < candidates.size()) && (i < (pageStart + pageSize)); i++) {
		unsigned tmpcols = 0;
		for (auto k = candidates[i].begin(); k != candidates[i].end(); ++k)
			tmpcols += isFullWidth(*k) ? 2: 1;
		if (wndcols < tmpcols) wndcols = tmpcols;
	}
	if (!candidates.empty()) {
		CodeConv::tostringstream o; o << _T("(") << (candidateNum + 1) << _T(" / ") << candidates.size() << _T(")");
		if ((wndcols + 4) < o.str().size()) wndcols = o.str().size() - 4;
		renderIMCandidateFrame(X, Y, wndcols + 4, pageSize + 1);
		myTextRenderer->NewText(TextID++, o.str(), X, Y, 1.0f, 1.0f, 0xff666666);
	}
	for (unsigned i = pageStart; (i < candidates.size()) && (i < (pageStart + pageSize)); i++) {
		CodeConv::tostringstream o;
		o << (i - pageStart + 1) << ". " << candidates[i];
		myTextRenderer->NewText(TextID++, o.str(), X, Y + 20 * (i - pageStart + 1), 1.0f, 1.0f,
			(candidateNum == i) ? 0xffff6600 : 0xff333333);
	}
}

void EditBox::renderCursor(IMStat& imStat, int X, int Y, signed& cursorcol) {
	D3DXVECTOR2 vec[] = {
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * halffontsz), Geometry::WindowScale() * Y),
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * halffontsz), Geometry::WindowScale() * (Y + 18)),
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
	/* Initialize */
	int X, Y; unsigned width; std::tie(X, Y, width) = myRegion;
	unsigned TextID = 0u, cols = 0u; signed cursorcol = -1;
	IMStat imStat(myHWnd);

	/* Textbox */
	renderFrame(X, Y, width);

	/* Text */
	renderNormalText(0u, cursorPos, X, Y, TextID, cols, cursorcol);
	renderIMText(imStat, X, Y, TextID, cols, cursorcol);
	renderNormalText(cursorPos, myText.size(), X, Y, TextID, cols, cursorcol);
	if (cursorcol == -1) cursorcol = cols;

	/* Candidate words */
	renderIMCandidates(imStat, X, Y + 20, TextID);

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
