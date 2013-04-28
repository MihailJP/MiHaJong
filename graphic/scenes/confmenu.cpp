#include "confmenu.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../../common/version.h"
#include "../event.h"
#include <iomanip>

namespace mihajong_graphic {

ConfigMenuProto::ConfigMenuProto(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	myButtonPic = new ButtonPic(manipulator->getDevice());
	menuCursor = 0; buttonCursor = -1; buttonDown = -1;
}

ConfigMenuProto::~ConfigMenuProto() {
	delete myButtonPic;
}

void ConfigMenuProto::CreateButton(unsigned btnID, int X, int Y, unsigned Width, unsigned Height, const CodeConv::tstring& caption) {
	const float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // アス比×0.75(横幅調整用)
	myButtonPic->setButton(btnID, ButtonPic::clear,
		X * (WidthRate * Geometry::WindowScale()), Y * Geometry::WindowScale(),
		Width * (WidthRate * Geometry::WindowScale()), Height * Geometry::WindowScale(),
		0xffffffff, caption, true);
	setRegion(btnID + btnRegionStart, X, Y, X + Width, Y + Height);
}

void ConfigMenuProto::Render() {
	clearWithGameTypeColor(); // バッファクリア
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // アス比×0.75(横幅調整用)
	{
		myTextRenderer->NewText(123, Caption(), 540 * WidthRate, 25, 2.0f, WidthRate, 0xffffffff);
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
	if ((menuCursor += val) >= numberOfItems()) menuCursor = numberOfItems() - 1;
	myTimer.skipTo(0); redrawItems();
}
void ConfigMenuProto::BtnEvent_Content_Page_Prev() {
	sound::Play(sound::IDs::sndClick);
	if ((menuCursor -= itemsPerPage()) < 0) menuCursor += itemsPerPage();
	myTimer.skipTo(0); redrawItems();
}
void ConfigMenuProto::BtnEvent_Content_Page_Next() {
	sound::Play(sound::IDs::sndClick);
	if ((menuCursor += itemsPerPage()) >= numberOfItems()) menuCursor -= itemsPerPage();
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
	if ((++buttonCursor) >= numberOfButtons()) buttonCursor = 0;
	for (unsigned i = 0; i < numberOfButtons(); i++)
		myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
	myTimer.skipTo(0); redrawItems();
}

void ConfigMenuProto::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	switch (od->dwOfs) {
	case DIK_UP: case DIK_K: // 前の項目
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(1);
		break;
	case DIK_DOWN: case DIK_J: // 次の項目
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(1);
		break;
	case DIK_LEFT: case DIK_H: // 前の選択肢
		if (od->dwData) {
			if (buttonCursor == -1)
				BtnEvent_Content_Roll_Down();
			else
				BtnEvent_Button_Next();
		}
		break;
	case DIK_RIGHT: case DIK_L: // 次の選択肢
		if (od->dwData) {
			if (buttonCursor == -1)
				BtnEvent_Content_Roll_Up();
			else
				BtnEvent_Button_Prev();
		}
		break;
	case DIK_HOME: // 前のカラム
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(20);
		break;
	case DIK_END: // 次のカラム
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(20);
		break;
	case DIK_PRIOR: // 前のページ
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Prev(itemsPerPage());
		break;
	case DIK_NEXT: // 次のページ
		if ((od->dwData) && (buttonCursor == -1))
			BtnEvent_Content_Item_Next(itemsPerPage());
		break;
	case DIK_ESCAPE: case DIK_X: // キャンセルキー
		if (od->dwData) BtnEvent_Cancel_Down();
		break;
	case DIK_RETURN: case DIK_SPACE: case DIK_Z: // 決定キー
		if (od->dwData) BtnEvent_OK_Down();
		else BtnEvent_OK_Up();
		break;
	}
}

void ConfigMenuProto::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const int scaledX = (int)((float)X / Geometry::WindowScale() / ((float)Geometry::WindowWidth * 0.75f / (float)Geometry::WindowHeight));
	const int scaledY = (int)((float)Y / Geometry::WindowScale());
	const int region = whichRegion(scaledX, scaledY);
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
		if ((region >= 0) && (region <= (itemsPerPage() - 1))) {
			if ((region != menuCursor % itemsPerPage()) || (buttonCursor != -1)) {
				sound::Play(sound::IDs::sndCursor);
				menuCursor = menuCursor / itemsPerPage() * itemsPerPage() + region;
				buttonCursor = -1;
				for (unsigned i = 0; i < numberOfButtons(); i++) myButtonPic->setButton(i, ButtonPic::clear);
				myTimer.skipTo(0); redrawItems();
			}
		}
		else if ((region >= btnRegionStart) && (region <= (btnRegionStart + numberOfButtons() - 1))) {
			if (region != (buttonCursor + btnRegionStart)) {
				sound::Play(sound::IDs::sndCursor);
				buttonCursor = region - btnRegionStart;
				for (unsigned i = 0; i < numberOfButtons(); i++)
					myButtonPic->setButton(i, (i == buttonCursor) ? ButtonPic::raised : ButtonPic::clear);
				myTimer.skipTo(0); redrawItems();
			}
		}
	};
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
		setcursor();
		break;
	case DIMOFS_Z: // ホイールの操作
		if ((region >= 0) && (region <= (itemsPerPage() - 1))) {
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
		if ((od->dwData) && (region >= 0) && (region <= (itemsPerPage() - 1))) {
			setcursor();
			BtnEvent_Content_Roll_Up();
		}
		else if ((region >= btnRegionStart) && (region < (btnRegionStart + numberOfButtons()))) {
			setcursor();
			if (od->dwData) BtnEvent_OK_Down();
			else BtnEvent_OK_Up();
		}
		break;
	}
}

CodeConv::tstring ConfigMenuProto::verInfoText() {
	CodeConv::tostringstream o; SYSTEMTIME Zeit; GetLocalTime(&Zeit);
	o << _T("MiHaJong version ") _T(MIHAJONG_VER) _T(" / 現在日時 ") <<
		std::setw(4) << Zeit.wYear << _T("年") <<
		std::setw(2) << Zeit.wMonth << _T("月") <<
		std::setw(2) << Zeit.wDay << _T("日 ") <<
		((Zeit.wHour / 12 == 0) ? _T("午前") : _T("午後")) <<
		std::setw(2) << (Zeit.wHour % 12) << _T("時") <<
		std::setw(2) << std::setfill(_T('0')) << Zeit.wMinute << _T("分");
	return o.str();
}

}
