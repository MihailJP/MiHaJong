#include "editbox.h"
#include <cmath>
#include "../extchar.h"
#include "../resource.h"
#include "../loadtex.h"
#include "../sprite.h"
#include "../chrwidth.h"
#include <cassert>
#include <algorithm>
#include <stdexcept>

namespace mihajong_graphic {

using namespace character_width;

EditBox::EditBox(HWND hwnd, DevicePtr device, int X, int Y, unsigned width, float scale) {
	assert(width >= 8);
	myHWnd = hwnd; myDevice = device;
	myRegion = std::make_tuple(X, Y, width);
	myTextRenderer = new SmallTextRenderer(device);
#if defined(_WIN32) && defined(WITH_DIRECTX)
	D3DXCreateLine(device, &cursorLine);
#endif
	maxStr = 0u; cursorPos = 0u; scrollPos = 0u;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_TEXTBOX));
	isActive = false;
	myScale = scale;
}

EditBox::~EditBox() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (myTexture) myTexture->Release();
	if (cursorLine) cursorLine->Release();
#endif
	if (myTextRenderer) delete myTextRenderer;
}

TransformMatrix EditBox::getMatrix(int X, int Y, unsigned width) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix mat, mat1; D3DXMatrixIdentity(&mat); D3DXMatrixIdentity(&mat1);
	D3DXMatrixTranslation(&mat, -X, -Y, 0.0f);
	D3DXMatrixScaling(&mat1, (float)(width * halffontsz) / 77.0f, 1.0f, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixScaling(&mat1, myScale, myScale, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixTranslation(&mat1, X, Y, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixScaling(&mat1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	return mat;
#else
	/* TODO: OpenGL変換行列 */
	return 0;
#endif
}

void EditBox::renderFrame(int X, int Y, unsigned width) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix matrixScale; D3DXMatrixIdentity(&matrixScale); TransformMatrix matrixScale1; D3DXMatrixIdentity(&matrixScale1);
	D3DXMatrixTranslation(&matrixScale, (float)(-X), (float)(-Y), 0.0f);
	D3DXMatrixScaling(&matrixScale1, myScale, myScale, 1.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
	D3DXMatrixTranslation(&matrixScale1, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
	D3DXMatrixScaling(&matrixScale1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);

	RECT rect; rect.left = 0; rect.right = 5;
	if (isActive) {rect.top = 28; rect.bottom = 56;}
	else {rect.top = 0; rect.bottom = 28;}
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X - 5, Y - 5, 5, 28, 0xffffffff, &rect, 0, 0, &matrixScale);
	rect.left = 5; rect.right = 82;
	TransformMatrix mat = getMatrix(X, Y, width);
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X, Y - 5, width * halffontsz, 28, 0xffffffff, &rect, 0, 0, &mat);
	rect.left = 82; rect.right = 87;
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X + width * halffontsz, Y - 5, 5, 28, 0xffffffff, &rect, 0, 0, &matrixScale);
#else
	/* TODO: OpenGL変換行列 */
#endif
}

void EditBox::renderIMCandidateFrame(int X, int Y, unsigned width, unsigned lines) {
	assert(lines != 0);
	RECT rect;
	unsigned spriteNum = 0u;
	auto drawLine = [&rect, &spriteNum, X, Y, width, this](int y) -> void {
#if defined(_WIN32) && defined(WITH_DIRECTX)
		TransformMatrix matrixScale; D3DXMatrixIdentity(&matrixScale); TransformMatrix matrixScale1; D3DXMatrixIdentity(&matrixScale1);
		D3DXMatrixTranslation(&matrixScale, (float)(-X), (float)(-Y), 0.0f);
		D3DXMatrixScaling(&matrixScale1, myScale, myScale, 1.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
		D3DXMatrixTranslation(&matrixScale1, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);
		D3DXMatrixScaling(&matrixScale1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&matrixScale, &matrixScale, &matrixScale1);

		rect.left = 0; rect.right = 5;
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X - 5, Y + y, 5, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &matrixScale);
		rect.left = 5; rect.right = 82;
		TransformMatrix mat = getMatrix(X, Y, width);
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X, Y + y, width * halffontsz, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &mat);
		rect.left = 82; rect.right = 87;
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X + width * halffontsz, Y + y, 5, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &matrixScale);
#else
	/* TODO: OpenGL変換行列 */
#endif
	};
	rect.top = 28 + 0; rect.bottom = 28 + 5; drawLine(-5);
	rect.top = 28 + 4; rect.bottom = 28 + 24;
	for (unsigned i = 0; i < lines; i++) drawLine(20 * i);
	rect.top = 28 + 24; rect.bottom = 28 + 28; drawLine(20 * lines);
}

