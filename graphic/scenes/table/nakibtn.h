#pragma once

#include "../table.h"
#include "../../widgets/button.h"
#include <bitset>
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "../../../common/mutex.h"
#include "../../timer.h"

namespace mihajong_graphic {

class GameTableScreen::ButtonReconst {
public:
	static const unsigned ButtonRegionNum = 20u;
private:
	GameTableScreen* caller;
	ButtonPic* buttons;
#ifndef _WIN32
	bool initialized;
#endif /*_WIN32*/
public: // �{�^���ԍ�
	enum ButtonID {
		btnChii1, btnChii2, btnChii3, btnPon, btnKan, btnPass, btnRon,
		btnMAXIMUM,
		btnOpenRiichi = 0, btnRiichi, btnKyuushu, btnFlower, btnTsumo = btnRon,
	};
private: // �{�^������
	struct BtnData { // �{�^���̑����i�[
		LPCTSTR label;
		const int x, y;
		const ArgbColor color;
	};
	static const BtnData buttonDat[2][btnMAXIMUM];
public:
	enum ButtonSet {btnSetNormal, btnSetTsumo};
	void ChangeButtonSet(ButtonSet btnSet);
private: // �{�^���̗L���E�����̏��
	ButtonSet currentButtonSet;
public:
	ButtonSet getButtonSet();
private:
	std::bitset<btnMAXIMUM> buttonEnabled; // �{�^���L���E�����̏��
public:
	std::bitset<btnMAXIMUM> areEnabled();
	bool isEnabled(ButtonID buttonID);
	void enable(ButtonID buttonID);
	void disable(ButtonID buttonID);
	void enable(const std::bitset<btnMAXIMUM>& flagset);
private: // �{�^���̉�������
	int sunkenButton;
	static const int NoSunkenButton = -3;
public:
	bool isSunkenButtonExists();
	int getSunkenButtonID();
	void setSunkenButton(int buttonID = NoSunkenButton);
private: // �J�[�\��
	int cursor;
	static const int CursorDisabled = -3;
public:
	bool isCursorEnabled();
	int getCursor();
	void setCursor(int cursorPos = CursorDisabled);
	int incCursor();
	int decCursor();
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
