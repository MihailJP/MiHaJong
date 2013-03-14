#pragma once

#include "sysscr.h"
#include "../text.h"
#include "../../common/strcode.h"
#include "../rule.h"
#include "../../common/nmrules.h"
#include "../button.h"

namespace mihajong_graphic {

class RuleConfigScene : public SystemScreen {
private:
	static const unsigned btnRegionStart = 50u;
	static const unsigned buttons = 4u;
	void itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent);
	void redrawItems();
	signed short int menuCursor;
	signed short int buttonCursor;
	signed short int buttonDown;
	int rulestat[RULESIZE];
	char RuleConf[RULE_LINES][RULE_IN_LINE + 4];
	ButtonPic* myButtonPic;
	void saveRule();
	void CreateButton(unsigned btnID, int X, int Y, unsigned Width, unsigned Height, const CodeConv::tstring& caption);
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
	RuleConfigScene(ScreenManipulator* const manipulator);
	virtual ~RuleConfigScene();
	void Render();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
};

}
