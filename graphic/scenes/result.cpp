#include "result.h"

namespace mihajong_graphic {

using namespace mihajong_structs;

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
	for (int i = 0; i < Players; ++i)
		if ((myTimer.elapsed() >= (1000000 + i * 500000)) && (rankRenderer[i] == nullptr))
			rankRenderer[i] = new RankRenderer(caller->getDevice(), i);
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) (*k)->Render();
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
