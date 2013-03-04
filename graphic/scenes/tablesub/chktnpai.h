#pragma once

#include "call.h"
#include "../../text.h"
#include "../../showtile.h"

namespace mihajong_graphic {
	
class TableSubsceneCheckTenpai : public TableSubsceneCallFadeProto {
private:
	TextRenderer* txtRenderer;
	ShowTile* tileRenderer;
	bool tenpaiflag[Players];
	MachihaiInfo machiInfo[Players];
private:
	void RecalcTenpaiFlag();
	void CalculateTenpaiFlag(PlayerID player, int x, int y);
	void ShowTenpaiFlag(PlayerID player, int x, int y);
public:
	TableSubsceneCheckTenpai(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCheckTenpai();
	void Render();
};

}