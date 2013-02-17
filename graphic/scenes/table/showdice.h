#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::DiceReconst {
private:
	GameTableScreen* caller;
private:
	static const unsigned int DiceWidth = 24;
	static const unsigned int DiceHeight = 34;
	static const unsigned int DicePadding = 2;
	static const unsigned int DicePosH = TableSize - 230;
	static const unsigned int DicePosV = TableSize - 70;
	static const unsigned int DicePosInterstice = DiceWidth / 3;
	LPDIRECT3DTEXTURE9 tDice; // �T�C�R��
	void ShowDice(const GameTable* gameStat); // �T�C�R����\������
public:
	void Render();
	explicit DiceReconst(GameTableScreen* parent);
	~DiceReconst();
};

}
