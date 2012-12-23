#pragma once

#include "exports.h"
#include "../mjcore/strcode.h"
#include <d3dx9.h>
#include <dxerr.h>

namespace mihajong_graphic {
namespace logwnd {

EXPORT void append(LPCTSTR logstr);

#ifdef GRAPHIC_EXPORTS
CodeConv::tstring getlog();

class LogWindow {
private:
	static unsigned const halffontsz = 9; // îºäpÇÃâ°ïù
	HWND myHWnd;
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 myTexture;
	LPD3DXSPRITE mysprite;
	int x, y; unsigned width, height;
	void renderFrame();
	D3DXMATRIX getMatrix(int X, int Y, unsigned Width);
public:
	LogWindow(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned Width, unsigned Height);
	~LogWindow();
	void Render();
};
#endif

}
}
