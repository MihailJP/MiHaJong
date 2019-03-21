#pragma once

#include "../table.h"
#include "../../widgets/button.h"
#include <bitset>
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "../../../common/mutex.h"
#include "../../timer.h"
#include "../../../common/discard.h"

namespace mihajong_graphic {

class GameTableScreen::ButtonReconst {
public:
	static constexpr unsigned ButtonRegionNum = 20u;
private:
	GameTableScreen* caller;
	ButtonPic* buttons;
#ifndef _WIN32
	bool initialized;
#endif /*_WIN32*/
public: // ボタン番号
	enum ButtonID {
		btnChii1, btnChii2, btnChii3, btnPon, btnKan, btnPass, btnRon,
		btnMAXIMUM,
		btnOpenRiichi = 0, btnRiichi, btnKyuushu, btnFlower, btnTsumo = btnRon,
	};
private: // ボタン属性
	struct BtnData { // ボタンの属性格納
		LPCTSTR label;
		const int x, y;
		const ArgbColor color;
	};
	static const BtnData buttonDat[2][btnMAXIMUM];
public:
	enum ButtonSet {btnSetNormal, btnSetTsumo};
	void ChangeButtonSet(ButtonSet btnSet);
private: // ボタンの有効・無効の状態
	ButtonSet currentButtonSet;
public:
	ButtonSet getButtonSet();
private:
	std::bitset<btnMAXIMUM> buttonEnabled; // ボタン有効・無効の状態
public:
	std::bitset<btnMAXIMUM> areEnabled();
	bool isEnabled(ButtonID buttonID);
	void enable(ButtonID buttonID);
	void disable(ButtonID buttonID);
	void enable(const std::bitset<btnMAXIMUM>& flagset);
	void setMode(DiscardTileNum::discardType, ButtonID, std::function<bool(int, GameTable*)> = [](int, GameTable*){return false;});
private: // ボタンの押し込み
	int sunkenButton;
	static constexpr int NoSunkenButton = -3;
public:
	bool isSunkenButtonExists();
	int getSunkenButtonID();
	void setSunkenButton(int buttonID = NoSunkenButton);
private: // カーソル
	int cursor;
	static constexpr int CursorDisabled = -3;
public:
	bool isCursorEnabled();
	int getCursor();
	void setCursor(int cursorPos = CursorDisabled);
	int incCursor();
	int decCursor();
private: // 再構築・表示処理
	void reconstruct(ButtonID buttonID);
	MUTEXLIB::recursive_mutex reconstructionCS;
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
