#include "gari.h"
#include "../../gametbl.h"
#include "../../utils.h"

namespace mihajong_graphic {

using utils::playerRelative;

void GameTableScreen::GariReconst::Render() {
}

GameTableScreen::GariReconst::GariReconst(GameTableScreen* parent) {
	caller = parent;
}

GameTableScreen::GariReconst::~GariReconst() {
}

}
