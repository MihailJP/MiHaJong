#include "ruleconf.h"
#include "../geometry.h"

namespace mihajong_graphic {

RuleConfigScene::RuleConfigScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	textRenderer = new TextRenderer(manipulator->getDevice());
	menuCursor = 0;
	redrawItems();
}

RuleConfigScene::~RuleConfigScene() {
	delete textRenderer;
}

void RuleConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// ���ڂ�\��
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	unsigned itmNameCols = 0u; // ����(���{���2��)
	for (auto k = prmName.begin(); k != prmName.end(); ++k) {
		if (*k <= _T('\x7f')) itmNameCols += 1;
		else itmNameCols += 2;
	}
	D3DCOLOR menuColor = (menuCursor % 40 == prmID) ? 0xffffffff : 0x9fffffff;
	textRenderer->NewText(prmID * 3, prmName,
		(prmID / 20 * 720 + 50) * WidthRate, 160 + (prmID % 20) * 40, 1.0f,
		WidthRate * ((itmNameCols <= 8) ? 1.0f : 8.0f / (float)itmNameCols),
		menuColor);
	textRenderer->NewText(prmID * 3 + 1, _T(":"),
		(prmID / 20 * 720 + 50 + 144) * WidthRate, 160 + (prmID % 20) * 40, 1.0, WidthRate, menuColor);
	textRenderer->NewText(prmID * 3 + 2, prmContent,
		(prmID / 20 * 720 + 50 + 162) * WidthRate, 160 + (prmID % 20) * 40, 1.0, WidthRate, menuColor);
}

void RuleConfigScene::redrawItems() {
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	for (int i = 0; i < 40; i++) {
		CodeConv::tostringstream o; o << _T("ItemNr.") << (menuCursor / 40 * 40) + i;
		itemText(i, o.str(), _T("Lorem ipsum dolor sit amet"));
	}
}

void RuleConfigScene::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
	D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // �o�b�t�@�N���A
	textRenderer->Render();
}

void RuleConfigScene::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	switch (od->dwOfs) {
	case DIK_UP: // �O�̍���
		if (od->dwData) {--menuCursor; redrawItems();}
		break;
	case DIK_DOWN: // ���̍���
		if (od->dwData) {++menuCursor; redrawItems();}
		break;
	case DIK_LEFT: // �O�̑I����
		break;
	case DIK_RIGHT: // ���̑I����
		break;
	case DIK_HOME: // �O�̃J����
		if (od->dwData) {menuCursor -= 20; redrawItems();}
		break;
	case DIK_END: // ���̃J����
		if (od->dwData) {menuCursor += 20; redrawItems();}
		break;
	case DIK_PRIOR: // �O�̃y�[�W
		if (od->dwData) {menuCursor -= 40; redrawItems();}
		break;
	case DIK_NEXT: // ���̃y�[�W
		if (od->dwData) {menuCursor += 40; redrawItems();}
		break;
	}
}

}
