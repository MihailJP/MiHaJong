#ifndef GRAPHIC_SCENES_RULECONF_H
#define GRAPHIC_SCENES_RULECONF_H

#include "../scrmanip.h"
#include "proto.h"
#include "../text.h"

namespace mihajong_graphic {

class RuleConfigScene : public Scene {
private:
	TextRenderer* textRenderer;
	void itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent);
	void redrawItems();
	signed short int menuCursor;
public:
	RuleConfigScene(ScreenManipulator* const manipulator);
	virtual ~RuleConfigScene();
	void Render();
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
};

}
#endif
