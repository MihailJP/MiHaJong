#include "nakibtn.h"

#include "../../geometry.h"

namespace mihajong_graphic {

const GameTableScreen::ButtonReconst::BtnData
	GameTableScreen::ButtonReconst::buttonDat[2][GameTableScreen::ButtonReconst::btnMAXIMUM] = {
		{
			{_T("左チー"),   5 + 117 * 0, Geometry::BaseSize - 40, 0xffccff66},
			{_T("嵌チー"),   5 + 117 * 1, Geometry::BaseSize - 40, 0xff99ff99},
			{_T("右チー"),   5 + 117 * 2, Geometry::BaseSize - 40, 0xff66ff99},
			{_T("ポン"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("カン"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T(""),         5 + 117 * 5, Geometry::BaseSize - 40, 0xff666666},
			{_T("ロン"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffcc66},
			{_T("パス"),     950        , Geometry::BaseSize - 40, 0xffffffff},
		}, {
			{_T(""),         5 + 117 * 0, Geometry::BaseSize - 40, 0xff666666},
			{_T(""),         5 + 117 * 1, Geometry::BaseSize - 40, 0xff666666},
			{_T("九種九牌"), 5 + 117 * 2, Geometry::BaseSize - 40, 0xff99ffff},
			{_T("花牌"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("カン"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("リーチ"),   5 + 117 * 5, Geometry::BaseSize - 40, 0xffff66ff},
			{_T("ツモ"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffffff},
			{_T("パス"),     950        , Geometry::BaseSize - 40, 0xffffffff},
		},
};

void GameTableScreen::ButtonReconst::Render() {
	buttons->Render();
	buttons->Render();
}

GameTableScreen::ButtonReconst::ButtonReconst(GameTableScreen* parent) {
	caller = parent;
	buttons = new ButtonPic(caller->caller->getDevice());
	for (unsigned i = 0; i < btnMAXIMUM; ++i) {
		buttons->setButton(i, ButtonPic::clear,
			buttonDat[0][i].x * Geometry::WindowScale(), buttonDat[0][i].y * Geometry::WindowScale(),
			117 * Geometry::WindowScale(), 36 * Geometry::WindowScale(),
			buttonDat[0][i].color, buttonDat[0][i].label);
		const Region nullRegion = {0, 0, -1, -1};
		if (caller->regions.size() <= (i + 20))
			caller->regions.resize(21 + i, nullRegion);
		caller->regions[i + 20].Left   = buttonDat[0][i].x;
		caller->regions[i + 20].Top    = buttonDat[0][i].y + 117;
		caller->regions[i + 20].Right  = buttonDat[0][i].x;
		caller->regions[i + 20].Bottom = buttonDat[0][i].y + 36;
	}
}

GameTableScreen::ButtonReconst::~ButtonReconst() {
	delete buttons;
}

}
