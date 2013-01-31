#include "nakibtn.h"

namespace mihajong_graphic {

const GameTableScreen::ButtonReconst::BtnData
	GameTableScreen::ButtonReconst::buttonDat[GameTableScreen::ButtonReconst::btnMAXIMUM] = {
		{_T(""), 0, 0, 20, 20, 0xffffffff},
		{_T(""), 0, 0, 20, 20, 0xffffffff},
		{_T(""), 0, 0, 20, 20, 0xffffffff},
		{_T(""), 0, 0, 20, 20, 0xffffffff},
		{_T(""), 0, 0, 20, 20, 0xffffffff},
		{_T(""), 0, 0, 20, 20, 0xffffffff},
		{_T(""), 0, 0, 20, 20, 0xffffffff},
};

void GameTableScreen::ButtonReconst::Render() {
	buttons->Render();
}

GameTableScreen::ButtonReconst::ButtonReconst(GameTableScreen* parent) {
	caller = parent;
	buttons = new ButtonPic(caller->caller->getDevice());
	for (unsigned i = 0; i < btnMAXIMUM; ++i)
		buttons->setButton(i, ButtonPic::clear,
		buttonDat[i].x, buttonDat[i].y, buttonDat[i].width, buttonDat[i].height,
		buttonDat[i].color, buttonDat[i].label);
}

GameTableScreen::ButtonReconst::~ButtonReconst() {
	delete buttons;
}

}
