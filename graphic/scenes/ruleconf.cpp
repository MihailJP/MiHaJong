#include "ruleconf.h"
#include <cstring>
#include "../scrmanip.h"
#include "../geometry.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../../common/version.h"
#include "../event.h"
#include <iomanip>

namespace mihajong_graphic {

RuleConfigScene::RuleConfigScene(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	myButtonPic = new ButtonPic(manipulator->getDevice());
	for (unsigned short i = 0; i < RULESIZE; i++)
		rulestat[i] = rules::getRule(i);
	menuCursor = 0; buttonCursor = -1; buttonDown = -1;
	char* RuleConfPtr[RULE_LINES];
	for (int i = 0; i < RULE_LINES; i++) RuleConfPtr[i] = RuleConf[i];
	rules::exportRule(RuleConfPtr);
	redrawItems();
	CreateButton(0, 1240, 1000, 156, 48, _T("�n �j"));
	CreateButton(1, 1060, 1000, 156, 48, _T("CANCEL"));
	CreateButton(2, 880, 1000, 156, 48, _T("NEXT ��"));
	CreateButton(3, 700, 1000, 156, 48, _T("�� PREV"));
}

RuleConfigScene::~RuleConfigScene() {
	delete myButtonPic;
}

void RuleConfigScene::CreateButton(unsigned btnID, int X, int Y, unsigned Width, unsigned Height, const CodeConv::tstring& caption) {
	const float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	myButtonPic->setButton(btnID, ButtonPic::clear,
		X * (WidthRate * Geometry::WindowScale()), Y * Geometry::WindowScale(),
		Width * (WidthRate * Geometry::WindowScale()), Height * Geometry::WindowScale(),
		0xffffffff, caption);
	if (regions.size() <= (btnID + btnRegionStart + 1)) {
		Region nullRegion = {0, 0, -1, -1};
		regions.resize(btnID + btnRegionStart + 1, Region(nullRegion));
	}
	regions[btnID + btnRegionStart].Left = X; regions[btnID + btnRegionStart].Top = Y; 
	regions[btnID + btnRegionStart].Right = X + Width;
	regions[btnID + btnRegionStart].Bottom = Y + Height; 
}

void RuleConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// ���ڂ�\��
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	unsigned itmNameCols = strwidth(prmName); // ����(���{���2��)
	D3DCOLOR baseColor = ((prmContent == _T("�m�^�`")) || (prmContent.empty()) || (rules::reqFailed(menuCursor / RULES_IN_PAGE * RULES_IN_PAGE + prmID, rulestat))) ? 0x00bfbfbf : 0x00ffffff;
	D3DCOLOR menuColor = ((menuCursor % RULES_IN_PAGE == prmID) && (buttonCursor == -1)) ? 0xff000000 : 0x7f000000;
	myTextRenderer->NewText(prmID * 3, prmName,
		(prmID / 20 * 720 + 50) * WidthRate, 135 + (prmID % 20) * 40, 1.0f,
		WidthRate * ((itmNameCols <= 8) ? 1.0f : 8.0f / (float)itmNameCols),
		menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 1, _T(":"),
		(prmID / 20 * 720 + 50 + 144) * WidthRate, 135 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 2, prmContent,
		(prmID / 20 * 720 + 50 + 162) * WidthRate, 135 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
	if (regions.size() <= prmID) {
		Region nullRegion = {0, 0, -1, -1};
		regions.resize(prmID + 1, Region(nullRegion));
	}
	regions[prmID].Left = (prmID / 20 * 720 + 50); regions[prmID].Top = 135 + (prmID % 20) * 40; 
	regions[prmID].Right = (prmID / 20 * 720 + 670); regions[prmID].Bottom = regions[prmID].Top + 35; 
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
		if (regions.size() <= 40) {
			Region nullRegion = {0, 0, -1, -1};
			regions.resize(40 + 1, Region(nullRegion));
		}
		regions[40].Left = (1400 - 15 * ((captionCols > 76) ? 76 : captionCols));
		regions[40].Top = 45; regions[40].Right = 1400; regions[40].Bottom = 104; 
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
			if (caption.empty()) {
				CodeConv::tostringstream o; SYSTEMTIME Zeit; GetLocalTime(&Zeit);
				o << _T("MiHaJong version ") _T(MIHAJONG_VER) _T(" / ���ݓ��� ") <<
					std::setw(4) << Zeit.wYear << _T("�N") <<
					std::setw(2) << Zeit.wMonth << _T("��") <<
					std::setw(2) << Zeit.wDay << _T("�� ") <<
					((Zeit.wHour / 12 == 0) ? _T("�ߑO") : _T("�ߌ�")) <<
					std::setw(2) << (Zeit.wHour % 12) << _T("��") <<
					std::setw(2) << std::setfill(_T('0')) << Zeit.wMinute << _T("��");
				caption = o.str();
			}
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

void RuleConfigScene::Render() {
	clearWithGameTypeColor(); // �o�b�t�@�N���A
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	{
		myTextRenderer->NewText(123, _T("���[���ݒ�"), 540 * WidthRate, 25, 2.0f, WidthRate, 0xffffffff);
	}
	ShowMessageBelow();
	ShowPageCaption();
	myTextRenderer->Render();
	myButtonPic->Render();
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

void RuleConfigScene::BtnEvent_OK_Down() {
	if (buttonCursor != -1) {
		sound::Play(sound::IDs::sndButton);
		buttonDown = buttonCursor;
		myButtonPic->setButton(buttonCursor, ButtonPic::sunken);
		redrawItems();
	}
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
		for (unsigned i = 0; i < buttons; i++)
			myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	}
}
void RuleConfigScene::BtnEvent_Cancel_Down() {
	sound::Play(sound::IDs::sndCursor);
	if (buttonCursor == -1) {
		buttonCursor = 0;
	} else {
		buttonCursor = -1;
	}
	for (unsigned i = 0; i < buttons; i++)
		myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
		myTimer.skipTo(0); redrawItems();
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

void RuleConfigScene::BtnEvent_Content_Item_Prev(unsigned short val) {
	sound::Play(sound::IDs::sndCursor);
	if ((menuCursor -= val) < 0) menuCursor = 0;
	myTimer.skipTo(0); redrawItems();
}
void RuleConfigScene::BtnEvent_Content_Item_Next(unsigned short val) {
	sound::Play(sound::IDs::sndCursor);
	if ((menuCursor += val) >= RULESIZE) menuCursor = RULESIZE - 1;
	myTimer.skipTo(0); redrawItems();
}
void RuleConfigScene::BtnEvent_Content_Page_Prev() {
	sound::Play(sound::IDs::sndClick);
	if ((menuCursor -= RULES_IN_PAGE) < 0) menuCursor += RULES_IN_PAGE;
	myTimer.skipTo(0); redrawItems();
}
void RuleConfigScene::BtnEvent_Content_Page_Next() {
	sound::Play(sound::IDs::sndClick);
	if ((menuCursor += RULES_IN_PAGE) >= RULESIZE) menuCursor -= RULES_IN_PAGE;
	myTimer.skipTo(0); redrawItems();
}

void RuleConfigScene::BtnEvent_Button_Prev() {
	sound::Play(sound::IDs::sndCursor);
	if ((--buttonCursor) < 0) buttonCursor = buttons - 1;
	for (unsigned i = 0; i < buttons; i++)
		myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	myTimer.skipTo(0); redrawItems();
}
void RuleConfigScene::BtnEvent_Button_Next() {
	sound::Play(sound::IDs::sndCursor);
	if ((++buttonCursor) >= buttons) buttonCursor = 0;
	for (unsigned i = 0; i < buttons; i++)
		myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	myTimer.skipTo(0); redrawItems();
}

void RuleConfigScene::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	switch (od->dwOfs) {
	case DIK_UP: case DIK_K: // �O�̍���
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(1);
		break;
	case DIK_DOWN: case DIK_J: // ���̍���
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(1);
		break;
	case DIK_LEFT: case DIK_H: // �O�̑I����
		if (od->dwData) {
			if (buttonCursor == -1)
				BtnEvent_Content_Roll_Down();
			else
				BtnEvent_Button_Next();
		}
		break;
	case DIK_RIGHT: case DIK_L: // ���̑I����
		if (od->dwData) {
			if (buttonCursor == -1)
				BtnEvent_Content_Roll_Up();
			else
				BtnEvent_Button_Prev();
		}
		break;
	case DIK_HOME: // �O�̃J����
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(20);
		break;
	case DIK_END: // ���̃J����
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(20);
		break;
	case DIK_PRIOR: // �O�̃y�[�W
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(RULES_IN_PAGE);
		break;
	case DIK_NEXT: // ���̃y�[�W
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(RULES_IN_PAGE);
		break;
	case DIK_ESCAPE: case DIK_X: // �L�����Z���L�[
		if (od->dwData) BtnEvent_Cancel_Down();
		break;
	case DIK_RETURN: case DIK_SPACE: case DIK_Z: // ����L�[
		if (od->dwData) BtnEvent_OK_Down();
		else BtnEvent_OK_Up();
		break;
	}
}

void RuleConfigScene::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const int scaledX = X / Geometry::WindowScale() * (Geometry::WindowWidth * 0.75f / Geometry::WindowHeight);
	const int scaledY = Y / Geometry::WindowScale();
	const int region = whichRegion(scaledX, scaledY);
#if 0
	{
		CodeConv::tostringstream o;
		o << _T("(") << scaledX << _T(", ") << scaledY << _T(") ");
		if (region != -1) o << _T("Region ") << region;
		else o << _T("No Region");
		myTextRenderer->NewText(144, o.str(), 0, 1000);
	}
#endif
	auto setcursor = [&]() -> void {
		if ((region >= 0) && (region <= (RULES_IN_PAGE - 1))) {
			if ((region != menuCursor % RULES_IN_PAGE) || (buttonCursor != -1)) {
				sound::Play(sound::IDs::sndCursor);
				menuCursor = menuCursor / RULES_IN_PAGE * RULES_IN_PAGE + region;
				buttonCursor = -1;
				for (unsigned i = 0; i < buttons; i++) myButtonPic->setButton(i, ButtonPic::clear);
				myTimer.skipTo(0); redrawItems();
			}
		}
		else if ((region >= btnRegionStart) && (region <= (btnRegionStart + buttons - 1))) {
			if (region != (buttonCursor + btnRegionStart)) {
				sound::Play(sound::IDs::sndCursor);
				buttonCursor = region - btnRegionStart;
				for (unsigned i = 0; i < buttons; i++)
					myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
				myTimer.skipTo(0); redrawItems();
			}
		}
	};
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // �}�E�X�J�[�\���𓮂������ꍇ
		setcursor();
		break;
	case DIMOFS_Z: // �z�C�[���̑���
		if ((region >= 0) && (region <= (RULES_IN_PAGE - 1))) {
			setcursor();
			if ((LONG)od->dwData > 0) BtnEvent_Content_Roll_Up();
			else if ((LONG)od->dwData < 0) BtnEvent_Content_Roll_Down();
		} else if (region == 40) {
			if ((LONG)od->dwData > 0)
				BtnEvent_Content_Page_Next();
			else if ((LONG)od->dwData < 0)
				BtnEvent_Content_Page_Prev();
		}
		break;
	case DIMOFS_BUTTON0: // ���N���b�N
		if ((od->dwData) && (region >= 0) && (region <= (RULES_IN_PAGE - 1))) {
			setcursor();
			BtnEvent_Content_Roll_Up();
		}
		else if ((region >= btnRegionStart) && (region < (btnRegionStart + buttons))) {
			setcursor();
			if (od->dwData) BtnEvent_OK_Down();
			else BtnEvent_OK_Up();
		}
		break;
	}
}

}
