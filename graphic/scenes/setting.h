#pragma once

#include "confmenu.h"
//#include "../text.h"
//#include "../../common/strcode.h"
#include "../rule.h"
#include "../../common/nmrules.h"
#include "../widgets/button.h"

namespace mihajong_graphic {

class PreferenceConfigScene : public ConfigMenuProto {
private:
	const CodeConv::tstring Caption() {return _T("ä¬ã´ê›íË");}
	const unsigned itemsPerPage() {return RULES_IN_PAGE;}
	const unsigned numberOfItems() {return PREFERENCE_ITEMS;}
	const unsigned numberOfButtons() {return 2u;}
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
public:
	PreferenceConfigScene(ScreenManipulator* const manipulator);
	virtual ~PreferenceConfigScene();
};

}
