#pragma once

#include "../table.h"
#include "../../widgets/button.h"
#include <bitset>
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include <mutex>
#include "../../timer.h"
#include "../../../common/discard.h"

namespace mihajong_graphic {

enum class ButtonID {
	chii = 2, pon, kan, pass, ron,
	maximum,
	openRiichi = 0, riichi, kyuushu, flower, tsumo = ron,
};
constexpr auto btnMAXIMUM = static_cast<unsigned int>(ButtonID::maximum);
enum class ButtonSet {normal, tsumo};

class GameTableScreen::ButtonReconst {
public:
	static constexpr unsigned ButtonRegionNum = 20u;
private:
	GameTableScreen* caller;
	ButtonPic* buttons;
#ifndef _WIN32
	bool initialized;
#endif /*_WIN32*/
private: // ボタン属性
	struct BtnData { // ボタンの属性格納
		LPCTSTR label;
		const int x, y;
		const ArgbColor color;
	};
	static const BtnData buttonDat[2][btnMAXIMUM];
	BtnData getButtonData(const ButtonSet buttonSet, const ButtonID buttonID) { return buttonDat[static_cast<int>(buttonSet)][static_cast<int>(buttonID)]; }
public:
	void ChangeButtonSet(ButtonSet btnSet);
private: // ボタンの有効・無効の状態
	ButtonSet currentButtonSet;
public:
	ButtonSet getButtonSet();
	class ButtonFlags : public std::bitset<btnMAXIMUM> {
	public:
		constexpr bool operator[] (ButtonID btn) const { return std::bitset<btnMAXIMUM>::operator[](static_cast<std::size_t>(btn)); }
		reference operator[] (ButtonID btn) { return std::bitset<btnMAXIMUM>::operator[](static_cast<std::size_t>(btn)); }
		ButtonFlags() = default;
		ButtonFlags(const ButtonFlags&) = default;
		ButtonFlags& operator = (const ButtonFlags& p) = default;
	};
private:
	ButtonFlags buttonEnabled; // ボタン有効・無効の状態
public:
	ButtonFlags areEnabled();
	bool isEnabled(ButtonID buttonID);
	void enable(ButtonID buttonID);
	void disable(ButtonID buttonID);
	void enable(const std::bitset<btnMAXIMUM>& flagset);
	void setMode(mihajong_structs::DiscardType, ButtonID, std::function<bool(int, GameTable*)> = [](int, GameTable*){return false;});
private: // ボタンの押し込み
	ButtonID sunkenButton;
	static constexpr auto NoSunkenButton = static_cast<ButtonID>(-3);
public:
	bool isSunkenButtonExists();
	ButtonID getSunkenButtonID();
	void setSunkenButton(ButtonID buttonID = NoSunkenButton);
private: // カーソル
	ButtonID cursor;
	static constexpr auto CursorDisabled = static_cast<ButtonID>(-3);
public:
	bool isCursorEnabled();
	ButtonID getCursor();
	void setCursor(ButtonID cursorPos = CursorDisabled);
	void setCursor(int cursorPos) { setCursor(static_cast<ButtonID>(cursorPos)); }
	ButtonID incCursor();
	ButtonID decCursor();
private: // 再構築・表示処理
	void reconstruct(ButtonID buttonID);
	std::recursive_mutex reconstructionCS;
public:
	void btnSetForDahai();
	void btnSetForNaki();
	void ButtonPressed();
	void reconstruct();
	void Render();
	explicit ButtonReconst(GameTableScreen* parent);
	ButtonReconst(const ButtonReconst&) = delete; // Delete unexpected copy constructor
	ButtonReconst& operator= (const ButtonReconst&) = delete; // Delete unexpected assign operator
	~ButtonReconst();
private: // 時間関係
	Timer myTimer;
};

}
