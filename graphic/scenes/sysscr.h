#pragma once

#include "proto.h"
#include <array>
#include <cstdint>
#include <map>
#include "../text.h"
#include "../../common/strcode.h"
#include "../timer.h"

namespace mihajong_graphic {

class SystemScreen : public Scene {
protected:
	void clearWithGameTypeColor();
	Timer myTimer;
	static constexpr unsigned timePerFrame = 16667u;
	TextRenderer* myTextRenderer;
	unsigned strwidth(const std::wstring& str);
	unsigned strwidth(const std::string& str);
	class Background;
	class TitleBackground;
public:
	SystemScreen(ScreenManipulator* const manipulator);
	SystemScreen(const SystemScreen&) = delete; // Delete unexpected copy constructor
	SystemScreen& operator= (const SystemScreen&) = delete; // Delete unexpected assign operator
	virtual ~SystemScreen();
};

class SystemScreen::Background {
private:
	static TexturePtr texture;
	DevicePtr myDevice;
protected:
	SystemScreen* caller;
	static std::map<ScreenManipulator*, Timer> timers;
	Timer& timer() {return timers[caller->caller];}
	static constexpr unsigned int PictureWidth = 3072u;
	static constexpr unsigned int PictureHeight = 2048u;
	static constexpr unsigned int MainPosY = 300u;
public:
	static void LoadTexture(DevicePtr device);
	static void DisposeTexture();
	Background(SystemScreen* const);
	Background(const Background&) = delete; // Delete unexpected copy constructor
	Background& operator= (const Background&) = delete; // Delete unexpected assign operator
	~Background();
	virtual RECT getRect();
	virtual void show();
	void skipIntoLoop();
};

class SystemScreen::TitleBackground : public SystemScreen::Background {
public:
	TitleBackground(SystemScreen* const, bool cutIn = false);
	TitleBackground(const TitleBackground&) = delete; // Delete unexpected copy constructor
	TitleBackground& operator= (const TitleBackground&) = delete; // Delete unexpected assign operator
	~TitleBackground();
	virtual RECT getRect() override;
};

}
