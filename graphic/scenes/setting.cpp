#include "setting.h"
#include "../event.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"

namespace mihajong_graphic {

PreferenceConfigScene::PreferenceConfigScene(ScreenManipulator* const manipulator) : ConfigMenuProto(manipulator) {
	for (unsigned short i = 0; i < PREFERENCE_ITEMS; i++)
		prefstat[i] = rules::getPreference(i);
	CreateButton(0, 1240, 1000, 156, 48, _T("‚n ‚j"));
	CreateButton(1, 1060, 1000, 156, 48, _T("CANCEL"));
	redrawItems();
}

PreferenceConfigScene::~PreferenceConfigScene() {
}

void PreferenceConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// €–Ú‚ð•\Ž¦
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // ƒAƒX”ä~0.75(‰¡•’²®—p)
	unsigned itmNameCols = strwidth(prmName); // Œ…”(“ú–{Œê‚Í2Œ…)
	D3DCOLOR baseColor = ((prmContent == _T("‚m^‚`")) || (prmContent.empty())) ? 0x00bfbfbf : 0x00ffffff;
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

void PreferenceConfigScene::redrawItems() {
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // ƒAƒX”ä~0.75(‰¡•’²®—p)
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

void PreferenceConfigScene::savePreference() {
	std::memset(&PrefConf[0][0], 0, sizeof(PrefConf));
	for (unsigned i = 0; i < PREFERENCE_ITEMS; i++) {
		TCHAR preftxt[128]; rules::getPreferenceTxt(preftxt, 128, i, 0);
		if ((CodeConv::tstring(preftxt) == _T("")) || (CodeConv::tstring(preftxt) == _T("‚m^‚`")))
			PrefConf[i / RULE_IN_LINE][i % RULE_IN_LINE] = '-';
		else
			PrefConf[i / RULE_IN_LINE][i % RULE_IN_LINE] = rules::digit[prefstat[i]];
	}
	const char* prefLine[PREFERENCE_LINES];
	for (unsigned i = 0; i < PREFERENCE_LINES; i++) prefLine[i] = PrefConf[i];
	rules::storePref(prefLine);
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
	redrawItems();
}
void PreferenceConfigScene::BtnEvent_Content_Roll_Down() {
	sound::Play(sound::IDs::sndClick);
	while (true) {
		if ((--prefstat[menuCursor]) < 0) prefstat[menuCursor] = rules::getPreferenceSize(menuCursor) - 1;
		TCHAR menuitem[128]; rules::getPreferenceTxt(menuitem, 128, menuCursor, prefstat[menuCursor]);
		if (CodeConv::tstring(menuitem) != CodeConv::tstring(_T(">>>"))) break;
	}
	redrawItems();
}

}
