#include "confmenu.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../../common/version.h"
#include "../event.h"
#include <iomanip>
#ifndef _WIN32
#include "../keycode.h"
#endif /*_WIN32*/
#include <cassert>

namespace mihajong_graphic {

ConfigMenuProto::ConfigMenuProto(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	myButtonPic = new ButtonPic(manipulator->getDevice());
	menuCursor = 0; buttonCursor = -1; buttonDown = -1;
#ifndef _WIN32
	menuInitFlag = false;
#endif /*_WIN32*/
}

ConfigMenuProto::~ConfigMenuProto() {
	delete myButtonPic;
}

void ConfigMenuProto::CreateButton(unsigned btnID, int X, int Y, unsigned Width, unsigned Height, const CodeConv::tstring& caption) {
	myButtonPic->setButton(btnID, ButtonPic::clear,
		scaleX(X), scaleY(Y), scaleW(Width), scaleH(Height),
		0xffffffff, caption, true);
	setRegion(btnID + btnRegionStart, X, Y, X + Width, Y + Height);
}

void ConfigMenuProto::Render() {
	clearWithGameTypeColor(); // バッファクリア
#ifndef _WIN32
	if (!menuInitFlag) {
		objInit();
		menuInitFlag = true;
	}
#endif /*_WIN32*/
	{
		myTextRenderer->NewText(123, Caption(), adjX(540), 25, 2.0f, WidthRate(), 0xffffffff);
	}
	ShowMessageBelow();
	ShowPageCaption();
	myTextRenderer->Render();
	myButtonPic->Render();
}

void ConfigMenuProto::BtnEvent_OK_Down() {
	if (buttonCursor != -1) {
		sound::Play(sound::IDs::sndButton);
		buttonDown = buttonCursor;
		myButtonPic->setButton(buttonCursor, ButtonPic::sunken);
		redrawItems();
	}
}
void ConfigMenuProto::BtnEvent_Cancel_Down() {
	sound::Play(sound::IDs::sndCursor);
	if (buttonCursor == -1) {
		buttonCursor = 0;
	} else {
		buttonCursor = -1;
	}
	for (unsigned i = 0; i < numberOfButtons(); i++)
		myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	myTimer.skipTo(0); redrawItems();
}

void ConfigMenuProto::BtnEvent_Content_Item_Prev(unsigned short val) {
	sound::Play(sound::IDs::sndCursor);
	if ((menuCursor -= val) < 0) menuCursor = 0;
	myTimer.skipTo(0); redrawItems();
}
void ConfigMenuProto::BtnEvent_Content_Item_Next(unsigned short val) {
	sound::Play(sound::IDs::sndCursor);
	assert(numberOfItems() > 0);
	if (static_cast<unsigned>(menuCursor += val) >= numberOfItems()) menuCursor = numberOfItems() - 1;
	myTimer.skipTo(0); redrawItems();
}
void ConfigMenuProto::BtnEvent_Content_Page_Prev() {
	sound::Play(sound::IDs::sndClick);
	if ((menuCursor -= itemsPerPage()) < 0) menuCursor += itemsPerPage();
	myTimer.skipTo(0); redrawItems();
}
void ConfigMenuProto::BtnEvent_Content_Page_Next() {
	sound::Play(sound::IDs::sndClick);
	if (static_cast<unsigned>(menuCursor += itemsPerPage()) >= numberOfItems()) menuCursor -= itemsPerPage();
	myTimer.skipTo(0); redrawItems();
}

void ConfigMenuProto::BtnEvent_Button_Prev() {
	sound::Play(sound::IDs::sndCursor);
	if ((--buttonCursor) < 0) buttonCursor = numberOfButtons() - 1;
	for (unsigned i = 0; i < numberOfButtons(); i++)
		myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	myTimer.skipTo(0); redrawItems();
}
void ConfigMenuProto::BtnEvent_Button_Next() {
	sound::Play(sound::IDs::sndCursor);
	if (static_cast<unsigned>(++buttonCursor) >= numberOfButtons()) buttonCursor = 0;
	for (unsigned i = 0; i < numberOfButtons(); i++)
		myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	myTimer.skipTo(0); redrawItems();
}

#ifdef _WIN32
void ConfigMenuProto::KeyboardInput(LPDIDEVICEOBJECTDATA od)
#else /*_WIN32*/
void ConfigMenuProto::KeyboardInput(const XEvent* od)
#endif /*_WIN32*/
{
#ifdef _WIN32
	const bool keyDown = od->dwData;
	switch (od->dwOfs)
#else /*_WIN32*/
	constexpr bool keyDown = od->type == KeyPress;
	switch (od->xkey.keycode)
#endif /*_WIN32*/
	{
	case DIK_UP: case DIK_K: // 前の項目
		if (keyDown && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(1);
		break;
	case DIK_DOWN: case DIK_J: // 次の項目
		if (keyDown && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(1);
		break;
	case DIK_LEFT: case DIK_H: // 前の選択肢
		if (keyDown) {
			if (buttonCursor == -1)
				BtnEvent_Content_Roll_Down();
			else
				BtnEvent_Button_Next();
		}
		break;
	case DIK_RIGHT: case DIK_L: // 次の選択肢
		if (keyDown) {
			if (buttonCursor == -1)
				BtnEvent_Content_Roll_Up();
			else
				BtnEvent_Button_Prev();
		}
		break;
	case DIK_HOME: // 前のカラム
		if (keyDown && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(20);
		break;
	case DIK_END: // 次のカラム
		if (keyDown && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(20);
		break;
	case DIK_PRIOR: // 前のページ
		if (keyDown && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(itemsPerPage());
		break;
	case DIK_NEXT: // 次のページ
		if (keyDown && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(itemsPerPage());
		break;
	case DIK_ESCAPE: case DIK_X: // キャンセルキー
		if (keyDown) BtnEvent_Cancel_Down();
		break;
	case DIK_RETURN: case DIK_SPACE: case DIK_Z: // 決定キー
		if (keyDown) BtnEvent_OK_Down();
		else BtnEvent_OK_Up();
		break;
	}
}

#ifdef _WIN32
void ConfigMenuProto::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y)
#else /*_WIN32*/
void ConfigMenuProto::MouseInput(const XEvent* od, int X, int Y)
#endif /*_WIN32*/
{
	const int region = whichRegion(scaleInvX(X), scaleInvY(Y));
#if 0
	{
		CodeConv::tostringstream o;
		o << _T("(") << scaledX << _T(", ") << scaledY << _T(") ");
		if (region != -1) o << _T("Region ") << region;
		else o << _T("No Region");
		myTextRenderer->NewText(144, o.str(), 0, 1000);
	}
#endif
	auto setcursor = [&]() -> void {
		assert(itemsPerPage() > 0);
		if ((region >= 0) && (region <= (static_cast<int>(itemsPerPage()) - 1))) {
			if ((region != menuCursor % itemsPerPage()) || (buttonCursor != -1)) {
				sound::Play(sound::IDs::sndCursor);
				menuCursor = menuCursor / itemsPerPage() * itemsPerPage() + region;
				buttonCursor = -1;
				for (unsigned i = 0; i < numberOfButtons(); i++) myButtonPic->setButton(i, ButtonPic::clear);
				myTimer.skipTo(0); redrawItems();
			}
		}
		else if ((region >= static_cast<int>(btnRegionStart)) && (region <= static_cast<int>(btnRegionStart + numberOfButtons() - 1))) {
			if (region != (buttonCursor + btnRegionStart)) {
				sound::Play(sound::IDs::sndCursor);
				buttonCursor = region - btnRegionStart;
				for (unsigned i = 0; i < numberOfButtons(); i++)
					myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
				myTimer.skipTo(0); redrawItems();
			}
		}
	};
#ifdef _WIN32
	switch (od->dwOfs)
#else /*_WIN32*/
	switch (od->type)
#endif /*_WIN32*/
	{
#ifdef _WIN32
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
#else /*_WIN32*/
	case MotionNotify: // マウスカーソルを動かした場合
#endif /*_WIN32*/
		setcursor();
		break;
#ifdef _WIN32
	case DIMOFS_Z: // ホイールの操作
		assert(itemsPerPage() > 0);
		if ((region >= 0) && (region <= static_cast<int>(itemsPerPage() - 1))) {
			setcursor();
			if ((LONG)od->dwData > 0) BtnEvent_Content_Roll_Up();
			else if ((LONG)od->dwData < 0) BtnEvent_Content_Roll_Down();
		} else if (region == 40) {
			if ((LONG)od->dwData > 0)
				BtnEvent_Content_Page_Next();
			else if ((LONG)od->dwData < 0)
				BtnEvent_Content_Page_Prev();
		}
		break;
	case DIMOFS_BUTTON0: // 左クリック
		assert(itemsPerPage() > 0);
		if ((od->dwData) && (region >= 0) && (region <= static_cast<int>(itemsPerPage() - 1)))
#else /*_WIN32*/
	case ButtonPress: // マウスの左ボタン
		if ((od->xbutton.button == Button1) && (region >= 0) && (region <= (itemsPerPage() - 1)))
#endif /*_WIN32*/
		{
			setcursor();
			BtnEvent_Content_Roll_Up();
		}
#ifdef _WIN32
		else if ((region >= static_cast<int>(btnRegionStart)) && (region < static_cast<int>(btnRegionStart + numberOfButtons()))) {
			setcursor();
			if (od->dwData) BtnEvent_OK_Down();
			else BtnEvent_OK_Up();
		}
		break;
#else /*_WIN32*/
		else if ((od->xbutton.button == Button1) && (region >= btnRegionStart) && (region < (btnRegionStart + numberOfButtons()))) {
			setcursor();
			BtnEvent_OK_Down();
		} else if (od->xbutton.button == Button4) { // ホイールの操作（プラス方向）
			if ((region >= 0) && (region <= (itemsPerPage() - 1))) {
				setcursor();
				BtnEvent_Content_Roll_Up();
			} else if (region == 40) {
				BtnEvent_Content_Page_Prev();
			}
		} else if (od->xbutton.button == Button5) { // ホイールの操作（マイナス方向）
			if ((region >= 0) && (region <= (itemsPerPage() - 1))) {
				setcursor();
				BtnEvent_Content_Roll_Down();
			} else if (region == 40) {
				BtnEvent_Content_Page_Next();
			}
		}
		break;
	case ButtonRelease: // マウスの左ボタン
		if ((od->xbutton.button == Button1) && (region >= btnRegionStart) && (region < (btnRegionStart + numberOfButtons()))) {
			setcursor();
			BtnEvent_OK_Up();
		}
		break;
#endif /*_WIN32*/
	}
}

CodeConv::tstring ConfigMenuProto::verInfoText() {
#ifdef _WIN32
	CodeConv::tostringstream o; SYSTEMTIME Zeit; GetLocalTime(&Zeit);
	o << _T("MiHaJong version ") _T(MIHAJONG_VER) _T(" / 現在日時 ") <<
		std::setw(4) << Zeit.wYear << _T("年") <<
		std::setw(2) << Zeit.wMonth << _T("月") <<
		std::setw(2) << Zeit.wDay << _T("日 ") <<
		((Zeit.wHour / 12 == 0) ? _T("午前") : _T("午後")) <<
		std::setw(2) << (Zeit.wHour % 12) << _T("時") <<
		std::setw(2) << std::setfill(_T('0')) << Zeit.wMinute << _T("分");
	return o.str();
#else /*_WIN32*/
	CodeConv::tostringstream o;
	time_t Zeitzahl = time(nullptr); tm Zeit; = localtime_s(&Zeit, &Zeitzahl);
	o << _T("MiHaJong version ") _T(MIHAJONG_VER) _T(" / 現在日時 ") <<
		std::setw(4) << (Zeit.tm_year + 1900) << _T("年") <<
		std::setw(2) << (Zeit.tm_mon + 1) << _T("月") <<
		std::setw(2) << Zeit.tm_mday << _T("日 ") <<
		((Zeit.tm_hour / 12 == 0) ? _T("午前") : _T("午後")) <<
		std::setw(2) << (Zeit.tm_hour % 12) << _T("時") <<
		std::setw(2) << std::setfill(_T('0')) << Zeit.tm_min << _T("分");
	return o.str();
#endif /*_WIN32*/
}

}
