#include "agariscr.h"
#include "../../loadtex.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../geometry.h"
#include <cmath>

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::TableSubsceneAgariScreenProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myDevice = device;
	LoadTexture(device, &windowTexture, MAKEINTRESOURCE(IDB_PNG_AGARI_WINDOW), 600, 864);
}
TableSubsceneAgariScreenProto::~TableSubsceneAgariScreenProto() {
	windowTexture->Release();
}

void TableSubsceneAgariScreenProto::renderWindow() {
	const double Zeit = (double)(currTime() - startTime) / 10000000.0;
	const int yOffset = (Zeit >= 1.0) ? 0 : (int)(pow(1.0 - Zeit, 2) * (double)Geometry::BaseSize);
	SpriteRenderer::instantiate(myDevice)->ShowSprite(
		windowTexture,
		((signed)Geometry::BaseSize - 600) / 2,
		((signed)Geometry::BaseSize - 864) / 2 - yOffset,
		600, 864);
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreen::TableSubsceneAgariScreen(LPDIRECT3DDEVICE9 device) : TableSubsceneAgariScreenProto(device) {
}
TableSubsceneAgariScreen::~TableSubsceneAgariScreen() {
}
void TableSubsceneAgariScreen::Render() {
	renderWindow();
}

// -------------------------------------------------------------------------

}
