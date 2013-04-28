#pragma once

#include <windows.h>
#include "../directx.h"
#include <vector>
#include <tuple>
#include "../../common/strcode.h"
#include "../text.h"

namespace mihajong_graphic {

class ButtonPic {
public:
	enum ButtonStat {absent, clear, raised, sunken,};
	explicit ButtonPic(DevicePtr device);
	~ButtonPic();
	void setText(unsigned ButtonID);
	void setButton(unsigned ButtonID, ButtonStat stat, int X, int Y, unsigned Width, unsigned Height, ArgbColor color, const CodeConv::tstring& caption, bool adjustWidth = false);
	void setButton(unsigned ButtonID, ButtonStat stat);
	void Render();
private:
	TexturePtr myTexture;
	DevicePtr myDevice;
	TextRenderer* myTextRenderer;
	std::vector<std::tuple<ButtonStat, int, int, unsigned, unsigned, ArgbColor, CodeConv::tstring, bool> > mySprites;
};

}
