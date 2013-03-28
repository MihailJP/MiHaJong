#pragma once

#include <Windows.h>
#include "directx.h"
#include <vector>
#include <tuple>
#include "../../common/strcode.h"
#include "../text.h"

namespace mihajong_graphic {

class ButtonPic {
public:
	enum ButtonStat {absent, clear, raised, sunken,};
	explicit ButtonPic(LPDIRECT3DDEVICE9 device);
	~ButtonPic();
	void setText(unsigned ButtonID);
	void setButton(unsigned ButtonID, ButtonStat stat, int X, int Y, unsigned Width, unsigned Height, D3DCOLOR color, const CodeConv::tstring& caption, bool adjustWidth = false);
	void setButton(unsigned ButtonID, ButtonStat stat);
	void Render();
private:
	LPDIRECT3DTEXTURE9 myTexture;
	LPDIRECT3DDEVICE9 myDevice;
	TextRenderer* myTextRenderer;
	std::vector<std::tuple<ButtonStat, int, int, unsigned, unsigned, D3DCOLOR, CodeConv::tstring, bool> > mySprites;
};

}
