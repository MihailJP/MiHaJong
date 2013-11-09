#include "logwnd.h"
#include "../resource.h"
#include "../loadtex.h"
#include "../sprite.h"
#include <cassert>
#include <algorithm>
#include "../chrwidth.h"
#include "../../common/mutex.h"

namespace mihajong_graphic {
namespace logwnd {

using namespace character_width;

// -------------------------------------------------------------------------

namespace {
	CodeConv::tostringstream logdata;
	MHJMutex LogWindowMutex;
	bool updated = false;
}

EXPORT void reset() {
	LogWindowMutex.syncDo<void>([]() -> void {
		logdata.clear(); logdata.str(_T(""));
		updated = true;
	});
}

EXPORT void append(LPCTSTR logstr) {
	LogWindowMutex.syncDo<void>([logstr]() -> void {
		logdata << logstr; logdata.flush();
		updated = true;
	});
}

EXPORT LPCTSTR getlogptr() {
	static CodeConv::tstring logstr;
	return LogWindowMutex.syncDo<LPCTSTR>([]() -> LPCTSTR {
		logstr = logdata.str();
		return logstr.c_str();
	});
}

CodeConv::tstring getlog() {
	return LogWindowMutex.syncDo<CodeConv::tstring>([]() -> CodeConv::tstring {
		return logdata.str();
	});
}

// -------------------------------------------------------------------------

#ifdef _WIN32
LogWindow::LogWindow(HWND hwnd, DevicePtr device, int X, int Y, unsigned Width, unsigned Height)
#else /*_WIN32*/
LogWindow::LogWindow(Window hwnd, DevicePtr device, int X, int Y, unsigned Width, unsigned Height)
#endif /*_WIN32*/
{
	assert(Width >= 8); assert(Height >= 1);
	myHWnd = hwnd;
	myDevice = device; x = X; y = Y; width = Width; height = Height;
	LoadTexture(device, &myTexture, MAKEINTRESOURCE(IDB_PNG_TEXTBOX));
	myTextRenderer = new SmallTextRenderer(myDevice);
	reconstruct_lines();
}
LogWindow::~LogWindow() {
	if (myTextRenderer) delete myTextRenderer;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (myTexture) myTexture->Release();
#endif
}

TransformMatrix LogWindow::getMatrix(int X, int Y, unsigned width) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix mat, mat1; D3DXMatrixIdentity(&mat); D3DXMatrixIdentity(&mat1);
	D3DXMatrixTranslation(&mat, -X, -Y, 0.0f);
	D3DXMatrixScaling(&mat1, (float)(width * halffontsz) / 77.0f, 1.0f, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixTranslation(&mat1, X, Y, 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
	D3DXMatrixScaling(&mat1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&mat, &mat, &mat1);
#else
	glPushMatrix(); glLoadIdentity();
	glTranslatef(0.0f, (float)Geometry::WindowHeight, 0.0f);
	// 横幅拡大は不要
	glScalef(Geometry::WindowScale(), Geometry::WindowScale(), 1.0f);
	glTranslatef(0.0f, -(float)Geometry::WindowHeight, 0.0f);
	TransformMatrix mat; glGetFloatv(GL_MODELVIEW_MATRIX, &mat[0]);
	glPopMatrix();
#endif
	return mat;
}

void LogWindow::renderFrame() {
	RECT rect;
	unsigned spriteNum = 0u;
	auto drawLine = [&rect, &spriteNum, this](int Y) -> void {
		rect.left = 0; rect.right = 5;
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, x - 5, y + Y, 5, rect.bottom - rect.top, 0xffffffff, &rect);
		rect.left = 5; rect.right = 82;
		TransformMatrix mat = getMatrix(x, y, width);
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, x, y + Y, width * halffontsz, rect.bottom - rect.top, 0xffffffff, &rect, 0, 0, &mat);
		rect.left = 82; rect.right = 87;
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, x + width * halffontsz, y + Y, 5, rect.bottom - rect.top, 0xffffffff, &rect);
	};
	rect.top = 0 + 0; rect.bottom = 0 + 5; drawLine(-5);
	rect.top = 0 + 9; rect.bottom = 0 + 19;
	for (unsigned i = 0; i < height; i++) {
		drawLine(20 * i); drawLine(20 * i + 10);
	}
	rect.top = 0 + 24; rect.bottom = 0 + 28; drawLine(20 * height);
}

void LogWindow::Render() {
	using std::max;
	renderFrame();
	LogWindowMutex.syncDo<void>([this]() -> void {
		if (updated) {
			reconstruct_lines(); updated = false;
		}
	});
	unsigned linenum = 0;
	for (unsigned i = (unsigned)(max(0, (signed)lines.size() - (signed)height)); i < lines.size(); ++i) { // ログの最後の部分を表示
	//for (unsigned i = 0; i < min(height, lines.size()); ++i) { // ログの最初の部分を表示
		myTextRenderer->NewText(linenum, lines[i], x, y + lineheight * linenum);
		++linenum;
	}
	myTextRenderer->Render();
}

void LogWindow::reconstruct_lines() { // 行に分割
	lines.clear();
	unsigned startPos = 0, currentDigit = 0;
	CodeConv::tstring tmplog;
	{
		const CodeConv::tstring tmplog1(logwnd::getlog());
		for (const auto& k : tmplog1)
			if (k != _T('\r'))
				tmplog += k;
	}
	for (unsigned i = 0; i < tmplog.size(); ++i) {
		if (tmplog[i] == _T('\n')) {
			lines.push_back(tmplog.substr(startPos, i - startPos));
			startPos = i + 1; currentDigit = 0;
		} else {
			currentDigit += (isFullWidth(tmplog[i]) ? 2 : 1);
			if (currentDigit > width) {
				/*  0    0    1    1   1|
				**  0....5....0....5...9v
				**  aいろはにほへとちりぬ
				**  01 2 3 4 5 6 7 8 9 10 */
				lines.push_back(tmplog.substr(startPos, i - startPos));
				startPos = i; currentDigit = 2;
			} else if ((currentDigit == width) && (isLeadingByte(tmplog, i))) {
				/*  0    0    1    1   1|
				**  0....5....0....5...9v
				**  aいろはにほへとちりぬ
				**  012345678901234567890 */
				lines.push_back(tmplog.substr(startPos, i - startPos));
				startPos = i; currentDigit = 1;
			} else if (currentDigit == width) {
				/*  0    0    1    1   1|
				**  0....5....0....5...9v
				**  いろはにほへとちりぬる
				**  0 1 2 3 4 5 6 7 8 9 10 */
				lines.push_back(tmplog.substr(startPos, i - startPos + 1));
				startPos = i + 1; currentDigit = (isFullWidth(tmplog[i]) ? 2 : 1);
			}
		}
	}
	if (startPos < tmplog.size())
		lines.push_back(tmplog.substr(startPos, tmplog.size()));
}

// -------------------------------------------------------------------------

}
}
