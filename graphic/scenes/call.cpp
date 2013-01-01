#include "call.h"

namespace mihajong_graphic {

GameTableCall::GameTableCall(ScreenManipulator* const manipulator) : GameTableScreen(manipulator) {
}

GameTableCall::~GameTableCall() {
}

void GameTableCall::Render() {
	GameTableScreen::Render(); /* ‰¼’u‚« */
}

}
