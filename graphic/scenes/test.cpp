#include "test.h"
#include "../scrmanip.h"

namespace mihajong_graphic {

TestScene::TestScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	myLogWindow = new logwnd::LogWindow(caller->getHWnd(), caller->getDevice(), 100, 100, 30, 10);
}

TestScene::~TestScene() {
	delete myLogWindow;
}

void TestScene::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xff336600, 1.0f, 0);
	myLogWindow->Render();
}

}
