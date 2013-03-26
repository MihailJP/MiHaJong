#pragma once

#include <d3dx9.h>
#include <dxerr.h>
#include "../../common/strcode.h"
#include "../text.h"

namespace mihajong_graphic {

class CheckBox {
private:
	LPDIRECT3DTEXTURE9 myTexture;
	LPDIRECT3DDEVICE9 myDevice;
	TextRenderer* myTextRenderer;
	CodeConv::tstring myCaption;
public:
	explicit CheckBox(LPDIRECT3DDEVICE9 device, const CodeConv::tstring& caption = _T("foobar"));
	~CheckBox();
	const size_t captionWidth() {return myTextRenderer->strWidthByCols(myCaption);}
	const size_t captionWidthPx() {return myTextRenderer->strWidthByPix(myCaption);}
};

}
