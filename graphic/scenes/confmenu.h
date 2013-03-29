#pragma once

#include "sysscr.h"
#include "../widgets/button.h"
#include "../../common/strcode.h"

namespace mihajong_graphic {

class ConfigMenuProto : public SystemScreen {
protected:
	static const unsigned btnRegionStart = 50u;
	virtual const CodeConv::tstring Caption() = 0;
protected:
	ButtonPic* myButtonPic;
	void CreateButton(unsigned btnID, int X, int Y, unsigned Width, unsigned Height, const CodeConv::tstring& caption);
	signed short int buttonCursor;
	signed short int buttonDown;
protected:
	signed short int menuCursor;
	virtual void redrawItems() = 0;
	virtual void ShowPageCaption() = 0;
	virtual void ShowMessageBelow() = 0;
public:
	ConfigMenuProto(ScreenManipulator* const manipulator);
	virtual ~ConfigMenuProto() = 0;
	virtual void Render();
};

}
