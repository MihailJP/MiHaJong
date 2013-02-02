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

void GameTableScreen::ButtonReconst::reconstruct(ButtonID buttonID) {
	union Color { // �Ƃ肠���������ɉ��u��
		D3DCOLOR rgbaAsOneValue;
		/* ���ʂ̓X���[���G���f�B�A���̂ݑΉ� */
		struct {unsigned b:8, g:8, r:8, a:8;} rgbaAsStruct;
	};
	static_assert(sizeof (Color) == 4, "union Color is not 32-bit long");
	Color btnColor; btnColor.rgbaAsOneValue = buttonDat[currentButtonSet][buttonID].color;
	if (!buttonEnabled[buttonID]) { // �Ó]����
		btnColor.rgbaAsStruct.r /= 3;
		btnColor.rgbaAsStruct.g /= 3;
		btnColor.rgbaAsStruct.b /= 3;
	}
	buttons->setButton(buttonID, ButtonPic::clear,
		buttonDat[currentButtonSet][buttonID].x * Geometry::WindowScale(),
		buttonDat[currentButtonSet][buttonID].y * Geometry::WindowScale(),
		117 * Geometry::WindowScale(), 36 * Geometry::WindowScale(),
		btnColor.rgbaAsOneValue, buttonDat[currentButtonSet][buttonID].label);
	const Region nullRegion = {0, 0, -1, -1};
	if (caller->regions.size() <= (buttonID + 20))
		caller->regions.resize(21 + buttonID, nullRegion);
	caller->regions[buttonID + 20].Left   = buttonDat[currentButtonSet][buttonID].x;
	caller->regions[buttonID + 20].Top    = buttonDat[currentButtonSet][buttonID].y + 117;
	caller->regions[buttonID + 20].Right  = buttonDat[currentButtonSet][buttonID].x;
	caller->regions[buttonID + 20].Bottom = buttonDat[currentButtonSet][buttonID].y + 36;
}

void GameTableScreen::ButtonReconst::ChangeButtonSet(ButtonSet btnSet) {
	currentButtonSet = btnSet;
	for (unsigned i = 0; i < btnMAXIMUM; ++i)
		reconstruct((ButtonID)i);
}

void GameTableScreen::ButtonReconst::enable(ButtonID buttonID) {
	buttonEnabled[buttonID] = true; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::disable(ButtonID buttonID) {
	buttonEnabled[buttonID] = false; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::enable(const std::bitset<btnMAXIMUM>& flagset) {
	buttonEnabled = flagset;
	for (unsigned i = 0; i < btnMAXIMUM; ++i)
		reconstruct((ButtonID)i);
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
