#include "nakibtn.h"

#include "../../geometry.h"

namespace mihajong_graphic {

const GameTableScreen::ButtonReconst::BtnData
	GameTableScreen::ButtonReconst::buttonDat[2][GameTableScreen::ButtonReconst::btnMAXIMUM] = {
		{
			{_T("���`�["),   5 + 117 * 0, Geometry::BaseSize - 40, 0xffccff66},
			{_T("�ƃ`�["),   5 + 117 * 1, Geometry::BaseSize - 40, 0xff99ff99},
			{_T("�E�`�["),   5 + 117 * 2, Geometry::BaseSize - 40, 0xff66ff99},
			{_T("�|��"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("�J��"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("�p�X"),     5 + 117 * 5, Geometry::BaseSize - 40, 0xffcccccc},
			{_T("����"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffcc66},
		}, {
			{_T(""),         5 + 117 * 0, Geometry::BaseSize - 40, 0xff666666},
			{_T(""),         5 + 117 * 1, Geometry::BaseSize - 40, 0xff666666},
			{_T("����v"), 5 + 117 * 2, Geometry::BaseSize - 40, 0xff99ffff},
			{_T("�Ԕv"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("�J��"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("���[�`"),   5 + 117 * 5, Geometry::BaseSize - 40, 0xffff66ff},
			{_T("�c��"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffffff},
		},
};

void GameTableScreen::ButtonReconst::Render() {
	buttons->Render();
	buttons->Render();
}

void GameTableScreen::ButtonReconst::ChangeButtonSet(ButtonSet btnSet) {
	for (unsigned i = 0; i < btnMAXIMUM; ++i) {
		buttons->setButton(i, ButtonPic::clear,
			buttonDat[btnSet][i].x * Geometry::WindowScale(), buttonDat[btnSet][i].y * Geometry::WindowScale(),
			117 * Geometry::WindowScale(), 36 * Geometry::WindowScale(),
			buttonDat[btnSet][i].color, buttonDat[btnSet][i].label);
		const Region nullRegion = {0, 0, -1, -1};
		if (caller->regions.size() <= (i + 20))
			caller->regions.resize(21 + i, nullRegion);
		caller->regions[i + 20].Left   = buttonDat[btnSet][i].x;
		caller->regions[i + 20].Top    = buttonDat[btnSet][i].y + 117;
		caller->regions[i + 20].Right  = buttonDat[btnSet][i].x;
		caller->regions[i + 20].Bottom = buttonDat[btnSet][i].y + 36;
	}
}

GameTableScreen::ButtonReconst::ButtonReconst(GameTableScreen* parent) {
	caller = parent;
	buttons = new ButtonPic(caller->caller->getDevice());
	ChangeButtonSet(btnSetNormal);
}

GameTableScreen::ButtonReconst::~ButtonReconst() {
	delete buttons;
}

}
