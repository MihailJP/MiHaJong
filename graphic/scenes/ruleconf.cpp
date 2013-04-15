#include "ruleconf.h"
#include <cstring>
#include "../scrmanip.h"
#include "../geometry.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../event.h"
#include <iomanip>

namespace mihajong_graphic {

RuleConfigScene::RuleConfigScene(ScreenManipulator* const manipulator) : ConfigMenuProto(manipulator) {
	for (unsigned short i = 0; i < RULESIZE; i++)
		rulestat[i] = rules::getRule(i);
	char* RuleConfPtr[RULE_LINES];
	for (int i = 0; i < RULE_LINES; i++) RuleConfPtr[i] = RuleConf[i];
	rules::exportRule(RuleConfPtr);
	CreateButton(0, 1240, 1000, 156, 48, _T("�n �j"));
	CreateButton(1, 1060, 1000, 156, 48, _T("CANCEL"));
	CreateButton(2, 880, 1000, 156, 48, _T("NEXT ��"));
	CreateButton(3, 700, 1000, 156, 48, _T("�� PREV"));
	redrawItems();
}

RuleConfigScene::~RuleConfigScene() {
}

void RuleConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// ���ڂ�\��
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	unsigned itmNameCols = strwidth(prmName); // ����(���{���2��)
	ArgbColor baseColor = ((prmContent == _T("�m�^�`")) || (prmContent.empty()) || (rules::reqFailed(menuCursor / RULES_IN_PAGE * RULES_IN_PAGE + prmID, rulestat))) ? 0x00bfbfbf : 0x00ffffff;
	ArgbColor menuColor = ((menuCursor % RULES_IN_PAGE == prmID) && (buttonCursor == -1)) ? 0xff000000 : 0x7f000000;
	myTextRenderer->NewText(prmID * 3, prmName,
		(prmID / 20 * 720 + 50) * WidthRate, 135 + (prmID % 20) * 40, 1.0f,
		WidthRate * ((itmNameCols <= 8) ? 1.0f : 8.0f / (float)itmNameCols),
		menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 1, _T(":"),
		(prmID / 20 * 720 + 50 + 144) * WidthRate, 135 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 2, prmContent,
		(prmID / 20 * 720 + 50 + 162) * WidthRate, 135 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
	setRegion(prmID,
		(prmID / 20 * 720 + 50) , 135 + (prmID % 20) * 40,
		(prmID / 20 * 720 + 670), 135 + (prmID % 20) * 40 + 35);
}

void RuleConfigScene::redrawItems() {
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	for (int i = 0; i < RULES_IN_PAGE; i++) {
		const unsigned ItemNum = (menuCursor / RULES_IN_PAGE * RULES_IN_PAGE) + i;
		TCHAR menuitem[128]; rules::getRuleName(menuitem, 128, ItemNum);
		TCHAR itmfield[128]; rules::getRuleTxt(itmfield, 128, ItemNum, rulestat[ItemNum]);
		itemText(i, CodeConv::tstring(menuitem), CodeConv::tstring(itmfield));
	}
}

void RuleConfigScene::ShowPageCaption() {
	const float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	{
		CodeConv::tostringstream o; o << _T("Page ") << std::setw(2) << (menuCursor / RULES_IN_PAGE + 1) << _T("/") << RULE_PAGES;
		CodeConv::tstring pagecaption(o.str());
		unsigned captionCols = strwidth(pagecaption); // ����(���{���2��)
		myTextRenderer->NewText(121, pagecaption,
			(1400 - 15 * ((captionCols > 76) ? 76 : captionCols)) * WidthRate, 45, 0.833333f,
			(captionCols > 76) ? 76.0f / (float)captionCols * WidthRate : WidthRate, 0xffffffff);
	}
	{
		TCHAR pagecap[128]; rules::getPageCaption(pagecap, 128, menuCursor / RULES_IN_PAGE);
		CodeConv::tstring pagecaption(pagecap);
		unsigned captionCols = strwidth(pagecaption); // ����(���{���2��)
		myTextRenderer->NewText(122, pagecaption,
			(1400 - 15 * ((captionCols > 76) ? 76 : captionCols)) * WidthRate, 75, 0.833333f,
			(captionCols > 76) ? 76.0f / (float)captionCols * WidthRate : WidthRate, 0xffffffff);
		setRegion(40, (1400 - 15 * ((captionCols > 76) ? 76 : captionCols)), 45, 1400, 104);
	}
}
void RuleConfigScene::ShowMessageBelow() {
	const float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	TimerMicrosec t = myTimer.elapsed();
	CodeConv::tstring caption = _T("");
	if (buttonCursor == -1) {
		switch ((t / 5000000u) % 6) {
		case 0:
			TCHAR menuitem[128]; rules::getRuleDescription(menuitem, 128, menuCursor);
			caption = CodeConv::tstring(menuitem);
			if (caption.empty())
				caption = verInfoText();
			break;
		case 1:
			caption = CodeConv::tstring(_T("�L�[�{�[�h����  ��/��:�J�[�\���ړ�  ��/��:�I�𒆂̍��ڂ�ύX  Esc/X:�{�^���I����"));
			break;
		case 2:
			caption = CodeConv::tstring(_T("�L�[�{�[�h����  Home/End:���E�J�����Ԃ̈ړ�  PageUp/PageDown:�y�[�W�Ԃ̈ړ�"));
			break;
		case 3:
			caption = CodeConv::tstring(_T("�}�E�X����  ���ڏ�ō��N���b�N/�z�C�[����]:�I�𒆂̍��ڂ�ύX"));
			break;
		case 4:
			caption = CodeConv::tstring(_T("�}�E�X����  �E�㌩�o����Ńz�C�[����]:�y�[�W�Ԃ̈ړ�"));
			break;
		case 5:
			caption = CodeConv::tstring(_T("�ʐM�ΐ펞�̃��[���ݒ�̓z�X�g���̐ݒ肪�K�p����܂�"));
			break;
		}
	} else {
		switch ((t / 5000000u) % 3) {
		case 0:
			switch (buttonCursor) {
			case 0:
				caption = _T("�ݒ��ۑ����A�^�C�g����ʂɖ߂�܂�");
				break;
			case 1:
				caption = _T("�ݒ��j�����A�^�C�g����ʂɖ߂�܂�");
				break;
			case 2:
				caption = _T("���̃y�[�W�Ɉړ����܂�");
				break;
			case 3:
				caption = _T("�O�̃y�[�W�Ɉړ����܂�");
				break;
			}
			break;
		case 1:
			caption = CodeConv::tstring(_T("�L�[�{�[�h����  ��/��:�J�[�\���ړ�  Enter/Space/Z:����  Esc/X:���[���ݒ�ɖ߂�"));
			break;
		case 2:
			caption = CodeConv::tstring(_T("�ʐM�ΐ펞�̃��[���ݒ�̓z�X�g���̐ݒ肪�K�p����܂�"));
			break;
		}
	}
	unsigned captionCols = strwidth(caption); // ����(���{���2��)
	myTextRenderer->NewText(120, caption,
		(720 - 9 * ((captionCols > 76) ? 76 : captionCols)) * WidthRate, 955, 1.0f,
		(captionCols > 76) ? 76.0f / (float)captionCols * WidthRate : WidthRate,
		((t % 5000000u) < 500000u) ? (55u + ((t % 5000000u) / 2500u)) << 24 | 0x00ffffff : 0xffffffff);
}

void RuleConfigScene::saveRule() {
	std::memset(&RuleConf[0][0], 0, sizeof(RuleConf));
	for (unsigned i = 0; i < RULESIZE; i++) {
		TCHAR ruletxt[128]; rules::getRuleTxt(ruletxt, 128, i, 0);
		if ((CodeConv::tstring(ruletxt) == _T("")) || (CodeConv::tstring(ruletxt) == _T("�m�^�`")))
			RuleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] = '-';
		else
			RuleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] = rules::digit[rulestat[i]];
	}
	const char* ruleLine[RULE_LINES];
	for (unsigned i = 0; i < RULE_LINES; i++) ruleLine[i] = RuleConf[i];
	rules::storeRule(ruleLine);
	rules::saveConfigFile(rules::conffile.c_str());
	return;
}

