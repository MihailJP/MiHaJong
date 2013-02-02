#pragma once

#include "../table.h"
#include "../../button.h"
#include <bitset>

namespace mihajong_graphic {

class GameTableScreen::ButtonReconst {
private:
	GameTableScreen* caller;
	ButtonPic* buttons;
public:
	enum ButtonID {
		btnChii1, btnChii2, btnChii3, btnPon, btnKan, btnPass, btnRon,
		btnMAXIMUM,
		btnKyuushu = 2, btnFlower, btnRiichi = 5, btnTsumo,
	};
private:
	struct BtnData { // ボタンの属性格納
		LPCTSTR label;
		const int x, y;
		const D3DCOLOR color;
	};
	static const BtnData buttonDat[2][btnMAXIMUM];
public:
	enum ButtonSet {btnSetNormal, btnSetTsumo};
	ButtonSet currentButtonSet;
	void ChangeButtonSet(ButtonSet btnSet);
private:
	std::bitset<btnMAXIMUM> buttonEnabled; // ボタン有効・無効の状態
public:
	std::bitset<btnMAXIMUM> areEnabled() {return buttonEnabled;}
	bool isEnabled(ButtonID buttonID) {return buttonEnabled[buttonID];}
	void enable(ButtonID buttonID);
	void disable(ButtonID buttonID);
	void enable(const std::bitset<btnMAXIMUM>& flagset);
private:
	void reconstruct(ButtonID buttonID);
public:
	void Render();
	explicit ButtonReconst(GameTableScreen* parent);
	~ButtonReconst();
};

}
