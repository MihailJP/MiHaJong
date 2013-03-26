#include "checkbox.h"
#include "../loadtex.h"
#include "../resource.h"

namespace mihajong_graphic {

CheckBox::CheckBox(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_CHECKBOX), 72, 72);
}

CheckBox::~CheckBox() {
	if (myTexture) myTexture->Release();
}

}
