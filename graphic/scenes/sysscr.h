#pragma once

#include "proto.h"
#include <array>
#include <cstdint>
#include "../text.h"
#include "../../common/strcode.h"
#include "../timer.h"

namespace mihajong_graphic {

class SystemScreen : public Scene {
protected:
	void clearWithGameTypeColor();
	Timer myTimer;
	static const unsigned timePerFrame = 16667u;
	TextRenderer* myTextRenderer;
	unsigned strwidth(const std::wstring& str);
	unsigned strwidth(const std::string& str);
public:
	SystemScreen(ScreenManipulator* const manipulator);
	SystemScreen(const SystemScreen&) = delete; // Delete unexpected copy constructor
	SystemScreen& operator= (const SystemScreen&) = delete; // Delete unexpected assign operator
	virtual ~SystemScreen();
};

}
