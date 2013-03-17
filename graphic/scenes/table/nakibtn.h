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
public: // �{�^���ԍ�
	enum ButtonID {
		btnChii1, btnChii2, btnChii3, btnPon, btnKan, btnPass, btnRon,
		btnMAXIMUM,
		btnKyuushu = 2, btnFlower, btnRiichi = 5, btnTsumo,
	};
private: // �{�^������
	struct BtnData { // �{�^���̑����i�[
		LPCTSTR label;
		const int x, y;
		const D3DCOLOR color;
	};
	static const BtnData buttonDat[2][btnMAXIMUM];
public:
	enum ButtonSet {btnSetNormal, btnSetTsumo};
	void ChangeButtonSet(ButtonSet btnSet);
private: // �{�^���̗L���E�����̏��
	ButtonSet currentButtonSet;
public:
	ButtonSet getButtonSet() {return currentButtonSet;}
private:
	std::bitset<btnMAXIMUM> buttonEnabled; // �{�^���L���E�����̏��
public:
	std::bitset<btnMAXIMUM> areEnabled() {return buttonEnabled;}
	bool isEnabled(ButtonID buttonID) {return buttonEnabled[buttonID];}
	void enable(ButtonID buttonID);
	void disable(ButtonID buttonID);
	void enable(const std::bitset<btnMAXIMUM>& flagset);
private: // �{�^���̉�������
	int sunkenButton;
	static const int NoSunkenButton = -3;
public:
	bool isSunkenButtonExists() {return sunkenButton != NoSunkenButton;}
	int getSunkenButtonID() {return sunkenButton;}
	void setSunkenButton(int buttonID = NoSunkenButton) {sunkenButton = buttonID;}
private: // �J�[�\��
	int cursor;
	static const int CursorDisabled = -3;
public:
	bool isCursorEnabled() {return cursor != CursorDisabled;}
	int getCursor() {return cursor;}
	void setCursor(int cursorPos = CursorDisabled) {cursor = cursorPos;}
	int incCursor() {return ++cursor;}
	int decCursor() {return --cursor;}
private: // �č\�z�E�\������
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
private: // ���Ԋ֌W
	Timer myTimer;
};

}
