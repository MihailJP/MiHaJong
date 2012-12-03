#include "title.h"
#include <cassert>
#include <cstdint>
#include <cmath>
#include "../scrmanip.h"
#include "../gametbl.h"
#include "../loadtex.h"
#include "../resource.h"
#include "../sprite.h"
#include "../event.h"
#include "../../sound/sound.h"
#include "../../mjcore/bgmid.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TitleScreen::TitleScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	caller = manipulator;
	TitleSprite::LoadTexture(caller->getDevice());
	for (int i = 0; i < nsTitleLogo; i++)
		sTitleLogo[i] = new TitleSprite(caller->getDevice(), 500 * i, 0, (i == 2) ? 700 : 500, 300);
	myTextRenderer = new TextRenderer(caller->getDevice());
	GetSystemTimeAsFileTime(&startTime);
	menuCursor = 1;
}

TitleScreen::~TitleScreen() {
	TitleSprite::DisposeTexture();
	for (int i = 0; i < nsTitleLogo; i++)
		if (sTitleLogo[i]) delete sTitleLogo[i];
	delete myTextRenderer;
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
	double t = ((double)elapsed() / (double)timePerFrame - (double)startF) / 2.0;
	if ((t >= 0.0f) && (t < ((float)(endF - startF) * 1.1118f)))
		sprite->show(X, Y,
			powf((float)((double)(endF - startF) - t) / (float)(endF - startF) * 4.0f, 2.0f) + 0.8f,
			(int)(96.0f * (2.0f - sqrt(abs((float)((double)(endF - startF) - t) / (float)(endF - startF) * 4.0f)))));
	else if (t >= ((float)(endF - startF) * 1.1118f))
		sprite->show(X, Y, 1.0f, 128);
}

void TitleScreen::menuLabelSlide(unsigned ID, const CodeConv::tstring& menustr, int X, int Y, unsigned startF, unsigned endF) {
	const auto hsv2rgb = [](double Hue, double Saturation, double Value) -> uint32_t {
		const double circleAngle = 360.0;
		const double h = Hue - floor(Hue / circleAngle) * circleAngle;
		const double s = max(0.0, min(1.0, Saturation));
		const double v = max(0.0, min(1.0, Value));
		const double f = h / 60.0 - ((int)h / 60);
		const double p = v * (1.0 - s);
		const double q = v * (1.0 - f * s);
		const double t = v * (1.0 - (1.0 - f) * s);
		switch ((int)h / 60) {
		case 0:
			return ((int)(v * 255) << 16) | ((int)(t * 255) << 8) | (int)(p * 255);
		case 1:
			return ((int)(q * 255) << 16) | ((int)(v * 255) << 8) | (int)(p * 255);
		case 2:
			return ((int)(p * 255) << 16) | ((int)(v * 255) << 8) | (int)(t * 255);
		case 3:
			return ((int)(p * 255) << 16) | ((int)(q * 255) << 8) | (int)(v * 255);
		case 4:
			return ((int)(t * 255) << 16) | ((int)(p * 255) << 8) | (int)(v * 255);
		case 5:
			return ((int)(v * 255) << 16) | ((int)(p * 255) << 8) | (int)(q * 255);
		default:
			return 0xffffffff;
		}
	};
	double t = ((double)elapsed() / (double)timePerFrame - (double)startF);
	float virt_width = (float)Geometry::WindowWidth / Geometry::WindowScale();
	if ((t >= 0.0f) && (t < (double)(endF - startF))) {
		myTextRenderer->NewText(ID,
		menustr, X + virt_width * pow(1.0 - t / (double)(endF - startF), 2.0),
		Y, 2.0f,
		1.6f * Geometry::WindowWidth * 0.75f / Geometry::WindowHeight,
		0x33ffffff);
	} else if (t >= (double)(endF - startF)) {
		if (ID == (menuCursor - 1))
			myTextRenderer->NewText(ID, menustr, X, Y, 2.0f,
			1.6f * Geometry::WindowWidth * 0.75f / Geometry::WindowHeight,
			0xcc000000 | (0x00ffffff & hsv2rgb(t - (double)(endF - startF), 0.25, 1.0))
			);
		else
			myTextRenderer->NewText(ID, menustr, X, Y, 2.0f,
			1.6f * Geometry::WindowWidth * 0.75f / Geometry::WindowHeight,
			0x33ffffff);
	}
}

void TitleScreen::menuLabels() {
	auto center = [](unsigned cols) {return signed(Geometry::BaseSize * 2 / 3) - signed(1.6 * 18 * cols);};
	menuLabelSlide(0, _T("Standalone Game"), center(15), 400, 120, 180);
	menuLabelSlide(1, _T("Network Game (Server)"), center(21), 480, 125, 180);
	menuLabelSlide(2, _T("Network Game (Client)"), center(21), 560, 130, 180);
	menuLabelSlide(3, _T("Rule Configuration"), center(18), 640, 135, 180);
	menuLabelSlide(4, _T("System Configuration"), center(20), 720, 140, 180);
	menuLabelSlide(5, _T("Exit"), center(4), 800, 145, 180);
	myTextRenderer->Render();
}

void TitleScreen::Render() {
	clearWithGameTypeColor();
	menuLabels();
	zoomingLogo(sTitleLogo[0],  220, 168,   0, 30);
	zoomingLogo(sTitleLogo[1],  640, 168,  30, 60);
	zoomingLogo(sTitleLogo[2], 1120, 168,  60, 90);
}

void TitleScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	const bool flag = ((elapsed() > 180u * timePerFrame) && (od->dwData));
	switch (od->dwOfs) {
	case DIK_UP: // カーソル上
		if (flag) {
			sound::Play(sound::IDs::sndCursor);
			if (--menuCursor == 0) menuCursor = 6;
		}
		break;
	case DIK_DOWN: // カーソル下
		if (flag) {
			sound::Play(sound::IDs::sndCursor);
			if (++menuCursor > 6) menuCursor = 1;
		}
		break;
	case DIK_RETURN: case DIK_Z: case DIK_SPACE: // 決定
		if (flag) {
			if (menuCursor == 6) {
				sound::Play(sound::IDs::sndButton);
				ui::UIEvent->set(menuCursor); // イベントをセット、カーソル番号をメッセージとする
			} else {
				sound::Play(sound::IDs::sndCuohu); // 未実装
			}
		}
		break;
	case DIK_ESCAPE: case DIK_X: // キャンセル
		if (flag) {
			sound::Play(sound::IDs::sndClick);
			if (menuCursor != 6) {
				menuCursor = 6; // Exitにカーソルを合わせる
			} else {
				ui::UIEvent->set(menuCursor); // イベントをセット、カーソル番号をメッセージとする
			}
		}
		break;
	}
}

// -------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 TitleScreen::TitleSprite::texture = nullptr;

void TitleScreen::TitleSprite::LoadTexture(LPDIRECT3DDEVICE9 device) {
	mihajong_graphic::LoadTexture(device, &texture, MAKEINTRESOURCE(IDB_PNG_TITLE), 1700, 300);
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

// -------------------------------------------------------------------------

}