unsigned EditBox::scrollRBound(IMStat& imStat) {
	bool imeflag = imStat.isOpened();
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	unsigned cols = 0;
	CodeConv::tstring s(myText.substr(0, cursorPos) + imStat.getGCSCompStr() + myText.substr(cursorPos, myText.size()));
	for (unsigned i = scrollPos; i < s.size(); i++) {
		cols += isFullWidth(s[i]) ? 2 : 1;
		if (cols > width) return i;
		else if (cols == width) return i + 1;
	}
	return s.size();
}

void EditBox::renderNormalText(IMStat& imStat, unsigned start, unsigned end, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol) {
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	unsigned start_ = start, end_ = end;
	if (start < cursorPos) {
		if ((start_ += scrollPos) >= end_) return; // Do nothing if completely out of frame
	} else {
		if (cols >= width) return; // Do nothing if completely out of frame
	}
	const unsigned startcol = cols;
	for (unsigned i = start_; i < end_; i++) {
		if (i == cursorPos) cursorcol = cols;
		cols += isFullWidth(myText[i]) ? 2 : 1;
		if (cols > width) end_ = i;
		else if (cols == width) end_ = i + 1;
	}
	if (isLeadingByte(myText, end_ - 1)) --end_;
	myTextRenderer->NewText(TextID++, myText.substr(start_, end_ - start_), X + startcol * myScale * halffontsz, Y, myScale, 1.0f, 0xffffffff);
}

void EditBox::renderIMText(IMStat& imStat, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol) {
	unsigned startcol = 0u, startchr = 0u; BYTE tmpInfo;
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	CodeConv::tstring convStr(imStat.getGCSCompStr());
	std::vector<BYTE> charInfo(imStat.getGCSCompAttr());
	int imCursor(imStat.getGCSCursorPos());
	startcol = cols;
	if (!convStr.empty()) cursorcol = -1;
	unsigned strStartPos = 0u;
	if (cursorPos < scrollPos) strStartPos = startchr = (scrollPos - cursorPos);
	for (unsigned i = strStartPos; i <= convStr.size(); i++) {
		if (convStr.empty()) break;
		if (i == imCursor) cursorcol = cols;
		if ((cols >= width) || (i == convStr.size()) || ((i > strStartPos) && (i < charInfo.size()) && (tmpInfo != charInfo[i]))) {
			ArgbColor color;
			switch (tmpInfo) {
			case ATTR_INPUT:
				color = 0xff00ccff; break;
			case ATTR_CONVERTED:
				color = 0xff00cc33; break;
			case ATTR_TARGET_CONVERTED:
				color = 0xffff6600; break;
			case ATTR_TARGET_NOTCONVERTED:
				color = 0xff9900ff; break;
			default:
				color = 0xffff0000; break;
			}
			if (isLeadingByte(convStr, i - 1)) --i;
			myTextRenderer->NewText(TextID++, convStr.substr(startchr, i - startchr), X + startcol * myScale * halffontsz, Y, myScale, 1.0f, color);
			startcol = cols; startchr = i;
			if ((cols >= width) || (i == convStr.size())) break;
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
		myTextRenderer->NewText(TextID++, o.str(), X, Y, myScale, 1.0f, 0xffcccccc);
	}
	for (unsigned i = pageStart; (i < candidates.size()) && (i < (pageStart + pageSize)); i++) {
		CodeConv::tostringstream o;
		o << (i - pageStart + 1) << ". " << candidates[i];
		myTextRenderer->NewText(TextID++, o.str(), X, Y + 20 * myScale * (i - pageStart + 1), myScale, 1.0f,
			(candidateNum == i) ? 0xffff6600 : 0xffffffff);
	}
}

void EditBox::renderCursor(IMStat& imStat, int X, int Y, signed& cursorcol) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	D3DXVECTOR2 vec[] = {
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * myScale * halffontsz), Geometry::WindowScale() * Y),
		D3DXVECTOR2(Geometry::WindowScale() * (X + cursorcol * myScale * halffontsz), Geometry::WindowScale() * (Y + int(18.0f * myScale))),
	};
	cursorLine->SetWidth(2);
	cursorLine->Begin();
	cursorLine->Draw(vec, 2,
		(imStat.isOpened() ? 0xffcc0000 : 0xff000066) |
		((int)(sin((double)myTimer.elapsed() / 200000.0) * 96.0 + 120.0) << 8)
		);
	cursorLine->End();
#else
	/* TODO: カーソル描画 */
#endif
}

