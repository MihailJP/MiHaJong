#include "ruleconf.h"

RuleConfigScene::RuleConfigScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	textRenderer = new TextRenderer(manipulator->getDevice());
}

RuleConfigScene::~RuleConfigScene() {
	delete textRenderer;
}

void RuleConfigScene::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // バッファクリア
	static int k = 0; static int l = 0; 
	if ((k++) >= 60) {
		for (int i = 0; i < 20; i++)
			textRenderer->NewText(i, _T("コンフィグこうもくExample"), 200, 200 + i * 40, 1.0, 1.0, 0x99ffffff);
		for (int i = 0; i < 20; i++)
			textRenderer->NewText(i + 20, _T("コンフィグこうもくExample"), 700, 200 + i * 40, 1.0, 1.0, 0xffffffff);
		k = 0;
		if ((l++) > 9) {
			for (int i = 0; i < 40; i++)
				textRenderer->DelText(i);
			l = 0;
		}
	}
	textRenderer->Render();
}
