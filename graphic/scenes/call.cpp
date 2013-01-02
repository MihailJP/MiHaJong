#include "call.h"
#include "../resource.h"
#include "../sprite.h"

namespace mihajong_graphic {

GameTableCall::GameTableCall(ScreenManipulator* const manipulator) : GameTableScreen(manipulator) {
	startTime = currTime();
	LoadTexture(&tCall, MAKEINTRESOURCE(IDB_PNG_CALL_TEXT), 384, 1632); InitSprite(&sCall);
}

GameTableCall::~GameTableCall() {
	if (tCall) tCall->Release();
	if (sCall) sCall->Release();
}

/* åªç›éûçè(WindowsÇ≈ÇÕ100nsíPà ) */
std::uint64_t GameTableCall::currTime() {
	FILETIME Zeit; GetSystemTimeAsFileTime(&Zeit);
	return ((std::uint64_t)Zeit.dwHighDateTime << 32) | Zeit.dwLowDateTime;
}

/* ï\é¶èàóù */
void GameTableCall::ShowCall(int x, int y) {
	const std::uint64_t curr = currTime();
	const int animationLength = 5000000;
	const float scale = (curr >= (startTime + animationLength)) ? 1.0f : std::pow((float)(animationLength - ((signed)curr - (signed)startTime)) / 2.5e6f + 1.0f, 2);
	const D3DCOLOR col = D3DCOLOR_ARGB(
		(curr >= (startTime + animationLength)) ? 255 :
		(int)std::pow((float)((curr - startTime) * 255) / animationLength / 16.0f, 2),
		0xff, 0xff, 0xff);
	D3DXMATRIX matrix, matrix1;
	D3DXMatrixIdentity(&matrix); D3DXMatrixIdentity(&matrix1);
	D3DXMatrixTranslation(&matrix1, (float)(-x), (float)(-y), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, scale, scale, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, (float)x, (float)y, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	RECT rect = {
		0  , 96 * (0    ),
		384, 96 * (0 + 1),
	};
	SpriteRenderer::ShowSprite(sCall, tCall, x, y, 384, 96, col, &rect, 192, 48, &matrix);
}

void GameTableCall::RenderCall() {
	ShowCall(TableSize / 2      ,                 192);
	ShowCall(                256, TableSize / 2      );
	ShowCall(TableSize     - 256, TableSize / 2      );
	ShowCall(TableSize / 2      , TableSize     - 192);
}

void GameTableCall::RenderTable() {
	GameTableScreen::RenderTable();
	RenderCall();
}

}
