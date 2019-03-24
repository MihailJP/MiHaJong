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
#include "../../matrix.h"
#include "../../../common/datetime.h"

constexpr float pi = 3.1415927f;

namespace mihajong_graphic {

unsigned GameTableScreen::Clock::scaledClockDiameter;
int GameTableScreen::Clock::clockPosX;
int GameTableScreen::Clock::clockPosY;

GameTableScreen::Clock::Clock(GameTableScreen* caller) {
	constexpr unsigned int clockPadding = 10u;
	constexpr int clockCenterY = 300;
	parent = caller;
	parent->LoadTexture(&myTexture, MAKEINTRESOURCE(IDB_PNG_MOON_CLOCK));
	scaledClockDiameter = Geometry::SidebarWidth() < (clockDiameter + clockPadding) ? Geometry::SidebarWidth() - clockPadding : clockDiameter;
	clockPosX = (Geometry::SidebarWidth() - scaledClockDiameter) / 2 + Geometry::BaseSize;
	clockPosY = clockCenterY - scaledClockDiameter / 2;
}

GameTableScreen::Clock::~Clock() {
}

TransformMatrix GameTableScreen::Clock::setClockMatrix(float angle) {
	return getMatrix(
		static_cast<float>(clockPosX),
		static_cast<float>(clockPosY),
		static_cast<float>(scaledClockDiameter) / static_cast<float>(clockDiameter),
		static_cast<float>(scaledClockDiameter) / static_cast<float>(clockDiameter),
		angle,
		static_cast<float>(clockDiameter) / 2.0f,
		static_cast<float>(clockDiameter) / 2.0f
	);
}

void GameTableScreen::Clock::renderMoon() {
	constexpr RECT rect = {0, 0, 512, 512};
	const TransformMatrix matrix(setClockMatrix());

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderShadow() {
	struct Vertex {float x, y, z; uint32_t color;};

	const float Top = (static_cast<float>(clockPosY) + 2.0f) * Geometry::WindowScale(),
		Bottom = (static_cast<float>(clockPosY) + static_cast<float>(scaledClockDiameter) - 2.0f) * Geometry::WindowScale(),
		Left = (static_cast<float>(clockPosX) + 2.0f) * Geometry::WindowScale(),
		Right = (static_cast<float>(clockPosX) + static_cast<float>(scaledClockDiameter) - 2.0f) * Geometry::WindowScale(),
		CenterX = (Left + Right) / 2.0f,
		CenterY = (Top + Bottom) / 2.0f,
		Radius = CenterX - Left;

#ifdef _WIN32
	SYSTEMTIME st; GetSystemTime(&st);
#else /* _WIN32 */
	timespec st; clock_gettime(CLOCK_REALTIME, &st);
#endif /* _WIN32 */
	MOONPHASE mp = calc_moon_phase(systime_to_julian(&st));

	constexpr unsigned vertices = 60;
	Vertex circleVert[vertices];
	for (unsigned i = 0; i < vertices; ++i) {
		const float angleNum = static_cast<float>((i + 1) / 2) / static_cast<float>(vertices / 2);
		const float angle = angleNum * pi * (i % 2 == 0 ? 1.0f : -1.0f);
		circleVert[i].x = CenterX - Radius * sin(angle);
		circleVert[i].y = CenterY - Radius * cos(angle);
		const float x1 = (angleNum - 0.5f) * 2.0f * Radius;
		if ((mp.MoonPhase < 0.5) && (i % 2 == 1)) {
			const float x2 = 1.0f - static_cast<float>(mp.MoonIllum) * 2.0f;
			circleVert[i].x = sqrt(Radius * Radius - x1 * x1) * x2 + CenterX;
			circleVert[i].y = angleNum * Radius * 2.0f + Top;
		} else if ((mp.MoonPhase > 0.5) && (i % 2 == 0)) {
			const float x2 = static_cast<float>(mp.MoonIllum * 2.0 - 1.0);
			circleVert[i].x = sqrt(Radius * Radius - x1 * x1) * x2 + CenterX;
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
			static_cast<double>((circleVert[i].color & 0x00ff0000) >> 16) / 255.0,
			static_cast<double>((circleVert[i].color & 0x0000ff00) >>  8) / 255.0,
			static_cast<double>((circleVert[i].color & 0x000000ff)      ) / 255.0,
			static_cast<double>((circleVert[i].color & 0xff000000) >> 24) / 255.0);
		glVertex2f(circleVert[i].x, circleVert[i].y);
	}
	glEnd();
	glPopMatrix();
#endif /* _WIN32 */
}

void GameTableScreen::Clock::renderPanel() {
	constexpr RECT rect = {0, 512, 512, 1024};
	const TransformMatrix matrix(setClockMatrix());

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderHour() {
	const auto lt(DateTime::localTime());

	constexpr RECT rect = {1024, 512, 1536, 1024};
	const float angle = static_cast<float>((lt.hour % 12) * 60 + lt.minute) * pi / 360.0f;
	const TransformMatrix matrix(setClockMatrix(angle));

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderMinute() {
	const auto lt(DateTime::localTime());

	constexpr RECT rect = {1024, 0, 1536, 512};
	const float angle = static_cast<float>((lt.minute % 60) * 60 + lt.second) * pi / 1800.0f;
	const TransformMatrix matrix(setClockMatrix(angle));

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderPin() {
	constexpr RECT rect = {512, 512, 1024, 1024};
	const TransformMatrix matrix(setClockMatrix());

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::renderFrame() {
	constexpr RECT rect = {512, 0, 1024, 512};
	const TransformMatrix matrix(setClockMatrix());

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, clockDiameter, clockDiameter, 0xffffffff, &rect, 0, 0, &matrix);
}

void GameTableScreen::Clock::Render() {
	renderMoon(); renderShadow();
	renderPanel(); renderHour(); renderMinute(); renderPin();
	renderFrame();
}

}
