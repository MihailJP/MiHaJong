#pragma once

#include "../directx.h"
#include "../../common/strcode.h"
#include "../text.h"

namespace mihajong_graphic {

class CheckBox {
private:
	TexturePtr myTexture;
	DevicePtr myDevice;
	TextRenderer* myTextRenderer;
	CodeConv::tstring myCaption;
	bool checkFlag, focused;
	int myX, myY;
public:
	explicit CheckBox(DevicePtr device,
		const CodeConv::tstring& caption,
		int x, int y, bool checked = false);
	~CheckBox();
	const size_t captionWidth() {return myTextRenderer->strWidthByCols(myCaption);}
	const size_t captionWidthPx() {return myTextRenderer->strWidthByPix(myCaption);}
	bool isChecked() {return checkFlag;}
	bool isFocused() {return focused;}
	void check(bool flag) {checkFlag = flag;}
	void focus(bool flag) {focused = flag;}
	void Render();
};

}
