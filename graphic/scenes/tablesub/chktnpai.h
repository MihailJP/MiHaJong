#pragma once

#include "call.h"
#include "../../text.h"
#include "../../showtile.h"

namespace mihajong_graphic {
	
class TableSubsceneCheckTenpai : public TableSubsceneCallFadeProto {
private:
	TextRenderer* txtRenderer;
	ShowTile* tileRenderer;
	bool tenpaiflag[PLAYERS];
	MachihaiInfo machiInfo[PLAYERS];
private:
	void RecalcTenpaiFlag();
	void CalculateTenpaiFlag(PLAYER_ID player, int x, int y);
	void ShowTenpaiFlag(PLAYER_ID player, int x, int y);
public:
	TableSubsceneCheckTenpai(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCheckTenpai();
	void Render();
};

}
