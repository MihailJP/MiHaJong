#pragma once

#include "sysscr.h"
#include "../widgets/button.h"
#include "../../common/strcode.h"

namespace mihajong_graphic {

class ConfigMenuProto : public SystemScreen {
protected:
	static const unsigned btnRegionStart = 50u;
	virtual const CodeConv::tstring Caption() = 0;
	virtual const unsigned itemsPerPage() = 0;
	virtual const unsigned numberOfItems() = 0;
	virtual const unsigned numberOfButtons() = 0;
protected:
	ButtonPic* myButtonPic;
	void CreateButton(unsigned btnID, int X, int Y, unsigned Width, unsigned Height, const CodeConv::tstring& caption);
	signed short int buttonCursor;
	signed short int buttonDown;
protected:
	virtual void BtnEvent_OK_Down();
	virtual void BtnEvent_OK_Up() = 0;
	virtual void BtnEvent_Cancel_Down();
	virtual void BtnEvent_Content_Roll_Up() = 0;
	virtual void BtnEvent_Content_Roll_Down() = 0;
	virtual void BtnEvent_Content_Item_Prev(unsigned short val);
	virtual void BtnEvent_Content_Item_Next(unsigned short val);
	virtual void BtnEvent_Content_Page_Prev();
	virtual void BtnEvent_Content_Page_Next();
	virtual void BtnEvent_Button_Prev();
	virtual void BtnEvent_Button_Next();
protected:
	signed short int menuCursor;
#ifndef _WIN32
	bool menuInitFlag;
	virtual void objInit() = 0;
#endif /*_WIN32*/
	virtual void redrawItems() = 0;
	virtual void ShowPageCaption() = 0;
	virtual void ShowMessageBelow() = 0;
	virtual CodeConv::tstring verInfoText();
public:
	ConfigMenuProto(ScreenManipulator* const manipulator);
	ConfigMenuProto(const ConfigMenuProto&) = delete; // Delete unexpected copy constructor
	ConfigMenuProto& operator= (const ConfigMenuProto&) = delete; // Delete unexpected assign operator
	virtual ~ConfigMenuProto() = 0;
	virtual void Render();
#ifdef _WIN32
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
#else /*_WIN32*/
	virtual void KeyboardInput(const XEvent* od);
	virtual void MouseInput(const XEvent* od, int X, int Y);
#endif /*_WIN32*/
};

}
