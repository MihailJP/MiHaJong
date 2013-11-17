#pragma once

#include "../exports.h"
#include "../../common/strcode.h"
#ifdef GRAPHIC_EXPORTS
#include "../directx.h"
#include <vector>
#include "../text.h"
#endif

namespace mihajong_graphic {
namespace logwnd {

EXPORT void reset();
EXPORT void append(LPCTSTR logstr);
EXPORT LPCTSTR getlogptr();

#ifdef GRAPHIC_EXPORTS
CodeConv::tstring getlog();

class LogWindow {
private:
	static unsigned const halffontsz = 9; // îºäpÇÃâ°ïù
#ifdef _WIN32
	HWND myHWnd;
#else /*_WIN32*/
	Window myHWnd;
#endif /*_WIN32*/
	DevicePtr myDevice;
	TexturePtr myTexture;
	SmallTextRenderer* myTextRenderer;
	int x, y; unsigned width, height;
	void renderFrame();
	TransformMatrix getMatrix(int X, int Y, unsigned Width);
	bool reconstFlag;
private:
	static unsigned const lineheight = 20;
	std::vector<CodeConv::tstring> lines;
	void reconstruct_lines();
public:
#ifdef _WIN32
	LogWindow(HWND hwnd, DevicePtr device, int X, int Y, unsigned Width, unsigned Height);
#else /*_WIN32*/
	LogWindow(Window hwnd, DevicePtr device, int X, int Y, unsigned Width, unsigned Height);
#endif /*_WIN32*/
	~LogWindow();
	void Render();
};
#endif

}
}
