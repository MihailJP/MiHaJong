#ifndef GRAPHIC_SCENES_TITLE_H
#define GRAPHIC_SCENES_TITLE_H

#include "proto.h"

class TitleScreen : public Scene {
public:
	TitleScreen(ScreenManipulator* const manipulator);
	~TitleScreen();
	void Render();
};

#endif
