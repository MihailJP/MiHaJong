#ifndef GRAPHIC_SCENES_TEST_H
#define GRAPHIC_SCENES_TEST_H

#include "proto.h"

class TestScene : public Scene {
private:
	LPD3DXFONT pFont;
public:
	TestScene(ScreenManipulator* const manipulator);
	~TestScene();
	void Render();
};

#endif
