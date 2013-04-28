#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::TrayReconst {
private:
	GameTableScreen* caller;
private: /**** 起家マーク・ヤキトリマークを置く凹み ****/
	static const unsigned int TrayHLeft = 275;
	static const unsigned int TrayHTop = 75;
	static const unsigned int TrayHRight = 373;
	static const unsigned int TrayHBottom = 115;
	static const unsigned int TrayHWidth = TrayHRight - TrayHLeft;
	static const unsigned int TrayHHeight = TrayHBottom - TrayHTop;
	static const unsigned int TrayVLeft = 377;
	static const unsigned int TrayVTop = 1;
	static const unsigned int TrayVRight = 417;
	static const unsigned int TrayVBottom = 99;
	static const unsigned int TrayVWidth = TrayVRight - TrayVLeft;
	static const unsigned int TrayVHeight = TrayVBottom - TrayVTop;
	static const unsigned int TrayPosH = TableSize - 200;
	static const unsigned int TrayPosV = TableSize - 24;
	void ShowTray(); // 起家マークを置く場所表示
private: /**** 起家マーク ****/
	static const unsigned int PlateWidthH = 45;
	static const unsigned int PlateHeightH = 35;
	static const unsigned int PlateWidthV = 32;
	static const unsigned int PlateHeightV = 48;
	static const unsigned int PlatePadding = 1;
	static const unsigned int PlatePosH = TrayPosH + (PlateWidthH / 2) + (PlatePadding * 2);
	static const unsigned int PlatePosV = TrayPosV;
	TexturePtr tChiicha; // 起家マーク
	void ShowChiicha(const GameTable* gameStat); // 起家マークを表示する
private: /**** ヤキトリマーク ****/
	static const unsigned int PlateID_Yakitori = 7;
	static const unsigned int YakitoriPosH = TrayPosH - (PlateWidthH / 2) - (PlatePadding * 2);
	static const unsigned int YakitoriPosV = TrayPosV;
	void ShowYakitori(const GameTable* gameStat); // ヤキトリマークを表示する
public:
	void Render();
	explicit TrayReconst(GameTableScreen* parent);
	~TrayReconst();
};

}
