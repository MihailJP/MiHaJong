#include "checkbox.h"
#include "../loadtex.h"
#include "../resource.h"
#include "../sprite.h"

namespace mihajong_graphic {

CheckBox::CheckBox(DevicePtr device, const CodeConv::tstring& caption, int x, int y, bool checked) {
	myDevice = device;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_CHECKBOX));
	myTextRenderer = new TextRenderer(device);
	myCaption = caption;
	checkFlag = checked;
	focused = false;
	myX = x; myY = y;
}

CheckBox::~CheckBox() {
	delete myTextRenderer;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (myTexture) myTexture->Release();
#endif
}

void CheckBox::Render() {
	RECT rect = {
		0  + 36 * (checkFlag ? 1 : 0), 0  + 36 * (focused ? 1 : 0),
		36 + 36 * (checkFlag ? 1 : 0), 36 + 36 * (focused ? 1 : 0),
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, myX, myY, 36, 36, 0xffffffff, &rect);
	myTextRenderer->NewText(0, myCaption, myX + 36, myY, 1.0f, 1.0f,
		focused ? 0xffffffff : 0xccffffff);
	myTextRenderer->Render();
}

}
