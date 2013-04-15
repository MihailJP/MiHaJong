#include "setting.h"
#include "../event.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../extchar.h"
#include <iomanip>

namespace mihajong_graphic {

PreferenceConfigScene::PreferenceConfigScene(ScreenManipulator* const manipulator) : ConfigMenuProto(manipulator) {
	for (unsigned short i = 0; i < PREFERENCE_ITEMS; i++)
		prefstat[i] = rules::getPreference(i);
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		editBoxes[i] = nullptr;
	CreateButton(0, 1240, 1000, 156, 48, _T("�n �j"));
	CreateButton(1, 1060, 1000, 156, 48, _T("CANCEL"));
	redrawItems();
}

PreferenceConfigScene::~PreferenceConfigScene() {
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		if (editBoxes[i])
			delete editBoxes[i];
}

void PreferenceConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// ���ڂ�\��
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	const unsigned ItemNum = (menuCursor / RULES_IN_PAGE * RULES_IN_PAGE) + prmID;
	unsigned itmNameCols = strwidth(prmName); // ����(���{���2��)
	ArgbColor baseColor = ((prmContent == _T("�m�^�`")) || ((prmContent.empty()) && (rules::getPreferenceInputSize(ItemNum) == 0))) ? 0x00bfbfbf : 0x00ffffff;
	ArgbColor menuColor = ((menuCursor % RULES_IN_PAGE == prmID) && (buttonCursor == -1)) ? 0xff000000 : 0x7f000000;
	const int xPos = (prmID / 20 * 720 + 50), yPos = 135 + (prmID % 20) * 40;
	myTextRenderer->NewText(prmID * 3, prmName,
		xPos * WidthRate, yPos, 1.0f,
		WidthRate * ((itmNameCols <= 8) ? 1.0f : 8.0f / (float)itmNameCols),
		menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 1, _T(":"),
		(xPos + 144) * WidthRate, yPos, 1.0, WidthRate, menuColor | baseColor);
	if (!rules::getPreferenceInputSize(ItemNum))
		myTextRenderer->NewText(prmID * 3 + 2, prmContent,
		(xPos + 162) * WidthRate, yPos, 1.0, WidthRate, menuColor | baseColor);
	setRegion(prmID,
		(prmID / 20 * 720 + 50) , 135 + (prmID % 20) * 40,
		(prmID / 20 * 720 + 670), 135 + (prmID % 20) * 40 + 35);
	if (rules::getPreferenceInputSize(ItemNum)) { // �G�f�B�b�g�{�b�N�X
		if (editBoxes[prmID] == nullptr) {
			editBoxes[prmID] = new EditBox(caller->getHWnd(), caller->getDevice(),
				(xPos + 162 + 4) * WidthRate, yPos + 6, 32, 1.5f);
			const CodeConv::tstring txt(CodeConv::EnsureTStr(rules::getPreferenceRawStr(prmID)));
			editBoxes[prmID]->setText(txt);
		}
	} else {
		if (editBoxes[prmID] != nullptr)
			delete editBoxes[prmID];
	}
}

