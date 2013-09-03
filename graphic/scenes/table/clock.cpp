#include "clock.h"
#include "../../sprite.h"
#include "../../resource.h"
#include <d3d9types.h>
#include <cassert>
#include <cmath>

namespace mihajong_graphic {

TableProtoScene::Clock::Clock(TableProtoScene* caller) {
	parent = caller;
	mihajong_graphic::LoadTexture(parent->caller->getDevice(), &myTexture, MAKEINTRESOURCE(IDB_PNG_MOON_CLOCK));
}

TableProtoScene::Clock::~Clock() {
}

void TableProtoScene::Clock::setClockMatrix(TransformMatrix* matrix) {
	D3DXMatrixIdentity(matrix); TransformMatrix tmpMatrix; D3DXMatrixIdentity(&tmpMatrix);
	D3DXMatrixScaling(&tmpMatrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixTranslation(&tmpMatrix, -clockPosX * Geometry::WindowScale(), -clockPosY * Geometry::WindowScale(), 0);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixScaling(&tmpMatrix, 0.625f, 0.625f, 0.0f);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
	D3DXMatrixTranslation(&tmpMatrix, clockPosX * Geometry::WindowScale(), clockPosY * Geometry::WindowScale(), 0);
	D3DXMatrixMultiply(matrix, matrix, &tmpMatrix);
}

void TableProtoScene::Clock::renderMoon() {
	RECT rect = {0, 0, 512, 512};

	TransformMatrix matrix;
	setClockMatrix(&matrix);

	SpriteRenderer::instantiate(parent->caller->getDevice())->ShowSprite(
		myTexture, clockPosX, clockPosY, 512, 512, 0xffffffff, &rect, 0, 0, &matrix);
}

void TableProtoScene::Clock::renderShadowL() {
	struct Vertex {float x, y, z; uint32_t color;};

	const float Top = ((float)clockPosY + 2.0f) * Geometry::WindowScale(),
		Bottom = ((float)clockPosY + 512.0f * 0.625f - 2.0f) * Geometry::WindowScale(),
		Left = ((float)clockPosX + 2.0f) * Geometry::WindowScale(),
		Right = ((float)clockPosX + 512.0f * 0.625f - 2.0f) * Geometry::WindowScale(),
		CenterX = (Left + Right) / 2.0f,
		CenterY = (Top + Bottom) / 2.0f,
		Radius = CenterX - Left;

	const float pi = atan2(1.0f, 1.0f) * 4.0f;

	const unsigned vertices = 60;
	const int moonPhase = 30;
	Vertex circleVert[vertices];
	for (unsigned i = 0; i < vertices; ++i) {
		const float angle = (float)((i + 1) / 2) / (float)(vertices / 2) * pi * (i % 2 == 0 ? 1.0f : -1.0f);
		const float x1 = CenterX - Radius * sin(angle), x2 = CenterX - Radius * sin(-angle);
		circleVert[i].x = CenterX - Radius * sin(angle);
		circleVert[i].y = CenterY - Radius * cos(angle);
		if ((moonPhase >= 0 && moonPhase < 180) && (i % 2 == 1))
			circleVert[i].x
			= x1 * ((cos(((float)       moonPhase  / 180.0f) * pi) + 1.0f) / 2.0f)
			+ x2 * ((cos(((float)(180 - moonPhase) / 180.0f) * pi) + 1.0f) / 2.0f);
		else if ((moonPhase >= 180 && moonPhase < 360) && (i % 2 == 0))
			circleVert[i].x
			= x1 * ((cos(((float)(360 - moonPhase) / 180.0f) * pi) + 1.0f) / 2.0f)
			+ x2 * ((cos(((float)(moonPhase - 180) / 180.0f) * pi) + 1.0f) / 2.0f);
		circleVert[i].z = 0; circleVert[i].color = 0x7f000000;
	}

	parent->caller->getDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	parent->caller->getDevice()->SetTexture(0, nullptr);
	parent->caller->getDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertices - 2, circleVert, sizeof (Vertex));
}

void TableProtoScene::Clock::Render() {
	renderMoon(); renderShadowL();
}

}
