#ifndef GRAPHIC_SCENES_RULECONF_H
#define GRAPHIC_SCENES_RULECONF_H

#include "../scrmanip.h"
#include "proto.h"
#include "../text.h"

class RuleConfigScene : public Scene {
private:
	TextRenderer* textRenderer;
public:
	RuleConfigScene(ScreenManipulator* const manipulator);
	virtual ~RuleConfigScene();
	void Render();
};

#endif
