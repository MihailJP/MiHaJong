#include "test.h"
#include "../scrmanip.h"

namespace mihajong_graphic {

TestScene::TestScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	myEditBox = new EditBox(caller->getHWnd(), caller->getDevice(), 100, 100, 200, 200);
}

TestScene::~TestScene() {
	delete myEditBox;
}

void TestScene::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xffddeeff, 1.0f, 0);
	myEditBox->Render();
}

void TestScene::KeyboardInput(WPARAM wParam, LPARAM lParam) {
	myEditBox->KeyboardInput(wParam, lParam);
}

void TestScene::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	myEditBox->IMEvent(message, wParam, lParam);
}

}
