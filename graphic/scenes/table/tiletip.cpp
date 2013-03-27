#include "tiletip.h"

namespace mihajong_graphic {

GameTableScreen::TileTipReconst::TileTipReconst(GameTableScreen* parent) {
	caller = parent;
	myTextRenderer = new TextRenderer(parent->caller->getDevice());
}

GameTableScreen::TileTipReconst::~TileTipReconst() {
	delete myTextRenderer;
}

void GameTableScreen::TileTipReconst::reconstruct() {
	myTextRenderer->NewText(0, _T("foobar"), TipX, TipY);
}

void GameTableScreen::TileTipReconst::Render() {
	reconstruct();
	myTextRenderer->Render();
}

}
