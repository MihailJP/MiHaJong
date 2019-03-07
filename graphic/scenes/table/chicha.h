#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::TrayReconst {
private:
	GameTableScreen* caller;
private: /**** 起家マーク・ヤキトリマークを置く凹み ****/
	static constexpr unsigned int TrayHLeft = 275;
	static constexpr unsigned int TrayHTop = 75;
	static constexpr unsigned int TrayHRight = 373;
	static constexpr unsigned int TrayHBottom = 115;
	static constexpr unsigned int TrayHWidth = TrayHRight - TrayHLeft;
	static constexpr unsigned int TrayHHeight = TrayHBottom - TrayHTop;
	static constexpr unsigned int TrayVLeft = 377;
	static constexpr unsigned int TrayVTop = 1;
	static constexpr unsigned int TrayVRight = 417;
	static constexpr unsigned int TrayVBottom = 99;
	static constexpr unsigned int TrayVWidth = TrayVRight - TrayVLeft;
	static constexpr unsigned int TrayVHeight = TrayVBottom - TrayVTop;
	static constexpr unsigned int TrayPosH = TableSize - 200;
	static constexpr unsigned int TrayPosV = TableSize - 24;
	void ShowTray(); // 起家マークを置く場所表示
private: /**** 起家マーク ****/
	static constexpr unsigned int PlateWidthH = 45;
	static constexpr unsigned int PlateHeightH = 35;
	static constexpr unsigned int PlateWidthV = 32;
	static constexpr unsigned int PlateHeightV = 48;
	static constexpr unsigned int PlatePadding = 1;
	static constexpr unsigned int PlatePosH = TrayPosH + (PlateWidthH / 2) + (PlatePadding * 2);
	static constexpr unsigned int PlatePosV = TrayPosV;
	TexturePtr tChiicha; // 起家マーク
	void ShowChiicha(const GameTable* gameStat); // 起家マークを表示する
private: /**** ヤキトリマーク ****/
	static constexpr unsigned int PlateID_Yakitori = 7;
	static constexpr unsigned int YakitoriPosH = TrayPosH - (PlateWidthH / 2) - (PlatePadding * 2);
	static constexpr unsigned int YakitoriPosV = TrayPosV;
	void ShowYakitori(const GameTable* gameStat); // ヤキトリマークを表示する
public:
	void Render();
	explicit TrayReconst(GameTableScreen* parent);
	TrayReconst(const TrayReconst&) = delete; // Delete unexpected copy constructor
	TrayReconst& operator= (const TrayReconst&) = delete; // Delete unexpected assign operator
	~TrayReconst();
};

}
