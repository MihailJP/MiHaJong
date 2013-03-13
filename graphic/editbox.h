#pragma once

#include "../common/strcode.h"
#include "directx.h"
#include <WinUser.h>
#include <Imm.h>
#include <tuple>
#include "text.h"
#include <vector>
#include <functional>

namespace mihajong_graphic {

class EditBox { // 自前のエディットボックスもどき
private:
	static unsigned const halffontsz = 9; // 半角の横幅
	HWND myHWnd;
	LPDIRECT3DTEXTURE9 myTexture;
	CodeConv::tstring myText;
	std::tuple<int, int, unsigned> myRegion;
	SmallTextRenderer* myTextRenderer;
	LPDIRECT3DDEVICE9 myDevice;
	unsigned maxStr;
	LPD3DXLINE cursorLine;
	unsigned cursorPos;
	unsigned scrollPos;
	unsigned long long cursorBlinkStart;
	unsigned long long currTime();
	class IMStat;
	unsigned scrollRBound(IMStat& imStat);
	void scroll(IMStat& imStat);
	D3DXMATRIX getMatrix(int X, int Y, unsigned width);
	void renderFrame(int X, int Y, unsigned width);
	void renderIMCandidateFrame(int X, int Y, unsigned width, unsigned lines);
	void renderNormalText(IMStat& imStat, unsigned start, unsigned end, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol);
	void renderIMText(IMStat& imStat, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol);
	void renderIMCandidates(IMStat& imStat, int X, int Y, unsigned& TextID);
	void renderCursor(IMStat& imStat, int X, int Y, signed& cursorcol);
	bool isActive;
public:
	EditBox(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned width);
	~EditBox();
	void Render();
	void KeyboardInput(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void activate() {isActive = true;}
	void deactivate() {isActive = false;}
};

class EditBox::IMStat { // IMEの状態
private:
	HWND hwnd; HIMC hIMC;
	IMStat(const IMStat&) {}; // disable copy constructor
	std::vector<BYTE> getCompositionString(DWORD InfoType);
public:
	IMStat(HWND hWnd);
	~IMStat();
	bool isOpened();
	std::tuple<DWORD, DWORD> getConvStat();
	CodeConv::tstring getGCSCompStr();
	std::vector<BYTE> getGCSCompAttr();
	CodeConv::tstring getGCSResultStr();
	int getGCSCursorPos();
	std::tuple<unsigned, std::vector<CodeConv::tstring>, unsigned, unsigned> getCandidateList();
};

}
