#include "ruleconf.h"

RuleConfigScene::RuleConfigScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
}

RuleConfigScene::~RuleConfigScene() {
}

void RuleConfigScene::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // バッファクリア
}
