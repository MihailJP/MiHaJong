#include "ruleconf.h"
#include <cstring>
#include "../scrmanip.h"
#include "../geometry.h"
#include "../../sound/sound.h"
#include "../../mjcore/bgmid.h"
#include "../../mihajong/version.h"
#include <iomanip>

namespace mihajong_graphic {

RuleConfigScene::RuleConfigScene(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	memset(rulestat, 0, sizeof(rulestat));
	menuCursor = 0;
	redrawItems();
}

RuleConfigScene::~RuleConfigScene() {
}

void RuleConfigScene::itemText(unsigned prmID, const CodeConv::tstring& prmName, const CodeConv::tstring& prmContent) {
	// ���ڂ�\��
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	unsigned itmNameCols = 0u; // ����(���{���2��)
	for (auto k = prmName.begin(); k != prmName.end(); ++k) {
		if (*k <= _T('\x7f')) itmNameCols += 1;
		else itmNameCols += 2;
	}
	D3DCOLOR baseColor = ((prmContent == _T("�m�^�`")) || (prmContent.empty())) ? 0x00bfbfbf : 0x00ffffff;
	D3DCOLOR menuColor = (menuCursor % 40 == prmID) ? 0xff000000 : 0x7f000000;
	myTextRenderer->NewText(prmID * 3, prmName,
		(prmID / 20 * 720 + 50) * WidthRate, 160 + (prmID % 20) * 40, 1.0f,
		WidthRate * ((itmNameCols <= 8) ? 1.0f : 8.0f / (float)itmNameCols),
		menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 1, _T(":"),
		(prmID / 20 * 720 + 50 + 144) * WidthRate, 160 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
	myTextRenderer->NewText(prmID * 3 + 2, prmContent,
		(prmID / 20 * 720 + 50 + 162) * WidthRate, 160 + (prmID % 20) * 40, 1.0, WidthRate, menuColor | baseColor);
}

void RuleConfigScene::redrawItems() {
	float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
	for (int i = 0; i < 40; i++) {
		const unsigned ItemNum = (menuCursor / 40 * 40) + i;
		TCHAR menuitem[128]; rules::getRuleName(menuitem, 128, ItemNum);
		TCHAR itmfield[128]; rules::getRuleTxt(itmfield, 128, ItemNum, rulestat[ItemNum]);
		itemText(i, CodeConv::tstring(menuitem), CodeConv::tstring(itmfield));
	}
}

void RuleConfigScene::Render() {
	clearWithGameTypeColor(); // �o�b�t�@�N���A
	{
		float WidthRate = Geometry::WindowWidth * 0.75 / Geometry::WindowHeight; // �A�X��~0.75(���������p)
		uint64_t t = elapsed();
		CodeConv::tstring caption = _T("");
		switch ((t / 50000000u) % 4) {
		case 0:
			TCHAR menuitem[128]; rules::getRuleDescription(menuitem, 128, menuCursor);
			caption = CodeConv::tstring(menuitem);
			if (caption.empty()) {
				CodeConv::tostringstream o; SYSTEMTIME Zeit; GetLocalTime(&Zeit);
				o << _T("MiHaJong version ") _T(MIHAJONG_VER) _T(" / ���ݓ��� ") <<
					std::setw(4) << Zeit.wYear << _T("�N") <<
					std::setw(2) << Zeit.wMonth << _T("��") <<
					std::setw(2) << Zeit.wDay << _T("�� ") <<
					((Zeit.wHour / 12 == 0) ? _T("�ߑO") : _T("�ߌ�")) <<
					std::setw(2) << (Zeit.wHour % 12) << _T("��") <<
					std::setw(2) << std::setfill(_T('0')) << Zeit.wMinute << _T("��");
				caption = o.str();
			}
			break;
		case 1:
			caption = CodeConv::tstring(_T("��/��:�J�[�\���ړ�  ��/��:�I�𒆂̍��ڂ�ύX"));
			break;
		case 2:
			caption = CodeConv::tstring(_T("Home/End:���E�J�����Ԃ̈ړ�  PageUp/PageDown:�y�[�W�Ԃ̈ړ�"));
			break;
		case 3:
			caption = CodeConv::tstring(_T("�ʐM�ΐ펞�̃��[���ݒ�̓z�X�g���̐ݒ肪�K�p����܂�"));
			break;
		}
		unsigned captionCols = 0u; // ����(���{���2��)
		for (auto k = caption.begin(); k != caption.end(); ++k) {
			if (*k <= _T('\x7f')) captionCols += 1;
			else captionCols += 2;
		}
		myTextRenderer->NewText(120, caption,
			(720 - 9 * ((captionCols > 76) ? 76 : captionCols)) * WidthRate, 980, 1.0f,
			(captionCols > 76) ? 76.0f / (float)captionCols * WidthRate : WidthRate,
			((t % 50000000u) < 5000000u) ? (55u + ((t % 50000000u) / 25000u)) << 24 | 0x00ffffff : 0xffffffff);
	}
	myTextRenderer->Render();
}

void RuleConfigScene::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	switch (od->dwOfs) {
	case DIK_UP: // �O�̍���
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if (--menuCursor < 0) menuCursor = 0;
			skipto(0); redrawItems();
		}
		break;
	case DIK_DOWN: // ���̍���
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if (++menuCursor >= RULESIZE) menuCursor = RULESIZE - 1;
			skipto(0); redrawItems();
		}
		break;
	case DIK_LEFT: // �O�̑I����
		if (od->dwData) {
			sound::Play(sound::IDs::sndClick);
			if ((--rulestat[menuCursor]) < 0) rulestat[menuCursor] = rules::getRuleSize(menuCursor) - 1;
			redrawItems();
		}
		break;
	case DIK_RIGHT: // ���̑I����
		if (od->dwData) {
			sound::Play(sound::IDs::sndClick);
			if ((++rulestat[menuCursor]) >= rules::getRuleSize(menuCursor)) rulestat[menuCursor] = 0;
			redrawItems();
		}
		break;
	case DIK_HOME: // �O�̃J����
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor -= 20) < 0) menuCursor = 0;
			skipto(0); redrawItems();
		}
		break;
	case DIK_END: // ���̃J����
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor += 20) >= RULESIZE) menuCursor = RULESIZE - 1;
			skipto(0); redrawItems();
		}
		break;
	case DIK_PRIOR: // �O�̃y�[�W
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor -= 40) < 0) menuCursor = 0;
			skipto(0); redrawItems();
		}
		break;
	case DIK_NEXT: // ���̃y�[�W
		if (od->dwData) {
			sound::Play(sound::IDs::sndCursor);
			if ((menuCursor += 40) >= RULESIZE) menuCursor = RULESIZE - 1;
			skipto(0); redrawItems();
		}
		break;
	}
}

}
