#include "setting.h"
#include "../event.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../extchar.h"

namespace mihajong_graphic {

PreferenceConfigScene::PreferenceConfigScene(ScreenManipulator* const manipulator) : ConfigMenuProto(manipulator) {
	for (unsigned short i = 0; i < PREFERENCE_ITEMS; i++)
		prefstat[i] = rules::getPreference(i);
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		editBoxes[i] = nullptr;
	CreateButton(0, 1240, 1000, 156, 48, _T("Çn Çj"));
	CreateButton(1, 1060, 1000, 156, 48, _T("CANCEL"));
	redrawItems();
}

PreferenceConfigScene::~PreferenceConfigScene() {
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		if (editBoxes[i])
			delete editBoxes[i];
}

void PreferenceConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// çÄñ⁄Çï\é¶
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // ÉAÉXî‰Å~0.75(â°ïùí≤êÆóp)
	const unsigned ItemNum = (menuCursor / RULES_IN_PAGE * RULES_IN_PAGE) + prmID;
	unsigned itmNameCols = strwidth(prmName); // åÖêî(ì˙ñ{åÍÇÕ2åÖ)
	D3DCOLOR baseColor = ((prmContent == _T("ÇmÅ^Ç`")) || ((prmContent.empty()) && (rules::getPreferenceInputSize(ItemNum) == 0))) ? 0x00bfbfbf : 0x00ffffff;
	D3DCOLOR menuColor = ((menuCursor % RULES_IN_PAGE == prmID) && (buttonCursor == -1)) ? 0xff000000 : 0x7f000000;
	const int xPos = (prmID / 20 * 720 + 50), yPos = 135 + (prmID % 20) * 40;
	myTextRenderer->NewText(prmID * 3, prmName,
		xPos * WidthRate, yPos, 1.0f,
		WidthRate * ((itmNameCols <= 8) ? 1.0f : 8.0f / (float)itmNameCols),
		menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 1, _T(":"),
		(xPos + 144) * WidthRate, yPos, 1.0, WidthRate, menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 2, prmContent,
		(xPos + 162) * WidthRate, yPos, 1.0, WidthRate, menuColor | baseColor);
	if (regions.size() <= prmID) {
		Region nullRegion = {0, 0, -1, -1};
		regions.resize(prmID + 1, Region(nullRegion));
	}
	regions[prmID].Left = (prmID / 20 * 720 + 50); regions[prmID].Top = 135 + (prmID % 20) * 40;
	regions[prmID].Right = (prmID / 20 * 720 + 670); regions[prmID].Bottom = regions[prmID].Top + 35;
	if (rules::getPreferenceInputSize(ItemNum)) { // ÉGÉfÉBÉbÉgÉ{ÉbÉNÉX
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
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // ÉAÉXî‰Å~0.75(â°ïùí≤êÆóp)
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
}

void PreferenceConfigScene::Render() {
	ConfigMenuProto::Render();
	for (unsigned short i = 0; i < RULES_IN_PAGE; i++)
		if (editBoxes[i]) editBoxes[i]->Render();
}

void PreferenceConfigScene::savePreference() {
	std::memset(&PrefConf[0][0], 0, sizeof(PrefConf));
	for (unsigned i = 0; i < PREFERENCE_ITEMS; i++) {
		TCHAR preftxt[128]; rules::getPreferenceTxt(preftxt, 128, i, 0);
		if ((CodeConv::tstring(preftxt) == _T("")) || (CodeConv::tstring(preftxt) == _T("ÇmÅ^Ç`")))
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
	setActiveTextbox(menuCursor);
	redrawItems();
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
	if ((menuCursor >= 0) && (editBoxes[menuCursor]) && (activeTxtBox != menuCursor) &&
		((wParam == CHARDAT_CURSOR_ENTER) || (wParam == 'z') || (wParam == 'Z') || (wParam == ' ') || (wParam == '\t'))) {
			sound::Play(sound::IDs::sndClick);
			setActiveTextbox(menuCursor);
	} else if ((activeTxtBox >= 0) && editBoxes[activeTxtBox]) {
		if ((wParam == CHARDAT_CURSOR_ENTER) || (wParam == CHARDAT_CURSOR_ESCAPE) || (wParam == '\t')) {
			sound::Play(sound::IDs::sndClick);
			editBoxes[activeTxtBox]->deactivate();
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
