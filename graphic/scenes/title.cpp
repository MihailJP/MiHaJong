#include "title.h"
#include <cassert>
#include <cstdint>
#include <cmath>
#include "../scrmanip.h"
#include "../gametbl.h"
#include "../loadtex.h"
#include "../resource.h"
#include "../sprite.h"

TitleScreen::TitleScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	caller = manipulator;
	TitleSprite::LoadTexture(caller->getDevice());
	for (int i = 0; i < nsTitleLogo; i++)
		sTitleLogo[i] = new TitleSprite(caller->getDevice(), 500 * i, 0, (i == 2) ? 700 : 500, 300);
	GetSystemTimeAsFileTime(&startTime);
}

TitleScreen::~TitleScreen() {
	TitleSprite::DisposeTexture();
	for (int i = 0; i < nsTitleLogo; i++)
		if (sTitleLogo[i]) delete sTitleLogo[i];
}

void TitleScreen::clearWithGameTypeColor() {
	// バッファクリア
	switch (GameStatus::gameStat()->gameType) {
	case Yonma:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 64, 0), 1.0f, 0);
		break;
	case Sanma:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 0, 64), 1.0f, 0);
		break;
	case Sanma4:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(0, 64, 64), 1.0f, 0);
		break;
	case SanmaS:
		caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(64, 0, 64), 1.0f, 0);
		break;
	default:
		assert(false); // This may not occur.
	}
}

uint64_t TitleScreen::elapsed() {
	FILETIME currTime; GetSystemTimeAsFileTime(&currTime);
	uint64_t st = ((uint64_t)startTime.dwHighDateTime << 32) | startTime.dwLowDateTime;
	uint64_t ct = ((uint64_t)currTime.dwHighDateTime << 32) | currTime.dwLowDateTime;
	assert(ct >= st);
	//if ((ct - st) >= 30000000) startTime = currTime; // debug loop
	return ct - st;
}

void TitleScreen::zoomingLogo(TitleSprite* sprite, int X, int Y, unsigned startF, unsigned endF) {
	double t = (double)elapsed() / (double)timePerFrame - (double)startF / 2.0f;
	if ((t >= 0.0f) && (t < ((float)(endF - startF) * 1.1118f)))
		sprite->show(X, Y,
			powf((float)((double)(endF - startF) - t) / (float)(endF - startF) * 4.0f, 2.0f) + 0.8f,
			(int)(96.0f * (2.0f - sqrt(abs((float)((double)(endF - startF) - t) / (float)(endF - startF) * 4.0f)))));
	else if (t >= ((float)(endF - startF) * 1.1118f))
		sprite->show(X, Y, 1.0f, 128);
}

void TitleScreen::Render() {
	clearWithGameTypeColor();
	zoomingLogo(sTitleLogo[0],  220, 128,   0,  60);
	zoomingLogo(sTitleLogo[1],  640, 128,  60, 120);
	zoomingLogo(sTitleLogo[2], 1120, 128, 120, 180);
}

// -------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 TitleScreen::TitleSprite::texture = nullptr;

void TitleScreen::TitleSprite::LoadTexture(LPDIRECT3DDEVICE9 device) {
	::LoadTexture(device, &texture, MAKEINTRESOURCE(IDB_PNG_TITLE), 1700, 300);
}
void TitleScreen::TitleSprite::DisposeTexture() {
	if (texture) texture->Release();
}

TitleScreen::TitleSprite::TitleSprite(LPDIRECT3DDEVICE9 device, int X, int Y, int Width, int Height) {
	rect.left = X; rect.top = Y; rect.right = X + Width; rect.bottom = Y + Height;
	width = Width; height = Height;
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		throw _T("スプライトの生成に失敗しました");
}
TitleScreen::TitleSprite::~TitleSprite() {
	if (sprite) sprite->Release();
}
void TitleScreen::TitleSprite::show(int X, int Y, float scale, uint8_t opacity) {
	D3DXMATRIX matrix, matrix1;
	D3DXMatrixIdentity(&matrix);
	D3DXMatrixTranslation(&matrix1, (float)(-X), (float)(-Y), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, scale, scale, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1,
		(float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight * Geometry::WindowScale(),
		Geometry::WindowScale(),
		0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	SpriteRenderer::ShowSprite(
		sprite, texture, X, Y, width, height,
		(opacity << 24) | 0xffffff, &rect, width/2, height/3, &matrix);
}
