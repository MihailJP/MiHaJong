#ifndef GRAPHIC_SCENES_SYSSCR_H
#define GRAPHIC_SCENES_SYSSCR_H

#include "proto.h"
#include <array>
#include <cstdint>
#include "../text.h"
#include "../../mjcore/strcode.h"

namespace mihajong_graphic {

class SystemScreen : public Scene {
protected:
	void clearWithGameTypeColor();
	FILETIME startTime;
	static const unsigned timePerFrame = 166667u;
	uint64_t elapsed();
	TextRenderer* myTextRenderer;
public:
	SystemScreen(ScreenManipulator* const manipulator);
	virtual ~SystemScreen();
};

}
#endif
