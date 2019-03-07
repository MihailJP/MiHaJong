#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::DiceReconst {
private:
	GameTableScreen* caller;
private:
	static constexpr unsigned int DiceWidth = 24;
	static constexpr unsigned int DiceHeight = 34;
	static constexpr unsigned int DicePadding = 2;
	static constexpr unsigned int DicePosH = TableSize - 230;
	static constexpr unsigned int DicePosV = TableSize - 70;
	static constexpr unsigned int DicePosInterstice = DiceWidth / 3;
	TexturePtr tDice; // サイコロ
	void ShowDice(const GameTable* gameStat); // サイコロを表示する
public:
	void Render();
	explicit DiceReconst(GameTableScreen* parent);
	DiceReconst(const DiceReconst&) = delete; // Delete unexpected copy constructor
	DiceReconst& operator= (const DiceReconst&) = delete; // Delete unexpected assign operator
	~DiceReconst();
};

}
