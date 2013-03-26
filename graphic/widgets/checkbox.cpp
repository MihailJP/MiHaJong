#include "checkbox.h"
#include "../loadtex.h"
#include "../resource.h"
#include "../sprite.h"

namespace mihajong_graphic {

CheckBox::CheckBox(LPDIRECT3DDEVICE9 device, const CodeConv::tstring& caption, int x, int y, bool checked) {
	myDevice = device;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_CHECKBOX), 72, 72);
	myTextRenderer = new TextRenderer(device);
	myCaption = caption;
	checkFlag = checked;
	myX = x; myY = y;
}

CheckBox::~CheckBox() {
	delete myTextRenderer;
	if (myTexture) myTexture->Release();
}

void CheckBox::Render() {
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, myX, myY, 36, 36);
	myTextRenderer->NewText(0, myCaption, myX + 36, myY);
	myTextRenderer->Render();
}

}
