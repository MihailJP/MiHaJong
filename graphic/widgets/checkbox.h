#pragma once

#include "../../common/strcode.h"
#include "../text.h"
#include "../directx.h"

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
	CheckBox(const CheckBox&) = delete; // Delete unexpected copy constructor
	CheckBox& operator= (const CheckBox&) = delete; // Delete unexpected assign operator
	~CheckBox();
	const unsigned int captionWidth() {return myTextRenderer->strWidthByCols(myCaption);}
	const unsigned int captionWidthPx() {return myTextRenderer->strWidthByPix(myCaption);}
	bool isChecked() {return checkFlag;}
	bool isFocused() {return focused;}
	void check(bool flag) {checkFlag = flag;}
	void focus(bool flag) {focused = flag;}
	void Render();
};

}
