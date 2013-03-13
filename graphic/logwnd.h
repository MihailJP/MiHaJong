#pragma once

#include "exports.h"
#include "../common/strcode.h"
#ifdef GRAPHIC_EXPORTS
#include "directx.h"
#include <vector>
#include "text.h"
#endif

namespace mihajong_graphic {
namespace logwnd {

EXPORT void append(LPCTSTR logstr);
EXPORT LPCTSTR getlogptr();

#ifdef GRAPHIC_EXPORTS
CodeConv::tstring getlog();

class LogWindow {
private:
	static unsigned const halffontsz = 9; // îºäpÇÃâ°ïù
	HWND myHWnd;
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 myTexture;
	SmallTextRenderer* myTextRenderer;
	int x, y; unsigned width, height;
	void renderFrame();
	D3DXMATRIX getMatrix(int X, int Y, unsigned Width);
private:
	static unsigned const lineheight = 20;
	std::vector<CodeConv::tstring> lines;
	void reconstruct_lines();
public:
	LogWindow(HWND hwnd, LPDIRECT3DDEVICE9 device, int X, int Y, unsigned Width, unsigned Height);
	~LogWindow();
	void Render();
};
#endif

}
}
