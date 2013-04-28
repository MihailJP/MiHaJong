#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::TrayReconst {
private:
	GameTableScreen* caller;
private: /**** �N�ƃ}�[�N�E���L�g���}�[�N��u������ ****/
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
	void ShowTray(); // �N�ƃ}�[�N��u���ꏊ�\��
private: /**** �N�ƃ}�[�N ****/
	static const unsigned int PlateWidthH = 45;
	static const unsigned int PlateHeightH = 35;
	static const unsigned int PlateWidthV = 32;
	static const unsigned int PlateHeightV = 48;
	static const unsigned int PlatePadding = 1;
	static const unsigned int PlatePosH = TrayPosH + (PlateWidthH / 2) + (PlatePadding * 2);
	static const unsigned int PlatePosV = TrayPosV;
	TexturePtr tChiicha; // �N�ƃ}�[�N
	void ShowChiicha(const GameTable* gameStat); // �N�ƃ}�[�N��\������
private: /**** ���L�g���}�[�N ****/
	static const unsigned int PlateID_Yakitori = 7;
	static const unsigned int YakitoriPosH = TrayPosH - (PlateWidthH / 2) - (PlatePadding * 2);
	static const unsigned int YakitoriPosV = TrayPosV;
	void ShowYakitori(const GameTable* gameStat); // ���L�g���}�[�N��\������
public:
	void Render();
	explicit TrayReconst(GameTableScreen* parent);
	~TrayReconst();
};

}
