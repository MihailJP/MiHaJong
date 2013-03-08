#include "result.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

ResultScreen::ResultScreen(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	rankRenderer.fill(nullptr);
}

ResultScreen::~ResultScreen() {
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) delete *k;
}

void ResultScreen::Render() {
	clearWithGameTypeColor();
}

void ResultScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
}

void ResultScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
}

// -------------------------------------------------------------------------

ResultScreen::RankRenderer::RankRenderer(LPDIRECT3DDEVICE9 device, int id) {
	myDevice = device;
	myID = id;
}

ResultScreen::RankRenderer::~RankRenderer() {
}

void ResultScreen::RankRenderer::Render() {
}

// -------------------------------------------------------------------------

}
