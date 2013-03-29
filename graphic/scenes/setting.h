#pragma once

#include "confmenu.h"
//#include "../text.h"
//#include "../../common/strcode.h"
//#include "../rule.h"
//#include "../../common/nmrules.h"
#include "../widgets/button.h"

namespace mihajong_graphic {

class PreferenceConfigScene : public ConfigMenuProto {
private:
	const CodeConv::tstring Caption() {return _T("ä¬ã´ê›íË");}
	static const unsigned buttons = 4u;
	void itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent);
	void redrawItems();
	//int rulestat[RULESIZE];
	//char RuleConf[RULE_LINES][RULE_IN_LINE + 4];
	void saveRule();
	void ShowPageCaption();
	void ShowMessageBelow();
	void BtnEvent_OK_Down();
	void BtnEvent_OK_Up();
	void BtnEvent_Cancel_Down();
	void BtnEvent_Content_Roll_Up();
	void BtnEvent_Content_Roll_Down();
	void BtnEvent_Content_Item_Prev(unsigned short val);
	void BtnEvent_Content_Item_Next(unsigned short val);
	void BtnEvent_Content_Page_Prev();
	void BtnEvent_Content_Page_Next();
	void BtnEvent_Button_Prev();
	void BtnEvent_Button_Next();
public:
	PreferenceConfigScene(ScreenManipulator* const manipulator);
	virtual ~PreferenceConfigScene();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
};

}
