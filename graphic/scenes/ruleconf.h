#ifndef GRAPHIC_SCENES_RULECONF_H
#define GRAPHIC_SCENES_RULECONF_H

#include "../scrmanip.h"
#include "proto.h"

class RuleConfigScene : public Scene {
public:
	RuleConfigScene(ScreenManipulator* const manipulator);
	virtual ~RuleConfigScene();
	void Render();
};

#endif
