#include "test.h"
#include "../scrmanip.h"

namespace mihajong_graphic {

TestScene::TestScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
}

TestScene::~TestScene() {
}

void TestScene::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
}

}