void PreferenceConfigScene::redrawItems() {
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	for (int i = 0; i < RULES_IN_PAGE; i++) {
		const unsigned ItemNum = (menuCursor / RULES_IN_PAGE * RULES_IN_PAGE) + i;
		TCHAR menuitem[128]; rules::getPreferenceName(menuitem, 128, ItemNum);
		TCHAR itmfield[128]; rules::getPreferenceTxt(itmfield, 128, ItemNum, prefstat[ItemNum]);
		itemText(i, CodeConv::tstring(menuitem), CodeConv::tstring(itmfield));
	}
}
void PreferenceConfigScene::ShowPageCaption() {
}
void PreferenceConfigScene::ShowMessageBelow() {
	const float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	TimerMicrosec t = myTimer.elapsed();
	CodeConv::tstring caption = _T("");
	if (getActiveTextbox() != -1) {
		switch ((t / 5000000u) % 2) {
		case 0:
			caption = CodeConv::tstring(_T("�������͒�  Esc/Tab/Enter:�߂�"));
			break;
		case 1:
			caption = verInfoText();
			break;
		}
	} else if (buttonCursor == -1) {
		switch ((t / 5000000u) % 5) {
		case 0:
			TCHAR menuitem[128]; rules::getPreferenceDescription(menuitem, 128, menuCursor);
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
			caption = CodeConv::tstring(_T("�L�[�{�[�h����  Tab/Enter/Space/Z�F�������̓��[�h"));
			break;
		case 4:
			caption = CodeConv::tstring(_T("�}�E�X����  ���ڏ�ō��N���b�N/�z�C�[����]:�I�𒆂̍��ڂ�ύX/�������̓��[�h"));
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

void PreferenceConfigScene::Render() {
	ConfigMenuProto::Render();
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		if (editBoxes[i]) editBoxes[i]->Render();
}

void PreferenceConfigScene::savePreference() {
	memset(&PrefConf[0][0], 0, sizeof(PrefConf));
	for (unsigned i = 0; i < PREFERENCE_ITEMS; i++) {
		TCHAR preftxt[128]; rules::getPreferenceTxt(preftxt, 128, i, 0);
		if ((CodeConv::tstring(preftxt) == _T("")) || (CodeConv::tstring(preftxt) == _T("�m�^�`")))
			PrefConf[i / RULE_IN_LINE][i % RULE_IN_LINE] = '-';
		else
			PrefConf[i / RULE_IN_LINE][i % RULE_IN_LINE] = rules::digit[prefstat[i]];
	}
	const char* prefLine[PREFERENCE_LINES];
	for (unsigned i = 0; i < PREFERENCE_LINES; i++) prefLine[i] = PrefConf[i];
	rules::storePref(prefLine);
	for (unsigned i = 0; i < PREFERENCE_ITEMS; i++) {
		if (editBoxes[i]) {
			CodeConv::tstring ttxt = editBoxes[i]->getText();
			std::string txt =
#ifdef _UNICODE
				CodeConv::toANSI
#endif
				(ttxt);
			rules::setPreferenceFreeStr(i, txt.c_str());
		}
	}
	rules::savePreferenceFile(rules::preffile.c_str());
	return;
}

void PreferenceConfigScene::BtnEvent_OK_Up() {
	if ((buttonCursor != -1) && (buttonDown == buttonCursor)) {
		switch (buttonCursor) {
		case 1:
			ui::UIEvent->set(1);
			break;
		case 0:
			savePreference();
			ui::UIEvent->set(0);
			break;
		}
		buttonDown = -1;
		for (unsigned i = 0; i < numberOfButtons(); i++)
			myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	}
}

void PreferenceConfigScene::BtnEvent_Content_Roll_Up() {
	sound::Play(sound::IDs::sndClick);
	while (true) {
		if ((++prefstat[menuCursor]) >= rules::getPreferenceSize(menuCursor)) prefstat[menuCursor] = 0;
		TCHAR menuitem[128]; rules::getPreferenceTxt(menuitem, 128, menuCursor, prefstat[menuCursor]);
		if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
	}
	setVolume();
	setActiveTextbox(menuCursor);
	redrawItems();
}
void PreferenceConfigScene::BtnEvent_Content_Roll_Down() {
	sound::Play(sound::IDs::sndClick);
	while (true) {
		if ((--prefstat[menuCursor]) < 0) prefstat[menuCursor] = rules::getPreferenceSize(menuCursor) - 1;
		TCHAR menuitem[128]; rules::getPreferenceTxt(menuitem, 128, menuCursor, prefstat[menuCursor]);
		if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
	}
	setVolume();
	setActiveTextbox(menuCursor);
	redrawItems();
}

void PreferenceConfigScene::setVolume() {
	/* XXX: �����̓R���t�B�O�ԍ����n�[�h�R�[�f�B���O */
	using namespace sound;
	auto getvolume = [this] (unsigned index) -> double {
		int volperc = ((20 + prefstat[index]) % 21) * 5;
		return (double)volperc / 100.0;
	};
	for (unsigned i = IDs::BgmStart; i <= IDs::BgmEnd; i++)
		SetVolume(i, getvolume(5));
	for (unsigned i = IDs::SndStart; i <= IDs::SndEnd; i++)
		SetVolume(i, getvolume(4));
	for (unsigned i = IDs::VoxStart; i <= IDs::VoxEnd; i++)
		SetVolume(i, getvolume(4));
}

void PreferenceConfigScene::setActiveTextbox(int textBoxID) {
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		if (editBoxes[i])
			editBoxes[i]->deactivate();
	if (editBoxes[textBoxID])
		editBoxes[textBoxID]->activate();
}
int PreferenceConfigScene::getActiveTextbox() {
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		if ((editBoxes[i]) && (editBoxes[i]->is_Active()))
			return i;
	return -1;
}

void PreferenceConfigScene::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	int activeTxtBox = getActiveTextbox();
	if ((activeTxtBox >= 0) && editBoxes[activeTxtBox])
		editBoxes[activeTxtBox]->IMEvent(message, wParam, lParam);
}
void PreferenceConfigScene::KeyboardInput(WPARAM wParam, LPARAM lParam) {
	int activeTxtBox = getActiveTextbox();
	if ((buttonCursor == -1) && (menuCursor >= 0) && (editBoxes[menuCursor]) && (activeTxtBox != menuCursor) &&
		((wParam == CHARDAT_CURSOR_ENTER) || (wParam == 'z') || (wParam == 'Z') || (wParam == ' ') || (wParam == '\t'))) {
			sound::Play(sound::IDs::sndClick);
			setActiveTextbox(menuCursor);
			myTimer.skipTo(0);
	} else if ((activeTxtBox >= 0) && editBoxes[activeTxtBox]) {
		if ((wParam == CHARDAT_CURSOR_ENTER) || (wParam == CHARDAT_CURSOR_ESCAPE) || (wParam == '\t')) {
			sound::Play(sound::IDs::sndClick);
			editBoxes[activeTxtBox]->deactivate();
			myTimer.skipTo(0);
		} else {
			editBoxes[activeTxtBox]->KeyboardInput(wParam, lParam);
		}
	}
}
void PreferenceConfigScene::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	if (getActiveTextbox() == -1)
		ConfigMenuProto::KeyboardInput(od);
}

}
