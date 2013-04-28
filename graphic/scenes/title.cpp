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
#include "../../common/bgmid.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TitleScreen::TitleScreen(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	TitleSprite::LoadTexture(caller->getDevice());
	for (int i = 0; i < nsTitleLogo; i++)
		sTitleLogo[i] = new TitleSprite(caller->getDevice(), 500 * i, 0, (i == 2) ? 700 : 500, 300);
	menuCursor = 1;
}

TitleScreen::~TitleScreen() {
	TitleSprite::DisposeTexture();
	for (int i = 0; i < nsTitleLogo; i++)
		if (sTitleLogo[i]) delete sTitleLogo[i];
}

void TitleScreen::zoomingLogo(TitleSprite* sprite, int X, int Y, unsigned startF, unsigned endF) {
	double t = ((double)myTimer.elapsed() / (double)timePerFrame - (double)startF) / 2.0;
	if ((t >= 0.0f) && (t < ((float)(endF - startF) * 1.1118f)))
		sprite->show(X, Y,
			powf((float)((double)(endF - startF) - t) / (float)(endF - startF) * 4.0f, 2.0f) + 0.8f,
			(int)(96.0f * (2.0f - sqrt(abs((float)((double)(endF - startF) - t) / (float)(endF - startF) * 4.0f)))));
	else if (t >= ((float)(endF - startF) * 1.1118f))
		sprite->show(X, Y, 1.0f, 128);
}

void TitleScreen::menuLabelSlide(unsigned ID, const CodeConv::tstring& menustr, int X, int Y, unsigned startF, unsigned endF) {
	const auto hsv2rgb = [](double Hue, double Saturation, double Value) -> uint32_t {
#ifndef _MSC_VER
		using std::max; using std::min;
#endif
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
	double t = ((double)myTimer.elapsed() / (double)timePerFrame - (double)startF);
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
	setRegion(ID, X / ((float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight), Y,
		(Geometry::BaseSize * 4 / 3) - X / ((float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight), Y + 71);
}

void TitleScreen::menuLabels() {
	auto center = [](unsigned cols) {
		return (signed(Geometry::BaseSize * 2 / 3) - signed(1.6 * 18 * cols)) * ((float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight);
	};
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
	const bool flag = ((myTimer.elapsed() > 180u * timePerFrame) && (od->dwData));
	switch (od->dwOfs) {
	case DIK_UP: case DIK_K: // カーソル上
		if (flag) {
			sound::Play(sound::IDs::sndCursor);
			if (--menuCursor == 0) menuCursor = 6;
		}
		break;
	case DIK_DOWN: case DIK_J: // カーソル下
		if (flag) {
			sound::Play(sound::IDs::sndCursor);
			if (++menuCursor > 6) menuCursor = 1;
		}
		break;
	case DIK_RETURN: case DIK_Z: case DIK_SPACE: // 決定
		if (flag) {
			sound::Play(sound::IDs::sndButton);
			ui::UIEvent->set(menuCursor); // イベントをセット、カーソル番号をメッセージとする
		} else if (od->dwData) {
			sound::Play(sound::IDs::sndClick);
			myTimer.skipTo(180 * timePerFrame);
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

void TitleScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const bool flag1 = (myTimer.elapsed() > 180u * timePerFrame);
	const int scaledX = (int)((float)X / Geometry::WindowScale() / ((float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight));
	const int scaledY = (int)((float)Y / Geometry::WindowScale());
	const int region = whichRegion(scaledX, scaledY);
#if 0
	{
		CodeConv::tostringstream o;
		o << _T("(") << scaledX << _T(", ") << scaledY << _T(") ");
		if (region != -1) o << _T("Region ") << region;
		else o << _T("No Region");
		myTextRenderer->NewText(9, o.str(), 0, 1000);
	}
#endif
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
		if (flag1) {
			switch (region) {
			case 0: case 1: case 2: case 3: case 4: case 5:
				if (region != (menuCursor - 1)) {
					sound::Play(sound::IDs::sndCursor);
					menuCursor = region + 1;
				}
				break;
			}
		}
		break;
	case DIMOFS_BUTTON0: // マウスの左ボタン
		if (od->dwData) {
			if ((flag1) && (region != -1))  {
				sound::Play(sound::IDs::sndButton);
				ui::UIEvent->set(menuCursor); // イベントをセット、カーソル番号をメッセージとする
			} else if (!flag1) {
				sound::Play(sound::IDs::sndClick);
				myTimer.skipTo(180 * timePerFrame);
			}
		}
		break;
	}
}

// -------------------------------------------------------------------------

#if defined(_WIN32) && defined(WITH_DIRECTX)
TexturePtr TitleScreen::TitleSprite::texture = nullptr;
#else
TexturePtr TitleScreen::TitleSprite::texture = 0;
#endif

void TitleScreen::TitleSprite::LoadTexture(DevicePtr device) {
	mihajong_graphic::LoadTexture(device, &texture, MAKEINTRESOURCE(IDB_PNG_TITLE));
}
void TitleScreen::TitleSprite::DisposeTexture() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (texture) texture->Release();
#endif
}

TitleScreen::TitleSprite::TitleSprite(DevicePtr device, int X, int Y, int Width, int Height) {
	rect.left = X; rect.top = Y; rect.right = X + Width; rect.bottom = Y + Height;
	width = Width; height = Height;
	myDevice = device;
}
TitleScreen::TitleSprite::~TitleSprite() {
}
void TitleScreen::TitleSprite::show(int X, int Y, float scale, uint8_t opacity) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix matrix, matrix1;
	D3DXMatrixIdentity(&matrix);
	D3DXMatrixTranslation(&matrix1, (float)(-X), (float)(-Y), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, scale, scale, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1,
		(float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight * Geometry::WindowScale(),
		Geometry::WindowScale(),
		0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
#else
	glPushMatrix(); glLoadIdentity();
	glTranslatef(0.0f, (float)Geometry::WindowHeight, 0.0f);
	glTranslatef((float)X * Geometry::WindowScale(), -(float)Y * Geometry::WindowScale(), 0.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-(float)X * Geometry::WindowScale(), (float)Y * Geometry::WindowScale(), 0.0f);
	glScalef(
		(float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight * Geometry::WindowScale(),
		Geometry::WindowScale(), 1.0f);
	glTranslatef(0.0f, -(float)Geometry::WindowHeight, 0.0f);
	TransformMatrix matrix; glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
	glPopMatrix();
#endif
	SpriteRenderer::instantiate(myDevice)->ShowSprite(
		texture, X, Y, width, height,
		(opacity << 24) | 0xffffff, &rect, width/2, height/3, &matrix);
}

// -------------------------------------------------------------------------

}
