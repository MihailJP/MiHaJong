#include "logwnd.h"
#include "resource.h"
#include "loadtex.h"
#include "sprite.h"
#include <cassert>

namespace mihajong_graphic {
namespace logwnd {

// -------------------------------------------------------------------------

namespace {
	CodeConv::tostringstream logdata;
}

EXPORT void append(LPCTSTR logstr) {
	logdata << logstr; logdata.flush();
}

CodeConv::tstring getlog() {
	return logdata.str();
}

// -------------------------------------------------------------------------

LogWindow::LogWindow(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned Width, unsigned Height) {
	assert(Width >= 8); assert(Height >= 1);
	myHWnd = hwnd; myDevice = device; x = X; y = Y; width = Width; height = Height;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_TEXTBOX), 88, 56);
	if (FAILED(D3DXCreateSprite(myDevice, &mysprite)))
		throw _T("スプライトの生成に失敗しました");
}
LogWindow::~LogWindow() {
	if (mysprite) mysprite->Release();
	if (myTexture) myTexture->Release();
}

D3DXMATRIX LogWindow::getMatrix(int X, int Y, unsigned width) {
	D3DXMATRIX mat, mat1; D3DXMatrixIdentity(&mat); D3DXMatrixIdentity(&mat1);
	D3DXMatrixTranslation(&mat, -X, -Y, 0.0f);
	D3DXMatrixScaling(&mat1, (float)(width * halffontsz) / 77.0f, 1.0f, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixTranslation(&mat1, X, Y, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixScaling(&mat1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	return mat;
}

void LogWindow::renderFrame() {
	RECT rect;
	unsigned spriteNum = 0u;
	auto drawLine = [&rect, &spriteNum, this](int Y) -> void {
		rect.left = 0; rect.right = 5;
		SpriteRenderer::ShowSprite(mysprite, myTexture, x - 5, y + Y, 5, rect.bottom - rect.top, 0xffffffff, &rect);
		rect.left = 5; rect.right = 82;
		D3DXMATRIX mat = getMatrix(x, y, width);
		SpriteRenderer::ShowSprite(mysprite, myTexture, x, y + Y, width * halffontsz, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &mat);
		rect.left = 82; rect.right = 87;
		SpriteRenderer::ShowSprite(mysprite, myTexture, x + width * halffontsz, y + Y, 5, rect.bottom - rect.top, 0xffffffff, &rect);
	};
	rect.top = 0 + 0; rect.bottom = 0 + 5; drawLine(-5);
	rect.top = 0 + 9; rect.bottom = 0 + 19;
	for (unsigned i = 0; i < height; i++) {
		drawLine(20 * i); drawLine(20 * i + 10);
	}
	rect.top = 0 + 24; rect.bottom = 0 + 28; drawLine(20 * height);
}

void LogWindow::Render() {
	renderFrame();
}

// -------------------------------------------------------------------------

}
}
