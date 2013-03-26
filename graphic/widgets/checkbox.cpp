#include "checkbox.h"
#include "../loadtex.h"
#include "../resource.h"

namespace mihajong_graphic {

CheckBox::CheckBox(LPDIRECT3DDEVICE9 device, const CodeConv::tstring& caption) {
	myDevice = device;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_CHECKBOX), 72, 72);
	myTextRenderer = new TextRenderer(device);
	myCaption = caption;
}

CheckBox::~CheckBox() {
	delete myTextRenderer;
	if (myTexture) myTexture->Release();
}

}
