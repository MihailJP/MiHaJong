#include "clock.h"
#include "../../sprite.h"
#include "../../resource.h"
#if defined(_WIN32) && defined(WITH_DIRECTX)
#include <d3d9types.h>
#endif
#include <cassert>
#include <cmath>
#undef EXPORT
#include "../../../astro/astro.h"
#include <ctime>

namespace mihajong_graphic {

GameTableScreen::Clock::Clock(GameTableScreen* caller) {
	parent = caller;
	parent->LoadTexture(&myTexture, MAKEINTRESOURCE(IDB_PNG_MOON_CLOCK));
}

GameTableScreen::Clock::~Clock() {
}

void GameTableScreen::Clock::setClockMatrix(TransformMatrix* matrix, float angle) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
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
#else
	const float
		phi = (float)scaledClockDiameter,
		h = Geometry::WindowHeight,
		s = Geometry::WindowScale(),
		r = ((float)clockDiameter / 2.0f),
		x = clockPosX, y = clockPosY,
		theta = angle;

	(*matrix)[ 0] =  (phi * s * cos(theta)) / (2.0f * r);
	(*matrix)[ 1] = -(phi * s * sin(theta)) / (2.0f * r);
	(*matrix)[ 2] = 0.0f;
	(*matrix)[ 3] = 0.0f;

	(*matrix)[ 4] = (phi * s * sin(theta)) / (2.0f * r);
	(*matrix)[ 5] = (phi * s * cos(theta)) / (2.0f * r);
	(*matrix)[ 6] = 0.0f;
	(*matrix)[ 7] = 0.0f;

	(*matrix)[ 8] = 0.0f;
	(*matrix)[ 9] = 0.0f;
	(*matrix)[10] = 0.0f;
	(*matrix)[11] = 0.0f;

	(*matrix)[12] =
		  (phi * y * s * sin(theta)) / (2.0f * r)
		- (phi * x * s * cos(theta)) / (2.0f * r)
		+ x * s
		- (phi * h * s * sin(theta)) / (2.0f * r)
		+ (phi * r * s * sin(theta)) / (2.0f * r)
		- (phi * r * s * cos(theta)) / (2.0f * r)
		+ (phi * r * s             ) / (2.0f * r);
	(*matrix)[13] =
		  (phi * y * s * cos(theta)) / (2.0f * r)
		- y * s
		+ (phi * x * s * sin(theta)) / (2.0f * r)
		+ (phi * r * s * sin(theta)) / (2.0f * r)
		- (phi * h * s * cos(theta)) / (2.0f * r)
		+ (phi * r * s * cos(theta)) / (2.0f * r)
		- (phi * r * s             ) / (2.0f * r)
		+ h;
	(*matrix)[14] = 0.0f;
	(*matrix)[15] = 1.0f;
#endif
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

#ifdef _WIN32
	SYSTEMTIME st; GetSystemTime(&st);
#else /* _WIN32 */
	timespec st; clock_gettime(CLOCK_REALTIME, &st);
#endif /* _WIN32 */
	MOONPHASE mp = calc_moon_phase(systime_to_julian(&st));

	const unsigned vertices = 60;
	Vertex circleVert[vertices];
	for (unsigned i = 0; i < vertices; ++i) {
		const float angleNum = (float)((i + 1) / 2) / (float)(vertices / 2);
		const float angle = angleNum * pi * (i % 2 == 0 ? 1.0f : -1.0f);
		circleVert[i].x = CenterX - Radius * sin(angle);
		circleVert[i].y = CenterY - Radius * cos(angle);
		const float x1 = (angleNum - 0.5f) * 2.0f * Radius;
		if ((mp.MoonIllum >= 0) && (i % 2 == 1)) {
			const float x2 = 1.0f - mp.MoonIllum * 4.0f;
			circleVert[i].x
				= sqrt(Radius * Radius - x1 * x1)
				* sin(x2 * pi / 2.0f) + CenterX;
			circleVert[i].y = angleNum * Radius * 2.0f + Top;
		} else if ((mp.MoonIllum < 0) && (i % 2 == 0)) {
			const float x2 = mp.MoonIllum * 4.0f - 1.0f;
			circleVert[i].x
				= sqrt(Radius * Radius - x1 * x1)
				* sin(x2 * pi / 2.0f) + CenterX;
			circleVert[i].y = angleNum * Radius * 2.0f + Top;
		}
		circleVert[i].z = 0; circleVert[i].color = 0x7f000000;
	}

#if defined(_WIN32) && defined(WITH_DIRECTX)
	parent->caller->getDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	parent->caller->getDevice()->SetTexture(0, nullptr);
	parent->caller->getDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertices - 2, circleVert, sizeof (Vertex));
#else /* _WIN32 */
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLE_STRIP);
	for (unsigned i = 0; i < vertices; ++i) {
		glColor4d(
			(double)((circleVert[i].color & 0x00ff0000) >> 16) / 255.0,
			(double)((circleVert[i].color & 0x0000ff00) >>  8) / 255.0,
			(double)((circleVert[i].color & 0x000000ff)      ) / 255.0,
			(double)((circleVert[i].color & 0xff000000) >> 24) / 255.0);
		glVertex2f(circleVert[i].x, circleVert[i].y);
	}
	glEnd();
	glPopMatrix();
#endif /* _WIN32 */
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
