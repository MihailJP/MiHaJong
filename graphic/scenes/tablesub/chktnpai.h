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
#ifndef _WIN32
	bool initialized;
#endif /*_WIN32*/
	void RecalcTenpaiFlag();
	void CalculateTenpaiFlag(PlayerID player, int x, int y);
	void ShowTenpaiFlag(PlayerID player, int x, int y);
public:
	TableSubsceneCheckTenpai(DevicePtr device);
	~TableSubsceneCheckTenpai();
	void Render();
};

}
