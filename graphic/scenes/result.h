#pragma once

#include <array>
#include "sysscr.h"
#include "../../common/gametbl.h"
#include "../timer.h"
#include "../text.h"

namespace mihajong_graphic {

class ResultScreen : public SystemScreen {
private:
	class RankRenderer;
	std::array<RankRenderer*, mihajong_structs::Players> rankRenderer;
	Timer myTimer;
public:
	ResultScreen(ScreenManipulator* const manipulator);
	~ResultScreen();
	void Render();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
};

class ResultScreen::RankRenderer {
private:
	static const unsigned animTime = 500000u;
private:
	LPDIRECT3DDEVICE9 myDevice;
	Timer myTimer;
	int myID;
	int BaseY;
	mihajong_structs::PlayerID player;
private:
	HugeTextRenderer* rankRenderer;
	void RenderRank();
private:
	SmallTextRenderer* nameRenderer;
	unsigned stringWidth(const CodeConv::tstring& str);
	void RenderNameScore();
private:
	ScoreDigitRenderer* scoreRenderer;
	void RenderScore();
public:
	RankRenderer(LPDIRECT3DDEVICE9 device, int id);
	~RankRenderer();
	void Render();
};

}