void RuleConfigScene::BtnEvent_OK_Up() {
	if ((buttonCursor != -1) && (buttonDown == buttonCursor)) {
		switch (buttonCursor) {
		case 3:
			BtnEvent_Content_Page_Prev();
			break;
		case 2:
			BtnEvent_Content_Page_Next();
			break;
		case 1:
			ui::UIEvent->set(1);
			break;
		case 0:
			saveRule();
			ui::UIEvent->set(0);
			break;
		}
		buttonDown = -1;
		for (unsigned i = 0; i < numberOfButtons(); i++)
			myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	}
}

void RuleConfigScene::BtnEvent_Content_Roll_Up() {
	sound::Play(sound::IDs::sndClick);
	while (true) {
		if ((++rulestat[menuCursor]) >= rules::getRuleSize(menuCursor)) rulestat[menuCursor] = 0;
		TCHAR menuitem[128]; rules::getRuleTxt(menuitem, 128, menuCursor, rulestat[menuCursor]);
		if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
	}
	redrawItems();
}
void RuleConfigScene::BtnEvent_Content_Roll_Down() {
	sound::Play(sound::IDs::sndClick);
	while (true) {
		if ((--rulestat[menuCursor]) < 0) rulestat[menuCursor] = rules::getRuleSize(menuCursor) - 1;
		TCHAR menuitem[128]; rules::getRuleTxt(menuitem, 128, menuCursor, rulestat[menuCursor]);
		if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
	}
	redrawItems();
}

}
