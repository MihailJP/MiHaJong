#pragma once

#include "table.h"

namespace mihajong_graphic {

class GameTableCall : public GameTableScreen {
protected:
	std::uint64_t currTime();
	std::uint64_t startTime;
protected:
	LPDIRECT3DTEXTURE9 tCall; LPD3DXSPRITE sCall;
	void ShowCall(int x, int y);
	void RenderCall();
	virtual void RenderTable(); // ‘ì‚ð•\Ž¦
public:
	GameTableCall(ScreenManipulator* const manipulator);
	virtual ~GameTableCall();
};

}
