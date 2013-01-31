#pragma once

#include "../table.h"
#include "../../button.h"

namespace mihajong_graphic {

class GameTableScreen::ButtonReconst {
private:
	GameTableScreen* caller;
	ButtonPic* buttons;
public:
	enum ButtonID {
		btnChii1, btnChii2, btnChii3, btnPon, btnKan, btnRiichi, btnRon, btnPass,
		btnMAXIMUM,
		btnKyuushu = 2, btnFlower, btnTsumo = 6,
	};
private:
	struct BtnData { // É{É^ÉìÇÃëÆê´äiî[
		LPCTSTR label;
		const int x, y;
		const D3DCOLOR color;
	};
	static const BtnData buttonDat[2][btnMAXIMUM];
public:
	void Render();
	explicit ButtonReconst(GameTableScreen* parent);
	~ButtonReconst();
};

}
