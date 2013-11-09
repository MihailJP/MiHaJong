#pragma once

#include "../../common/strcode.h"
#include "../directx.h"
#ifdef _WIN32
#include <winuser.h>
#include <imm.h>
#endif /*_WIN32*/
#include <tuple>
#include "../text.h"
#include <vector>
#include <functional>
#include "../timer.h"

namespace mihajong_graphic {

class EditBox { // 自前のエディットボックスもどき
private:
	static unsigned const halffontsz = 9; // 半角の横幅
#ifdef _WIN32
	HWND myHWnd;
#else /*_WIN32*/
	Window myHWnd;
#endif /*_WIN32*/
	TexturePtr myTexture;
	CodeConv::tstring myText;
	std::tuple<int, int, unsigned> myRegion;
	SmallTextRenderer* myTextRenderer;
	DevicePtr myDevice;
	unsigned maxStr;
	LinePtr cursorLine;
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
#ifdef _WIN32
	EditBox(HWND hwnd, DevicePtr device, int X, int Y, unsigned width, float scale = 1.0f);
#else /*_WIN32*/
	EditBox(Window hwnd, DevicePtr device, int X, int Y, unsigned width, float scale = 1.0f);
#endif /*_WIN32*/
	EditBox(const EditBox&) = delete; // Delete unexpected copy constructor
	EditBox& operator= (const EditBox&) = delete; // Delete unexpected assign operator
	~EditBox();
	void Render();
#ifdef _WIN32
	void KeyboardInput(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
#else /*_WIN32*/
	/* TODO: Linuxでは日本語入力が未実装 */
	void KeyboardInput(const XEvent* od);
#endif /*_WIN32*/
	void activate() {isActive = true;}
	void deactivate() {isActive = false;}
	bool is_Active() {return isActive;}
	void setText(const CodeConv::tstring& newstr);
	CodeConv::tstring getText() {return myText;}
};

#ifdef _WIN32
class EditBox::IMStat { // IMEの状態
private:
	HWND hwnd; HIMC hIMC;
	std::vector<BYTE> getCompositionString(DWORD InfoType);
public:
	IMStat(HWND hWnd);
	IMStat(const IMStat&) = delete; // Delete unexpected copy constructor
	IMStat& operator= (const IMStat&) = delete; // Delete unexpected assign operator
	~IMStat();
	bool isOpened();
	std::tuple<DWORD, DWORD> getConvStat();
	CodeConv::tstring getGCSCompStr();
	std::vector<BYTE> getGCSCompAttr();
	CodeConv::tstring getGCSResultStr();
	int getGCSCursorPos();
	std::tuple<unsigned, std::vector<CodeConv::tstring>, unsigned, unsigned> getCandidateList();
};
#else /*_WIN32*/
/* TODO: Linuxでは日本語入力が未実装（この実装はダミー） */
class EditBox::IMStat { // IMEの状態(ダミー)
private:
	IMStat(const IMStat&) {}; // disable copy constructor
public:
	IMStat(void*) {}
	IMStat(const IMStat&) = delete; // Delete unexpected copy constructor
	IMStat& operator= (const IMStat&) = delete; // Delete unexpected assign operator
	~IMStat() {}
	bool isOpened() {return false;}
	std::tuple<uint32_t, uint32_t> getConvStat() {return std::make_tuple(0u, 0u);}
	CodeConv::tstring getGCSCompStr() {return _T("");}
	std::vector<char> getGCSCompAttr() {return std::vector<char>();}
	CodeConv::tstring getGCSResultStr() {return _T("");}
	int getGCSCursorPos() {return 0;}
	std::tuple<unsigned, std::vector<CodeConv::tstring>, unsigned, unsigned> getCandidateList() {
		return std::make_tuple(0u, std::vector<CodeConv::tstring>(), 0u, 0u);
	}
};
#endif /*_WIN32*/

}
