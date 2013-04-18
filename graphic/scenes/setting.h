#pragma once

#include "confmenu.h"
#include <array>
#include "../rule.h"
#include "../../common/nmrules.h"
#include "../widgets/button.h"
#include "../widgets/editbox.h"

namespace mihajong_graphic {

class PreferenceConfigScene : public ConfigMenuProto {
private:
	const CodeConv::tstring Caption() {return _T("ä¬ã´ê›íË");}
	const unsigned itemsPerPage() {return RULES_IN_PAGE;}
	const unsigned numberOfItems() {return PREFERENCE_ITEMS;}
	const unsigned numberOfButtons() {return 2u;}
private:
	std::array<EditBox*, RULES_IN_PAGE> editBoxes;
private:
	void itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent);
	void redrawItems();
	int prefstat[PREFERENCE_ITEMS];
	char PrefConf[PREFERENCE_LINES][RULE_IN_LINE + 4];
	void ShowPageCaption();
	void ShowMessageBelow();
	void savePreference();
	void BtnEvent_OK_Up();
	void BtnEvent_Content_Roll_Up();
	void BtnEvent_Content_Roll_Down();
private:
	void setActiveTextbox(int textBoxID);
	int getActiveTextbox();
private:
	void setVolume();
public:
	PreferenceConfigScene(ScreenManipulator* const manipulator);
	virtual ~PreferenceConfigScene();
	void Render();
#ifdef _WIN32
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void KeyboardInput(WPARAM wParam, LPARAM lParam);
	void KeyboardInput(LPDIDEVICEOBJECTDATA od);
#else /*_WIN32*/
	/* TODO: ñ¢é¿ëï */
#endif /*_WIN32*/
};

}
