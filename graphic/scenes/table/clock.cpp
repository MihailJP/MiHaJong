#include "clock.h"
#include "../../sprite.h"
#include "../../resource.h"
#include <d3d9types.h>
#include <cassert>
#include <cmath>
#undef EXPORT
#include "../../../astro/astro.h"
#include <ctime>

namespace mihajong_graphic {

GameTableScreen::Clock::Clock(GameTableScreen* caller) {
	parent = caller;
	mihajong_graphic::LoadTexture(parent->caller->getDevice(), &myTexture, MAKEINTRESOURCE(IDB_PNG_MOON_CLOCK));
}

GameTableScreen::Clock::~Clock() {
}

void GameTableScreen::Clock::setClockMatrix(TransformMatrix* matrix, float angle) {
	D3DXMatrixIdentity(matrix); TransformMatrix tmpMatrix; D3DXMatrixIdentity(&tmpMatrix);
	D3DXMatrixScaling(&tmpMatrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixTranslation(&tmpMatrix, -clockPosX * Geometry::WindowScale(), -clockPosY * Geometry::WindowScale(), 0);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixTranslation(&tmpMatrix, -((float)clockDiameter / 2.0f) * Geometry::WindowScale(), -((float)clockDiameter / 2.0f) * Geometry::WindowScale(), 0);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixRotationZ(&tmpMatrix, angle);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixTranslation(&tmpMatrix, ((float)clockDiameter / 2.0f) * Geometry::WindowScale(), ((float)clockDiameter / 2.0f) * Geometry::WindowScale(), 0);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixScaling(&tmpMatrix, (float)scaledClockDiameter / (float)clockDiameter, (float)scaledClockDiameter / (float)clockDiameter, 0.0f);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixTranslation(&tmpMatrix, clockPosX * Geometry::WindowScale(), clockPosY * Geometry::WindowScale(), 0);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
}

void GameTableScreen::Clock::renderMoon() {
	RECT rect = {0, 0, 512, 512};

	TransformMatrix matrix;
	setClockMatrix(&matrix);

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderShadow() {
	struct Vertex {float x, y, z; uint32_t color;};

	const float Top = ((float)clockPosY + 2.0f) * Geometry::WindowScale(),
		Bottom = ((float)clockPosY + (float)clockDiameter * ((float)scaledClockDiameter / (float)clockDiameter) - 2.0f) * Geometry::WindowScale(),
		Left = ((float)clockPosX + 2.0f) * Geometry::WindowScale(),
		Right = ((float)clockPosX + (float)clockDiameter * ((float)scaledClockDiameter / (float)clockDiameter) - 2.0f) * Geometry::WindowScale(),
		CenterX = (Left + Right) / 2.0f,
		CenterY = (Top + Bottom) / 2.0f,
		Radius = CenterX - Left;

	const float pi = atan2(1.0f, 1.0f) * 4.0f;

	SYSTEMTIME st; GetSystemTime(&st);
	MOONPHASE mp = calc_moon_phase(systime_to_julian(&st));

	const unsigned vertices = 60;
	Vertex circleVert[vertices];
	for (unsigned i = 0; i < vertices; ++i) {
		const float angle = (float)((i + 1) / 2) / (float)(vertices / 2) * pi * (i % 2 == 0 ? 1.0f : -1.0f);
		const float x1 = CenterX - Radius * sin(angle), x2 = CenterX - Radius * sin(-angle);
		circleVert[i].x = CenterX - Radius * sin(angle);
		circleVert[i].y = CenterY - Radius * cos(angle);
		if ((mp.MoonIllum >= 0) && (i % 2 == 1))
			circleVert[i].x
			= x1 * (0.5f - mp.MoonIllum) * 2.0f
			+ x2 *         mp.MoonIllum  * 2.0f;
		else if ((mp.MoonIllum < 0) && (i % 2 == 0))
			circleVert[i].x
			= x1 * (0.5f - (-mp.MoonIllum)) * 2.0f
			+ x2 *         (-mp.MoonIllum)  * 2.0f;
		circleVert[i].z = 0; circleVert[i].color = 0x7f000000;
	}

	parent->caller->getDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	parent->caller->getDevice()->SetTexture(0, nullptr);
	parent->caller->getDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertices - 2, circleVert, sizeof (Vertex));
}

void GameTableScreen::Clock::renderPanel() {
	RECT rect = {0, 512, 512, 1024};

	TransformMatrix matrix;
	setClockMatrix(&matrix);

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderHour() {
	time_t Zeit = time(nullptr);
	tm* lt = localtime(&Zeit);

	RECT rect = {1024, 512, 1536, 1024};

	const float pi = atan2(1.0f, 1.0f) * 4.0f;
	const float angle = (float)((lt->tm_hour % 12) * 60 + lt->tm_min) * pi / 360.0f;

	TransformMatrix matrix;
	setClockMatrix(&matrix, angle);

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderMinute() {
	time_t Zeit = time(nullptr);
	tm* lt = localtime(&Zeit);

	RECT rect = {1024, 0, 1536, 512};

	const float pi = atan2(1.0f, 1.0f) * 4.0f;
	const float angle = (float)((lt->tm_min % 60) * 60 + lt->tm_sec) * pi / 1800.0f;

	TransformMatrix matrix;
	setClockMatrix(&matrix, angle);

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderPin() {
	RECT rect = {512, 512, 1024, 1024};

	TransformMatrix matrix;
	setClockMatrix(&matrix);

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderFrame() {
	RECT rect = {512, 0, 1024, 512};

	TransformMatrix matrix;
	setClockMatrix(&matrix);

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::Render() {
	renderMoon(); renderShadow();
	renderPanel(); renderHour(); renderMinute(); renderPin();
	renderFrame();
}

}
