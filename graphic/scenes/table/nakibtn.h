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
		btnChii, btnPon, btnKan, btnFlower, btnRiichi, btnAgari, btnPass,
		btnMAXIMUM,
	};
private:
	struct BtnData { // É{É^ÉìÇÃëÆê´äiî[
		LPCTSTR label;
		const int x, y;
		const unsigned width, height;
		const D3DCOLOR color;
	};
	static const BtnData buttonDat[btnMAXIMUM];
public:
	void Render();
	explicit ButtonReconst(GameTableScreen* parent);
	~ButtonReconst();
};

}
