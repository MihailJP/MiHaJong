#pragma once

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
	void skipto(unsigned frames);
	unsigned strwidth(const std::wstring& str);
	unsigned strwidth(const std::string& str);
public:
	SystemScreen(ScreenManipulator* const manipulator);
	virtual ~SystemScreen();
};

}
