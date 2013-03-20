#pragma once

#include "../table.h"
#include "../../button.h"
#include <bitset>
#include <Windows.h>
#include "../../../common/mutex.h"
#include "../../timer.h"

namespace mihajong_graphic {

class GameTableScreen::ButtonReconst {
public:
	static const unsigned ButtonRegionNum = 20u;
private:
	GameTableScreen* caller;
	ButtonPic* buttons;
public: // ボタン番号
	enum ButtonID {
		btnChii1, btnChii2, btnChii3, btnPon, btnKan, btnPass, btnRon,
		btnMAXIMUM,
		btnKyuushu = 2, btnFlower, btnRiichi = 5, btnTsumo,
	};
private: // ボタン属性
	struct BtnData { // ボタンの属性格納
		LPCTSTR label;
		const int x, y;
		const D3DCOLOR color;
	};
	static const BtnData buttonDat[2][btnMAXIMUM];
public:
	enum ButtonSet {btnSetNormal, btnSetTsumo};
	void ChangeButtonSet(ButtonSet btnSet);
private: // ボタンの有効・無効の状態
	ButtonSet currentButtonSet;
public:
	ButtonSet getButtonSet() {return currentButtonSet;}
private:
	std::bitset<btnMAXIMUM> buttonEnabled; // ボタン有効・無効の状態
public:
	std::bitset<btnMAXIMUM> areEnabled() {return buttonEnabled;}
	bool isEnabled(ButtonID buttonID) {return buttonEnabled[buttonID];}
	void enable(ButtonID buttonID);
	void disable(ButtonID buttonID);
	void enable(const std::bitset<btnMAXIMUM>& flagset);
private: // ボタンの押し込み
	int sunkenButton;
	static const int NoSunkenButton = -3;
public:
	bool isSunkenButtonExists() {return sunkenButton != NoSunkenButton;}
	int getSunkenButtonID() {return sunkenButton;}
	void setSunkenButton(int buttonID = NoSunkenButton) {sunkenButton = buttonID;}
private: // カーソル
	int cursor;
	static const int CursorDisabled = -3;
public:
	bool isCursorEnabled() {return cursor != CursorDisabled;}
	int getCursor() {return cursor;}
	void setCursor(int cursorPos = CursorDisabled) {cursor = cursorPos;}
	int incCursor() {return ++cursor;}
	int decCursor() {return --cursor;}
private: // 再構築・表示処理
	void reconstruct(ButtonID buttonID);
	MHJMutex reconstructionCS;
public:
	void btnSetForDahai();
	void btnSetForNaki();
	void ButtonPressed();
	void reconstruct();
	void Render();
	explicit ButtonReconst(GameTableScreen* parent);
	~ButtonReconst();
private: // 時間関係
	Timer myTimer;
};

}
