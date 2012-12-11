#pragma once

#include "../mjcore/strcode.h"
#include <d3dx9.h>
#include <dxerr.h>
#include <Imm.h>
#include <tuple>
#include "text.h"
#include <vector>

namespace mihajong_graphic {

class EditBox { // 自前のエディットボックスもどき
private:
	HWND myHWnd;
	CodeConv::tstring myText;
	std::tuple<int, int, unsigned, unsigned> myRegion;
	SmallTextRenderer* myTextRenderer;
	LPDIRECT3DDEVICE9 myDevice;
	unsigned maxStr;
	class IMStat;
public:
	EditBox(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned width, unsigned height);
	~EditBox();
	void Render();
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
};

}
