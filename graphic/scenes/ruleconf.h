#ifndef GRAPHIC_SCENES_RULECONF_H
#define GRAPHIC_SCENES_RULECONF_H

#include "sysscr.h"
#include "../text.h"
#include "../../mjcore/strcode.h"
#include "../rule.h"
#include "../../mjcore/nmrules.h"

namespace mihajong_graphic {

class RuleConfigScene : public SystemScreen {
private:
	void itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent);
	void redrawItems();
	signed short int menuCursor;
	int rulestat[RULESIZE];
public:
	RuleConfigScene(ScreenManipulator* const manipulator);
	virtual ~RuleConfigScene();
	void Render();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
};

}
#endif