void EditBox::scroll(IMStat& imStat) {
	unsigned width; std::tie(std::ignore, std::ignore, width) = myRegion;
	const CodeConv::tstring s(myText.substr(0, cursorPos) + imStat.getGCSCompStr() + myText.substr(cursorPos, myText.size()));
	const auto compAttr(imStat.getGCSCompAttr());
	const int paragraphLength =
		std::count_if(compAttr.begin(), compAttr.end(), [](BYTE p) {
			return ((p == ATTR_TARGET_CONVERTED) || (p == ATTR_TARGET_NOTCONVERTED));
		}
	);
	const unsigned trueRBound = (scrollRBound(imStat) < paragraphLength) ? 0 : (scrollRBound(imStat) - paragraphLength);
	unsigned textRightmostToFill = s.size(), fillCols = 0u;
	for (; textRightmostToFill > 0; textRightmostToFill--) {
		if (isFullWidth(s[textRightmostToFill - 1])) fillCols += 2;
		else fillCols += 1;
		if (fillCols >= width) {textRightmostToFill--; break;}
	}
	if (fillCols > width) textRightmostToFill++;
	if ((textRightmostToFill > 0 ) && (isLeadingByte(s, textRightmostToFill - 1))) textRightmostToFill++;

	while (true) {
		const unsigned trueCursorPos = cursorPos + ((imStat.getGCSCursorPos() < 0) ? 0 : imStat.getGCSCursorPos());
		if (trueCursorPos > (scrollRBound(imStat) - paragraphLength)) {
			try {
				if (isLeadingByte(s, scrollPos)) ++scrollPos;
				++scrollPos;
			} catch (std::out_of_range&) {}
		} else if (trueCursorPos < scrollPos) {
			try {
				--scrollPos;
				if (isLeadingByte(s, scrollPos - 1)) --scrollPos;
			} catch (std::out_of_range&) {}
		} else if (scrollPos > textRightmostToFill) {
			scrollPos = textRightmostToFill;
			break;
		} else break;
	}
}

void EditBox::Render() {
	/* Initialize */
	int X, Y; unsigned width; std::tie(X, Y, width) = myRegion;
	unsigned TextID = 0u, cols = 0u; signed cursorcol = -1;
	IMStat imStat(myHWnd);

	/* Textbox */
	renderFrame(X, Y, width);

	/* Text */
	scroll(imStat);
	renderNormalText(imStat, 0u, cursorPos, X, Y, TextID, cols, cursorcol);
	if (isActive) renderIMText(imStat, X, Y, TextID, cols, cursorcol);
	renderNormalText(imStat, cursorPos, myText.size(), X, Y, TextID, cols, cursorcol);
	if (cursorcol == -1) cursorcol = cols;

	/* Candidate words */
	renderIMCandidates(imStat, X + cursorcol * halffontsz, Y + 20 * myScale, TextID);

	/* Commit */
	for (unsigned i = TextID; i < maxStr; i++)
		myTextRenderer->DelText(i);
	maxStr = TextID;
	myTextRenderer->Render();

	/* Cursor */
	if (isActive)
		renderCursor(imStat, X, Y, cursorcol);
}

void EditBox::KeyboardInput(WPARAM wParam, LPARAM lParam) {
	if (!isActive) return;
	if (wParam == CHARDAT_CURSOR_LEFT) { // Cursor key
		if (cursorPos > 0) --cursorPos;
		try {
			if (isLeadingByte(myText, cursorPos - 1)) --cursorPos;
		} catch (std::out_of_range&) {}
	} else if (wParam == CHARDAT_CURSOR_RIGHT) { // Cursor key
		try {
			if (isLeadingByte(myText, cursorPos)) ++cursorPos;
			if (cursorPos < myText.size()) ++cursorPos;
		} catch (std::out_of_range&) {}
	} else if (wParam == CHARDAT_CURSOR_ENTER) { // Enter key
		/* Do nothing */
	} else if (wParam == CHARDAT_CURSOR_ESCAPE) { // Escape key
		/* Do nothing */
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
					if (isLeadingByte(myText, cursorPos - 1)) {
						myText = myText.substr(0, cursorPos - 1) + myText.substr(cursorPos, myText.size());
						--cursorPos;
					}
				} catch (std::out_of_range&) {}
			}
		}
	}
}

void EditBox::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	if (!isActive) return;
	if ((message == WM_IME_COMPOSITION) && (lParam & GCS_RESULTSTR)) {
		IMStat imStat(myHWnd);
		CodeConv::tstring resultStr(imStat.getGCSResultStr());
		myText = myText.substr(0, cursorPos) + resultStr + myText.substr(cursorPos, myText.size());
		cursorPos += resultStr.size();
	}
}

void EditBox::setText(const CodeConv::tstring& newstr) {
	myText = newstr;
	cursorPos = myText.length();
	scrollPos = 0;
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
