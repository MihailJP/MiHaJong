#pragma once

#include "confmenu.h"
#include "../text.h"
#include "../../common/strcode.h"
#include "../rule.h"
#include "../../common/nmrules.h"
#include "../widgets/button.h"

namespace mihajong_graphic {

class RuleConfigScene : public ConfigMenuProto {
private:
	const CodeConv::tstring Caption() {return _T("ÉãÅ[Éãê›íË");}
	const unsigned itemsPerPage() {return RULES_IN_PAGE;}
	const unsigned numberOfItems() {return RULESIZE;}
	const unsigned numberOfButtons() {return 4u;}
	void itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent);
	void redrawItems();
	int rulestat[RULESIZE];
	char RuleConf[RULE_LINES][RULE_IN_LINE + 4];
	void saveRule();
	void ShowPageCaption();
	void ShowMessageBelow();
	void BtnEvent_OK_Up();
	void BtnEvent_Content_Roll_Up();
	void BtnEvent_Content_Roll_Down();
public:
	RuleConfigScene(ScreenManipulator* const manipulator);
	virtual ~RuleConfigScene();
};

}
