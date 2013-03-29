#include "confmenu.h"

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
	if (regions.size() <= (btnID + btnRegionStart + 1)) {
		Region nullRegion = {0, 0, -1, -1};
		regions.resize(btnID + btnRegionStart + 1, Region(nullRegion));
	}
	regions[btnID + btnRegionStart].Left = X; regions[btnID + btnRegionStart].Top = Y; 
	regions[btnID + btnRegionStart].Right = X + Width;
	regions[btnID + btnRegionStart].Bottom = Y + Height; 
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

}
