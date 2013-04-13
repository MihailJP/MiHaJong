#pragma once

#include "../../common/strcode.h"
#include "../directx.h"
#include <winuser.h>
#include <imm.h>
#include <tuple>
#include "../text.h"
#include <vector>
#include <functional>
#include "../timer.h"

namespace mihajong_graphic {

class EditBox { // 自前のエディットボックスもどき
private:
	static unsigned const halffontsz = 9; // 半角の横幅
	HWND myHWnd;
	TexturePtr myTexture;
	CodeConv::tstring myText;
	std::tuple<int, int, unsigned> myRegion;
	SmallTextRenderer* myTextRenderer;
	DevicePtr myDevice;
	unsigned maxStr;
	LPD3DXLINE cursorLine;
	unsigned cursorPos;
	unsigned scrollPos;
	Timer myTimer;
	class IMStat;
	unsigned scrollRBound(IMStat& imStat);
	void scroll(IMStat& imStat);
	float myScale;
	TransformMatrix getMatrix(int X, int Y, unsigned width);
	void renderFrame(int X, int Y, unsigned width);
	void renderIMCandidateFrame(int X, int Y, unsigned width, unsigned lines);
	void renderNormalText(IMStat& imStat, unsigned start, unsigned end, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol);
	void renderIMText(IMStat& imStat, int X, int Y, unsigned& TextID, unsigned& cols, signed& cursorcol);
	void renderIMCandidates(IMStat& imStat, int X, int Y, unsigned& TextID);
	void renderCursor(IMStat& imStat, int X, int Y, signed& cursorcol);
	bool isActive;
public:
	EditBox(HWND hwnd, DevicePtr device, int X, int Y, unsigned width, float scale = 1.0f);
	~EditBox();
	void Render();
	void KeyboardInput(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void activate() {isActive = true;}
	void deactivate() {isActive = false;}
	bool is_Active() {return isActive;}
	void setText(const CodeConv::tstring& newstr);
	CodeConv::tstring getText() {return myText;}
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
